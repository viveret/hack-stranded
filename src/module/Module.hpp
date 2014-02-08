#ifndef SURVIVOR_ENGINE_MODULE
#define SURVIVOR_ENGINE_MODULE

/*
 * 13-8-31, first version of the module engine
 *
 * The Engine Manager handles the main loop and basic program.
 */


#include "../Core.hpp"

#include <vector>
#include <string>


namespace SE_Modules
{
	// Default modules that must be fullfilled
	extern SE_Module_ID

	Mod_Thread,
	Mod_Filesystem,
	Mod_Window,
	Mod_Input,
	Mod_GUI,
	Mod_Graphics,
	Mod_Physics;
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


	// Recieve Module ID from registering key components
	SE_Module_ID Generate_Module( std::string Name ); 

	void Register_Module_Engine( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() );
	void Register_Module_Update( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() );
	void Register_Module_Render( SE_Module_ID m_Mod, uchar Init_Time, uint (*Func_Init)( const std::vector<std::string>& ), uint (*Func_Clean)() );

	// Hooks modules into the engine
	uint Module_Hook();

/**************    THREADS     ***************************/

	// Get called inside of Engine thread
	uint Register_Module_Thread_Engine( void (*Func_Engine)(), uchar Pos = Module_Positions::POS_LATE, uchar Priority = 1 );

	// Get called inside of Logic thread
	uint Register_Module_Thread_Logic( void (*Func_Update)(), uchar Pos = Module_Positions::POS_LATE, uchar Priority = 1 );

	// Get called inside of Render thread
	uint Register_Module_Thread_Render( void (*Func_Render)(), uchar Pos = Module_Positions::POS_LATE, uchar Priority = 1 );


/**************    EVENTS     ***************************/

	// Get called inside of Engine thread
	void Register_Module_Event_Engine( SE_Module_ID Mod_ID, bool (*Func_Engine)( SE_Events::Event_Base* ) );

	// Get called inside of Logic thread
	void Register_Module_Event_Logic( SE_Module_ID Mod_ID, bool (*Func_Update)( SE_Events::Event_Base* ) );

	// Get called inside of Render thread
	void Register_Module_Event_Render( SE_Module_ID Mod_ID, bool (*Func_Render)( SE_Events::Event_Base* ) ); //  SE_Events::Event_Base*


/************   ETC MODULE SETTINGS  ******************/

	void Module_Set_Color( float Col[3] );
	void Module_Set_Color( const char* Col ); // Ansi escape sequence


/************   USER SET OPERATIONS    ****************/

	uint User_Initialize( const std::vector<std::string>& mArgs ); // After engine initialize
	uint User_Cleanup(); // After engine cleanup


/************   ENGINE OPERATIONS    ******************/

	void Event_Add( SE_Events::Event_Base* nEv );
	void Event_Add_Engine( SE_Events::Event_Base* nEv );
	void Event_Add_Update( SE_Events::Event_Base* nEv );
	void Event_Add_Render( SE_Events::Event_Base* nEv );
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
}

SE_Thread::Thread_ID seThread_Engine();
SE_Thread::Thread_ID seThread_Update();
SE_Thread::Thread_ID seThread_Render();


void sePrintf( uchar pType, const char* format,  ... );
//void sePrintf( const char* format, ... );
//void sePrintf( const char* format, ... )
namespace sePrintf_Types
{enum{

	SEPRINT_NONE = 0, // white
	SEPRINT_INFO = 0, // white

	SEPRINT_WARN, // yellow
	SEPRINT_ERROR, // red
	SEPRINT_CRITICAL, // blinking red

	SEPRINT_DEBUG, // green
	SEPRINT_CONFIG, // light blue
	
	SEPRINT_MODULE // color based on cur module color, def white
 };}
using namespace sePrintf_Types;




#endif
