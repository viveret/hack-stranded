#ifndef SURVIVOR_ENGINE_CORE
#define SURVIVOR_ENGINE_CORE


#define SE_SIGNATURE 0xBABEF00D

#include "Types.hpp"

//#define SE_WD "content/" // where SE works in for the majority


#ifdef _WIN32
#pragma warning(disable:4996)//disables "unsafe" (deprecated) function warnings.

#define SE_OS "Win32"

#else

#define SE_OS "Linux"
#include <stdio.h>
#define HRESULT int

#endif

#define SE_USE_ASSERT 1

// FILESYSTEM CONSTANTS
#define SE_MODULE_LOG_DIR "data/module_logs/"

//Functionality
//#define USE_XBOX//to build with XBOX functionality
#define SE_INCLUDE_LIBS // to build with libs
//#include "gl/glu.h"
//#define USE_LUA // To build with Lua integration

#include <string>
std::string To_Str( int i, int r = 10 );
#define FILE_LINE (std::string(__FILE__) + ": " + To_Str( __LINE__ ))



#define PTR_ERROR (void*)(0x5BADF00D)


//ticks per second, 0 == no cap
#define ENGINE_TPS 60
#define DRAW_TPS 60
#define LOGIC_TPS 60//1024

//  PHYSICS

#define SEPHYS_ADD 0.01f
#define SEPHYS_DRAG .02f
#define SEPHYS_DRAG_1 0.05f

// APPLICATION
#define LOG_TIMESTAMP_DURATION 2.f // time between logs before needing another timestamp

//engine
class SE_Module;
typedef SE_Module* SE_Module_ID;

namespace SE_Virt_Filesys
{
	struct F_Node;
	struct F_Folder;
	struct F_File;
	struct F_Resource;
	struct F_Mesh;
}
namespace SE_Events
{
	struct Event_Base;
}

namespace SE_Thread
{
	struct Thread_Mgr;
	typedef Thread_Mgr* Thread_ID;
}


#endif
