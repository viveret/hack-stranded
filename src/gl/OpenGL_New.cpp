#include "../gl/OpenGL.hpp"
using namespace SE_GL_Interfaces::GL_New;

#include "../utility/Functions.hpp"

//#include "../engine/hpp"
#include "../module/Module.hpp"


RTT_New::RTT_New()
{
	this->Rendering_To = 0;
	this->Render_Tex = 0;
	this->Render_FBO = 0;
	this->Render_DepthBuf = 0;
}

RTT_New::~RTT_New()
{
	this->Destroy();
}

bool RTT_New::Initialize()
{
	return true;
}

void RTT_New::Destroy()
{
	//
}

void RTT_New::Start_Render(uint nWidth, uint nHeight, uint To)//to render a image to a texture
{
	if(this->Render_FBO != 0)//already rendering
		return;

	this->Rendering_To = To;
#ifdef _TEST
	if(this->Rendering_To != 0)
		this->Render_Tex = Resources.Get_GL( this->Rendering_To );
	else
		glGenTextures(1, &this->Render_Tex);
#endif

	OpenGL_Assert( "Failed to configure render texture",4 );


	glPushMatrix();


	// generate namespace for the frame buffer, colorbuffer and depthbuffer
	glGenFramebuffersEXT(1, &this->Render_FBO);

	OpenGL_Assert( "Failed to gen fbo",4 );
	glGenRenderbuffersEXT(1, &this->Render_DepthBuf);
	OpenGL_Assert( "Failed to gen render buffer",4 );
	//switch to our fbo so we can bind stuff to it
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->Render_FBO);
	OpenGL_Assert( "Failed to bind fbo",4 );

	//create the colorbuffer texture and attach it to the frame buffer
	glBindTexture(GL_TEXTURE_2D, this->Render_Tex );
	OpenGL_Assert( "Failed to bind texture",4 );
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, nWidth, nHeight, 0, GL_RGBA, GL_INT, NULL);
	OpenGL_Assert( "Failed to tex image",4 );

	glFramebufferTexture2DEXT(GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, this->Render_Tex, 0);
	OpenGL_Assert( "Failed to attatch texture",4 );

	// create a render buffer as our depth buffer and attach it
	glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, this->Render_DepthBuf);
	OpenGL_Assert( "Failed to bind render buffer",4 );
	glRenderbufferStorageEXT(GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT24, nWidth, nHeight);
	OpenGL_Assert( "Failed to get render buffer storage",4 );
	glFramebufferRenderbufferEXT(GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, this->Render_DepthBuf);
	OpenGL_Assert( "Failed to attatch render buffer storage",4 );
	// Go back to regular frame buffer rendering
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->Render_FBO);


	std::string FBO_Status;
	GLenum Ret_Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
	switch( Ret_Status )
	{
	case GL_FRAMEBUFFER_UNDEFINED:
		{
			FBO_Status="Undefined";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
		{
			FBO_Status="Incomplete Attatchment";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
		{
			FBO_Status="Incomplete Missing Attatchment";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
		{
			FBO_Status="Incomplete Draw Buffer";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
		{
			FBO_Status="Incomplete Read Buffer";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_MULTISAMPLE:
		{
			FBO_Status="Incomplete Multisample";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_LAYER_TARGETS:
		{
			FBO_Status="Incomplete Layer Targets";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_DIMENSIONS_EXT:
		{
			FBO_Status="Incomplete Dimensions";
		}break;
	case GL_FRAMEBUFFER_INCOMPLETE_FORMATS_EXT:
		{
			FBO_Status="Incomplete Formats";
		}break;
	case GL_FRAMEBUFFER_UNSUPPORTED_EXT:
		{
			FBO_Status="Unsupported";
		}break;
	}

	if( FBO_Status.length() > 0 || Ret_Status != GL_FRAMEBUFFER_COMPLETE_EXT)
	{
		OpenGL_Assert("Frame buffer not complete, " + To_Str(Ret_Status) + ", " + FBO_Status,5);
	}



	// FBO render pass
	glViewport (0, 0, nWidth, nHeight);
	// set The Current Viewport to the fbo size glBindTexture(GL_TEXTURE_2D, 0);
	glViewport (0, 0, nWidth, nHeight);
	glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, this->Render_FBO);
	glClearColor (.0f, .0f, .0f, .0f);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity ();
	// Framebuffer render pass
	/*glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,nWidth/(float)nHeight, Application.OpenGL_Settings.Z_Near, Engine.Application.OpenGL_Settings.Z_Far );
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();*/
#ifdef _TEST
	E_Application::Switch_To_3D();
#endif
	segl_Enable(GL_DEPTH_TEST);
}

uint RTT_New::End_Render()//save render to texture and return ID
{
	if(this->Render_FBO == 0)//not trying to render
		return 0;
	//glBindFramebufferEXT(GL_FRAMEBUFFER_EXT, 0);
	//glBindRenderbufferEXT(GL_RENDERBUFFER_EXT, 0);
	OpenGL_Assert( "Error after render",4 );

	// Return to rendering to the main framebuffer instead of 'fbo'.
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );
	OpenGL_Assert( "Failed to unbind fbo",4 );

   uint Ret = 0;

	if(this->Rendering_To != 0)
	{
		Ret = this->Rendering_To;
	}
	else
	{
#ifdef _TEST
		Ret = Resources.Add_Tex(this->Render_Tex);//add to list of images
#endif
	}

	this->Rendering_To = 0;
	this->Render_Tex = 0;
	this->Render_FBO = 0;
	this->Render_DepthBuf = 0;

#ifdef _TEST
	Application.Window.Reshape(Engine.Application.Window.Wind_w, Engine.Application.Window.Wind_h);
	E_Application::Switch_To_2D();
#endif

	glPopMatrix();

	return Ret;
}



Shader_Prog::Shader_Prog()
{
	this->m_ID = 0;

	this->Prog_ID = 0;
	this->Vert_ID = 0;
	this->Frag_ID = 0;

	// S_variables
	this->Lighting_Enabled = 0;
	this->Fog_Enabled = 0;//STATES

	this->Grey_Scale_Perc = 0;

	this->Texture_State = 0;
}

void Shader_Prog::Initialize()
{
	this->Texture_State = glGetUniformLocation(this->Prog_ID, "Texture_State");

	this->Lighting_Enabled = glGetUniformLocation(this->Prog_ID,"Light_Enabled");
	this->Fog_Enabled = glGetUniformLocation(this->Prog_ID,"Fog_Enabled");

	this->Grey_Scale_Perc = glGetUniformLocation(this->Prog_ID,"Grey_Scale");
}

void Shader_Prog::Cleanup()
{
	glDetachShader( this->Prog_ID, this->Vert_ID );
	glDetachShader( this->Prog_ID, this->Frag_ID );
}


Shader_New::Shader_New()
{
	this->Cur_Shader = NULL;
}

Shader_New::~Shader_New()
{
	this->Cur_Shader = NULL;
	for( uint i = 0;i < this->Shaders.size(); i++ )
		this->Shaders[i].Cleanup();

	this->Destroy();
}

bool Shader_New::Initialize()
{
	this->Load_Shader_V( "default" );

	if( this->Shaders.size() == 0 )// did not work
		return false;

	this->Load_Shader( "mesh" );

	segl_Set1i( SEGL_SHADER_PROG, 0 );// use default shader

	return true;
}

void Shader_New::Destroy()
{
	//
}



void Shader_New::Change(uint What,uint Value)
{
	OpenGL_Assert( "Change error BEFORE: " + To_Str(What, 16) + ", " + To_Str(Value, 16), 1 );

	switch(What)
	{
	case GL_TEXTURE_2D:
		{
			glUniform1i(this->Cur_Shader->Texture_State, Value);
		}break;
	case GL_LIGHTING:
		{
			glUniform1i(this->Cur_Shader->Lighting_Enabled,Value);
		}break;
	case SEGL_SHADER_PROG:// change shader
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
	}break;
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
			glUniform1ui(this->Cur_Shader->Fog_Enabled,Value);
		}break;
	case SEGL_GREYSCALE:
		{
			this->Change(SEGL_GREYSCALE, (float)Value);
		}break;
	default:// assume enableing or disabling
		{
			if( Value == TRUE )
				glEnable( What );
			else
				glDisable( What );
		}break;
	}

	OpenGL_Assert( "Change error: " + To_Str(What, 16) + ", " + To_Str(Value, 16), 1 );
}

void Shader_New::Change(uint What,float Value)
{
	OpenGL_Assert( "Change error BEFORE: " + To_Str(What, 16) + ", " + To_Str(Value), 1 );

	switch(What)
	{
	case SEGL_GREYSCALE:
		{
			glUniform1f(this->Cur_Shader->Grey_Scale_Perc, Value );
		}break;
	}

	OpenGL_Assert( "Change error: " + To_Str(What, 16) + ", " + To_Str(Value), 1 );
}

void Shader_New::Change(uint What,Vector3f Value)
{
	switch(What)
	{
	case GL_LIGHTING:
		{
//			GLfloat lightpos[] = {Value.x, Value.y, Value.z};
			glLightfv(GL_LIGHT0, GL_POSITION, Value);
		}break;
	}
}

void Shader_New::Shade_Light( SE_Render_Material::Material_Base* Mat )
{
	if( Mat == NULL ) return;

	//glColor4fv( this->Color );
	glLightfv( GL_LIGHT0, GL_AMBIENT, Mat->Color_Ambient );
	glLightfv( GL_LIGHT0, GL_DIFFUSE, Mat->Color_Diffuse );
	glLightfv( GL_LIGHT0, GL_SPECULAR, Mat->Color_Specular );
}

void Shader_New::Cull(bool Mode)
{
	if(Mode)
	{
		segl_Enable(GL_CULL_FACE);
		glPolygonMode(GL_BACK,GL_POINT);
	}
	else
	{
		segl_Disable(GL_CULL_FACE);
		glPolygonMode(GL_BACK,GL_FILL);
	}
}


#include "../filesys/Filesystem.hpp"
uint Shader_New::Load_Shader( std::string File )
{
	File = "shaders/" + File;

	Shader_Prog nShader;
	nShader.File_Name = File;
	SE_Raw_Buffer File_Buf;
	
	OpenGL_Assert( "Pre Shader Error", 1 );


	if( glCreateShader == NULL )
	{
		sePrintf( SEPRINT_INFO, "glCreateShader was NULL\n" );
		return false;
	}

	nShader.Vert_ID = glCreateShader(GL_VERTEX_SHADER);
	nShader.Frag_ID = glCreateShader(GL_FRAGMENT_SHADER);

	if( nShader.Vert_ID == 0 )
	{
		sePrintf( SEPRINT_INFO, "Could not make Shader Vert\n" );
		return false;
	}
	else if( nShader.Frag_ID == 0 )
	{
		sePrintf( SEPRINT_INFO, "Could not make Shader Frag\n" );
		return false;
	}
	int WentOk = GL_TRUE;

	File_Buf = SE_Filesys::File_Load_Instant( File + ".vert" );
	std::string vv( File_Buf.Data() ); File_Buf.Delete();
	const GLint vvLength=vv.length();
	const GLchar* vvChar=(GLchar*)vv.c_str();


	File_Buf = SE_Filesys::File_Load_Instant( File + ".frag" );
	std::string ff( File_Buf.Data() ); File_Buf.Delete();
	const GLint ffLength=ff.length();
	const GLchar* ffChar=(GLchar*)ff.c_str();

	glShaderSource(nShader.Vert_ID, 1, &vvChar,&vvLength);
	if( OpenGL_Assert( "Shade Vert Source", 3) ) return false;

	glShaderSource(nShader.Frag_ID, 1, &ffChar,&ffLength);
	if( OpenGL_Assert( "Shade Frag Source", 3 ) ) return false;


	glCompileShader(nShader.Vert_ID);
	glGetShaderiv(nShader.Vert_ID,GL_COMPILE_STATUS,&WentOk);
	if(WentOk==GL_FALSE)
	{
		GLint Info_Len;
		glGetShaderiv(nShader.Vert_ID,GL_INFO_LOG_LENGTH,&Info_Len);//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len + 1);
//		Mem_Add( Buffer, "new shader buffer" );

		glGetShaderInfoLog( nShader.Vert_ID,Info_Len,&Info_Len,Buffer);

		sePrintf( SEPRINT_ERROR, "Vert Shader Failed To Compile:\n%s\n", Buffer );
		return false;
	}

	glCompileShader(nShader.Frag_ID);
	glGetShaderiv(nShader.Frag_ID,GL_COMPILE_STATUS,&WentOk);
	if(WentOk==GL_FALSE)
	{
		GLint Info_Len;
		glGetShaderiv(nShader.Frag_ID,GL_INFO_LOG_LENGTH,&Info_Len);//get buffer length
		char* Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len + 1);
//		Mem_Add( Buffer, "new shader buffer" );

		glGetShaderInfoLog( nShader.Frag_ID,Info_Len,&Info_Len,Buffer);

		sePrintf( SEPRINT_ERROR, "Frag Shader Failed To Compile:\n%s\n", Buffer );
		return false;
	}

	nShader.Prog_ID = glCreateProgram();

	glAttachShader(nShader.Prog_ID, nShader.Vert_ID);
	glAttachShader(nShader.Prog_ID, nShader.Frag_ID);

	glLinkProgram( nShader.Prog_ID);
	glGetProgramiv( nShader.Prog_ID,GL_LINK_STATUS,&WentOk);
	if(WentOk==GL_FALSE)
	{
		GLint Info_Len;
		glGetProgramiv(nShader.Prog_ID,GL_INFO_LOG_LENGTH,&Info_Len);//get buffer length
		char* Buffer = NULL;
		std::string Err_Rep;
		if( Info_Len != 0 )
		{
//			Buffer = new char[Info_Len+1];memset(Buffer,0,Info_Len + 1);

			glGetProgramInfoLog( nShader.Prog_ID, Info_Len, &Info_Len, Buffer );
			Err_Rep = std::string(Buffer);
			delete[] Buffer;
		}
		sePrintf( SEPRINT_ERROR, "Failed To Link Shader:\n%s\n", Buffer );
		return false;
	}


	nShader.Initialize();
	nShader.m_ID = this->Shader_New_ID();
	this->Shaders.push_back( nShader );

	return nShader.m_ID;
}

uint Shader_New::Load_Shader_V( std::string File )// version sensitive
{
	return this->Load_Shader( File = "glsl " + SE_OpenGL::Version_GLSL_Str() + "/" + File );
}

uint Shader_New::Shader_New_ID()// get new id
{
	uint nID = 0;
	for( uint i = 0;i < this->Shaders.size(); i++ )
	{
		if( this->Shaders[i].m_ID == nID )
		{
			nID++;
			i = 0;
		}
	}

	return nID;
}

void Shader_New::Get_Log( GLuint Shader )
{
	int infologLength = 0;

	int charsWritten  = 0;
	char *infoLog;

	glGetShaderiv( Shader, GL_INFO_LOG_LENGTH, &infologLength );

	if (infologLength > 0)
	{
		infoLog = (char *)malloc(infologLength);
//		glGetShaderInfoLog( infologLength, &charsWritten, infoLog );

		std::string log = infoLog;

		free(infoLog);

		sePrintf( SEPRINT_INFO, "Shader Log: %s\n", log.c_str() );
	}
}
