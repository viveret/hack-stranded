#include "OpenGL.hpp"
#include "../engine/Engine.hpp"
#include "../module/Module.hpp"
#include "../utility/Functions.hpp"
#include <memory.h>


namespace SE_GL_Interfaces
{
/***********   GLOBALS   **********/
	ushort m_Version_OpenGL = ushort(-1), m_Version_GLSL = ushort(-1);
	std::vector< SE_GL_Shader* > Shaders;

/***********   FUNCTIONS    *********/

	// Check if all functions were set
	void Assert_SE_OpenGL();

	// Module operations
	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
}
using namespace SE_GL_Interfaces;
namespace SE_OpenGL
{
//	SE_GL_Interfaces::S_GL_Shader* GL_Shader = NULL;
//	SE_GL_Interfaces::S_GL_RTT* GL_RTT = NULL;

	// uint int_format, uint data_format, uint width, uint height, void* data 
	segl_ID (*Create_Texture)( uint, uint, uint, uint, void* ) = NULL;
//	SE_GL_Interfaces::segl_Shader Shader;
}
using namespace SE_OpenGL;



// Return true(1) if error, false(0) if OK
bool OpenGL_Assert( std::string Comment, uint A )
{
	GLenum nError = glGetError();

	if( nError == GL_NO_ERROR )// && Level != 5)
	{
		return false;
	}
	else
	{
		const char* glu_Str = (const char*)gluErrorString(nError);

		Comment = Comment + ": " + To_Str(nError, 16);

		if( glu_Str )
			Comment += " " + std::string(glu_Str);

		//this->Register_Error(S_Error(Info, Level) );
		sePrintf( SEPRINT_ERROR, "OpenGL Assertion Failed: %s\n", Comment.c_str() );

#ifdef SE_USE_ASSERT
		return true;
#else
		return false;
#endif
	}
}


/* 
S_GL_VBO::S_GL_VBO()
{
	this->Vert_Count = 0;
	this->Vertices = NULL;
	this->Tex_Coords = NULL;

	this->VBO_Verts = 0;
	this->VBO_TexCoords = 0;

	this->Triangle_Draw = GL_TRIANGLES;
}

void S_GL_VBO::Draw( u_char Flags )
{			
	glPushMatrix();

	/*Engine.Application.OpenGL.GL_Shader->Enable( GL_VERTEX_ARRAY );
	Engine.Application.OpenGL.GL_Shader->Enable( GL_TEXTURE_COORD_ARRAY );

	if( Engine.Application.OpenGL.VBO_Supported )
	{
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_Verts );
		glVertexPointer( 3, GL_FLOAT, 0, (char *) NULL );       // Set The Vertex Pointer To The Vertex Buffer
		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_TexCoords );
		glTexCoordPointer( 2, GL_FLOAT, 0, (char *) NULL );     // Set The TexCoord Pointer To The TexCoord Buffer
	}
	else
	{
		glVertexPointer( 3, GL_FLOAT, 0, this->Vertices ); // Set The Vertex Pointer To Our Vertex Data
		glTexCoordPointer( 2, GL_FLOAT, 0, this->Tex_Coords );  // Set The Vertex Pointer To Our TexCoord Data
	}

	glDrawArrays( GL_TRIANGLES, 0, this->Vert_Count );        // Draw All Of The Triangles At Once

					
	Engine.Application.OpenGL.GL_Shader->Disable( GL_VERTEX_ARRAY );
	Engine.Application.OpenGL.GL_Shader->Disable( GL_TEXTURE_COORD_ARRAY );

						 *//* 
	if( this->VBO_Verts == 0 )// did not build the VBO
	{
		segl_Enable( GL_VERTEX_ARRAY );
		//glEnableClientState(GL_COLOR_ARRAY);

		//glColorPointer(3, GL_FLOAT, 0, (float*)&this->Vertices);
		glVertexPointer(3, GL_FLOAT, 0, (float*)this->Vertices);

		if( this->Tex_Coords )						  
		{
			segl_Enable( GL_TEXTURE_COORD_ARRAY );	 
			glTexCoordPointer( 2, GL_FLOAT, 0, (float*)this->Tex_Coords );
		}

		glDrawArrays( this->Triangle_Draw, 0, sizeof(Vector3f) / sizeof(float) * this->Vert_Count );

		//glDisableClientState(GL_COLOR_ARRAY);		   	 
		if( this->Tex_Coords )	
			segl_Disable( GL_TEXTURE_COORD_ARRAY );

		segl_Disable( GL_VERTEX_ARRAY );
	}
	else// built VBO
	{
		//
	}

	glPopMatrix();
}

void S_GL_VBO::Make( uint nVert_Count, bool Tex )
{
	if( this->Vertices == NULL )
	{
		this->Vert_Count = nVert_Count;

		this->Vertices = new Vector3f[ nVert_Count ];
#ifdef _TEST
		Engine.Mem_Add( this->Vertices, "vbo vertices" );
#endif

		if( Tex )
		{
			this->Tex_Coords = new Vector2f[ nVert_Count ];
#ifdef _TEST
			Engine.Mem_Add( this->Tex_Coords, "vbo tex coords" );
#endif
		}
	}
}

void S_GL_VBO::Delete()
{		 
	if( this->Vertices != NULL )
	{
		this->Vert_Count = 0;
#ifdef _TEST
		Engine.Mem_Remove( this->Vertices ); delete[] this->Vertices;
#endif
		this->Vertices = NULL;

		if( this->Tex_Coords != NULL )
		{
#ifdef _TEST
			Engine.Mem_Remove( this->Tex_Coords ); delete[] this->Tex_Coords;
#endif
			this->Tex_Coords = NULL;
		}
	}
}					   

/*void S_GL_VBO::Get_From( E_Objects::O_Mesh::Mesh_Container* Mesh )
{
	//
}*/
/*
void S_GL_VBO::Build_VBO()
{	
#ifdef _TEST
	if( Engine.Application.OpenGL.VBO_Supported && this->Vertices != NULL )
	{				
		/*Engine.Application.OpenGL.GL_Shader->Enable( GL_VERTEX_ARRAY );
		Engine.Application.OpenGL.GL_Shader->Enable( GL_TEXTURE_COORD_ARRAY );	

		Engine.OpenGL_Assert( "Could not gen ARB vert buffer", 1 );

		// Generate And Bind The Vertex Buffer
		glGenBuffersARB( 1, &this->VBO_Verts );                  // Get A Valid Name
		Engine.OpenGL_Assert( "Could not gen ARB vert buffer", 1 );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_Verts );         // Bind The Buffer
		Engine.OpenGL_Assert( "Could not bind ARB vert buffer", 1 );
		// Load The Data
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, this->Vert_Count*3*sizeof(float), this->Vertices, GL_STATIC_DRAW_ARB );
		Engine.OpenGL_Assert( "Could not load ARB vert buffer", 1 );
 
		// Generate And Bind The Texture Coordinate Buffer
		glGenBuffersARB( 1, &this->VBO_TexCoords );                 // Get A Valid Name
		Engine.OpenGL_Assert( "Could not gen ARB tex buffer", 1 );

		glBindBufferARB( GL_ARRAY_BUFFER_ARB, this->VBO_TexCoords );        // Bind The Buffer
		Engine.OpenGL_Assert( "Could not bind ARB tex buffer", 1 );
		// Load The Data
		glBufferDataARB( GL_ARRAY_BUFFER_ARB, this->Vert_Count*2*sizeof(float), this->Tex_Coords, GL_STATIC_DRAW_ARB );
		Engine.OpenGL_Assert( "Could not load ARB tex buffer", 1 );
 
		// Our Copy Of The Data Is No Longer Necessary, It Is Safe In The Graphics Card		   
												   
		delete[] this->Vertices; this->Vertices = NULL;
		delete[] this->Tex_Coords; this->Tex_Coords = NULL;	 

		Engine.Application.OpenGL.GL_Shader->Disable( GL_VERTEX_ARRAY );
		Engine.Application.OpenGL.GL_Shader->Disable( GL_TEXTURE_COORD_ARRAY );	 */
/*	}
#endif
}


void S_GL_Shader::Enable(uint What)
{
	this->Change(What,(uint)true);
}

void S_GL_Shader::Disable(uint What)
{
	this->Change(What,(uint)false);
}


/*********************   MODULE OPERATIONS    ********************/
void Version_Detect();
std::string GL_Get_Str( uint Enum );

#include "segl_Versions.hpp"
uint SE_GL_Interfaces::Initialize( const std::vector<std::string>& mArgs )
{
	S_Engine::Module_Set_Color( "\x1b[33m" );
 
	for( uint i = 0; i < mArgs.size(); i++ )
	{
		if( mArgs[i].find("-gl-ver=") != std::string::npos )
		{
			ssize_t Get_Num = mArgs[i].find('=');
			std::string Num_Str = mArgs[i].substr( Get_Num + 1, mArgs[i].length() - Get_Num - 1 );

			m_Version_OpenGL = atoi( Num_Str.c_str() );
			sePrintf( SEPRINT_DEBUG, "Forced GL Ver: %d\n", m_Version_OpenGL );
		}
		else if( mArgs[i].find("-glsl-ver=") != std::string::npos )
		{
			ssize_t Get_Num = mArgs[i].find('=');
			std::string Num_Str = mArgs[i].substr( Get_Num + 1, mArgs[i].length() - Get_Num - 1 );

			m_Version_GLSL = atoi( Num_Str.c_str() );
			sePrintf( SEPRINT_DEBUG, "Forced GLSL Ver: %d\n", m_Version_GLSL );
		}
	}

	if( m_Version_OpenGL == ushort(-1) ) // not found so get
		m_Version_OpenGL = atof( GL_Get_Str(GL_VERSION).c_str() ) * 100;

	if( m_Version_GLSL == ushort(-1) )
		m_Version_GLSL = atof( GL_Get_Str(GL_SHADING_LANGUAGE_VERSION).c_str() ) * 100;

	Version_Detect();

//	Engine.Application.Main_Log.Print_Time();
	sePrintf( SEPRINT_MODULE, "OpenGL Version: %s\n", GL_Get_Str(GL_VERSION).c_str() );
	sePrintf( SEPRINT_MODULE, "GLSL Version: %s\n", GL_Get_Str(GL_SHADING_LANGUAGE_VERSION).c_str() );
	sePrintf( SEPRINT_MODULE, "OpenGL Vendor: %s\n", GL_Get_Str(GL_VENDOR).c_str() );
	sePrintf( SEPRINT_MODULE, "OpenGL Renderer: %s\n", GL_Get_Str(GL_RENDERER).c_str() );
	
	GLenum glewWentOK = glewInit();

	if( glewWentOK != GLEW_OK )
	{							
		sePrintf( SEPRINT_ERROR, "Failed To Initialize GLEW" );
		return SE_FAILURE;
	}		

#ifdef _WIN32
	if( WGL_EXT_SUPPORTED( "EXT_swap_control" ) )
		wglSwapIntervalEXT( 1 ); // Enable VSync
#else
	glXSwapIntervalSGI( 1 ); // enable VSync
#endif
//	glClearColor( Engine.Menu.Splash_R, Engine.Menu.Splash_G, Engine.Menu.Splash_B, 1 );
	glClearColor( 0.5f, 0.5f, 0.5f, 1 );


	sePrintf( SEPRINT_MODULE, "GLEW Version: %s\n", (const char*)glewGetString(GLEW_VERSION) );
/*
	if( Engine.Application.OpenGL_Settings.Print_Extensions )
	{
		std::string Extensions( this->Get_Str(GL_EXTENSIONS) );
		replaceAll( Extensions, " ", "\n\t" );  

		Extensions = Extensions.substr(0, Extensions.size()-1);


		Engine.Log( "Extensions: \n\t" + Extensions, Log_Type::TYPE_DBUG );
	}
*/
/*
	std::vector<std::string> Not_Supported;
	std::vector<std::string> Is_Supported;


	if( glewIsSupported("GL_VERSION_2_0") == GL_TRUE && glewIsSupported("GL_ARB_fragment_shader") == GL_TRUE && glewIsSupported("GL_ARB_vertex_shader") == GL_TRUE )
	{
		Is_Supported.push_back("custom shaders");
//		GL_Shader = new GL_New::Shader_New;
//		Engine.Mem_Add( this->GL_Shader, "custom shaders" );
	}
	else
	{
		Not_Supported.push_back("custom shaders");
//		GL_Shader = new GL_Default::Shader_Default;
//		Engine.Mem_Add( this->GL_Shader, "default shaders" );
		
//		this->GL_Shader_Is_Supported = false;
	}


	if( glewIsSupported("GL_VERSION_1_5") == GL_TRUE && glewIsSupported("GL_ARB_framebuffer_object") == GL_TRUE && glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_UNSUPPORTED )
	{
		Is_Supported.push_back("frame buffers");
//		GL_RTT = new GL_New::RTT_New;
//		Engine.Mem_Add( this->GL_RTT, "new RTT" );
	}
	else
	{
		Not_Supported.push_back("frame buffers");
//		GL_RTT = new GL_Default::RTT_Default;
//		Engine.Mem_Add( this->GL_RTT, "default RTT" );
	}


	if( glewIsSupported("GL_VERSION_1_5") == GL_TRUE && glewIsSupported("GL_ARB_vertex_buffer_object") == GL_TRUE )
	{
		Is_Supported.push_back("vertex buffers");
//		this->VBO_Supported = true;
	}
	else
	{
		Not_Supported.push_back("vertex buffers");
//		this->VBO_Supported = false;
	}


				
	sePrintf( SEPRINT_INFO, "NOT Supported:\n" );

	for(uint i = 0;i < Not_Supported.size(); i++)									
		sePrintf( SEPRINT_INFO, "\t%s\n", Not_Supported[i].c_str() );

	sePrintf( SEPRINT_INFO, "IS Supported:\n" );

	for(uint i = 0;i < Is_Supported.size(); i++)
		sePrintf( SEPRINT_INFO, "\t%s\n", Is_Supported[i].c_str() );
*/

//	Create_Texture = SEGL_Ver_300::Create_Texture;
	// Check if all functions were set
	Assert_SE_OpenGL();

//	segl_Shader::Initialize();

 /*
 	if( GL_Shader->Initialize() == false )//failed
	{
		sePrintf( SEPRINT_ERROR, "Failed To Initialize shader Interface\n" );return false;
	}

	if( GL_RTT->Initialize() == false )//failed
	{
		sePrintf( SEPRINT_ERROR, "Failed To Initialize fbo Interface\n" );return false;
	}
	*/
	//made it to here, successfully initialized
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_BLEND);
	glEnable(GL_ALPHA_TEST);


	glShadeModel(GL_SMOOTH);
	glClearColor( 0.f, 0.f, 0.f, 1.f );
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glAlphaFunc(GL_GREATER, 0.1f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);

	return SE_SUCCESS;
}

uint SE_GL_Interfaces::Cleanup()
{
/*	if( GL_RTT )
	{								  
#ifdef _TEST
		Engine.Mem_Remove( this->GL_RTT );
#endif
		delete GL_RTT;
		GL_RTT = NULL;
	}

	if( GL_Shader )
	{
#ifdef _TEST
		Engine.Mem_Remove( this->GL_Shader );
#endif
		delete GL_Shader;
		GL_Shader = NULL;
	}*/

	return true;
}
/*
S_GL_VBO* S_GL_Interface::Get_VBO( uint Index )
{
	return &this->VBOs[Index];
}

uint S_GL_Interface::Get_VBO( S_GL_VBO* Which )
{
	for(uint i = 0;i < this->VBOs.size(); i++)
		if( &this->VBOs[i] == Which )
			return i;

	return 0;
}
*/
std::string GL_Get_Str( GLenum Name )
{
	const char* Str = (const char*)glGetString( Name );

	if( Str )
		return std::string( Str );
	else
		return std::string();
}

#include "Shader.hpp"
#include "segl_Versions.hpp"

void Version_Detect()
{
	// set GL Version to version supported
	
	if( m_Version_OpenGL >= OPENGL_VERSION_300 )
		m_Version_OpenGL = OPENGL_VERSION_300;

	else
		m_Version_OpenGL = OPENGL_VERSION_110;

	switch( m_Version_OpenGL )
	{
	case OPENGL_VERSION_300:
		{
			segl_Shader::Set_Cur_Intface( SEGL_Ver_300::Ver_Intface() );
			SE_OpenGL::Create_Texture = SEGL_Ver_300::Create_Texture;
		}break;

	default:case OPENGL_VERSION_110:
		{
			segl_Shader::Set_Cur_Intface( SEGL_Ver_110::Ver_Intface() );
			SE_OpenGL::Create_Texture = SEGL_Ver_110::Create_Texture;
		}break;
	}

	sePrintf( SEPRINT_MODULE, "Set GL Intface Ver %d\n", m_Version_OpenGL );
}

ushort SE_OpenGL::Version_OpenGL()
{
	return m_Version_OpenGL;
}

ushort SE_OpenGL::Version_GLSL()
{
	return m_Version_GLSL;
}

std::string SE_OpenGL::Version_OpenGL_Str()
{
	char Buf[12];memset( Buf, 0, 12 );
	sprintf( Buf, "%i", m_Version_OpenGL );

	for( uchar i = 0;Buf[i] != 0; i++ )
	{
		if( Buf[i] == '.' )
			Buf[i] = '_';
	}

	return std::string( Buf );
}

std::string SE_OpenGL::Version_GLSL_Str()
{
	char Buf[12];memset( Buf, 0, 12 );
	sprintf( Buf, "%i", m_Version_GLSL );

	for( uchar i = 0;Buf[i] != 0; i++ )
	{
		if( Buf[i] == '.' )
			Buf[i] = '_';
	}

	return std::string( Buf );
}



#include "../module/Module.hpp"
namespace SE_Modules{
SE_Module_ID Mod_Graphics = NULL;
} using namespace SE_Modules;

#include "../app/Window.hpp"

void SE_GL_Interfaces::Register_Module()
{
	Mod_Graphics = S_Engine::Generate_Module( "opengl" );
	S_Engine::Register_Module_Render( Mod_Graphics, S_Engine::Reg_Mod_Time::LOW_LEVEL + 2, SE_GL_Interfaces::Initialize, SE_GL_Interfaces::Cleanup );
//	S_Engine::Register_Module_Thread_Engine( Mod_Window, Check_XLib_Events );

	// For drawing properly
	S_Engine::Register_Module_Thread_Render( segl_Frame_Start, S_Engine::Module_Positions::POS_START, 254 );
	S_Engine::Register_Module_Thread_Render( segl_Frame_End, S_Engine::Module_Positions::POS_END, 254 );
}





void SE_GL_Interfaces::Assert_SE_OpenGL()
{
//	assert( GL_Shader != NULL );
//	assert( GL_RTT != NULL );

	assert( Create_Texture != NULL );
}
