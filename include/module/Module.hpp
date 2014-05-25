#ifndef SURVIVOR_ENGINE_MODULE
#define SURVIVOR_ENGINE_MODULE

/*
 * 13-8-31, first version of the module engine
 *
 * The Engine Manager handles the main loop and basic program.
 */


#include "../Core.hpp"
#include <SE/module/Plugin.hpp>

#include <vector>
#include <string>



namespace SE_Modules
{
	// Default modules that must be fullfilled
	extern SE_Module_ID

	Mod_Thread,
	Mod_Input;
}


namespace S_Engine
{
/************   MODULE OPERATIONS    ******************/

	namespace Reg_Mod_Time
	{enum {
		LOW_LEVEL = 0, // 0 - 7
		LOW_LEVEL_NEXT = 8, // 8 - 63

		MED_LEVEL = 64, // 64 - 71
		MED_LEVEL_NEXT = 72, // 72 - 199

		AFTER_LEVEL = 200, // final steps

		LAST_LEVEL = 244,
		TIME_ANY = 255
	};}

	namespace Module_Levels
	{enum {

		LEVEL_TOP = 0,

		LEVEL_ENGINE, // the engine depends on it
		LEVEL_OS_INTERFACE, // Really low, but not hardware related. Ie filesystem, input
		LEVEL_HARDWARE_INTERFACE // Lowest level, ie OpenGL, OpenAL

	};}

	namespace Module_Positions
	{enum {
		// Increment every 51 = 255 / 5
		POS_START = 0,
		POS_EARLY = 51,
		POS_MIDDLE = 102, POS_MED = POS_MIDDLE,
		POS_LATE = 153,
		POS_LAST = uchar(-1),
		POS_END = POS_LAST

	};}


	// Generate Module ID from registering key components
	SE_API SE_Module_ID Generate_Module( const char* Name, const char* Name_Generic = NULL ); 

	// Recieve Module from name
	SE_API SE_Module_ID Get_Module( std::string Name, bool Silent = false );
	SE_API SE_Module_ID Get_Module_Generic( std::string Name, bool Silent = false );

	SE_API void Register_Module_Engine( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() );
	SE_API void Register_Module_Update( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() );
	SE_API void Register_Module_Render( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() );

	// Hooks modules into the engine
//	extern uint(*Module_Hook)();
	SE_API void Set_Module_Hook( uint(*Func)() );

/**************    THREADS     ***************************/

	// Get called inside of Engine thread
	SE_API uint Register_Module_Thread_Engine( void (*Func_Engine)(), uchar Pos = Module_Positions::POS_LATE, uchar Priority = 1 );

	// Get called inside of Logic thread
	SE_API uint Register_Module_Thread_Logic( void (*Func_Update)(), uchar Pos = Module_Positions::POS_LATE, uchar Priority = 1 );

	// Get called inside of Render thread
	SE_API uint Register_Module_Thread_Render( void (*Func_Render)(), uchar Pos = Module_Positions::POS_LATE, uchar Priority = 1 );


/**************    EVENTS     ***************************/

	// Get called inside of Engine thread
	SE_API void Register_Module_Event_Engine( SE_Module_ID Mod_ID, bool (*Func_Engine)( SE_Events::Event_Base* ) );

	// Get called inside of Logic thread
	SE_API void Register_Module_Event_Logic( SE_Module_ID Mod_ID, bool (*Func_Update)( SE_Events::Event_Base* ) );

	// Get called inside of Render thread
	SE_API void Register_Module_Event_Render( SE_Module_ID Mod_ID, bool (*Func_Render)( SE_Events::Event_Base* ) ); //  SE_Events::Event_Base*


/************   ETC MODULE SETTINGS  ******************/

	SE_API void Module_Set_Color( float Col[3] );
	SE_API void Module_Set_Color( const char* Col ); // Ansi escape sequence


/************   USER SET OPERATIONS    ****************/
	SE_API void Set_User_Init( uint(*Func)(const std::vector<std::string>&) ); // After engine initialize
	SE_API void Set_User_Clean( uint(*Func)() ); // After engine cleanup

	SE_API uint Main( uint Argc, char** Argv );
/************   ENGINE OPERATIONS    ******************/

	SE_API void Event_Add( SE_Events::Event_Base* nEv );
	SE_API void Event_Add_Engine( SE_Events::Event_Base* nEv );
	SE_API void Event_Add_Update( SE_Events::Event_Base* nEv );
	SE_API void Event_Add_Render( SE_Events::Event_Base* nEv );
	// Get specific thread DT
//	dt_precision Thread_DT( S_Thread_ID Which );

	// Get current thread DT
//	dt_precision Thread_DT(  );


/*	namespace Event
	{enum{

		EVENT_UNKNOWN = 0,

		EVENT_NOTIFY, // Warnings, errors or other messages
		EVENT_MODULE_REGISTER, // pass register_module function
		EVENT_MODULE_SHUTDOWN, // Signal module is shutting down
		EVENT_EXIT, // pass string with message
		EVENT_SHUTDOWN = EVENT_EXIT
	};}
	*/

	/************	DEBUG	*************/
	SE_API void Debug_Print_Module( bool T_Engine, bool T_Render, bool T_Update );
}

SE_API SE_Thread::Thread_ID seThread_Engine();
SE_API SE_Thread::Thread_ID seThread_Update();
SE_API SE_Thread::Thread_ID seThread_Render();

//SE_API extern void (*sePrintf)( uchar pType, const char* format,  ... );
//SE_API void sePrintf_Lean( uchar pType, const char* format,  ... );

//void sePrintf( const char* format, ... );
//void sePrintf( const char* format, ... )
namespace sePrintf_Types
{enum{
	SEPRINT_NONE = 0, // white
	SEPRINT_INFO = 0, // white

	SEPRINT_WARN, SEPRINT_WARNING = SEPRINT_WARN,// yellow
	SEPRINT_ERROR, // red
	SEPRINT_CRITICAL, // blinking red

	SEPRINT_DEBUG, // green
	SEPRINT_CONFIG, // light blue
	
	SEPRINT_MODULE // color based on cur module color, def white
};}
using namespace sePrintf_Types;




#endif
