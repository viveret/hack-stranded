#include "VFS.hpp"
using namespace SE_Virt_Filesys;

#include "../engine/Engine.hpp"


F_Node::F_Node( F_Node* Parent )
{
	this->m_Is_Pointer = -1;

	// any / all
	this->Perm_Set = -1;
	this->Perm_Get = -1;
}
/*
VFS_Type_Register* F_Node::Type_Get()
{
	return this->m_Type;
}

void F_Node::Type_Set( VFS_Type_Register* nType )
{
	if( this->m_Type == NULL )
		this->m_Type = nType;
}
*/
std::string F_Node::Type_Get()
{
	return this->m_Type;
}

void F_Node::Type_Set( std::string nType )
{
	if( this->m_Type.length() == 0 )
		this->m_Type = nType;
}

F_Node* F_Node::Parent_Get()
{
	return NULL;
}

void F_Node::Parent_Set( F_Node* nParent )
{
//	if( this->m_Parent == NULL )
//		this->m_Parent = nPArent;
}

/*F_Node_Reference* F_Node::Ref_Add()// std::string Name, F_Node* Parent
{
	return NULL;
}

void F_Node::Ref_Remove( F_Node_Reference* Ref )
{
	for( uint i = 0;i < this->References.size(); i++ )
	{
		if( this->References[i] == Ref )
		{
			Engine.Mem_Remove( this->References[i] );
			delete this->References[i];

			this->References.erase( this->References.begin() + i );
		}
	}
}

uchar F_Node::Ref_Count_Get()
{
	return this->References.size();
}*/

void* F_Node::Data_Get()
{
	return NULL;
}

bool F_Node::Data_Load()
{
	return true;
}












F_Folder::F_Folder( F_Node* Parent ) : F_Node( Parent )
{
	this->Type_Set( "folder" );
}

void F_Folder::Cleanup()
{
	for( uint i = 0; i < this->Children.size(); i++ )
	{
		this->Children[i]->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( this->Children[i] );
#endif
		delete this->Children[i];
	}

	this->Children.clear();
}

F_Node* F_Folder::Make_New( F_Node* Parent )
{
	return F_Folder::Make_New_Static( Parent );
}

F_Node* F_Folder::Make_New_Static( F_Node* Parent )
{
	F_Folder* nFolder = new F_Folder( Parent );
#ifdef _TEST
	Engine.Mem_Add( nFolder, "folder" );
#endif
	return nFolder;
}

void F_Folder::Add_Child( F_Node* nChild )
{
	this->Children.push_back( nChild );
}

F_Folder* F_Folder::Add_Folder( std::string Name )
{
	F_Folder* nFolder = (F_Folder*)this->Get_Child( Name, true );
	if( nFolder == NULL )// no folder with that name
	{
		nFolder = (F_Folder*)this->Make_New(this);
		nFolder->Name = Name;
		this->Children.push_back( nFolder );

		return nFolder;
	}
	else
		return nFolder;
}

void F_Folder::Remove_Child( std::string Name )
{
	for( uint i = 0; i < this->Children.size(); i++ )
	{
		if( this->Children[i]->Name == Name )
		{
			this->Children[i]->Cleanup();
#ifdef _TEST
			Engine.Mem_Remove( this->Children[i] );
#endif

			delete this->Children[i];
			this->Children.erase( this->Children.begin() + i );

			break;
		}
	}
}

#include <sstream>
F_Node* F_Folder::Get_Child( std::string Name, std::string Type, bool Probing )
{
	if( Name.length() == 0 )
		return NULL;

	//if( Probing == false )
	//Engine.Log( "Starting Search: " + Name + "\n", Log_Type::TYPE_DBUG );

	std::string Whole_Name = Name;

	F_Folder* Cur_Dir = this;
	F_Node* Ret_Dir = NULL;
	std::string Cur_Var_Name;

	ssize_t Start_Cur_Var_Name = Name.find_last_of('/') + 1;//find last var
	if( Start_Cur_Var_Name != std::string::npos )
	{
		Cur_Var_Name = Name.substr( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name );
		Name.erase( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name + 1 );
	}
	else
		Cur_Var_Name = Name;


	if( Name.find( '/' ) != Name.npos )// var exists and is a hierarchy
	{
		std::istringstream Name_Stream( Name );
		std::getline( Name_Stream, Name, '/' );// get starting branch
		Cur_Dir = (F_Folder*)Cur_Dir->Get_Child( Name, "folder" );

		if( Cur_Dir == NULL )
			goto Unfound;

		// first get through directories
		while( std::getline( Name_Stream, Name, '/' ) )// for each branch
		{
			Cur_Dir = (F_Folder*)Cur_Dir->Get_Child( Name, "folder" );
			if( Cur_Dir == NULL )// invalid var name
				goto Unfound;
		}
	}

	// founnd directory name
	for( ushort i = 0;i < Cur_Dir->Children.size(); i++ )
	{
		if( Cur_Dir->Children[i]->Name == Cur_Var_Name )
		{
			Ret_Dir	= Cur_Dir->Children[i];// if it has a child with the same name
			break;
		}
	}

	if( Ret_Dir != NULL && ( Ret_Dir->Type_Get() == Type || Type.length() == 0 ) )
		return Ret_Dir;

	Unfound:				 // CANNOT FIND INDIV SINGLE LAST FILE
#ifdef _TEST
	if( Probing == false )
		Engine.Log( "Could not find file: " + Whole_Name + "\n", Log_Type::TYPE_EROR );
#endif

	return NULL;
}

F_Node* F_Folder::Get_Child( std::string Name, bool Probing )
{
	return this->Get_Child( Name, "", Probing );
}









/*C_Ext_Register::C_Ext_Register()
{
	this->Make_New = NULL;
	this->Load = NULL;
}



void F_Folder::Clear()
{
	for(uint i = 0;i < this->Children.size(); i++)
	{
		this->Children[i]->Clear();
		Engine.Mem_Remove( this->Children[i] );
		delete this->Children[i];
		this->Children[i] = NULL;
	}

	this->Children.clear();
}

u_char F_Folder::Get_Type()
{
	return NODE_FOLDER;
}

void F_Folder::Add_Child( F_Node* nChild )
{
	this->Children.push_back( nChild );
}

F_Folder* F_Folder::Add_Folder( std::string Name )
{
	F_Folder* nFolder = (F_Folder*)this->Get_Child( Name );
	if( nFolder == NULL )// no folder with that name
	{
		nFolder = new F_Folder;
		Engine.Mem_Add( nFolder, "folder" );
		nFolder->Name = Name;
		this->Children.push_back( nFolder );

		return nFolder;
	}
	else
		return nFolder;
}

void F_Folder::Remove_Child( std::string Name )
{
	for(uint i = 0;i < this->Children.size(); i++)
	{
		if( this->Children[i]->Name == Name )
		{
			this->Children[i]->Clear();
			Engine.Mem_Remove( this->Children[i] );
			delete this->Children[i];
			this->Children.erase( this->Children.begin() + i );

			break;
		}
	}
}
#include <sstream>
/*/		/*ORIGINAL

F_Node* F_Folder::Get_Child( std::string Name, uint Type )
{
	Engine.Log( "Starting Search: " + Name + "\n", Log_Type::TYPE_DBUG );
	std::string Whole_Name = Name;

	uint Start_Cur_File_Name = Name.find_last_of('/') + 1;//find last directory
	std::string Cur_File_Name = Name.substr( Start_Cur_File_Name, Name.length() - Start_Cur_File_Name );
	Name.erase( Start_Cur_File_Name, Name.length() - Start_Cur_File_Name + 1 );


	std::istringstream Name_Stream( Name );

	F_Folder* Cur_Dir = this;
	F_Node* Ret_Dir = NULL;

	// first get through directories
    while( std::getline( Name_Stream, Name, '/' ) )// for each branch
	{
		for(uint i = 0;i < Cur_Dir->Children.size(); i++)// for each child
		{
			if( Cur_Dir->Children[i]->Name == Name)// found item with same name
			{
				if( Cur_Dir->Children[i]->Get_Type() == NODE_FOLDER )// if it is a folder
				{
					Cur_Dir = (F_Folder*)Cur_Dir->Children[i];
				}
			}
		}
	}

	//now find file
	for( uint i = 0;i < Cur_Dir->Children.size(); i++)
	{
		if( Cur_Dir->Children[i]->Name == Cur_File_Name )// if it has a child with the same name
			return Cur_Dir->Children[i];
	}

	if( Whole_Name.length() > 0 )
		Engine.Log( "Could not find file: " + Whole_Name + "\n", Log_Type::TYPE_EROR );

	return Ret_Dir;
} */

/*

F_Node* F_Folder::Get_Child( std::string Name, uint Type, bool Probing ) // probing == not using, do not log
{
	if( Name.length() == 0 )
		return NULL;

	//if( Probing == false )
	//Engine.Log( "Starting Search: " + Name + "\n", Log_Type::TYPE_DBUG );

	std::string Whole_Name = Name;

	F_Folder* Cur_Dir = this;
	F_Node* Ret_Dir = NULL;
	std::string Cur_Var_Name;

	uint Start_Cur_Var_Name = Name.find_last_of('/') + 1;//find last var
	if( Start_Cur_Var_Name != std::string::npos )
	{
		Cur_Var_Name = Name.substr( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name );
		Name.erase( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name + 1 );
	}
	else
		Cur_Var_Name = Name;


	if( Name.find( '/' ) != Name.npos )// var exists and is a hierarchy
	{
		std::istringstream Name_Stream( Name );
		std::getline( Name_Stream, Name, '/' );// get starting branch
		Cur_Dir = (F_Folder*)Cur_Dir->Get_Child( Name, NODE_FOLDER );

		if( Cur_Dir == NULL )
			goto Unfound;

		// first get through directories
		while( std::getline( Name_Stream, Name, '/' ) )// for each branch
		{
			Cur_Dir = (F_Folder*)Cur_Dir->Get_Child( Name, NODE_FOLDER );
			if( Cur_Dir == NULL )// invalid var name
				goto Unfound;
		}
	}

	// founnd directory name
	for( ushort i = 0;i < Cur_Dir->Children.size(); i++ )
	{
		if( Cur_Dir->Children[i]->Name == Cur_Var_Name )
		{
			Ret_Dir	= Cur_Dir->Children[i];// if it has a child with the same name
			break;
		}
	}

	if( Ret_Dir != NULL && ( Ret_Dir->Get_Type() == Type || Type == (uint)(-1) ) )
		return Ret_Dir;

Unfound:				 // CANNOT FIND INDIV SINGLE LAST FILE
	if( Probing )
		Engine.Log( "Could not find file: " + Whole_Name + "\n", Log_Type::TYPE_EROR );

	return NULL;
}

void F_File::Clear()
{
	//
}

u_char F_File::Get_Type()
{
	return NODE_FILE;
}

void F_File::Load()
{
	//
}

F_Resource::F_Resource()
{
	this->ID = 0;
	this->Res_Type = 0;
}

void F_Resource::Clear()
{
	this->ID = 0;
}

u_char F_Resource::Get_Type()
{
	return NODE_RESOURCE;
}

void F_Resource::Load()
{
	if( this->ID == 0 )
	{
		switch( this->Res_Type )
		{
		case F_Resource::RES_TEXTURE:
			this->ID = Engine.Resources.Load_Texture( this->File_Name );
			break;
		case F_Resource::RES_SOUND:
			this->ID = Engine.Bass_Manager.Load_Sample( this->File_Name );
			break;
		case F_Resource::RES_MUSIC:
			//this->ID = Engine.Bass_Manager.Load_Music( this->File_Name );
			break;
		}
	}
}


void F_String::Clear()
{
	this->My_Text.clear();
}

u_char F_String::Get_Type()
{
	return NODE_STRING;
}

void F_String::Load()
{
	if( this->My_Text.length() == 0 )
		this->My_Text = Engine.Application.Input.Load_File_Str( this->File_Name );
}


void F_Binary::Clear()
{
	this->Buffer.Delete();
}

u_char F_Binary::Get_Type()
{
	return NODE_BINARY;
}

void F_Binary::Load()
{
	if( this->Buffer.Get_Data() == NULL )// no data
		this->Buffer = Engine.Application.Input.Load_File( this->File_Name );
}

F_Mesh::F_Mesh()
{
	this->Mesh_Ptr = 0;
}

uchar F_Mesh::Get_Type()
{
	return NODE_MESH;
}

void F_Mesh::Load()
{
	if( this->Mesh_Ptr == 0 )
		this->Mesh_Ptr = Engine.Content.Mesh_Manager.Load_Mesh( this->File_Name );
}

void F_Mesh::Clear()
{
	this->Mesh_Ptr = 0;
}

F_Extra::F_Extra()
{
	this->Data = NULL;
	this->Data_Type = NULL;
}

void F_Extra::Clear()
{
	this->Data_Type->Delete( &this->Data );
	this->Data = NULL;
	/*
	if( this->Data )
	{
		Engine.Mem_Remove( this->Data );
		delete this->Data;
		this->Data = NULL;
	}

	this->Data_Type = NULL;					*/ /*
}

u_char F_Extra::Get_Type()
{
	return NODE_EXT;
}

void F_Extra::Load()
{
	if( this->Data_Type )
		this->Data_Type->Load( &this->Data, this->File_Name );
}
*/
