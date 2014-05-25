#ifndef SURVIVOR_ENGINE_EVENT_DEFINITIONS
#define SURVIVOR_ENGINE_EVENT_DEFINITIONS


// Events are grouped by concept,
// And must be sent per thread.
// Some modules use multiple event groups

#define EVENT_UNKNOWN = 0;

namespace Event_Module // Mod = NULL
{enum{
	EVENT_MODULE_SHUTDOWN,
	EVENT_EXIT,
	EVENT_SHUTDOWN = EVENT_EXIT,
	EVENT_THREAD_NEW,
	EVENT_THREAD_END
};}

namespace Event_Graphic // Mod_Graphics
{enum{	
	EVENT_RESIZE = 1 // 2 uints
};}

namespace Event_Window // Mod_Window
{enum{
	EVENT_MOVE = 1,
	EVENT_ACTIVE
};}

namespace Event_GUI // Mod_GUI
{enum{
	EVENT_PAUSE = 1,
	EVENT_RESUME,
	EVENT_ENTER,
	EVENT_LEAVE,

	EVENT_MENU_LOAD,
	EVENT_MENU_CLEAN
};}

namespace Event_Input // Mod_Input
{enum{
	EVENT_KEYUP = 1,
	EVENT_KEYDOWN,
	EVENT_MOUSE,
	EVENT_GAMEPAD,

	EVENT_SET // set input value -> simulate event
};}


#endif
