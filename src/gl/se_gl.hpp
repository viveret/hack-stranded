#ifndef SURVIVOR_ENGINE_OPENGL_API
#define SURVIVOR_ENGINE_OPENGL_API

#include "../Types.hpp"
#include "../math/Vector.hpp"

// Correct OpenGL inclusion order
#include <GL/glew.h>
#include <GL/gl.h>
#ifdef _WIN32
#include <GL/wglew.h>
#else
#include <GL/glxew.h>
#include <GL/glx.h>
#endif
#include <GL/glext.h>

#include <string>

struct SE_GL_Shader;

#define segl_Enable(X)	segl_Set1i( X, true )
#define segl_Disable(X)	segl_Set1i( X, false)

#ifdef _WIN32
#define WGL_EXT_SUPPORTED(X) (wglGetProcAddress(X) != NULL )
#endif

namespace segl
{
	typedef GLuint segl_ID; // resource IDs

	// Dirty and cheap: TODO: move to VFS / content system
	segl_ID Texture_Load( std::string File );
	segl_ID segl_Gen_Tex();
	void segl_Use_Texture( segl_ID What );

	// PARAM SET
	void segl_Set1f( uint Which, float Value );
	void segl_Set2f( uint Which, float Value_1, float Value_2 );
	void segl_Set3f( uint Which, float Value_1, float Value_2, float Value_3 );

	void segl_Set1i( uint Which, uint Value );
	void segl_Set2i( uint Which, uint Value_1, uint Value_2 );
	void segl_Set3i( uint Which, uint Value_1, uint Value_2, uint Value_3 );

	void segl_Set1p( uint Which, void* Value );

	// OTHER FUNCS

	void segl_Clear_Screen();
	extern Vector3f segl_Clear_Screen_Col;

	void segl_Reshape( uint w, uint h );


	// Push all opengl settings as a profile (dpeth test, clearcolor, etc )
	void segl_Push_Profile(); 

	// Handle extra OpenGL stuff
	void segl_Frame_Start();
	void segl_Frame_End();
		
namespace segl_Params
{enum{

	SEGL_UNKNOWN = 0xF0000000,
	SEGL_SHADER_PROG, // 1i
	SEGL_CLEARCOLOR, // 3f
	SEGL_GREYSCALE, // 1f
};}


	void Register_Module();
}

using namespace segl;
using namespace segl_Params;

#define SEGL_TEXTURE		GL_TEXTURE_2D
#define SEGL_TEXTURE_NONE	0x0
#define SEGL_TEXTURE_ALL  	0x1
#define SEGL_TEXTURE_RGB  	0x2
#define SEGL_TEXTURE_ALPHA	0x3

#define SEGL_SHADER			0x0aa


#define OPENGL_VERSION_000	000
#define OPENGL_VERSION_110	110
#define OPENGL_VERSION_120	120
#define OPENGL_VERSION_121	121
#define OPENGL_VERSION_130	130
#define OPENGL_VERSION_140	140
#define OPENGL_VERSION_150	150
#define OPENGL_VERSION_200	200
#define OPENGL_VERSION_210	210
#define OPENGL_VERSION_300	300
#define OPENGL_VERSION_301	310
#define OPENGL_VERSION_320	320
#define OPENGL_VERSION_330	330
#define OPENGL_VERSION_400	400
#define OPENGL_VERSION_410	410
#define OPENGL_VERSION_420	420
#define OPENGL_VERSION_430	430

#endif
