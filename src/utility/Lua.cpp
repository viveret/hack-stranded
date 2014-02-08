#ifdef USE_LUA
#include "../utility/Lua.hpp"



using namespace E_Lua;


#include "../engine/Engine.hpp"


L_Lua_Handler::L_Lua_Handler()
{
	//
}

L_Lua_Handler::~L_Lua_Handler()
{
	this->Destroy();
}

#include <sstream>
static int l_my_print(lua_State* L) {
    int nargs = lua_gettop(L);

	std::stringstream Writer;

    for (int i=1; i <= nargs; ++i) {
		Writer << lua_tostring(L, i);
    }

	sePrintf( SEPRINT_MODULE, Writer.str() );
 
    return 0;
}

static const struct luaL_Reg printlib [] = {
  {"print", l_my_print},
  {NULL, NULL} /* end of array */
};


class TABlol
{
public:
	int lt(lua_State* L);

	int a,b;
};

void L_Lua_Handler::Initialize()
{
	this->My_State = luaL_newstate();

	luaL_openlibs( this->My_State );
	lua_getglobal( this->My_State , "_G");
	luaL_setfuncs( this->My_State , printlib, 0);


	// push the C++ function to be called from Lua			  
	lua_register(this->My_State, "printf", Lua_Printf);

	//lua_register(this->My_State, "testf", TABlol::lt);

	sePrintf( SEPRINT_MODULE, "Initialized Lua Interface\n" );

	this->Do_File("lua/test.txt");
}

void L_Lua_Handler::Destroy()
{				
	//lua_close( this->My_State );
}

void L_Lua_Handler::Do_File( std::string File )
{				
	sePrintf( SEPRINT_MODULE, "Executing Lua script: %s\n", File.c_str() );
    // run the Lua script
	luaL_dofile(this->My_State, File.c_str());  	

	sePrintf( SEPRINT_MODULE, "\n" ); // incase of bad lua printf
}


											   
int E_Lua::Lua_Printf(lua_State* Arg) 
{
	sePrintf( SEPRINT_MODULE, "Hello World!" );

	return 0;
}




int TABlol::lt(lua_State* L)
{			  
	this->a = 4;
	this->b = 2;

	sePrintf( SEPRINT_MODULE, "TABlol works" );

	return 0;
}

#endif // use_lua
