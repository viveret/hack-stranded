#include "Stranded_Core.hpp"
#include <SE/Logger/Logger.hpp>

namespace Stranded_Lib
{
	int Wrap_Gen_Planet( lua_State* L );

	int Wrap_Solarsys_Load( lua_State* L );
	int Wrap_Solarsys_Next( lua_State* L );
	int Wrap_Solarsys_Radius( lua_State* L );
	int Wrap_Solarsys_Clear( lua_State* L );
	int Wrap_Conv_Load( lua_State* L );

	int Wrap_Gen_Trigger( lua_State* L );
	int Wrap_Get_Player( lua_State* L );

	luaL_Reg Stranded_Lualib [] = {
		{ "gen_planet", Wrap_Gen_Planet },
		{ "load_solarsys", Wrap_Solarsys_Load },
		{ "next_solarsys", Wrap_Solarsys_Next },
		{ "solarsys_radius", Wrap_Solarsys_Radius },
		{ "clear_solarsys", Wrap_Solarsys_Clear },
		{ "load_conv", Wrap_Conv_Load },
		{ "gen_trigger", Wrap_Gen_Trigger },
		{ "get_player", Wrap_Get_Player },
		{ NULL, NULL }
	};

	extern 	SE_Log_Interface* seLog;
	extern SE_Physics_Interface* SE_Physics;

	extern SE_Physics_Lib::Body_ID Player_Obj;
	extern std::string Solar_System_Next, Solar_System_Current;
	extern float Solar_System_Radius_Sqrd;
//	extern m_Dialogue_Man Dialogue;
	extern std::vector< Trigger_Spot > Triggers;
}
using namespace Stranded_Lib;

/* *****************	LUA INTERFACE	********************* */
int Stranded_Lib::Wrap_Gen_Planet( lua_State* L )
{
	Grav_Node* This = new Grav_Node;
	int lPos = 0;
	std::string Err;

	if( lua_islightuserdata(L, -1) != 0 )
	{
		lPos = -1;
		This->Parent = (Grav_Node*)lua_topointer(L, -1);
	}

	if( lua_isstring(L, lPos-7) )
		This->Name = lua_tostring(L, lPos-7);
	else
	{
		Err = "name";
		goto lError;
	}
	
	if( lua_isnumber(L, lPos-6) )
		This->Tex = lua_tonumber(L, lPos-6);
	else
	{
		Err = "tex";
		goto lError;
	}

	if( lua_isnumber(L, lPos-5) )
		This->Orbit_Origin.x() = lua_tonumber(L, lPos-5);
	else
	{
		Err = "x";
		goto lError;
	}

	if( lua_isnumber(L, lPos-4) )
		This->Orbit_Origin.y() = lua_tonumber(L, lPos-4);
	else
	{
		Err = "y";
		goto lError;
	}

	if( lua_isnumber(L, lPos-3) )
		This->Radius = lua_tonumber(L, lPos-3);
	else
	{
		Err = "r";
		goto lError;
	}

	if( lua_isnumber(L, lPos-2) )
		This->Orbit_Speed = lua_tonumber(L, lPos-2);
	else
	{
		Err = "speed";
		goto lError;
	}

	if( lua_isnumber(L, lPos-1) )
		This->Mass = lua_tonumber(L, lPos-1);
	else
	{
		Err = "mass";
		goto lError;
	}


	Register_Gravnode( This );
	
	lua_pushlightuserdata(L, (void*)This );
	return 1;

lError:

<<<<<<< HEAD
	seLog->Printf( "Stranded-Lua", SELOG_NOTICE, "$(w)at %s - gen_planet( string, type, x, y, r, speed, mass, [Parent] )\n", Err.c_str() );
=======
	seLog->Printf( "Stranded-Lua", SELOG_ALERT, "$(w)at %s - gen_planet( string, type, x, y, r, speed, mass, [Parent] )\n", Err.c_str() );
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1
	delete This;
	return 0;
}

int Stranded_Lib::Wrap_Solarsys_Load( lua_State* L )
{
	return 0;
}

int Stranded_Lib::Wrap_Solarsys_Next( lua_State* L )
{
	if( lua_isstring(L, -1) )
		Solar_System_Next = lua_tostring(L, -1);
	else
<<<<<<< HEAD
		seLog->Printf( "Stranded-Lua", SELOG_NOTICE, "$(w)next_solarsys( string )\n" );
=======
		seLog->Printf( "Stranded-Lua", SELOG_ALERT, "$(w)next_solarsys( string )\n" );
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1

	return 0;
}

int Stranded_Lib::Wrap_Solarsys_Radius( lua_State* L )
{
	if( lua_isnumber(L, -1) )
	{
		Solar_System_Radius_Sqrd = lua_tonumber(L, -1);
		Solar_System_Radius_Sqrd *= Solar_System_Radius_Sqrd;
	}
	else
<<<<<<< HEAD
		seLog->Printf( "Stranded-Lua", SELOG_NOTICE, "$(w)solarsys_radius( number )\n" );
=======
		seLog->Printf( "Stranded-Lua", SELOG_ALERT, "$(w)solarsys_radius( number )\n" );
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1

	return 0;

}

int Stranded_Lib::Wrap_Solarsys_Clear( lua_State* L )
{
	Clear_Solar_System(); // Just start fresh
	return 0;
}

int Stranded_Lib::Wrap_Conv_Load( lua_State* L )
{
	if( lua_isstring(L, -1) )
		;//Dialogue.Load( std::string("conv/") + lua_tostring(L, -1) + ".txt" );
	else
<<<<<<< HEAD
		seLog->Printf( "Stranded-Lua", SELOG_NOTICE, "$(w)load_conv( string )\n" );
=======
		seLog->Printf( "Stranded-Lua", SELOG_ALERT, "$(w)load_conv( string )\n" );
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1

	return 0;
}

int Stranded_Lib::Wrap_Gen_Trigger( lua_State* L )
{
	Trigger_Spot nTrig;

	if( lua_isstring(L, -1) )
		nTrig.Conv = std::string("conv/") + lua_tostring(L, -1) + ".txt";
	else
		goto lError;

	if( lua_isnumber(L, -4) )
		nTrig.Pos.x() = lua_tonumber(L, -4);
	else
		goto lError;

	if( lua_isnumber(L, -3) )
		nTrig.Pos.y() = lua_tonumber(L, -3);
	else
		goto lError;

	if( lua_isnumber(L, -2) )
	{
		nTrig.Radius_Sqrd = lua_tonumber(L, -2);
		nTrig.Radius_Sqrd *= nTrig.Radius_Sqrd;
	}
	else
		goto lError;

	Triggers.push_back( nTrig );

	return 0;

lError:

<<<<<<< HEAD
	seLog->Printf( "Stranded-Lua", SELOG_NOTICE, "gen_trigger( x, y, r, string )\n" );
=======
	seLog->Printf( "Stranded-Lua", SELOG_ALERT, "gen_trigger( x, y, r, string )\n" );
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1
	return 0;
}

int Stranded_Lib::Wrap_Get_Player( lua_State* L )
{
	lua_pushlightuserdata(L, (void*)Player_Obj );
	return 1;
}


