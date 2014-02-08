#include "segl_Versions.hpp"
#include "OpenGL.hpp"
#include "Shader.hpp"
#include "se_gl.hpp"
using namespace SE_GL_Interfaces;
using namespace segl_Shader;

#include "../utility/Functions.hpp"
#include "../module/Module.hpp"

namespace SEGL_Ver_110
{
	void Set1f( uint Which, float Value );
	void Set2f( uint Which, float Value_1, float Value_2 );
	void Set3f( uint Which, float Value_1, float Value_2, float Value_3 );

	void Set1i( uint Which, uint Value );
	void Set2i( uint Which, uint Value_1, uint Value_2 );
	void Set3i( uint Which, uint Value_1, uint Value_2, uint Value_3 );

	void Set1p( uint Which, void* Value );
}
using namespace SEGL_Ver_110;


segl_Shader::Shader_Interface SEGL_Ver_110::Ver_Intface()
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

void SEGL_Ver_110::Set1f( uint Which, float Value )
{
	switch( Which )
	{
	case SEGL_GREYSCALE:
		{
		}break;
	}
}

void SEGL_Ver_110::Set2f( uint Which, float Value_1, float Value_2 )
{
	switch( Which )
	{

	}
}

void SEGL_Ver_110::Set3f( uint Which, float Value_1, float Value_2, float Value_3 )
{
	switch( Which )
	{
		case SEGL_CLEARCOLOR:
			{
				segl_Clear_Screen_Col.Set( Value_1, Value_2, Value_3 );
			}break;
	}
}

void SEGL_Ver_110::Set1i( uint Which, uint Value )
{
	switch( Which )
	{
	default:// assume enableing or disabling
		{
			if( Value == TRUE )
				glEnable( Which );
			else
				glDisable( Which );
		}break;
	}
}

void SEGL_Ver_110::Set2i( uint Which, uint Value_1, uint Value_2 )
{

}

void SEGL_Ver_110::Set3i( uint Which, uint Value_1, uint Value_2, uint Value_3 )
{

}

void SEGL_Ver_110::Set1p( uint Which, void* Value )
{
	switch( Which )
	{
	case SEGL_SHADER: 
		{
			Set_Cur_Shader( (segl_Shader::Shader_ID)Value );
		}break;
	}
}




uint SEGL_Ver_110::Create_Texture( uint intFormat, uint Data_Format, uint Width, uint Height, void* Data )
{
	glTexStorage2D( GL_TEXTURE_2D, 1, Data_Format, Width, Height );

	if( OpenGL_Assert( "Failed to tex storage", 2 ) )
	{
		sePrintf( SEPRINT_ERROR, "Failed\n" );
		return 1;
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, Width, Height, intFormat, GL_UNSIGNED_BYTE, Data );
	if( OpenGL_Assert( "Failed to tex sub image", 2 ) )
	{
		sePrintf( SEPRINT_ERROR, "Failed\n" );
		return 1;
	}

	return 0;
}

