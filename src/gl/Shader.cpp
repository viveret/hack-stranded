#include "Shader.hpp"
#include "../debug/Debug.hpp"
#include "../module/Module.hpp"
#include "../utility/Array.hpp"
#include "OpenGL.hpp"
#include "../filesys/Filesystem.hpp"

namespace segl_Shader
{
/***************   GLOBALS   ***********************/
	std::vector< Shader_ID > Shader_List;
	bool Get_Uniforms( Shader_ID This );

	Shader_Data Shader_Default;
	Shader_ID Shader_Current = &Shader_Default;


	Shader_Interface g_Shader_Intface;

	namespace Shader_Values
	{
	}
}
using namespace segl_Shader;


Shader_Data::Shader_Data()
{
	this->Prog_ID = 0;
	this->Vert_ID = 0;
	this->Frag_ID = 0;

	this->Lighting_Enabled = 0;
	this->Fog_Enabled = 0;
	this->Grey_Scale_Perc = 0;
	this->Texture_State = 0;
}


bool segl_Shader::Get_Uniforms( Shader_ID This )
{
	This->Texture_State = glGetUniformLocation( This->Prog_ID, "Texture_State");
	OpenGL_Assert( FILE_LINE, 1 );
	glUniform1i( This->Texture_State, SEGL_TEXTURE_ALL );
	OpenGL_Assert( FILE_LINE, 1 );

	This->Lighting_Enabled = glGetUniformLocation( This->Prog_ID,"Light_Enabled");
	OpenGL_Assert( FILE_LINE, 1 );
	glUniform1i( This->Lighting_Enabled, false );
	OpenGL_Assert( FILE_LINE, 1 );

	This->Fog_Enabled = glGetUniformLocation( This->Prog_ID,"Fog_Enabled");
	OpenGL_Assert( FILE_LINE, 1 );
	glUniform1i( This->Fog_Enabled, false );
	OpenGL_Assert( FILE_LINE, 1 );

	This->Grey_Scale_Perc = glGetUniformLocation( This->Prog_ID,"Grey_Scale");
	OpenGL_Assert( FILE_LINE, 1 );
	glUniform1f( This->Grey_Scale_Perc, 100.f );
	OpenGL_Assert( FILE_LINE, 1 );

//	sePrintf( SEPRINT_DEBUG, "Shader unifs: tex_state: %d, light: %d, fog: %d, grey: %d\n",
//			This->Texture_State, This->Lighting_Enabled, This->Fog_Enabled, This->Grey_Scale_Perc );

	return true;
}

Shader_ID segl_Shader::Load( std::string File, bool Ver )
{
	if( SE_OpenGL::Version_OpenGL() < OPENGL_VERSION_130 )
		return NULL;

	if( Ver ) // Use version-ified name
		File = "glsl " + SE_OpenGL::Version_GLSL_Str() + "/" + File;

	Shader_ID Ret = new Shader_Data;
	SE_Debug::Watch( Ret, "shader", sizeof( Shader_Data ) );
	Shader_List.push_back( Ret );


	File = "shaders/" + File;

	Ret->File_Name = File;
	SE_Raw_Buffer File_Buf;
	
	OpenGL_Assert( "Pre Shader Error", 1 );


	if( glCreateShader == NULL )
	{
		sePrintf( SEPRINT_INFO, "glCreateShader was NULL\n" );
		return NULL;
	}

	Ret->Vert_ID = glCreateShader(GL_VERTEX_SHADER);
	Ret->Frag_ID = glCreateShader(GL_FRAGMENT_SHADER);

	if( Ret->Vert_ID == 0 )
	{
		sePrintf( SEPRINT_INFO, "Could not make Shader Vert\n" );
		return NULL;
	}
	else if( Ret->Frag_ID == 0 )
	{
		sePrintf( SEPRINT_INFO, "Could not make Shader Frag\n" );
		return NULL;
	}
	int WentOk = GL_TRUE;

	File_Buf = SE_Filesys::File_Load_Instant( File + ".vert" );
	if( File_Buf.Data() == NULL )
		return NULL;
	std::string vv( File_Buf.Data() ); File_Buf.Delete();
	const GLint vvLength = vv.length();
	const GLchar* vvChar = (GLchar*)vv.c_str();


	File_Buf = SE_Filesys::File_Load_Instant( File + ".frag" );
	if( File_Buf.Data() == NULL )
		return NULL;
	std::string ff( File_Buf.Data() ); File_Buf.Delete();
	const GLint ffLength = ff.length();
	const GLchar* ffChar = (GLchar*)ff.c_str();

	glShaderSource( Ret->Vert_ID, 1, &vvChar,&vvLength);
	if( OpenGL_Assert( "Shade Vert Source", 3) ) return NULL;

	glShaderSource( Ret->Frag_ID, 1, &ffChar,&ffLength);
	if( OpenGL_Assert( "Shade Frag Source", 3 ) ) return NULL;


	glCompileShader( Ret->Vert_ID );
	glGetShaderiv( Ret->Vert_ID, GL_COMPILE_STATUS, &WentOk );
	if(WentOk==GL_FALSE)
	{
		GLint Info_Len;
		glGetShaderiv( Ret->Vert_ID, GL_INFO_LOG_LENGTH, &Info_Len );//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len + 1);
//		Mem_Add( Buffer, "new shader buffer" );

		glGetShaderInfoLog( Ret->Vert_ID,Info_Len, &Info_Len, Buffer );

		sePrintf( SEPRINT_ERROR, "Vert Shader Failed To Compile:\n%s\n", Buffer );
		return NULL;
	}

	glCompileShader( Ret->Frag_ID );
	glGetShaderiv( Ret->Frag_ID, GL_COMPILE_STATUS, &WentOk );
	if(WentOk==GL_FALSE)
	{
		GLint Info_Len;
		glGetShaderiv( Ret->Frag_ID, GL_INFO_LOG_LENGTH, &Info_Len );//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len + 1);
//		Mem_Add( Buffer, "new shader buffer" );

		glGetShaderInfoLog( Ret->Frag_ID, Info_Len, &Info_Len,Buffer );

		sePrintf( SEPRINT_ERROR, "Frag Shader Failed To Compile:\n%s\n", Buffer );
		return NULL;
	}

	Ret->Prog_ID = glCreateProgram();

	glAttachShader( Ret->Prog_ID, Ret->Vert_ID);
	glAttachShader( Ret->Prog_ID, Ret->Frag_ID);

	glLinkProgram( Ret->Prog_ID );
	glGetProgramiv( Ret->Prog_ID, GL_LINK_STATUS, &WentOk );
	if(WentOk==GL_FALSE)
	{
		GLint Info_Len;
		glGetProgramiv( Ret->Prog_ID, GL_INFO_LOG_LENGTH, &Info_Len );//get buffer length
		char* Buffer = NULL;
		std::string Err_Rep;
		if( Info_Len != 0 )
		{
//			Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len + 1);

			glGetProgramInfoLog( Ret->Prog_ID, Info_Len, &Info_Len, Buffer );
			Err_Rep = std::string(Buffer);
			delete[] Buffer;
		}
		sePrintf( SEPRINT_ERROR, "Failed To Link Shader:\n%s\n", Buffer );
		return NULL;
	}
	glUseProgram( Ret->Prog_ID );
	OpenGL_Assert( "glUseProg", 3 );

//	nShader.m_ID = this->Shader_New_ID();
//	this->Shaders.push_back( nShader );

//	return nShader.m_ID;

	if( Get_Uniforms( Ret ) != true )
		;//return NULL;

	sePrintf( SEPRINT_DEBUG, "Successfully loaded shader %s\n", File.c_str() );

	// Check list
	if( Shader_List.size() == 1 )
		Shader_Default = *Ret;

	Set_Cur_Shader( Ret );

	return Ret;
}

uint segl_Shader::Free( Shader_ID Which )
{
	assert( Which != NULL );

	glDetachShader( Which->Prog_ID, Which->Vert_ID );
	glDetachShader( Which->Prog_ID, Which->Frag_ID );

	SE_Debug::Unwatch( Which );
	delete Which;

	return true;
}

Shader_ID segl_Shader::Get_Cur_Shader()
{
	return Shader_Current;
}

void segl_Shader::Set_Cur_Shader( Shader_ID What )
{
	if( What == NULL )
		Shader_Current = &Shader_Default;
	else
		Shader_Current = What;

	if( glUseProgram )
	{
		glUseProgram( Shader_Current->Prog_ID );
		OpenGL_Assert( "glUseProg", 3 );
	}
}


/*******************   SHADER INTERFACE   ***********************/

Shader_Interface::Shader_Interface()
{
	this->Set1f = NULL; this->Set2f = NULL; this->Set3f = NULL;
	this->Set1i = NULL; this->Set2i = NULL; this->Set3i = NULL;

	this->Set1p = NULL;
}

Shader_Interface* segl_Shader::Get_Cur_Intface() // Just return current
{
	return NULL;
}

void segl_Shader::Set_Cur_Intface( const Shader_Interface& Intf ) // can only be used once
{
	if( g_Shader_Intface.Set1i == NULL ) // Has not been set yet
	{
		// Check if valid
		assert( Intf.Set1f != NULL );
		assert( Intf.Set2f != NULL );
		assert( Intf.Set3f != NULL );

		assert( Intf.Set1i != NULL );
		assert( Intf.Set2i != NULL );
		assert( Intf.Set3i != NULL );
		
		assert( Intf.Set1p != NULL );

		// Passed
		g_Shader_Intface = Intf;
	}
	else
		assert( false );
}


// SEGL Set Param Interface
void segl::segl_Set1f( uint Which, float Value )
{
//	OpenGL_Assert( "glSet1f BEFORE: " + To_Str(Which, 16) + ", " + To_Str(Value, 16), 1 );
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set1f( Which, Value );

	OpenGL_Assert( FILE_LINE, 1 );
//	OpenGL_Assert( "glSet1f AFTER: " + To_Str(What, 16) + ", " + To_Str(Value, 16), 1 );
}

void segl::segl_Set2f( uint Which, float Value_1, float Value_2 )
{
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set2f( Which, Value_1, Value_2 );

	OpenGL_Assert( FILE_LINE, 1 );
}

void segl::segl_Set3f( uint Which, float Value_1, float Value_2, float Value_3 )
{
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set3f( Which, Value_1, Value_2, Value_3 );

	OpenGL_Assert( FILE_LINE, 1 );
}

void segl::segl_Set1i( uint Which, uint Value )
{
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set1i( Which, Value );

	if( OpenGL_Assert( FILE_LINE, 1 ) )
		sePrintf( SEPRINT_ERROR, "\tWhich: %x, Value: %d\n", Which, Value );
}

void segl::segl_Set2i( uint Which, uint Value_1, uint Value_2 )
{
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set2i( Which, Value_1, Value_2 );

	OpenGL_Assert( FILE_LINE, 1 );
}

void segl::segl_Set3i( uint Which, uint Value_1, uint Value_2, uint Value_3 )
{
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set3i( Which, Value_1, Value_2, Value_3 );

	OpenGL_Assert( FILE_LINE, 1 );
}

void segl::segl_Set1p( uint Which, void* Value )
{
	OpenGL_Assert( FILE_LINE, 1 );

	g_Shader_Intface.Set1p( Which, Value );

	OpenGL_Assert( FILE_LINE, 1 );
}
