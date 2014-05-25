#ifndef SURVIVOR_ENGINE_EXCEPTION
#define SURVIVOR_ENGINE_EXCEPTION

#include "../Types.hpp"

// Used for errors, warnings, or other non-kosher events

namespace SE_Exception
{enum{
	SEEX_DEBUG = 0, // Lowest, can also be toggled on/off
	SEEX_INFO = 10,
	// Verbose settings = # <= SEEX_INFO

	SEEX_NOTICE, // Normal but significant condition
	
	SEEX_WARN,
	SEEX_WARNING = SEEX_WARN,
	
	SEEX_ERR,
	SEEX_ERROR = SEEX_ERR,
	
	SEEX_CRIT,
	SEEX_CRITICAL = SEEX_CRIT,

	SEEX_ALERT, // Action must be taken immediately

	SEEX_EMERG, // Engine is unusable
	SEEX_PANIC = SEEX_EMERG
};}
using namespace SE_Exception;

void seError( uchar Level, const char* Format, ... );

#endif
