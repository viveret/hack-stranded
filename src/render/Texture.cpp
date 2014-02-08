#include "Texture.hpp"
#include "../filesys/Filesystem.hpp"
#include "../gl/OpenGL.hpp"
#include "../module/Module.hpp"

namespace SE_Texture
{
	struct Tex_Stats
	{
		Tex_Stats()
		{
			this->Img_ID = 0;
			this->m_File_ID = NULL;
		}

		segl_ID Img_ID;
		SE_Filesys::File_ID m_File_ID;
	};
/*******************    GLOBALS    **************************/
	std::vector< Tex_ID > Active_Textures;
	Tex_Stats NULL_Tex;

	SE_Module_ID Mod_Texture = NULL;

/* *****************    TEXTURE OPERATIONS    ************** */
	bool Tex_Exists( Tex_ID Which );

	void Read_Img_Part( SE_Raw_Buffer Chunk, void* This );

/* *****************    MODULE OPERATIONS    ************** */
	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
}
using namespace SE_Texture;



/*****************    TEXTURE OPERATIONS    **************************/
bool SE_Texture::Tex_Exists( Tex_ID Which )
{
	for( uint i = 0; i < Active_Textures.size(); i++ )
	{
		if( Active_Textures[i] == Which )
			return true;
	}

	return false;
}

Tex_ID SE_Texture::Retrieve_Texture( const std::string& VFS_Name )
{
	return 0;
}

void SE_Texture::Use_Texture( Tex_ID Which )
{
	if( Tex_Exists( Which ) )
		segl_Set1i( GL_TEXTURE_2D, Which->Img_ID );

	else
		segl_Set1i( GL_TEXTURE_2D, NULL_Tex.Img_ID );
}

void Read_Str_Stream( SE_Raw_Buffer Data, void* This );
void Read_Str_Stream_Done( void* This );

std::string Test_Str;
uint SE_Texture::Initialize( const std::vector<std::string>& mArgs )
{
//	SE_Filesys::File_ID Str_File = SE_Filesys::Open_File_Read( "data/note.txt", &Test_Str, Read_Str_Stream, Read_Str_Stream_Done );

	return SE_SUCCESS;
}

uint SE_Texture::Cleanup()
{
	return SE_SUCCESS;
}

void SE_Texture::Register_Module()
{
	Mod_Texture = S_Engine::Generate_Module( "texture cache" );
	S_Engine::Register_Module_Render( Mod_Texture, S_Engine::Reg_Mod_Time::LOW_LEVEL_NEXT + 10, Initialize, Cleanup );
}

void Read_Str_Stream( SE_Raw_Buffer Data, void* This )
{
	((std::string*)This)->append( Data.Data() );
}

void Read_Str_Stream_Done( void* This )
{
	sePrintf( SEPRINT_DEBUG, "Streamed data: %s\n", ((std::string*)This)->c_str() );
}
