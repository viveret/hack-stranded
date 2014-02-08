#include "Module.hpp"
#include "Thread.hpp"
#include "Events.hpp"
#include "Event_Defs.hpp"
#include "../filesys/Filesystem.hpp"

#include <fstream>
#include <vector>

namespace S_Engine
{
	// Keep track of thread specific data
	struct Thread_Stat: public SE_Events::Event_Manager_Base
	{
		bool Handle_Event( SE_Events::Event_Base* Event );

		SE_Thread::Thread_ID m_ID;
		SE_Module_ID Mod_Cur;

		Thread_Stat()
		{
			this->m_ID = NULL;
			this->Mod_Cur = NULL;		
		}
	};
	
	struct Reg_Func_Thread
	{
		SE_Module_ID m_Mod;
		void(*m_Func)();

		uchar Pos_Priority, m_Pos;

		Reg_Func_Thread()
		{
			this->m_Mod = NULL;
			this->m_Func = NULL;

			this->Pos_Priority = 0;
			this->m_Pos = Module_Positions::POS_LATE;
		}
	};
	typedef std::vector< Reg_Func_Thread > Reg_Func_Thread_List;

	struct Reg_Module_Thread
	{
		SE_Module_ID m_Mod;
		uchar Init_Time;

		uint (*Func_Init)( const std::vector<std::string>& );
		uint (*Func_Clean)();
	};
	typedef std::vector< Reg_Module_Thread > Reg_Mod_Thread_List;
/*	struct Reg_Func_Event
	{
		SE_Module_ID m_Mod;
		void(*m_Func)( void* );

		Reg_Func()
		{
			this->m_Mod = NULL;
			this->m_Func = NULL;
		}
	};
	typedef std::vector< Reg_Func_Event > Reg_Func_Event_List;
	*/


/************   GLOBALS   *************/

	static std::vector< SE_Module_ID > Modules;
	static Reg_Func_Thread_List Func_Thread_Engine, Func_Thread_Update, Func_Thread_Render;
	static Reg_Mod_Thread_List Mod_Thread_Engine, Mod_Thread_Update, Mod_Thread_Render;

	std::vector< std::string > Main_Args;
	namespace SE_Settings
	{
		bool Log_Color_Enabled = true;
	}

	bool Is_Running = true;

	SE_Filesys::File_ID Log_Engine = NULL;

//	std::map< , Thread_State > Thread_Events;
	Thread_Stat Thread_Engine, Thread_Update, Thread_Render;

	namespace Version_Desc
	{
		std::string OS, OS_Version;
		std::string Build_Name;
	}

/************   FUNCTIONS   *************/
	uint Initialize();
	void Init_Update();
	void Init_Render();

	uint Shutdown(); // start cleanup process
	uint Cleanup();
	void Clean_Update();
	void Clean_Render();

	bool Main_Event_Engine( SE_Events::Event_Base* nEv );
	bool Main_Event_Update( SE_Events::Event_Base* nEv );
	bool Main_Event_Render( SE_Events::Event_Base* nEv );

	void Terminate_Callback(); // called from stdc

	void Set_Current_Module( SE_Module_ID nMod );
	SE_Module_ID Get_Current_Module();

	// Thread functions
	void Module_Call_Engine( void* Arg = NULL );
	void Module_Call_Update( void* Arg = NULL );
	void Module_Call_Render( void* Arg = NULL );
}
using namespace S_Engine;


/************    MODULE CLASS    **********************/

class SE_Module
{
public:
	SE_Module();

	std::string Name;

	SE_Filesys::File_ID m_Log;
	const char* m_Log_Color;

	// flags for listening events
	uint Event_Listen_Flags;

	bool (*Engine_Event)( SE_Events::Event_Base* );
	bool (*Update_Event)( SE_Events::Event_Base* );
	bool (*Render_Event)( SE_Events::Event_Base* );
};


/************   MODULE OPERATIONS    ******************/

// Recieve Module ID from registering key components
SE_Module_ID S_Engine::Generate_Module( std::string Name )
{
	SE_Module_ID Ret = new SE_Module();
	Ret->Name = Name;

	Modules.push_back( Ret );

	return Ret;
}

void S_Engine::Register_Module_Engine( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() )
{
	Reg_Module_Thread nReg;

	nReg.m_Mod = m_Mod;
	nReg.Init_Time = Init_Time;
	nReg.Func_Init = Func_Init;
	nReg.Func_Clean = Func_Clean;

	uint Insert_Pos = 0;
	while( Insert_Pos < Mod_Thread_Engine.size() )
	{
		if( Mod_Thread_Engine[Insert_Pos].Init_Time > Init_Time )
			break;
		else
			Insert_Pos++;
	}

	Mod_Thread_Engine.insert( Mod_Thread_Engine.begin() + Insert_Pos, nReg );
}

void S_Engine::Register_Module_Update( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() )
{
	Reg_Module_Thread nReg;

	nReg.m_Mod = m_Mod;
	nReg.Init_Time = Init_Time;
	nReg.Func_Init = Func_Init;
	nReg.Func_Clean = Func_Clean;
	
	uint Insert_Pos = 0;
	while( Insert_Pos < Mod_Thread_Update.size() )
	{
		if( Mod_Thread_Update[Insert_Pos].Init_Time > Init_Time )
			break;
		else
			Insert_Pos++;
	}

	Mod_Thread_Update.insert( Mod_Thread_Update.begin() + Insert_Pos, nReg );
}

void S_Engine::Register_Module_Render( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() )
{
	Reg_Module_Thread nReg;

	nReg.m_Mod = m_Mod;
	nReg.Init_Time = Init_Time;
	nReg.Func_Init = Func_Init;
	nReg.Func_Clean = Func_Clean;
	
	uint Insert_Pos = 0;
	while( Insert_Pos < Mod_Thread_Render.size() )
	{
		if( Mod_Thread_Render[Insert_Pos].Init_Time > Init_Time )
			break;
		else
			Insert_Pos++;
	}

	Mod_Thread_Render.insert( Mod_Thread_Render.begin() + Insert_Pos, nReg );
}


void S_Engine::Set_Current_Module( SE_Module_ID nMod )
{
	SE_Thread::Thread_ID Cur_Thread = SE_Thread::Get_Current();
	if( Cur_Thread == Thread_Update.m_ID )
		Thread_Update.Mod_Cur = nMod;

	else if( Cur_Thread == Thread_Engine.m_ID )
		Thread_Engine.Mod_Cur = nMod;

	else if( Cur_Thread == Thread_Render.m_ID )
		Thread_Render.Mod_Cur = nMod;
}

SE_Module_ID S_Engine::Get_Current_Module()
{
	SE_Thread::Thread_ID Cur_Thread = SE_Thread::Get_Current();
	if( Cur_Thread == Thread_Update.m_ID )
		return Thread_Update.Mod_Cur;

	else if( Cur_Thread == Thread_Engine.m_ID )
		return Thread_Engine.Mod_Cur;

	else if( Cur_Thread == Thread_Render.m_ID )
		return Thread_Render.Mod_Cur;

	return NULL;
}


void S_Engine::Module_Set_Color( const char* Col )
{
	SE_Module_ID nMod = Get_Current_Module();
	if( nMod )
		nMod->m_Log_Color = Col;
	else
		sePrintf( SEPRINT_WARN, "Could not set color, curmod: %p\n", nMod );

}

/**************    THREADS     ***************************/

uint Register_Module_Thread( Reg_Func_Thread_List* List, void (*Func)(), uchar Pos, uchar Priority )
{
	assert( Func != NULL );

	Reg_Func_Thread nFunc;
	nFunc.m_Func = Func;
	nFunc.m_Pos = Pos;
	nFunc.Pos_Priority = Priority;

	uint Ins_Pos = 0;
	while( Ins_Pos < List->size() && (*List)[ Ins_Pos ].m_Pos <= Pos )
	{
		// If the position is in range, check priority
		if( (*List)[ Ins_Pos ].m_Pos == Pos && (*List)[ Ins_Pos ].Pos_Priority > Priority )
			break;

		Ins_Pos++;
	}

	List->insert( List->begin() + Ins_Pos, nFunc );
//	sePrintf( "Could not register thread func %p, %s\n", Func_Engine, Where.c_str() );	return true;

	return true;
}
// Get called inside of Logic thread
uint S_Engine::Register_Module_Thread_Engine( void (*Func_Engine)(), uchar Pos, uchar Priority )
{
	return Register_Module_Thread( &Func_Thread_Engine, Func_Engine, Pos, Priority );
}

// Get called inside of Logic thread
uint S_Engine::Register_Module_Thread_Logic( void (*Func_Update)(), uchar Pos, uchar Priority )
{
	return Register_Module_Thread( &Func_Thread_Update, Func_Update, Pos, Priority );
}

// Get called inside of Render thread
uint S_Engine::Register_Module_Thread_Render( void (*Func_Render)(), uchar Pos, uchar Priority )
{
	return Register_Module_Thread( &Func_Thread_Render, Func_Render, Pos, Priority );
}


/**************    EVENTS     ***************************/

// Get called inside of Engine thread
void S_Engine::Register_Module_Event_Engine( SE_Module_ID Mod_ID, bool (*Func_Engine)( SE_Events::Event_Base* ) )
{
	assert( Mod_ID != NULL );
	Mod_ID->Engine_Event = Func_Engine;
}

// Get called inside of Logic thread
void S_Engine::Register_Module_Event_Logic( SE_Module_ID Mod_ID, bool (*Func_Update)( SE_Events::Event_Base* ) )
{
	assert( Mod_ID != NULL );
	Mod_ID->Update_Event = Func_Update;
}

// Get called inside of Render thread
void S_Engine::Register_Module_Event_Render( SE_Module_ID Mod_ID, bool (*Func_Render)( SE_Events::Event_Base* ) )
{
	assert( Mod_ID != NULL );
	Mod_ID->Render_Event = Func_Render;
}

/************   THREAD EVENT OPERATION ****************/

bool Thread_Stat::Handle_Event( SE_Events::Event_Base* Event )
{
	uint Num_Dont_Delete = 0;
	SE_Thread::Thread_ID Cur_Thread = SE_Thread::Get_Current();

	if( Cur_Thread == Thread_Update.m_ID )
	{
		for( uint m = 0; m < Modules.size(); m++ )
		{	
			if( Modules[m]->Update_Event )
			{
				Set_Current_Module( Modules[m] );
				Num_Dont_Delete += !Modules[m]->Update_Event( Event );
			}
		}
		Set_Current_Module( NULL );
		Main_Event_Update( Event );
	}
	else if( Cur_Thread == Thread_Engine.m_ID )
	{
		for( uint m = 0; m < Modules.size(); m++ )
		{
			if( Modules[m]->Engine_Event )
			{
				Set_Current_Module( Modules[m] );
				Num_Dont_Delete += !Modules[m]->Engine_Event( Event );
			}
		}
		Set_Current_Module( NULL );
		Main_Event_Engine( Event );
	}
	else if( Cur_Thread == Thread_Render.m_ID )
	{
		for( uint m = 0; m < Modules.size(); m++ )
		{
			if( Modules[m]->Render_Event )
			{
				Set_Current_Module( Modules[m] );
				Num_Dont_Delete += !Modules[m]->Render_Event( Event );
			}
		}
		Set_Current_Module( NULL );
		Main_Event_Render( Event );
	}
	
	return  (Num_Dont_Delete == 0); // delete if no one wants it
}

SE_Thread::Thread_ID seThread_Engine()
{
	return Thread_Engine.m_ID;
}

SE_Thread::Thread_ID seThread_Update()
{
	return Thread_Update.m_ID;
}

SE_Thread::Thread_ID seThread_Render()
{
	return Thread_Render.m_ID;
}

/************   ENGINE OPERATIONS    ******************/

#include <stdarg.h>
#define PRINTF_BUF_SIZE 512

const char* sePrintf_Cols[ SEPRINT_MODULE ] =
{
	"\x1b[0m", // Regular
	"\x1b[4;31m", // Yellow
	"\x1b[4;31m", // Red
	"\x1b[4;31m", // Blink red
	"\x1b[4;32m", // Green
	"\x1b[4;34m" // Light blue
};

void sePrintf( uchar pType, const char* format,  ... )
{
	static char Printf_Buf[ PRINTF_BUF_SIZE ];
	memset( Printf_Buf, 0, PRINTF_BUF_SIZE );

	va_list args;
	va_start( args, format );
	vsnprintf( Printf_Buf, PRINTF_BUF_SIZE, format, args );
	va_end( args );

#ifdef _WIN32
	OutputDebugString( Printf_Buf );
#endif

	SE_Module_ID nMod = Get_Current_Module();

	const char* pFront = "", *pBack = "";

	if( SE_Settings::Log_Color_Enabled )
	{
		pBack =	sePrintf_Cols[0];
		if( pType < SEPRINT_MODULE )
			pFront = sePrintf_Cols[pType];

		else if( nMod && nMod->m_Log_Color )
			pFront = nMod->m_Log_Color;
	}

	uint Printf_Buf_Len = strlen(Printf_Buf);
	// a \n with color colors the rest of the line,
	// so make sure to not have color when printing \n
	uint Sep_Pos = 0;
	for( uint l = 0;l < Printf_Buf_Len; l++ )
	{
		if( Printf_Buf[l] == '\n' )
		{
			Printf_Buf[l] = 0; // remove \n
			printf( "%s%s%s\n", pFront, Printf_Buf + Sep_Pos, pBack );

			Sep_Pos = l + 1;
			Printf_Buf[l] = '\n'; // restore
		}
		else if( l == Printf_Buf_Len - 1 )
		{
			printf( "%s%s%s", pFront, Printf_Buf + Sep_Pos, pBack );
			Sep_Pos = l + 1;
		}
	}

	// Now write to log
	if( nMod != NULL )
		SE_Filesys::Write_To( Printf_Buf, strlen(Printf_Buf), nMod->m_Log );
	
	else
		SE_Filesys::Write_To( Printf_Buf, strlen(Printf_Buf), Log_Engine );

}


// Check to see if modules were set
bool Module_Check_Defaults();

// Start the Engine
uint S_Engine_Main( uint Argc, char** Argv )
{
	for( uint i = 0; i < Argc; i++ )
		Main_Args.push_back( Argv[i] );

	std::set_terminate( Terminate_Callback );
	std::set_unexpected( Terminate_Callback );

	S_Engine::Module_Hook();
	S_Engine::Initialize();

	while( Is_Running )
		Module_Call_Engine();

	S_Engine::Cleanup();

	return 0;
}

// Thread functions
void S_Engine::Module_Call_Engine( void* Arg )
{
	// Update main-threaded modules
	for( uint m = 0; m < Func_Thread_Engine.size(); m++ )
	{
		Set_Current_Module( Func_Thread_Engine[m].m_Mod );
		Func_Thread_Engine[m].m_Func();
	}
	Set_Current_Module( NULL );

	// Check events
	Thread_Engine.Update();
}

void S_Engine::Module_Call_Update( void* Arg )
{
	// Update update-threaded modules
	for( uint m = 0; m < Func_Thread_Update.size(); m++ )
	{
		Set_Current_Module( Func_Thread_Update[m].m_Mod );
		if( Func_Thread_Update[m].m_Func )
			Func_Thread_Update[m].m_Func();
	}
	Set_Current_Module( NULL );

	// Check events
	Thread_Update.Update();
}

void S_Engine::Module_Call_Render( void* Arg )
{
	// Update render-threaded modules
	for( uint m = 0; m < Func_Thread_Render.size(); m++ )
	{
		Set_Current_Module( Func_Thread_Render[m].m_Mod );
		Func_Thread_Render[m].m_Func();
	}
	Set_Current_Module( NULL );

	// Check events
	Thread_Render.Update();
}

bool S_Engine::Main_Event_Engine( SE_Events::Event_Base* nEv )
{
	if( nEv->m_Type == NULL )//SE_Events::Types::TYPE_ENGINE:
	{
		switch( nEv->Code )
		{
		case Event_Module::EVENT_SHUTDOWN:
			{
				S_Engine::Shutdown();
			}break;
		}
	}

	return true;
}

bool S_Engine::Main_Event_Update( SE_Events::Event_Base* nEv )
{
	return true;
}

bool S_Engine::Main_Event_Render( SE_Events::Event_Base* nEv )
{
	return true;
}

void S_Engine::Event_Add( SE_Events::Event_Base* nEv )
{
	Thread_Render.Add_Event( nEv->Clone() );
	Thread_Update.Add_Event( nEv->Clone() );
	Thread_Engine.Add_Event( nEv );
}

void S_Engine::Event_Add_Engine( SE_Events::Event_Base* nEv )
{
	Thread_Engine.Add_Event( nEv );
}

void S_Engine::Event_Add_Update( SE_Events::Event_Base* nEv )
{
	Thread_Update.Add_Event( nEv );
}

void S_Engine::Event_Add_Render( SE_Events::Event_Base* nEv )
{
	Thread_Render.Add_Event( nEv );
}

#ifndef _WIN32
#include <sys/utsname.h>
#endif

#include "../utility/Timer.hpp"

uint S_Engine::Initialize()
{
	for( uint i = 0; i < Main_Args.size(); i++ )
	{
		if( Main_Args[i] == "--no-log-color" )
			SE_Settings::Log_Color_Enabled = false;
		
		else if( Main_Args[i] == "--log-color" )
			SE_Settings::Log_Color_Enabled = true;
	}	

	// Add engine functions where needed
	Thread_Engine.m_ID = SE_Thread::Get_Current();

	// Remove old logs
#ifdef _WIN32
	system( "del " SE_MODULE_LOG_DIR "*.log" );
#else
	system( "rm " SE_MODULE_LOG_DIR "*.log" );
#endif

	// Initialize main log
	Log_Engine = SE_Filesys::Open_For_Write_Stream( SE_MODULE_LOG_DIR "engine.log" );

	sePrintf( SEPRINT_INFO, "%s (%s) Pre-Alpha, %s : %s, " __DATE__ ", " __TIME__ "\n", Version_Desc::Build_Name.c_str(), SE_OS, Version_Desc::OS.c_str(), Version_Desc::OS_Version.c_str() );
	sePrintf( SEPRINT_INFO, "Initializing Survivor Engine, %d Module(s)...\n", Modules.size() );

	Timer Startup_Timer;
	Startup_Timer.Start();


	// Get system info
#ifdef _WIN32

	OSVERSIONINFO OS_Info;

    ZeroMemory( &OS_Info, sizeof(OSVERSIONINFO) );
    OS_Info.dwOSVersionInfoSize = sizeof(OSVERSIONINFO);
    GetVersionEx( &OS_Info );

	SYSTEM_INFO Sys_Info;
	GetSystemInfo( &Sys_Info );

	FILE * pFile = fopen( SE_MODULE_LOG_DIR "system.log", "w" );
	
	fprintf( pFile, "OS Version: %d.%d\nOS Build: %d\nPlatofrm: %d\nService Pack: %s\n\n", //Service Pack Version: %d:%d\n",
		OS_Info.dwMajorVersion, OS_Info.dwMinorVersion, OS_Info.dwBuildNumber, OS_Info.dwPlatformId,
		OS_Info.szCSDVersion );//, OS_Info.wServicePackMajor, OS_Info.wServicePackMinor );

	fprintf( pFile, "OEM ID: %d\nProcessor Architecture: %d\nPage Size: %d\nProccessors: %d\nProccessor Type: %d\n\n",
		Sys_Info.dwOemId, Sys_Info.wProcessorArchitecture, Sys_Info.dwPageSize, Sys_Info.dwNumberOfProcessors, Sys_Info.dwProcessorType );
	
	fclose( pFile );

#else
	utsname Get_OS_Buf;
	uname( &Get_OS_Buf );
/*
	this->m_Version.OS.append( Get_OS_Buf.version );
	this->m_Version.OS_Version.append( Get_OS_Buf.release );
*/
	system( "cat /proc/version > " SE_MODULE_LOG_DIR "system.log" );
	system( "cat /proc/vmstat >> " SE_MODULE_LOG_DIR "system.log" );
	system( "cat /proc/stat >> " SE_MODULE_LOG_DIR "system.log" );
	system( "cat /proc/misc >> " SE_MODULE_LOG_DIR "system.log" );
	system( "cat /proc/loadavg >> " SE_MODULE_LOG_DIR "system.log" );
	system( "cat /proc/meminfo >> " SE_MODULE_LOG_DIR "system.log" );
	system( "lscpu >> " SE_MODULE_LOG_DIR "system.log" );
	system( "cat /proc/cpuinfo >> " SE_MODULE_LOG_DIR "system.log" );
#endif



/**********   INITIALIZE MODULES    *****************/
	// Make sure all modules are registered
	Module_Check_Defaults();

	// Organize based on dependencies
/*	for( uint m = 0; m < Modules.size(); m++ )
	{
		uint n = 0;
		while( Modules[m]->Init_Time > Modules[n]->Init_Time && n < Modules.size() )
			n++;

		SE_Module_ID n_Mod = Modules[m];
		Modules.erase( Modules.begin() + m );
		Modules.insert( Modules.begin() + n, n_Mod );
	}
*/
	// Initialize them
	for( uint m = 0; m < Mod_Thread_Engine.size(); m++ )
	{
		Set_Current_Module( Mod_Thread_Engine[m].m_Mod );

		// Make log file for module
		Mod_Thread_Engine[m].m_Mod->m_Log = SE_Filesys::Open_For_Write_Stream( SE_MODULE_LOG_DIR + Mod_Thread_Engine[m].m_Mod->Name + ".log" );

		sePrintf( SEPRINT_INFO, "Initializing module-%d %s...\n", Mod_Thread_Engine[m].Init_Time, Mod_Thread_Engine[m].m_Mod->Name.c_str() );
		uint Mod_Ret = Mod_Thread_Engine[m].Func_Init( Main_Args );

		if( Mod_Ret == SE_SUCCESS )
			;//sePrintf( SEPRINT_INFO, "Successfully initialized %s\n", Modules[m]->Name.c_str() );

		else
		{
			sePrintf( SEPRINT_ERROR, "'%s' failed with error code\n", Mod_Thread_Engine[m].m_Mod->Name.c_str() );
//			return false;
		}
	}

	Set_Current_Module( NULL );

	sePrintf( SEPRINT_INFO, "Initialized all modules\nInitializing threads...\n" );

	SE_Thread::Thread_Func_Data Thread_Data;

	Thread_Data.Func_Init = Init_Update;
	Thread_Data.Func_Clean = Clean_Update;
	Thread_Data.Callback = Module_Call_Update;
	Thread_Update.m_ID = SE_Thread::Start( "update-module", Thread_Data );

	Thread_Data.Func_Init = Init_Render;
	Thread_Data.Func_Clean = Clean_Render;
	Thread_Data.Callback = Module_Call_Render;
	Thread_Render.m_ID = SE_Thread::Start( "render-module", Thread_Data );

	while( SE_Thread::Get_State( Thread_Update.m_ID ) != SE_Thread::Thread_State::STATE_PAUSED ||
			SE_Thread::Get_State( Thread_Render.m_ID ) != SE_Thread::Thread_State::STATE_PAUSED );

	SE_Thread::Continue( Thread_Update.m_ID );
	SE_Thread::Continue( Thread_Render.m_ID );

	sePrintf( SEPRINT_INFO, "Initialized Threads\nInitializing user-space..." );	

	if( User_Initialize( Main_Args ) == SE_SUCCESS )
		sePrintf( SEPRINT_INFO, "\n\n" );
	else
	{
		sePrintf( SEPRINT_ERROR, "Failed\n\n" );
//		return SE_FAILURE;
	}

	return SE_SUCCESS;
}

void S_Engine::Init_Update()
{
	for( uint m = 0; m < Mod_Thread_Update.size(); m++ )
	{
		Set_Current_Module( Mod_Thread_Update[m].m_Mod );

		// Make log file for module
		Mod_Thread_Update[m].m_Mod->m_Log = SE_Filesys::Open_For_Write_Stream( SE_MODULE_LOG_DIR + Mod_Thread_Update[m].m_Mod->Name + ".log" );

		sePrintf( SEPRINT_INFO, "Initializing module-%d %s...\n", Mod_Thread_Update[m].Init_Time, Mod_Thread_Update[m].m_Mod->Name.c_str() );
		uint Mod_Ret = Mod_Thread_Update[m].Func_Init( Main_Args );

		if( Mod_Ret == SE_SUCCESS )
			;//sePrintf( SEPRINT_INFO, "Successfully initialized %s\n", Modules[m]->Name.c_str() );

		else
		{
			sePrintf( SEPRINT_ERROR, "'%s' failed with error code\n", Mod_Thread_Update[m].m_Mod->Name.c_str() );
			return;
		}
	}
}

void S_Engine::Init_Render()
{
	for( uint m = 0; m < Mod_Thread_Render.size(); m++ )
	{
		Set_Current_Module( Mod_Thread_Render[m].m_Mod );

		// Make log file for module
		Mod_Thread_Render[m].m_Mod->m_Log = SE_Filesys::Open_For_Write_Stream( SE_MODULE_LOG_DIR + Mod_Thread_Render[m].m_Mod->Name + ".log" );

		sePrintf( SEPRINT_INFO, "Initializing module-%d %s...\n", Mod_Thread_Render[m].Init_Time, Mod_Thread_Render[m].m_Mod->Name.c_str() );
		uint Mod_Ret = Mod_Thread_Render[m].Func_Init( Main_Args );

		if( Mod_Ret == SE_SUCCESS )
			;//sePrintf( SEPRINT_INFO, "Successfully initialized %s\n", Modules[m]->Name.c_str() );

		else
		{
			sePrintf( SEPRINT_ERROR, "'%s' failed with error code\n", Mod_Thread_Render[m].m_Mod->Name.c_str() );
//			return;
		}
	}
}

void S_Engine::Clean_Update()
{

}

void S_Engine::Clean_Render()
{

}

uint S_Engine::Shutdown()
{
	sePrintf( SEPRINT_INFO, "Shutting down...\n" );
	Is_Running = false;

	return true;
}

uint S_Engine::Cleanup()
{
	Set_Current_Module( NULL );

	sePrintf( SEPRINT_INFO, "\nCleaning user-space..." );
	if( User_Cleanup() == SE_SUCCESS )
		sePrintf( SEPRINT_INFO, "\n" );
	else
	{
		sePrintf( SEPRINT_ERROR, "Failed\n" );
		return SE_FAILURE;
	}

	sePrintf( SEPRINT_INFO, "Cleaning Survivor Engine, %d Module(s)\n", Modules.size() );

	// Cleanup in reverse
/*	for( uint i = Modules.size() - 1; i <= Modules.size() - 1; i-- )
	{
		Set_Current_Module( Modules[i] );
		sePrintf( SEPRINT_INFO, "Cleaning module-%d %s...\n", Modules[i]->Init_Time, Modules[i]->Name.c_str() );

		if( Modules[i]->Cleanup() == true )
			;//sePrintf( SEPRINT_INFO, "Successfully cleaned module %s\n", Modules[i]->Name.c_str() );
		else
			sePrintf( SEPRINT_ERROR, "Failed with error code\n", Modules[i]->Name.c_str() );

		delete Modules[i];
	}*/
	Set_Current_Module( NULL );
	sePrintf( SEPRINT_INFO, "Cleaned all modules\n" );

	return SE_SUCCESS;
}

/*/ Get specific thread DT
dt_precision S_Engine::Thread_DT( S_Thread_ID Which )
{

}

// Get current thread DT
dt_precision S_Engine::Thread_DT(  )
{

}
*/
using namespace SE_Modules;
bool Module_Check_Defaults()
{
	assert( Mod_Thread != NULL );
	assert( Mod_Filesystem != NULL );
	assert( Mod_Window != NULL );
	assert( Mod_Input != NULL );
	assert( Mod_GUI != NULL );
	assert( Mod_Graphics != NULL );

	return true;
}

void S_Engine::Terminate_Callback() // called from stdc
{
	sePrintf( SEPRINT_ERROR, "Terminate Handler Called, Engine: %s, Update: %s, Render: %s\n", Thread_Engine.Mod_Cur->Name.c_str(),
			  Thread_Update.Mod_Cur->Name.c_str(), Thread_Render.Mod_Cur->Name.c_str() );

	abort();
}


/************    MODULE CLASS    *********/

SE_Module::SE_Module()
{
	this->Engine_Event = NULL;
	this->Update_Event = NULL;
	this->Render_Event = NULL;

	this->m_Log_Color = NULL;
}



/************   PLATFORM SPECIFICS    ******************/

#ifdef _WIN32

int WINAPI WinMain( HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd )
{
	OutputDebugString( "\n\n\n" );
	return S_Engine_Main( 1, &lpCmdLine );
}

#else

#include <X11/X.h>
#include <X11/Xlib.h>
int main( int argc, char** argv )
{
	XInitThreads();
	return S_Engine_Main( argc, argv );
}

#endif
