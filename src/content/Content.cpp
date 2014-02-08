#ifdef _TEST
#include "../engine/Engine.hpp"

#include "../content/Content.hpp"
using namespace E_Content;

#include "../utility/Functions.hpp"

using namespace SE_Virt_Filesys;

#include "File_Types.hpp"
using namespace SE_Content_File_Types;


C_Content_Package::C_Content_Package( std::string nName )
{
	this->Name = nName;
}



C_Registered_Type::C_Registered_Type()
{
	this->Make_New = NULL;
}



C_Content_Manager::C_Content_Manager() : VFS( NULL )
{
	this->VFS.Name = "root";
	// REGISTER DEFAULT TYPES

	this->Register_Type( "file", Type_File::Make_New_Static );
	this->Register_Type( "text", Type_Text::Make_New_Static );
	this->Register_Type( "texture", Type_Texture::Make_New_Static );
	this->Register_Type( "sound", Type_Sound::Make_New_Static );
}

bool C_Content_Manager::Initialize()
{
	if( this->Get_Packages() == false )
		return false;

	else if( this->Get_Pre_Req() == false )
		return false;

	else if( this->Load_Packages() == false )
		return false;

	else if( this->Get_Resources() == false )
		return false;

	this->VFS_Write();

	return true;
}

bool C_Content_Manager::Cleanup()
{
	this->VFS.Cleanup();

	return true;
}

bool C_Content_Manager::Get_Pre_Req()
{
#ifdef _TEST
	Engine.Log( "Loading required resources...\n", Log_Type::TYPE_DBUG );

	this->Mesh_Manager.Load_Mesh( "engine/error.ply" );

	Engine.Resources.Resource_Req.Error = Engine.Resources.Load_Texture( "engine/error.png" );
	Engine.Resources.Resource_Req.Text = Engine.Resources.Load_Texture( "fonts/text.png" );
	Engine.Resources.Resource_Req.GUI_Background = Engine.Resources.Load_Texture( "screen/gui/background.bmp" );


	Type_Texture* Get_Prereq = (Type_Texture*)this->VFS.Get_Child( "screen/gui/screen/splash.bmp" );
	if( Get_Prereq )
	{
		Get_Prereq->Data_Load();
		Engine.Resources.Resource_Req.GUI_Splash = Get_Prereq->ID;
	}

	Engine.Resources.Loaded_Necesities = true;
	Engine.Log( "Loaded required resources.\n", Log_Type::TYPE_DBUG );
#endif
	return true;
}

bool C_Content_Manager::Get_Resources()
{
#ifdef _TEST
	Engine.Log( "Getting resources...  ", Log_Type::TYPE_DBUG );

//	this->Load_Files( &this->Virtual_Folder );
//	Engine.Log( "Loaded Files\n", Log_Type::TYPE_DBUG );

	// assign hard-coded resources
	Engine.Resources.Resource_Req.Get_Resources();
//	Engine.Log( "Got Required Resources\n", Log_Type::TYPE_DBUG );

	Engine.Resources.Stuff->Get_Resources();
	Engine.Log( "Succeeded\n", Log_Type::TYPE_DBUG );

#endif
	return true;
}

bool C_Content_Manager::Get_Packages()
{
#ifdef _TEST
	Engine.Log( "Getting packages...\n", Log_Type::TYPE_DBUG );
	this->Package_Get_List();

	Engine.Log( "Packages:\n" );
	for( uint i = 0; i < this->Packages.size(); i++ )
	{
		std::string Content_Script = Engine.Application.Input.Load_File_Str("packages/" + this->Packages[i].Name + ".zip/content.txt" );

		if( this->Package_Handle_Script( Content_Script, &this->Packages[i] ) )
			Engine.Printf( "\'%s\' was successfully executed\n", Log_Type::TYPE_DBUG, this->Packages[i].Name.c_str() );
		else
		{
			Engine.Printf( "\'%s\' FAILED to execute\n", Log_Type::TYPE_EROR, this->Packages[i].Name.c_str() );
			continue;
		}
	}

	Engine.Log( "Finished Getting packages.\n" );
#endif
	return true;
}

bool C_Content_Manager::Load_Packages()
{
#ifdef _TEST
	Engine.Log( "Loading packages...\n", Log_Type::TYPE_DBUG );
	for( uint p = 0; p < this->Packages.size(); p++ )
	{
		if( this->Package_Load( &this->Packages[p] ) )
			Engine.Printf( "\'%s\' was successfully loaded\n", Log_Type::TYPE_INFO, this->Packages[p].Name.c_str() );
		else
		{
			Engine.Printf( "\'%s\' FAILED to load\n", Log_Type::TYPE_EROR, this->Packages[p].Name.c_str() );
			continue;
		}
	}

	Engine.Log( "Finished loading packages.\n" );
#endif
	return true;
}

void C_Content_Manager::Register_Type( std::string Name, F_Node* (*Make_New)(F_Node*) )
{
	C_Registered_Type nType;
	nType.Name = Name;
	nType.Make_New = Make_New;

	this->Registered_Types.push_back( nType );
}

C_Registered_Type* C_Content_Manager::Reg_Type_Get( std::string Name, bool Probe )
{
	for( uint i = 0; i < this->Registered_Types.size(); i++ )
	{
		if( Registered_Types[i].Name == Name )
			return &Registered_Types[i];
	}
#ifdef _TEST
	if( Probe == false )
		Engine.Register_Error(S_Error("Content type \'" + Name + "\' does not exist", 1 ));
#endif

	return NULL;
}

bool C_Content_Manager::Package_Get_List() // get list of packages
{
	std::string List;
#ifdef _TEST
	= Engine.Application.Input.Load_File_Str("data/packages.txt");
#endif

	if( List.length() == 0 )
	{
#ifdef _TEST
		Engine.Register_Error(S_Error("Cannot load content, no list data",3));
#endif
		return false;
	}

	size_t Sep_Pos = 0;
	while( (Sep_Pos = List.find("\r\n")) != std::string::npos )
	{
		if( Sep_Pos == 0 )
		{
			List.erase( 0, Sep_Pos + 1 );
			continue;
		}

		this->Packages.push_back( C_Content_Package(List.substr( 0, Sep_Pos )) );
		List.erase( 0, Sep_Pos + 1 );
	}

	return true;
}

bool C_Content_Manager::Package_Handle_Script( const std::string& Script, C_Content_Package* Pack ) // perform script
{
	std::ofstream Script_Log( "data/log_" + Pack->Name + "_scirpt.log" );

	std::vector< std::string > Tokens;

	bool Getting_Path = false;
	for( uint i = 0, Start_I = 0; i < Script.length(); i++ )
	{
		/*if( Script[i] != '\\' && Script[i - 1] == '\\' ) // '\'escape char
			continue;

		else */if( Script[i] == '#' ) // comment
		{
			Tokens.push_back( "\n" );
			i = Script.find( "\r\n", i ) - 1;
		}

		else if( ( Script[i] == ' ' && Getting_Path == false ) || Script[i] == '\n' || Script[i] == '\r' || i == Script.length() - 1 )
		{
			if(  i == Script.length() - 1 )
				i++;

			if( i - Start_I != 0 )
			{
				if( Script[Start_I] == '\'' || Script[Start_I] == '\"' )
					Tokens.push_back( Script.substr( Start_I + 1, i - Start_I - 2 ) );

				else
					Tokens.push_back( Script.substr( Start_I, i - Start_I ) );
			}

			Start_I = i + 1;
		}
		else if(  Script[i] == '\'' || Script[i] == '\"' )
			Getting_Path = !Getting_Path;
	}

	for( uint t = 0; t < Tokens.size(); t++ )
	{
		if( Tokens[t] == "add" )
		{
			t += 2; // name
			uint Start_Cur_File_Name = Tokens[t].find_last_of('/') + 1;//find last directory
			std::string Cur_File_Name = Tokens[t].substr( Start_Cur_File_Name, Tokens[t].length() - Start_Cur_File_Name );
			Tokens[t].erase( Start_Cur_File_Name, Tokens[t].length() - Start_Cur_File_Name + 1 );


			std::istringstream Name_Stream( Tokens[t] );
			F_Folder* Cur_Dir = &this->VFS;
			std::string Cur_Dir_Name;

			while( std::getline( Name_Stream, Cur_Dir_Name, '/' ) )
			{
				Cur_Dir = Cur_Dir->Add_Folder( Cur_Dir_Name );
			}

			if( Cur_Dir->Get_Child( Cur_File_Name, "", true ) )// there is already a file, update it
			{
				F_Node* Temp_File = (F_Node*)Cur_Dir->Get_Child( Cur_File_Name, "", true );//)->File_Name = Cur_File_Name;
				if( Temp_File )
				{
					Script_Log << "Updated " << Temp_File->Name << " from " << Temp_File->Name_FS << " to ";
					Temp_File->Name_FS = Pack->Name + "/" + Tokens[t] + Cur_File_Name;
					Script_Log << Temp_File->Name_FS << "\n";
				}
			}
			else
			{
				C_Registered_Type* nType = this->Reg_Type_Get( Tokens[t - 1] );

				if( nType != NULL )
				//	nType = this->Reg_Type_Get( "file" )
				{
					F_Node* nFile = nType->Make_New( Cur_Dir );

					nFile->Name_FS = "packages/" + Pack->Name + ".zip/" + Tokens[t] + Cur_File_Name;
					nFile->Name = Cur_File_Name;

					Cur_Dir->Add_Child( nFile );
					Pack->Items.push_back( nFile );

					Script_Log << "Added " << nType->Name << " " << nFile->Name << " -> " << nFile->Name_FS << "\n";
				}
			}
		}
		else if( Tokens[t] == "remove" )
		{
			t += 1;
		}
		else if( Tokens[t] == "create" || Tokens[t] == "claim" )
		{
			t += 2;
		}
		else if( Tokens[t] == "add" )
		{
			t += 2;
		}
		else if( Tokens[t] == "replace" )
		{
			t += 2;
		}
		else if( Tokens[t] == "require" )
		{
		}
		else if( Tokens[t] == "mod" || Tokens[t] == "set" )
		{
			t += 3;
		}
	}


	return true;
}

//#include <fstream>
bool C_Content_Manager::Package_Load( C_Content_Package* Pack )
{
	std::ofstream Script_Log( "data/log_" + Pack->Name + "_scirpt.log", std::fstream::app );

	Script_Log << "\n\nLoading content...  ";

	for( uint i = 0;i < Pack->Items.size(); i++ )
	{
		if( Pack->Items[i]->Data_Load() == false )
		{
			//Script_Log << "Failed.";
			//return false;
		}
	}

	Script_Log << "Succeeded.";
	return true;
}


void Write_Folder( SE_Application::XML_Node* At, F_Folder* Folder );// create filesystem at node

void C_Content_Manager::VFS_Write()
{
	SE_Application::XML_File Show_File;
	SE_Application::XML_Node File_Main = Show_File.append_child("main");

	Write_Folder( &File_Main, &this->VFS );

	Show_File.save_file( "data/log_filesystem.xml" );
}

void Write_Folder( SE_Application::XML_Node* At, F_Folder* Folder )// create filesystem at node
{
	At->set_name( Folder->Name.c_str() );

	for(uint i = 0;i < Folder->Children.size(); i++)
	{
		if( Folder->Children[i]->Type_Get() ==	"folder" )
		{
			SE_Application::XML_Node Child = At->append_child( "lol" );
			Write_Folder( &Child, (F_Folder*)Folder->Children[i] );
		}
		else
			At->append_attribute( Folder->Children[i]->Name.c_str() );
	}
}






/*


C_Content::C_Content() : Virtual_Folder( NULL )
{
	this->Percent = 0;
	this->Perc_Max = 0;

	this->Log_Unfound_Files = false;
}

void C_Content::Load_Stuff()
{
	this->nContentMgr.Get_Packages();

	this->Virtual_Folder.Name = "stuff";

//	Engine.Application.Main_Log.Print_Time();
	Engine.Log("Getting Content...\n");
	std::string Data = Engine.Application.Input.Load_File_Str("stuff/content.txt");

	if(Data.length()==0)
	{
		Engine.Register_Error(S_Error("Cannot load content, no data",3));
		return;
	}

	std::vector<std::string> Package_Names;

	char Buffer[64];memset(Buffer,0,64);
	char Buf_At=0;

	for(uint l=0;l<Data.size();l++)
	{
		if( Data[l]=='\n' )
		{
			if(strlen(Buffer) > 0)
				Package_Names.push_back(std::string(Buffer));
			memset(Buffer,0,64);
			Buf_At = 0;
		}
		else if( (isalnum(Data[l]) || Data[l] == ' ' || Data[l] == '_' ) && Data[l] != 13 && Buf_At<64 )
		{
			Buffer[Buf_At]=Data[l];
			Buf_At++;
		}
		else if(Buf_At == 64)
		{
			memset(Buffer, 0, 64);
			Buf_At = 0;
		}
	}

	for(uint i = 0;i < Package_Names.size(); i++)
	{
		Engine.Log( "\t" + Package_Names[i] + "\n" );
		this->Load_Package( Package_Names[i] );
	}
	Engine.Log( "Loaded Packages\n", Log_Type::TYPE_DBUG );


	this->Mesh_Manager.Load_Mesh( "stuff/error.ply" );
	Engine.Resources.Resource_Req.Error = Engine.Resources.Load_Texture( "stuff/error.png" );
	Engine.Resources.Resource_Req.Text = Engine.Resources.Load_Texture( "stuff/fonts/text.png" );
	Engine.Resources.Resource_Req.GUI_Background = Engine.Resources.Load_Texture( "stuff/screen/gui/background.bmp" );

/*	SE_Virt_Filesys::F_Resource* Get_Prereq = (SE_Virt_Filesys::F_Resource*)this->Virtual_Folder.Get_Child( "screen/gui/screen/splash.bmp" );
	if( Get_Prereq )
	{
		Get_Prereq->Load();
		Engine.Resources.Resource_Req.GUI_Splash = Get_Prereq->ID;
	}
	*/
/*
	Engine.Resources.Loaded_Necesities = true;
	Engine.Log( "Loaded Necesities\n", Log_Type::TYPE_DBUG );

	this->Load_Files( &this->Virtual_Folder );
	Engine.Log( "Loaded Files\n", Log_Type::TYPE_DBUG );

	// assign hard-coded resources
	Engine.Resources.Resource_Req.Get_Resources();
	Engine.Log( "Got Required Resources\n", Log_Type::TYPE_DBUG );

	Engine.Resources.Stuff->Get_Resources();
	Engine.Log( "Got Resources\n", Log_Type::TYPE_DBUG );



	//std::ofstream Show_Filesystem("stuff/filesystem.txt");
	//Show_Filesystem << this->Write_Filesystem( &this->Virtual_Folder );

	E_Application::XML_File Show_File;
	E_Application::XML_Node File_Main = Show_File.append_child("main");

	this->Write_Folder( &File_Main, &this->Virtual_Folder );

	Show_File.save_file( "stuff/filesystem.xml" );


	//this->Save_XML_Mesh( &this->Items.Pistol, "stuff/items/meshes/shotgun.xml" );

	Engine.Log("Successfully Loaded Content\n");
}

void C_Content::Load_Package( std::string File )
{
	File = "stuff/content/" + File + ".zip";

	std::string Package_Stuff = Engine.Application.Input.Load_Zip_File_Str( File + "/list.txt");

	std::vector<std::string> Stuff_Names;
	std::vector<std::string> Stuff_Commands;
	std::vector<std::string> Stuff_Types;

	char Buffer[128];memset(Buffer,0,128);
	char Buf_At=0;

	u_char Stuff_At = 0;// 0 = command, 1 = type, 2 = name

	for(uint l=0;l<Package_Stuff.size();l++)
	{
		if( Package_Stuff[l]==' ' && Stuff_At == 0 )//command
		{
			Stuff_Commands.push_back( Buffer );
			memset(Buffer,0,128);
			Buf_At = 0;

			Stuff_At = 1;
		}
		else if( Package_Stuff[l]==' ' && Stuff_At == 1 )//type
		{
			Stuff_Types.push_back( Buffer );
			memset(Buffer,0,128);
			Buf_At = 0;

			Stuff_At = 2;
		}
		else if( Package_Stuff[l]=='\n' )//name
		{
			if(strlen(Buffer) > 0 && Stuff_At == 2)
				Stuff_Names.push_back(std::string(Buffer));
			memset(Buffer,0,128);
			Buf_At = 0;

			Stuff_At = 0;
		}
		else if( (isalnum(Package_Stuff[l]) || Package_Stuff[l] == ' ' || Package_Stuff[l] == '/' || Package_Stuff[l]=='.' || Package_Stuff[l]=='_') && Package_Stuff[l] != 13 && Buf_At<128 )
		{
			Buffer[Buf_At] = Package_Stuff[l];
			Buf_At++;
		}
		else if(Buf_At == 128)
		{
			memset(Buffer, 0, 128);
			Buf_At = 0;
		}
	}

	for(uint i = 0;i < Stuff_Names.size(); i++)
	{
		uint Start_Cur_File_Name = Stuff_Names[i].find_last_of('/') + 1;//find last directory
		std::string Cur_File_Name = Stuff_Names[i].substr( Start_Cur_File_Name, Stuff_Names[i].length() - Start_Cur_File_Name );
		Stuff_Names[i].erase( Start_Cur_File_Name, Stuff_Names[i].length() - Start_Cur_File_Name + 1 );


		std::istringstream Name_Stream( Stuff_Names[i] );
/*		F_Folder* Cur_Dir = &this->Virtual_Folder;
		std::string Cur_Dir_Name;

		while( std::getline( Name_Stream, Cur_Dir_Name, '/' ) )
		{
			Cur_Dir = Cur_Dir->Add_Folder( Cur_Dir_Name );
		}

		if( Cur_Dir->Get_Child( Cur_File_Name ) )// there is already a file, update it
		{
			F_File* Temp_File = (F_File*)Cur_Dir->Get_Child( Cur_File_Name );//)->File_Name = Cur_File_Name;
			if( Temp_File )
				Temp_File->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
		}
		else
		{
			this->Perc_Max++;// only add new files to max

			if( Stuff_Types[i] == "file" )
			{
				F_File* nFile = new F_File;
				Engine.Mem_Add( nFile, "file" );

				nFile->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
				nFile->Name = Cur_File_Name;

				Cur_Dir->Add_Child( nFile );
			}
			else if( Stuff_Types[i] == "texture" )
			{
				F_Resource* nRes = new F_Resource;
				Engine.Mem_Add( nRes, "texture" );

				nRes->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
				nRes->Name = Cur_File_Name;
				nRes->Res_Type = F_Resource::RES_TEXTURE;

				Cur_Dir->Add_Child( nRes );
			}
			else if( Stuff_Types[i] == "sound" )
			{
				F_Resource* nRes = new F_Resource;
				Engine.Mem_Add( nRes, "sound" );

				nRes->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
				nRes->Name = Cur_File_Name;
				nRes->Res_Type = F_Resource::RES_SOUND;

				Cur_Dir->Add_Child( nRes );
			}
			else if( Stuff_Types[i] == "text" )
			{
				F_String* nText = new F_String;
				Engine.Mem_Add( nText, "text file" );

				nText->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
				nText->Name = Cur_File_Name;

				Cur_Dir->Add_Child( nText );
			}
			else if( Stuff_Types[i] == "binary" )
			{
				F_Binary* nBinary = new F_Binary;
				Engine.Mem_Add( nBinary, "binary file" );

				nBinary->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
				nBinary->Name = Cur_File_Name;

				Cur_Dir->Add_Child( nBinary );
			}
			else if( Stuff_Types[i] == "mesh" )
			{
				F_Mesh* nMesh = new F_Mesh;
				Engine.Mem_Add( nMesh, "mesh file" );

				nMesh->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
				nMesh->Name = Cur_File_Name;

				Cur_Dir->Add_Child( nMesh );
			}
			else
			{
				for( u_char t = 0;t < this->Registered_Types.size(); t++)
				{
					if( Stuff_Types[i] == this->Registered_Types[t].Name )
					{
						F_Extra* nExt_Reg = new F_Extra;
						Engine.Mem_Add( nExt_Reg, "extra file" );

						nExt_Reg->File_Name = File + "/" + Stuff_Names[i] + Cur_File_Name;
						nExt_Reg->Name = Cur_File_Name;

						nExt_Reg->Data = this->Registered_Types[t].Make_New();
						nExt_Reg->Data_Type = &this->Registered_Types[t];

						Cur_Dir->Add_Child( nExt_Reg );
					}
				}
			}
		}
		*//*
	}
}

void C_Content::Load_Files( F_Folder* nFolder )
{
	for(uint i = 0;i < nFolder->Children.size(); i++)
	{
/*		if( nFolder->Children[i]->Get_Type() == NODE_FOLDER )
			this->Load_Files( (F_Folder*)nFolder->Children[i] );
		else
		{
			((F_File*)nFolder->Children[i])->Load();
			this->Percent++;
		}
		*//*
	}
}

std::string C_Content::Write_Filesystem( F_Folder* Folder )
{
	std::string Ret = Folder->Name;

	for(uint i = 0;i < Folder->Children.size(); i++)
	{
		Ret += "\n\t";

/*		if( Folder->Children[i]->Get_Type() ==	NODE_FOLDER )
		{
			std::string Temp_Ret = this->Write_Filesystem( (F_Folder*)Folder->Children[i] );

			replaceAll( Temp_Ret, "\t", "\t    ");
			Ret += Temp_Ret;
		}
		else
			Ret += Folder->Children[i]->Name;
			*//*
	}

	return Ret;
}

void C_Content::Write_Folder( E_Application::XML_Node* At, F_Folder* Folder )// create filesystem at node
{
	At->set_name( Folder->Name.c_str() );

	for(uint i = 0;i < Folder->Children.size(); i++)
	{
/*		if( Folder->Children[i]->Get_Type() ==	NODE_FOLDER )
		{
			E_Application::XML_Node Child = At->append_child( "lol" );
			this->Write_Folder( &Child, (F_Folder*)Folder->Children[i] );
		}
		else
			At->append_attribute( Folder->Children[i]->Name.c_str() );
			*//*
	}
}

uint C_Content::Get_Res( std::string File )
{
	F_File* nRes = (F_File*)this->Virtual_Folder.Get_Child( File );

	if( nRes )
	{
/*		switch( nRes->Get_Type() )
		{
		case NODE_RESOURCE:return ((F_Resource*)nRes)->ID;
		case NODE_MESH:return ((F_Mesh*)nRes)->Mesh_Ptr;
		}
		*//*

	}

	return 0;
}

std::string* C_Content::Get_String( std::string File )
{
/*	F_String* nText = (F_String*)this->Virtual_Folder.Get_Child( File, NODE_STRING );

	if( nText )
		return &nText->My_Text;
	else
	*//*
		return NULL;
}

E_Application::Raw_Buffer* C_Content::Get_Raw( std::string File )
{
/*	F_Binary* nRaw = (F_Binary*)this->Virtual_Folder.Get_Child( File, NODE_BINARY );

	if( nRaw )
		return &nRaw->Buffer;
	else
	*//*
		return NULL;
}
/*
F_Extra* C_Content::Get_Ext( std::string File )
{
	F_Extra* nExt = (F_Extra*)this->Virtual_Folder.Get_Child( File, NODE_EXT );

	return nExt;
}
*//*
void* C_Content::Get_Ext( std::string File, std::string Type )
{
/*	F_Extra* nExt = (F_Extra*)this->Virtual_Folder.Get_Child( File, NODE_EXT );

	if( nExt )
		if( nExt->Data_Type )
			if( nExt->Data_Type->Name == Type )
				return nExt->Data;
*//*
	return NULL;
}

/*
			   						 /*
	uint File_Name_Pos = Name.find_last_of("/");
	std::string File_Name = Name.substr( File_Name_Pos, Name.length() - File_Name_Pos );
	Name.erase( File_Name_Pos, Name.length() - File_Name_Pos );*/


	/*	uint Start_Cur_File_Name = Name.find_last_of('/') + 1;//find last directory
		std::string Cur_File_Name = Name.substr( Start_Cur_File_Name, Name.length() - Start_Cur_File_Name );
		Name.erase( Start_Cur_File_Name, Name.length() - Start_Cur_File_Name + 1 );

								  */ /*
	std::string Whole_Name = Name;

	Name += "/";// to know where it ends
	std::istringstream Name_Stream( Name );

	F_Folder* Cur_Dir = this;
	F_Node* Ret_Dir = NULL;

    while( std::getline( Name_Stream, Name, '/' ) )// for each branch
	{
		for(uint i = 0;i < Cur_Dir->Children.size(); i++)// for each child
		{
			if( Cur_Dir->Children[i]->Name == Name)// found item with same name
			{
				if( Cur_Dir->Children[i]->Get_Type() == NODE_FOLDER )// if it is a folder
				{
					Cur_Dir = (F_Folder*)Cur_Dir->Children[i];
					if( Type == NODE_FOLDER )// we want to get a folder
						Ret_Dir = Cur_Dir;
					break;
				}
				else
				{
					return Cur_Dir->Children[i];
				}
			}
		}
	}

	if( Ret_Dir == NULL )
		Engine.Application.Error_Log.Write_Log("Could not find file: " + Whole_Name);

	return Ret_Dir;			 *//*


void C_Content::Register_Class_File( void* (*Make_New)(), void (*Load)(void**,std::string), void (*Delete)(void**), std::string Name )
{
/*	SE_Virt_Filesys::C_Ext_Register nExt_Reg;

	nExt_Reg.Make_New = Make_New;
	nExt_Reg.Load = Load;
	nExt_Reg.Name = Name;
	nExt_Reg.Delete = Delete;

	this->Registered_Types.push_back( nExt_Reg );
	*//*
}
*/
#endif
