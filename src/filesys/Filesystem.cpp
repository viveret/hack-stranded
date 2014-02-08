#include "Filesystem.hpp"
#include "File_Stats.hpp"
#include "../module/Thread.hpp"

#include <fstream>
#include <vector>
#include "../ss/ss_lib/File_Sys.hpp"
using namespace Survivor_Script_File_Sys;

#define DEF_BUF_SIZE 4096 * 16


namespace SE_Modules
{
SE_Module_ID Mod_Filesystem = NULL;
}
using namespace SE_Modules;

namespace SE_Filesys
{
	struct FS_Folder: public FS_Stats
	{
		uchar Type()
		{
			return 0;
		}

		std::vector< File_ID > Children_Files;
		std::vector< FS_Folder* > Children_Folders;
	};

	struct File_Stats_Active; // Loading / Reading / Writing

/*************************************/
	uint Initialize_FS( const std::vector<std::string>& mArgs );
	uint Cleanup_FS();
	bool FS_Add( File_ID What, std::string Name );

	void Thread_Engine();// Update / load files

	// Get active file from file ID
	File_Stats_Active* Get_Active( File_ID F_ID );

/**********  GLOBALS   ***************/

	std::vector< File_ID > File_Cache;
	std::vector< File_Stats_Active* > Active_Files;
	std::vector< File_Type_Desc* > File_Types;

	FS_Folder g_Folder;

	SE_Thread::Mutex m_Mutex; // For File vectors / containers, not indiv. files
}
using namespace SE_Filesys;

// File stats for active files
struct SE_Filesys::File_Stats_Active // Loading / Reading / Writing
{	
	virtual void Open( const std::string& Name ) = 0;
	virtual void Thread_Engine() = 0;

	File_Stats* m_File;

	uchar m_State;
	ssize_t Bytes_Total, Byte_Current;
	SE_Raw_Buffer Data;

	std::fstream m_File_Stream;

	File_Stats_Active()
	{
		this->m_File = NULL;
		this->m_State = 0;

		this->Bytes_Total = 0;
		this->Byte_Current = 0;
	}
};


/* void File_Stats_Stream_Read::Open( std::string Name )
{
	if( this->m_File_Stream.is_open() )
		return;

	this->m_File_Stream.open( Name.c_str(), std::ios::binary | std::fstream::in );
	if( this->m_File_Stream.good()  )
	{
		this->Name_FS = Name;
		this->m_State = Load_State::NO_ERROR;
	
		this->m_File_Stream.seekg( 0, std::ios::end );
		this->Bytes_Total = this->m_File_Stream.tellg();
		this->m_File_Stream.seekg( 0, std::ios::beg );

		if( this->Func_Read == NULL ) // Read whole part
			this->Data.Make( this->Bytes_Total, "file read stream buf" );
		
		else
			this->Data.Make( DEF_BUF_SIZE, "file read stream buf" );
//		sePrintf( "Successfully opened file \'%s\"\n", 0, Name.c_str() );
	}
	else
	{
		this->m_State = Load_State::NO_FILE;
		sePrintf( SEPRINT_ERROR, "Failed to open file \'%s\"\n", Name.c_str() );
	}
}
*/
/**********************   READING OPERATIONS    ********************************/

// Read and update by the chunk, initialize when done
struct File_Stats_Stream_Read_Chunky: public File_Stats_Active
{
	void Open( const std::string& Name )
	{
		if( this->m_File_Stream.is_open() )
			return;

		assert( this->m_File != NULL );
		assert( this->m_File->m_Type != NULL );
		assert( this->m_File->m_Type->Func_Stream != NULL );

		this->m_File_Stream.open( Name.c_str(), std::ios::binary | std::fstream::in );
		if( this->m_File_Stream.good()  )
		{
			this->m_State = File_State::FILE_NO_ERROR;
	
			this->m_File_Stream.seekg( 0, std::ios::end );
			this->Bytes_Total = this->m_File_Stream.tellg();
			this->m_File_Stream.seekg( 0, std::ios::beg );

			this->Data.Make( DEF_BUF_SIZE, "file read stream buf" );
			sePrintf( SEPRINT_DEBUG, "Successfully opened file \'%s\"\n", Name.c_str() );
		}
		else
		{
			this->m_State = File_State::FILE_NO_FILE;
			sePrintf( SEPRINT_ERROR, "Failed to open file \'%s\"\n", Name.c_str() );
		}
	}

	void Thread_Engine()
	{	
		if( this->m_State == File_State::FILE_NO_ERROR && this->Bytes_Total - this->m_File_Stream.tellg() != 0)
		{
			if( this->Bytes_Total - this->m_File_Stream.tellg() < this->Data.Size() ) // Does not fit (too small)
			{
				this->Data.Delete();
				this->Data.Make( this->Bytes_Total - this->m_File_Stream.tellg(), "file stream read buf" );
			}

			memset( this->Data.Data(), 0, this->Data.Size() );
			this->m_File_Stream.read( this->Data.Data(), this->Data.Size() );
			this->Byte_Current += this->Data.Size();
		}

		this->m_File->m_Type->Func_Stream( this->Data, this->m_File );

		if( this->m_File_Stream.eof() || this->Byte_Current >= this->Bytes_Total )
		{
			this->m_State = File_State::FILE_EOF;

			if( this->m_File->m_Type->Func_Init )
			{
				this->m_File->m_Type->Func_Init( this->m_File );
//				Buf.Delete();
			}
		}
	}
};

File_ID SE_Filesys::Open_For_Read_Chunky( const std::string& File, std::string Type )
{
	File_Type_Desc* nType = NULL;
	if( Type.length() != 0 )
		nType = File_Type_Get( Type );

	if( nType == NULL ) // or if we are using extension
		nType = File_Type_Get_Ext( File );

	if( nType == NULL )
	{
		sePrintf( SEPRINT_ERROR, "Could not open %s: No type associated.\n", File.c_str() );
		assert( false );
		return NULL;
	}

	File_Stats_Stream_Read_Chunky* nStream = new File_Stats_Stream_Read_Chunky;
	nStream->m_File = nType->Func_New();
	nStream->m_File->m_Type = nType;
	FS_Add( nStream->m_File, File );

	nStream->Open( File );

	if( nStream->m_State != File_State::FILE_NO_FILE )
	{	
		m_Mutex.Wait_For_Lock();
		m_Mutex.Lock();
		Active_Files.push_back( nStream );
		m_Mutex.Unlock();

		return nStream->m_File;
	}

	sePrintf( SEPRINT_ERROR, "Error with file %s\n", File.c_str() );

//	delete nStream->m_File;
	delete nStream;

	return NULL;
}

// Read by chunk, initialize when done
struct File_Stats_Stream_Read_Whole: public File_Stats_Active
{
	void Open( const std::string& Name )
	{
		if( this->m_File_Stream.is_open() )
			return;

		assert( this->m_File != NULL );
		assert( this->m_File->m_Type != NULL );

		this->m_File_Stream.open( Name.c_str(), std::ios::binary | std::fstream::in );
		if( this->m_File_Stream.good()  )
		{
			this->m_State = File_State::FILE_NO_ERROR;
	
			this->m_File_Stream.seekg( 0, std::ios::end );
			this->Bytes_Total = this->m_File_Stream.tellg();
			this->m_File_Stream.seekg( 0, std::ios::beg );

			this->Data.Make( this->Bytes_Total, "file read stream buf" );
			sePrintf( SEPRINT_DEBUG, "Successfully opened file \'%s\"\n", Name.c_str() );
		}
		else
		{
			this->m_State = File_State::FILE_NO_FILE;
			sePrintf( SEPRINT_ERROR, "Failed to open file \'%s\"\n", Name.c_str() );
		}

	}

	void Thread_Engine()
	{	
		ssize_t Read_Start = this->m_File_Stream.tellg();
		this->m_File_Stream.read( this->Data.Data() + this->Byte_Current, DEF_BUF_SIZE );//this->Data.Size() );
		this->Byte_Current += this->m_File_Stream.tellg() - (std::streamoff) Read_Start;
		
//		sePrintf( SEPRINT_DEBUG, "%f%%\n", (float)this->Byte_Current / this->Bytes_Total * 100 );

		if( this->m_File_Stream.eof() || this->Byte_Current >= this->Bytes_Total )
		{
			this->m_State = File_State::FILE_EOF;

			if( this->m_File->m_Type->Func_Init )
			{
				this->m_File->m_Type->Func_Init( this->m_File );
//				Buf.Delete();
			}
		}
	}
};

File_ID SE_Filesys::Open_For_Read_Whole( const std::string& File, std::string Type )
{
	File_Type_Desc* nType = NULL;
	if( Type.length() != 0 )
		nType = File_Type_Get( Type );

	if( nType == NULL ) // or if we are using extension
		nType = File_Type_Get_Ext( File );

	if( nType == NULL )
	{
		sePrintf( SEPRINT_ERROR, "Could not open %s: No type associated.\n", File.c_str() );
		return NULL;
	}

	File_Stats_Stream_Read_Whole* nStream = new File_Stats_Stream_Read_Whole;
	nStream->m_File = nType->Func_New();
	nStream->m_File->m_Type = nType;
	FS_Add( nStream->m_File, File );

	nStream->Open( File );

	if( nStream->m_State != File_State::FILE_NO_FILE )
	{		
		m_Mutex.Wait_For_Lock();
		m_Mutex.Lock();
		Active_Files.push_back( nStream );
		m_Mutex.Unlock();

		return nStream->m_File;
	}

	sePrintf( SEPRINT_ERROR, "Error with file %s\n", File.c_str() );

//	delete nStream->m_File;
	delete nStream;

	return NULL;
}

// Read a file instantly (bypass streamer)
SE_Raw_Buffer SE_Filesys::File_Load_Instant( const std::string& File )
{
	std::ifstream Reader( File, std::ios::binary | std::ios::in );

	SE_Raw_Buffer Ret;

	if( Reader.good() )
	{
		Reader.seekg( 0, std::ios::end );
		Ret.Make( (uint)Reader.tellg() + 1, "load file buffer" );
		Reader.seekg( 0, std::ios::beg );

		Reader.read( Ret.Data(), Ret.Size() );

		//sePrintf( "Success, Size: " + To_Str( Ret.Size() ) + "\n", 0 );//Log_Type::TYPE_LOAD );
	}
	else
	{
		std::string Report = "Failed to load file: " + File;

//		Engine.Register_Error(S_Error(Report,1));

		//if( Silent == false )
		//Engine.Log("Failed\n", Log_Type::TYPE_EROR);
		sePrintf( SEPRINT_ERROR, "Could not open file %s\n", File.c_str() );
	}

	return Ret;
}

/******************************    VFS OPERATIONS    *******************************/

File_Stats_Active* SE_Filesys::Get_Active( File_ID F_ID )
{
	for( uint i = 0; i < Active_Files.size(); i++ )
	{
		if( Active_Files[i]->m_File == F_ID )
			return Active_Files[i];
	}

	return NULL;
}


/*
void File_Stats_Stream_Read::Thread_Engine()
{
	if( this->Func_Read != NULL ) // is reading part by part
	{
		if( this->m_State == Load_State::NO_ERROR && this->Bytes_Total - this->m_File_Stream.tellg() != 0)
		{
			if( this->Bytes_Total - this->m_File_Stream.tellg() < this->Data.Size() ) // Does not fit (too small)
			{
				this->Data.Delete();
				this->Data.Make( this->Bytes_Total - this->m_File_Stream.tellg(), "file stream read buf" );
			}
	
			sePrintf( SEPRINT_INFO, "%f%%\n", (float)this->Byte_Current / this->Bytes_Total * 100.f );
		
			memset( this->Data.Data(), 0, this->Data.Size() );
			this->m_File_Stream.read( this->Data.Data(), this->Data.Size() );
			this->Byte_Current += this->Data.Size();
		}

		this->Func_Read( this->Data, this->Func_Data );
	}
	else
	{
		ssize_t Read_Start = this->m_File_Stream.tellg();
		this->m_File_Stream.read( this->Data.Data() + this->Byte_Current, this->Data.Size() );
		this->Byte_Current += this->m_File_Stream.tellg() - Read_Start;
	}

	if( this->m_File_Stream.eof() || this->Byte_Current >= this->Bytes_Total )
	{
		this->m_State = Load_State::FILE_EOF;

		if( this->Func_End )
		{
			this->Func_End( this->Func_Data );
//			Buf.Delete();
		}
	}
}
*/
/***********   FILESYSTEM OPERATIONS   **********/

struct File_Stats_Active_Write: public File_Stats_Active
{
	void Open( const std::string& Name )
	{
		if( this->m_File_Stream.is_open() )
			return;

		this->m_File_Stream.open( Name.c_str(), std::ios::binary | std::fstream::out );
		if( this->m_File_Stream.is_open()  )
		{
			this->m_File->Name = Name;
			this->m_State = File_State::FILE_NO_ERROR;
		}
		else
		{
			this->m_State = File_State::FILE_NO_FILE;
			sePrintf( SEPRINT_ERROR, "Failed to open file \'%s\"\n", Name.c_str() );
		}
	}

	void Thread_Engine()
	{
		// Do nothing
	}
};
struct File_Stats_Write: public File_Stats
{
	uchar Type()
	{
		return File_Type::TYPE_WRITE_STREAM;
	}
};

File_ID SE_Filesys::Open_For_Write_Stream( const std::string& File )
{
	// Assuming .txt
	
	File_Stats_Active_Write* nStream = new File_Stats_Active_Write;
	File_ID Ret = new File_Stats_Write;
	nStream->m_File = Ret;
	nStream->Open( File );

	if( nStream->m_State != File_State::FILE_NO_FILE )
	{
		m_Mutex.Wait_For_Lock();
		m_Mutex.Lock();
		Active_Files.push_back( nStream );
		m_Mutex.Unlock();

		return Ret;
	}
	else
	{	
		delete nStream->m_File;
		delete nStream;
		return NULL;
	}
}
/*
File_ID SE_Filesys::Open_File_Read( std::string File, void* Func_Data, void (*Func_Read)(SE_Raw_Buffer, void* ), void (*Func_End)(void*), uint Buffer_Size, uint Bytes_Per_Second )
{
//	assert( Func_Read != NULL );
//	assert( Func_End != NULL );

	File_Stats_Stream_Read* Ret = new File_Stats_Stream_Read();
	Ret->Func_Data = Func_Data;
	Ret->Func_Read = Func_Read;
	Ret->Func_End = Func_End;

	Ret->Open( File );

	if( Ret->m_State != Load_State::NO_FILE )
	{		
		Active_Files.push_back( Ret );
		return Ret;
	}

	return NULL;
}
/*
File_ID Open_File_Read_Part( std::string File, void* Func_Data, void (*Func_End)(void*), uint Buffer_Size, uint Bytes_Per_Second )
{
//	assert( Func_Read != NULL );
//	assert( Func_End != NULL );

	File_Stats_Read_Whole* Ret = new File_Stats_Read_Whole();
	Ret->Open( File );

	if( Ret->m_State != Load_State::NO_FILE )
	{
		Ret->Func_Data = Func_Data;
		Ret->Func_Read = Func_Read;
		Ret->Func_End = Func_End;
		

		Active_Files.push_back( Ret );
		return Ret;
	}

	return NULL;
}
*/

/*
SE_Raw_Buffer SE_Filesys::File_Load_Instant( const std::string& File )
{}
*/
void SE_Filesys::Write_To( const void* Data, uint Size, File_ID F_ID )
{
  	if( Get_State( F_ID ) != File_State::FILE_NO_ERROR )
		return;

	File_Stats_Active* nStream = Get_Active( F_ID );

	if( nStream == NULL ) return;

	nStream->m_File_Stream.write( (const char*)Data, Size );
	nStream->m_File_Stream.flush();
}

uchar SE_Filesys::Get_State( File_ID F_ID ) // Get state of file
{
	if( F_ID == NULL )
		return File_State::FILE_NO_FILE;
	
	else
	{
		File_Stats_Active* nActive = Get_Active(F_ID);
		assert( nActive != NULL );

		return nActive->m_State;
	}
}

std::string SE_Filesys::Get_FS_Name( File_ID F_ID ) // Get name
{
	if( F_ID == NULL )
		return std::string();
	
	else
	{
//		File_Stats_Active* nActive = Get_Active(F_ID);
//		assert( nActive != NULL );

		return F_ID->Name;
	}
}

SE_Raw_Buffer& SE_Filesys::Get_Data( File_ID F_ID ) // Get data
{
	assert( F_ID != NULL );

	File_Stats_Active* nActive = Get_Active(F_ID);
	assert( nActive != NULL );

	return nActive->Data;
}


void SE_Filesys::Thread_Engine()
{
	m_Mutex.Wait_For_Lock();
	m_Mutex.Lock();

	for( uint i = 0; i < Active_Files.size(); i++ )
	{
		if( Active_Files[i]->m_State == File_State::FILE_NO_ERROR )
			Active_Files[i]->Thread_Engine();

/*			File_Cache.push_back( Active_Files[i]->m_File );
			Active_Files.erase( Active_Files.begin() + i );
			i--;*/
	}

	m_Mutex.Unlock();
}

/******************************    VFS OPERATIONS    *******************************/

bool SE_Filesys::FS_Add( File_ID What, std::string Name )
{
	std::vector< std::string > Tokens;

	ssize_t Sep_Pos = 0;
	while( (Sep_Pos = Name.find('/')) != Name.npos )
	{
		Tokens.push_back( Name.substr(0, Sep_Pos) );
		Name.erase( 0, Sep_Pos + 1 );
	}

	m_Mutex.Wait_For_Lock();
	m_Mutex.Lock();

	// Name is now the name of the file
	FS_Folder* Cur_Folder = &g_Folder;
	for( uint i = 0; i < Tokens.size(); i++ )
	{
		if( Tokens[i].length() == 0 ) continue;

		for( uint j = 0; j < Cur_Folder->Children_Folders.size(); j++ )
		{
			if( Cur_Folder->Children_Folders[j]->Name == Tokens[i] )
			{
				Cur_Folder = Cur_Folder->Children_Folders[j];
				break;
			}
		}

		// Did not find current folder, create one
		FS_Folder* nFolder = new FS_Folder;
		nFolder->Name = Tokens[i];
		nFolder->Parent = Cur_Folder;

		Cur_Folder->Children_Folders.push_back( nFolder );
		Cur_Folder = nFolder;
	}

	// Now find file
	for( uint i = 0; i < Cur_Folder->Children_Files.size(); i++ )
	{
		if( Cur_Folder->Children_Files[i]->Name == Name )
			return false;
	}

	// Does not exist, can add
	Cur_Folder->Children_Files.push_back( What );
	What->Parent = Cur_Folder;
	What->Name = Name;

	m_Mutex.Unlock();
}

File_ID SE_Filesys::VFS_Get( std::string Name )
{
	std::vector< std::string > Tokens;

	ssize_t Sep_Pos = 0;
	while( (Sep_Pos = Name.find('/')) != Name.npos )
	{
		Tokens.push_back( Name.substr(0, Sep_Pos) );
		Name.erase( 0, Sep_Pos + 1 );
	}

	m_Mutex.Wait_For_Lock();
	m_Mutex.Lock();

	// Name is now the name of the file
	FS_Folder* Cur_Folder = &g_Folder;
	for( uint i = 0; i < Tokens.size(); i++ )
	{
		if( Tokens[i].length() == 0 ) continue;

		for( uint j = 0; j < Cur_Folder->Children_Folders.size(); j++ )
		{
			if( Cur_Folder->Children_Folders[j]->Name == Tokens[i] )
			{
				Cur_Folder = Cur_Folder->Children_Folders[j];
				break;
			}
		}

		// Did not find current folder, create one
		FS_Folder* nFolder = new FS_Folder;
		nFolder->Name = Tokens[i];
		nFolder->Parent = Cur_Folder;

		Cur_Folder->Children_Folders.push_back( nFolder );
		Cur_Folder = nFolder;
	}

	// Now find file
	for( uint i = 0; i < Cur_Folder->Children_Files.size(); i++ )
	{
		if( Cur_Folder->Children_Files[i]->Name == Name )
		{
			m_Mutex.Unlock();
			return Cur_Folder->Children_Files[i];
		}
	}

	m_Mutex.Unlock();
	return NULL;
}

uchar SE_Filesys::Register_File_Type( File_Type_Desc& nType )
{
	assert( nType.Func_New != NULL );

	for( uint i = 0; i < File_Types.size(); i++ )
	{
		if( nType.Name == File_Types[i]->Name )
		{
			sePrintf( SEPRINT_ERROR, "Reg f_type %s collides with %s\n", nType.Name.c_str(), File_Types[i]->Name.c_str() );
			return 0;
		}
		else
		{
			for( uint j = 0; j < File_Types[i]->Extension.size(); j++ )
			{
				for( uint k = 0; k < nType.Extension.size(); k++ )
				{
					if( File_Types[i]->Extension[j] == nType.Extension[k] )
					{
						sePrintf( SEPRINT_ERROR, "Reg f_type %s collides with %s, ext: %s\n", nType.Name.c_str(), File_Types[i]->Name.c_str(), nType.Extension[k].c_str() );
						return 0;
					}
				}
			}
		}
	}

	File_Type_Desc* nType_Copy = new File_Type_Desc;
	*nType_Copy = nType;

	File_Types.push_back( nType_Copy );

	return 0;
}

File_Type_Desc* SE_Filesys::File_Type_Get( std::string Name )
{
	for( uint i = 0; i < File_Types.size(); i++ )
	{
		if( File_Types[i]->Name == Name )
			return File_Types[i];
	}

	return NULL;
}

File_Type_Desc* SE_Filesys::File_Type_Get_Ext( const std::string& F_Name )// By file name
{
	std::string n_Ext = F_Name.substr( F_Name.find_last_of(".") + 1, F_Name.length() - 1 - F_Name.find_last_of(".") );
//	sePrintf( SEPRINT_DEBUG, "nExt: %s", n_Ext.c_str() );

	for( uint i = 0; i < File_Types.size(); i++ )
	{
		for( uint j = 0; j < File_Types[i]->Extension.size(); j++ )
		{
			if( File_Types[i]->Extension[j] == n_Ext )
				return File_Types[i];
		}
	}

	return NULL;
}


// Load in VFS struture
void SE_Filesys::VFS_Load( const std::string& Where )
{
	//
}

void SE_Filesys::VFS_Load_Script( const std::string& Script )
{
	std::vector< std::string > Tokens;

	uchar State = 0;
	uint Sep_Pos = 0;

	for( uint i = 0; i <= Script.length() + 1; i++ )
	{
		if( i == Script.length() )

		switch( State )
		{
		case 0: // Regular
			{
				if( Script[i] == '\'' || Script[i] == '\"' )
				{
//					Sep_Pos++;

					while( Script[i] != '\'' && Script[i] != '\"' && i < Script.length() )
						i++;
//					State = 1; // String
//					i--;
					Sep_Pos++;
					State = 2;
				}
				
				else if( Script[i] == ' ' || Script[i] == '\n' )
					State = 2; // End

				else if( Script[i] == '#' )
				{
					while( (Script[i] != '\n' || Script[i++] != '\r') && i < Script.length() )
						i++;
				}
				else if( i >= Script.length() - 1 )
				{
					State = 2;
					i++;
//					i--;					
				}
			}break;
/* 		case 1: // String
			{
				while( Script[i] == '\'' || Script[i] == '\"' )
					State = 0;
				}
			}break;*/
		case 2:
			{
				Tokens.push_back( Script.substr( Sep_Pos, i - Sep_Pos - 1 ) );
				State = 0;
				
				while( (Script[i] == '\n' || Script[i] == '\r' || Script[i] == '\t' || Script[i] == ' ') && i < Script.length() - 1 )
					i++;

				Sep_Pos = i;
			}break;
		}
	}

	sePrintf( SEPRINT_DEBUG, "Tokens:" );
	for( uint i = 0; i < Tokens.size(); i++ )
	{
//		sePrintf( SEPRINT_DEBUG, "\n\t%s", Tokens[i].c_str() );
		if( Tokens[i].size() == 0 )
			continue;

		else if( Tokens[i] == "include" )
		{
			if( Tokens.size() < i + 1 )
			{
				sePrintf( SEPRINT_ERROR, "\nExpected directory after \'%s\'", Tokens[i].c_str() );
				return;
			}

			sePrintf( SEPRINT_DEBUG, "\n\tIncluding \'%s\'", Tokens[i+1].c_str() );
		}
	}
	sePrintf( SEPRINT_DEBUG, "\n" );
}

void SE_Filesys::VFS_Load_XML( void* XML_File )
{
	//
}


/**********   MODULE OPERATIONS   ****************/

uint SE_Filesys::Initialize_FS( const std::vector<std::string>& mArgs )
{
	VFS_Load_Script( "inside 'packages'\n\tinclude 'Survivor.zip'\nleave" );
	return SE_SUCCESS;
}

uint SE_Filesys::Cleanup_FS()
{
	return SE_SUCCESS;
}

void SE_Filesys::Register_Module()
{
	Mod_Filesystem = S_Engine::Generate_Module( "filesystem" );
	S_Engine::Register_Module_Engine( Mod_Filesystem, 1, Initialize_FS, Cleanup_FS );
	S_Engine::Register_Module_Thread_Engine( Thread_Engine, S_Engine::Module_Positions::POS_START + 3, 200 );
}



/*********************    FILE STAT JUNK   ****************************/


File_Stats::File_Stats()
{
//	this->m_State = File_State::NO_FILE;
	this->m_Type = NULL;
//	this->Bytes_Total = 0;
}
/* 
void File_Stats::Open( std::string Name )
{
	if( this->m_File_Stream.is_open() )
		return;

	this->m_File_Stream.open( Name.c_str(), std::ios::binary  | std::fstream::out );// | std::fstream::in );
	if( this->m_File_Stream.fail() == false )
	{
		this->Name_FS = Name;
		this->m_State = Load_State::NO_ERROR;
//		sePrintf( "Successfully opened file \'%s\"\n", 0, Name.c_str() );
	}
	else
	{
		this->m_State = Load_State::NO_FILE;
		sePrintf( SEPRINT_ERROR, "Failed to open file \'%s\"\n", Name.c_str() );
	}
}

void File_Stats::Thread_Engine()
{

}
*/
/*****************  READ STREAM   ***************//*
#define DEF_BUF_SIZE 512
File_Stats_Stream_Read::File_Stats_Stream_Read()
{
	this->Func_Read = NULL;
	this->Func_End = NULL;
	this->Func_Data = NULL;
	
	this->Byte_Current = 0;
	this->Bytes_Per_Second = 0;

//	this->Buf.Make( DEF_BUF_SIZE, "file read stream buf" );
}
*/
