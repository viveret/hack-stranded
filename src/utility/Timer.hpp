#ifndef SURVIVOR_ENGINE_TIMER
#define SURVIVOR_ENGINE_TIMER

#include "../Core.hpp"

#ifndef _WIN32
#include <sys/time.h> // for gettimeofday()
#include <time.h>
#endif

struct Timer
{
	Timer();

	void Start();
	void End();
	void Update();
	//
	double Next_Poll;//per sec
	unsigned int Collect_Poll,Average;
	dt_precision Frame_Time;

	dt_precision Elapsed_Time;//time

#ifdef _WIN32
	LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER Time_Hold;           // ticks

	DWORD next_game_tick;
#else

	timeval Time_Start, Time_End, Time_Update;

#endif
};

#endif
