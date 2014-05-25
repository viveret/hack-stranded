#ifndef SURVIVOR_ENGINE_LUA
#define SURVIVOR_ENGINE_LUA
#ifdef USE_LUA


#ifdef _WIN32
extern "C"{
#include "../../lua/lua.h"
#include "../../lua/lualib.h"
#include "../../lua/lauxlib.h"
}								  
#pragma comment (lib, "lua52.lib")
#else
extern "C"{
#include <lua5.2/lua.h>
#include <lua5.2/lualib.h>
#include <lua5.2/lauxlib.h>
}
#endif

#include <string>

namespace E_Lua
{
	//

	struct L_Lua_Handler
	{
		L_Lua_Handler();
		~L_Lua_Handler();
		//

		lua_State* My_State;

		void Initialize();
		void Destroy();

		void Do_File(std::string File);
		//void Lua_Printf(char* s);
	};								   
		int Lua_Printf(lua_State* Arg) ;
}


#endif

#endif // use_lua
