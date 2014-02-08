#include "Exception.hpp"
#include "../module/Module.hpp"

#include <stdarg.h>
#define PRINTF_BUF_SIZE 512

namespace SE_Exception
{
	const char* Level_To_Str( uchar Level );
}
/* 
const char* sePrintf_Cols[ SEPRINT_MODULE ] =
{
	"\x1b[0m", // Regular
	"\x1b[4;31m", // Yellow
	"\x1b[4;31m", // Red
	"\x1b[4;31m", // Blink red
	"\x1b[4;32m", // Green
	"\x1b[4;34m" // Light blue
};*/



void seError( uchar Level, const char* Format,  ... )
{
	static char Printf_Buf[ PRINTF_BUF_SIZE ];
	memset( Printf_Buf, 0, PRINTF_BUF_SIZE );

	va_list args;
	va_start( args, Format );
	vsnprintf( Printf_Buf, PRINTF_BUF_SIZE, Format, args );
	va_end( args );


	const char* Level_Name = NULL;

	sePrintf( 0, "%s", Printf_Buf );
}

const char* SE_Exception::Level_To_Str( uchar Level )
{
	switch( Level )
	{
		case SEEX_DEBUG: return "[Debug]: ";
		case SEEX_NOTICE: return "[Notice]: ";
		case SEEX_WARN: return "[Warning]: ";
		case SEEX_ERROR: return "[Error]: ";
	}
}
