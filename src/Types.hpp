#ifndef SURVIVOR_ENGINE_TYPES
#define SURVIVOR_ENGINE_TYPES

#ifdef _WIN32
#include <stdlib.h> // Fixes wierd errors
#endif

typedef unsigned char uchar;
typedef signed char schar;

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
#include <Windows.h>
//#include <Winsock2.h>

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
	Right_Bottom
};}





#endif
