#include "segl_Versions.hpp"
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "se_gl.hpp"
using namespace SE_GL_Interfaces;
using namespace segl_Shader;

#include "../utility/Functions.hpp"
#include "../module/Module.hpp"

namespace SEGL_Ver_300
{
	void Set1f( uint Which, float Value );
	void Set2f( uint Which, float Value_1, float Value_2 );
	void Set3f( uint Which, float Value_1, float Value_2, float Value_3 );

	void Set1i( uint Which, uint Value );
	void Set2i( uint Which, uint Value_1, uint Value_2 );
	void Set3i( uint Which, uint Value_1, uint Value_2, uint Value_3 );

	void Set1p( uint Which, void* Value );
}
using namespace SEGL_Ver_300;


segl_Shader::Shader_Interface SEGL_Ver_300::Ver_Intface()
{
	segl_Shader::Shader_Interface m_Int;
	
	m_Int.Set1f = Set1f;
	m_Int.Set2f = Set2f;
	m_Int.Set3f = Set3f;

	m_Int.Set1i = Set1i;
	m_Int.Set2i = Set2i;
	m_Int.Set3i = Set3i;

	m_Int.Set1p = Set1p;

	return m_Int;
}

void SEGL_Ver_300::Set1f( uint Which, float Value )
{
	switch( Which )
	{
	case SEGL_GREYSCALE:
		{
			glUniform1f( Get_Cur_Shader()->Grey_Scale_Perc, Value );
		}break;
	}
}

void SEGL_Ver_300::Set2f( uint Which, float Value_1, float Value_2 )
{
	switch( Which )
	{

	}
}

void SEGL_Ver_300::Set3f( uint Which, float Value_1, float Value_2, float Value_3 )
{
	switch( Which )
	{
		case SEGL_CLEARCOLOR:
			{
				segl_Clear_Screen_Col.Set( Value_1, Value_2, Value_3 );
			}break;
	}
}

void SEGL_Ver_300::Set1i( uint Which, uint Value )
{
	switch( Which )
	{
		case SEGL_TEXTURE:
		{
			glUniform1i( Get_Cur_Shader()->Texture_State, Value );
			if( Value == 0 )
				glDisable( GL_TEXTURE_2D );
			else
				glEnable( GL_TEXTURE_2D );
		}break;

	case GL_LIGHTING:
		{
			glUniform1i( Get_Cur_Shader()->Lighting_Enabled, Value );
		}break;

/* 	case SEGL_SHADER_PROG:// change shader
	{
		if( this->Cur_Shader )
		{
			if( this->Cur_Shader->m_ID == Value )// same shader as current
				break;
			else
				this->Cur_Shader = NULL;// to test if we found the right shader
		}

		for( uint i = 0;i < this->Shaders.size(); i++ )
		{
			if( this->Shaders[i].m_ID == Value )
			{
				this->Cur_Shader = &this->Shaders[i];
				break;
			}
		}

		if( this->Cur_Shader == NULL )// did not find shader, resort to default
		{
			this->Change( SEGL_SHADER_PROG, 0u );
			return;
			break;
		}

		glUseProgram( this->Cur_Shader->Prog_ID );
	}break;*/
	case GL_VERTEX_ARRAY:
		{
			if( Value == TRUE )
				glEnableClientState( GL_VERTEX_ARRAY );
			else
			{
				glVertexPointer(2, GL_FLOAT, 0, NULL);
				glDisableClientState( GL_VERTEX_ARRAY );
			}
		}break;

	case GL_TEXTURE_COORD_ARRAY:
		{
			if( Value == TRUE )
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			else
			{
				glTexCoordPointer( 1, GL_FLOAT, 0, NULL );
				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
			}
		}break;

	case GL_FOG:
		{
			glUniform1ui( Get_Cur_Shader()->Fog_Enabled, Value );
		}break;

	case SEGL_GREYSCALE:
		{
			segl_Set1f( SEGL_GREYSCALE, (float)Value );
		}break;

	default:// assume enableing or disabling
		{
			if( Value == TRUE )
				glEnable( Which );
			else
				glDisable( Which );
		}break;
	}
}

void SEGL_Ver_300::Set2i( uint Which, uint Value_1, uint Value_2 )
{

}

void SEGL_Ver_300::Set3i( uint Which, uint Value_1, uint Value_2, uint Value_3 )
{

}

void SEGL_Ver_300::Set1p( uint Which, void* Value )
{
	switch( Which )
	{
	case SEGL_SHADER: 
		{
			Set_Cur_Shader( (segl_Shader::Shader_ID)Value );
		}break;
	}
}


/* 
uchar Test_Image[4][4][3] = {
	{{255,255,255},{255,255,255},{255,255,255},{255,255,255}},
	{{255,255,255},{255,255,255},{255,255,255},{255,255,255}},
	{{255,255,255},{255,255,255},{255,255,255},{255,255,255}},
	{{255,255,255},{255,255,255},{255,255,255},{255,255,255}}
};
*/

uint SEGL_Ver_300::Create_Texture( uint intFormat, uint Data_Format, uint Width, uint Height, void* Data )
{
/*	intFormat = GL_RGB;
	Data_Format = GL_RGB8;
	Data = Test_Image;
	Width = 4;
	Height = 4;*/
	glTexStorage2D( GL_TEXTURE_2D, 1, Data_Format, Width, Height );

	if( OpenGL_Assert( "Failed to tex storage", 2 ) )
	{
		sePrintf( SEPRINT_ERROR, "Args: intf: 0x%x, df: 0x%x, w: %d, h: %d\n", intFormat, Data_Format, Width, Height );
		return 1;
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, intFormat, GL_UNSIGNED_BYTE, Data );
	if( OpenGL_Assert( "Failed to tex sub image", 2 ) )
	{
//		sePrintf( SEPRINT_ERROR, "Failed\n" );// Log_Type::TYPE_EROR);
		return 1;
	}

	return 0;
}
