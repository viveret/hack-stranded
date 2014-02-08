#include "Timer.hpp"

/*#ifndef _WIN32
#include <limits.h>
timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}
#endif*/


Timer::Timer()
{
	this->Next_Poll = 0.;//per sec
	this->Collect_Poll = 0;
	this->Average = 0;
	this->Frame_Time = 0;

	this->Elapsed_Time = 0;//time
}

void Timer::Start()
{
#ifdef _WIN32
    // get ticks per second
    QueryPerformanceFrequency(&this->frequency);
    // start timer
    QueryPerformanceCounter(&this->Time_Hold);


	this->next_game_tick = GetTickCount();
#else
	// start timer
	gettimeofday(&this->Time_Start, NULL);
	gettimeofday(&this->Time_Update, NULL);

#endif
}

void Timer::End()
{
#ifdef _WIN32
	//
#else
	gettimeofday( &this->Time_End, NULL );

	this->Elapsed_Time = ( this->Time_End.tv_sec - this->Time_Start.tv_sec);
	this->Elapsed_Time += ( this->Time_End.tv_usec - this->Time_Start.tv_usec) / 1000. / 1000.;
#endif
}


const int FRAMES_PER_SECOND = 60;
const int SKIP_TICKS = 1000 / FRAMES_PER_SECOND;


void Timer::Update()
{
#ifdef _WIN32
	LARGE_INTEGER Temp_Time;
    QueryPerformanceCounter(&Temp_Time);

    // compute and print the elapsed time in millisec
	this->Elapsed_Time = (Temp_Time.QuadPart - this->Time_Hold.QuadPart) * 1000.f / this->frequency.QuadPart;

	this->next_game_tick += SKIP_TICKS;
    int sleep_time = this->next_game_tick - GetTickCount();
    if( sleep_time >= 0 ) {
        Sleep( sleep_time );
    }

	this->Time_Hold=Temp_Time;//update
#else

	this->Elapsed_Time = 0;
	while( this->Elapsed_Time <= 1000/60.f )
	{
		// stop timer
		gettimeofday( &this->Time_End, NULL );

		// compute and print the elapsed time in millisec
		this->Elapsed_Time = (this->Time_End.tv_sec - this->Time_Update.tv_sec) * 1000.0;      // sec to ms
		this->Elapsed_Time += (this->Time_End.tv_usec - this->Time_Update.tv_usec) / 1000.0;   // us to ms
	}

	gettimeofday( &this->Time_Update, NULL );

#endif

	this->Collect_Poll++;
	this->Next_Poll += this->Elapsed_Time;
	if(this->Next_Poll>=1000.f)	// 1000dt == 1 second
	{
		this->Average = this->Collect_Poll;
		this->Collect_Poll = 0;
		this->Next_Poll = 0.;
	}
}
