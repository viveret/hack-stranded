#include "File_Stream.hpp"
using namespace SE_File_Stream;

#include "../engine/Engine.hpp"



File_Base::File_Base()
{
	this->Total_Bytes = 1;// no divide by 0 error
	this->Cur_Byte = 0;

	this->Load_State = Load_State::LOAD_STATE_NO_ERROR;
	this->Data_State = 0;

	this->Bytes_Per_Load = FSTREAM_BUF_SIZE;
}

void File_Base::Cleanup()
{
	if( this->File_Reader.is_open() )
	{			 
		this->File_Reader.close();

		this->Total_Bytes = 1;// no divide by 0 error
		this->Cur_Byte = 0;

		this->Load_State = Load_State::LOAD_STATE_NO_ERROR;
		this->Data_State = 0;
		this->Data = NULL;

		this->File_Buf.Delete();
		this->Data_Buf.Delete();
	}
}

void File_Base::Load( std::string File, void* To_Data )
{
	if( this->File_Reader.is_open() == false )
	{
		this->File_Reader.open( File.c_str(), std::ios::binary );
		
		if( this->File_Reader.is_open() )
		{
			this->File_Reader.seekg( 0, std::ios::end );
			this->Total_Bytes = (uint)this->File_Reader.tellg();
			this->File_Reader.seekg( 0, std::ios::beg );


			this->File_Name = File;
			this->Data = To_Data;

#ifdef _WIN32
			this->File_Buf.Make( this->Bytes_Per_Load, "file stream buf" );
#endif
			this->Init_Data_Buf();

			
//			Engine.Stream_Manager.Add_File( this );
		}
		else
			this->Load_State = Load_State::LOAD_STATE_NO_FILE;
	}
}

void File_Base::Load_Partial()
{
	if( this->File_Reader.is_open() )
	{
		memset( this->File_Buf.Data(), 0, this->File_Buf.Size() );

		this->File_Reader.read( this->File_Buf.Data(), this->File_Buf.Size() );

		if( this->File_Reader.eof() )					  
			this->Load_State = Load_State::LOAD_STATE_EOF;

		else if( this->File_Reader.fail() )				  
			this->Load_State = Load_State::LOAD_STATE_INVALID_DATA;
		
		else if( this->File_Reader.bad() )
			this->Load_State = Load_State::LOAD_STATE_INVALID_DATA;
	}
	else
		this->Load_State = Load_State::LOAD_STATE_NO_FILE;
}



File_Stream_Manager::File_Stream_Manager()
{
	//
}

File_Stream_Manager::~File_Stream_Manager()
{
	this->Cleanup();
}

void File_Stream_Manager::Cleanup()
{
	for( uint i = 0;i < this->Files.size(); i++ )
	{
		this->Files[i]->Cleanup();			
#ifdef _TEST
		Engine.Mem_Remove( this->Files[i] );
#endif
	}

	this->Files.clear();
}

void File_Stream_Manager::Update()
{											
	this->File_Mutex.Wait_For_Lock( );// __FUNCTION__ );
	this->File_Mutex.Lock();

	for( uint i = 0;i < this->Files.size(); i++ )
	{
		this->Files[i]->Load_Partial();
		this->Files[i]->Update_Data();

		if( this->Files[i]->Load_State != Load_State::LOAD_STATE_NO_ERROR ) // finished
		{
			this->Files[i]->Cleanup();
#ifdef _TEST
			Engine.Mem_Remove( this->Files[i] );
#endif

			this->Files.erase( this->Files.begin() + i );
			i--;
		}
	}
	
	this->File_Mutex.Unlock();
}

void File_Stream_Manager::Add_File( File_Base* File )
{
	this->File_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	this->File_Mutex.Lock();


	this->Files.push_back( File );
#ifdef _TEST
	Engine.Mem_Add( File, "file streamer" );

	Engine.Log( "Started Stream: " + File->File_Name + "\n", Log_Type::TYPE_LOAD );
#endif


	this->File_Mutex.Unlock();
}




/*		DEFAULT FILE STREAM TYPES		*/
using namespace Def_Files;



void File_TXT::Init_Data_Buf()
{
	/*this->Data_Buf.Make( this->File_Buf.Get_Size() + 1, "str data buf" );
	memset( this->Data_Buf.Get_Data(), 0 , this->Data_Buf.Get_Size() );


	this->Data_Buf[ this->Data_Buf.Get_Size() - 1 ] = 0;*/
}

void File_TXT::Update_Data()
{
//	memcpy( this->Data_Buf.Get_Data(), this->File_Buf.Get_Data(), this->File_Buf.Get_Size() );

	if( this->Cur_Byte < this->Total_Bytes && this->Data )
		this->mData()->append( this->File_Buf.Data(), this->File_Buf.Size() );
	/*else
		this->Data->append( this->File_Buf.Get_Data(), this->Total_Bytes - this->File_Buf.Get_Size() );*/
}

std::string* File_TXT::mData()
{
	return (std::string*)this->Data;
}
