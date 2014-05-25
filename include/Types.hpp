#ifndef SURVIVOR_ENGINE_TYPES
#define SURVIVOR_ENGINE_TYPES

#ifdef _WIN32
#include <stdlib.h> // Fixes wierd errors
#endif

typedef unsigned char uchar;
typedef signed char schar;

typedef const char cchar;

typedef unsigned int uint;
typedef signed int sint;
typedef unsigned short ushort;
typedef signed short sshort;


typedef float dt_precision;
typedef float prec_float; // Precision float

#ifndef TRUE

#define TRUE true
#define FALSE false

#endif

#define SE_SUCCESS 0
#define SE_FAILURE 1

#ifndef NULL
#define NULL (void*)(0)
#endif

#if !defined(INFINITE) && !defined(_WIN32)
#define INFINITE (uint)(-1)
#endif



#define ASCII_ESCAPE 27


typedef char int8;
typedef signed char sint8;
typedef unsigned char uint8;

typedef short int16;
typedef signed short sint16;
typedef unsigned short uint16;

typedef int int32;
typedef signed int sint32;
typedef unsigned int uint32;

typedef long long int64;
typedef signed long long sint64;
typedef unsigned long long uint64;

typedef double float64;

#ifdef _WIN32
#include <windows.h>
//#include <winsock2.h>
//#include <ws2tcpip.h>
//#else
typedef int ssize_t;//SSIZE_T ssize_t;
//typedef int HWND;
#endif



namespace SE_Gravity
{enum {
	Left_Top = 0,// default
	Left_Mid,
	Left_Bottom,
	Mid_Top,
	Mid_Mid,
	Mid_Bottom,
	Right_Top,
	Right_Mid,
	Right_Bottom,
	Hidden // Do not show
};}


// XML
#define PUGIXML_NO_EXCEPTIONS 1
#include "SE/pugixml/pugixml.hpp"
typedef pugi::xml_document XML_File;
typedef pugi::xml_attribute XML_Attr;
typedef pugi::xml_node XML_Node;


template< typename T >
class SE_Array;
typedef SE_Array<char> SE_Raw_Buffer;




#ifdef _WIN32

#define DLL_EXPORT __declspec(dllexport)
#define DLL_IMPORT __declspec(dllimport)

#ifdef BUILD_DLL
#define SE_API __declspec(dllexport)
#else
#define SE_API __declspec(dllimport)
#endif // BUILD_DLL

#else // _WIN32

#define SE_API 
#define DLL_EXPORT
#define DLL_IMPORT

#endif





#endif
