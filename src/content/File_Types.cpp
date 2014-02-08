#include "File_Types.hpp"
using namespace SE_Content_File_Types;
using namespace SE_Virt_Filesys;

#include "../engine/Engine.hpp"


uint VFS_No_Exist_UINT = 0;
std::string VFS_No_Exist_Text;


Type_File::Type_File( F_Node* Parent ) : F_Node( Parent )
{
	this->Type_Set( "file" );
}

void Type_File::Cleanup()
{

}

F_Node* Type_File::Make_New( F_Node* Parent )
{
	return Type_File::Make_New_Static( Parent );
}

F_Node* Type_File::Make_New_Static( F_Node* Parent )
{
	Type_File* nFile = new Type_File( Parent );
#ifdef _TEST
	Engine.Mem_Add( nFile, "vfs file" );
#endif
	return nFile;
}




Type_Text::Type_Text( F_Node* Parent ) : F_Node( Parent )
{
	this->Type_Set( "text" );
}

bool Type_Text::Data_Load()
{
#ifdef _TEST
	this->m_Text = Engine.Application.Input.Load_File_Str( this->Name_FS );
#endif
	return (m_Text.length() != 0);
}

void Type_Text::Cleanup()
{

}

F_Node* Type_Text::Make_New( F_Node* Parent )
{
	return Type_Text::Make_New_Static( Parent );
}

F_Node* Type_Text::Make_New_Static( F_Node* Parent )
{
	Type_Text* nText = new Type_Text( Parent );
//	Engine.Mem_Add( nText, "vfs text" );
	return nText;
}

std::string* Type_Text::Get_From_VFS( std::string Name )
{
#ifdef _TEST
	Type_Text* nRet = (Type_Text*)Engine.Content.VFS.Get_Child( Name, "text" );

	if( nRet )
		return &nRet->m_Text;

	else
		return &VFS_No_Exist_Text;
#endif

	return NULL;
}





Type_Texture::Type_Texture( F_Node* Parent ) : F_Node( Parent )
{
	this->Type_Set( "texture" );
	this->ID = 0;
}

bool Type_Texture::Data_Load()
{
#ifdef _TEST
	this->ID = Engine.Resources.Load_Texture( this->Name_FS );
#endif
	return this->ID != 0;
}

void Type_Texture::Cleanup()
{

}

F_Node* Type_Texture::Make_New( F_Node* Parent )
{
	return Type_Texture::Make_New_Static( Parent );
}

F_Node* Type_Texture::Make_New_Static( F_Node* Parent )
{
	Type_Texture* nText = new Type_Texture( Parent );
//	Engine.Mem_Add( nText, "vfs texture" );
	return nText;
}

uint* Type_Texture::Get_From_VFS( std::string Name )
{
#ifdef _TEST
	Type_Texture* nRet = (Type_Texture*)Engine.Content.VFS.Get_Child( Name, "texture" );

	if( nRet )
		return &nRet->ID;

	else
		return &VFS_No_Exist_UINT;
#endif

	return NULL;
}




Type_Sound::Type_Sound( F_Node* Parent ) : F_Node( Parent )
{
	this->Type_Set( "sound" );
	this->ID = 0;
}

bool Type_Sound::Data_Load()
{
//	this->ID = Engine.BASS.Load_Texture( this->Name_FS );

//	return this->ID != 0;
	return true;
}

void Type_Sound::Cleanup()
{

}

F_Node* Type_Sound::Make_New( F_Node* Parent )
{
	return Type_Sound::Make_New_Static( Parent );
}

F_Node* Type_Sound::Make_New_Static( F_Node* Parent )
{
	Type_Sound* nSound = new Type_Sound( Parent );
//	Engine.Mem_Add( nSound, "vfs sound" );
	return nSound;
}

uint* Type_Sound::Get_From_VFS( std::string Name )
{
#ifdef _TEST
	Type_Sound* nRet = (Type_Sound*)Engine.Content.VFS.Get_Child( Name, "sound" );

	if( nRet )
		return &nRet->ID;

	else
		return &VFS_No_Exist_UINT;
#endif

	return NULL;
}
