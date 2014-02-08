#ifndef SURVIVOR_ENGINE_ENGINE
#define SURVIVOR_ENGINE_ENGINE


#include "Core.hpp"


#ifdef SE_INCLUDE_LIBS


#ifdef _WIN32
#pragma comment (lib, "winmm.lib")
#pragma comment (lib, "comctl32.lib")
#pragma comment (lib, "Ws2_32.lib")

#pragma comment (lib, "glew32.lib")
#pragma comment (lib, "opengl32.lib")
#pragma comment (lib, "glu32.lib")

#pragma comment (lib, "DevIL.lib")
#pragma comment (lib, "ILU.lib")
#pragma comment (lib, "ILUT.lib")
//#pragma comment (lib, "SOIL.lib")
//#pragma comment (lib, "libboost_system-mt.lib")
//#pragma comment (lib, "libboost_thread-mt.lib")

#pragma comment (lib, "bass.lib")

//winmm.lib;comctl32.lib;glew32.lib;GLFW.lib;GLFWDLL.lib;opengl32.lib;glu32.lib;SOIL.lib;;Ws2_32.lib;bass.lib;BulletMultiThreaded.lib;BulletDynamics.lib;BulletCollision.lib;LinearMath.lib;
//  -lboost_system -lboost_thread -lIL -lILU -lILUT -lassimp -lbass -lnoise -llua5.2 -lz -lzip

#pragma comment (lib, "libnoise.lib")

#ifdef _DEBUG
#pragma comment (lib, "BulletMultiThreaded_Debug.lib")
#pragma comment (lib, "BulletDynamics_Debug.lib")
#pragma comment (lib, "BulletCollision_Debug.lib")
#pragma comment (lib, "LinearMath_Debug.lib")

#pragma comment (lib, "freetype2412MT_D.lib")
//#pragma comment (lib, "ftgl_static_D.lib")


#else		 
#pragma comment (lib, "BulletMultiThreaded.lib")
#pragma comment (lib, "BulletDynamics.lib")
#pragma comment (lib, "BulletCollision.lib")
#pragma comment (lib, "LinearMath.lib")
											 
#pragma comment (lib, "freetype2412.lib")
//#pragma comment (lib, "ftgl_static.lib")
#endif	

///NODEFAULTLIB:MSVCRTD /LTCG /NODEFAULTLIB:LIBCMT 
#endif

#endif//win32


#include "engine/Engine.hpp"
#include "app/App.hpp"
#include "utility/Draw_Utility.hpp"

#endif