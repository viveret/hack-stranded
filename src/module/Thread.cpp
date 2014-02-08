#include "Thread.hpp"
#include "Module.hpp"
#include "../utility/Timer.hpp"

#include <vector>

#ifdef _WIN32
#include <assert.h>
#endif

#include <boost/thread.hpp>


namespace SE_Modules
{
SE_Module_ID Mod_Thread = NULL;
}
using namespace SE_Modules;

namespace SE_Thread
{
/********    GLOBALS    **********/
	std::vector< Thread_ID > g_Threads;
	Thread_ID g_Thread_Main = NULL;


/********    FUNCTIONS    **********/
	void Thread_Main_Loop( void* Which ); // while true

/*********   DEFENITIONS    **************/
#ifdef _WIN32
//	DWORD WINAPI Draw_Thread(void*);

	typedef DWORD Thread_ID_Type;
	typedef HANDLE Thread_Handle_Type;

	#define THREAD_ID_NONE NULL
#else
	void Draw_Thread();
	void Logic_Thread();

	typedef boost::thread::id Thread_ID_Type;
	typedef boost::thread* Thread_Handle_Type;
	#define THREAD_ID_NONE boost::thread::id()
#endif
}
using namespace SE_Thread;


/*********   THREAD STRUCT    **********/
struct SE_Thread::Thread_Mgr
{
	Thread_Func_Data Thread_Data;
	void* m_Arg;

	Thread_Handle_Type Handle;
	Thread_ID_Type m_ID;

	uchar m_State;
	Timer m_Timer;

	std::string Name;

	Thread_Mgr()
	{
		this->m_Arg = NULL;

		this->Handle = NULL;
		this->m_ID = THREAD_ID_NONE;

		this->m_State = 0;
	}
};

/*namespace Thread_State
{enum{

	STATE_INACTIVE = 0,
	STATE_INITIALIZING,
	STATE_RUNNING,
	STATE_SHUTDOWN,
	STATE_COMPLETE,

	STATE_ERROR
};}*/

Thread_Func_Data::Thread_Func_Data()
{
	this->Func_Init = NULL;
	this->Callback = NULL;
	this->Func_Clean = NULL;
}
/*********     THREAD OPERATIONS    *******/
/* 
Thread_ID SE_Thread::Start( std::string Name, void (*Callback)() )
{
	assert( Callback != NULL );
	assert( Name.length() != 0 );

	Thread_ID nThread = new Thread_Mgr;
	nThread->Name = Name;
	nThread->m_State = Thread_State::STATE_RUNNING;
	nThread->Callback = ()()Callback;
	nThread->m_Arg = Arg;

	g_Threads.push_back( nThread );

	nThread->Handle = new boost::thread( Thread_Main_Loop, nThread );
	
	return nThread;
}
*/
Thread_ID SE_Thread::Start( std::string Name, const Thread_Func_Data& Thread_Data, void* Arg )
{
	assert( Thread_Data.Callback != NULL );
	assert( Name.length() != 0 );

	Thread_ID nThread = new Thread_Mgr;
	nThread->Name = Name;
	nThread->m_State = Thread_State::STATE_INACTIVE;
	nThread->m_Arg = Arg;
	nThread->Thread_Data = Thread_Data;

	g_Threads.push_back( nThread );

	nThread->Handle = new boost::thread( Thread_Main_Loop, nThread );
	
	return nThread;
}

uint SE_Thread::End( Thread_ID Which )
{
	return true;
}

void SE_Thread::Thread_Main_Loop( void* Which ) // while true
{
	Thread_ID This = (Thread_ID)( Which );

#ifdef _WIN32
	This->m_ID = GetCurrentThreadId();
#else
	This->m_ID = boost::this_thread::get_id();
#endif

	This->m_Timer.Start();
	This->m_State = Thread_State::STATE_INITIALIZING;

	if( This->Thread_Data.Func_Init )
		This->Thread_Data.Func_Init();

	This->m_State = Thread_State::STATE_PAUSED;

	while( This->m_State != Thread_State::STATE_SHUTDOWN )
	{
		while( This->m_State == Thread_State::STATE_PAUSED );

		This->m_Timer.Update();
		This->Thread_Data.Callback( This->m_Arg );
	}

	if( This->Thread_Data.Func_Clean )
		This->Thread_Data.Func_Clean();

	This->m_Timer.End();
}

uchar SE_Thread::Get_State( Thread_ID Which )
{
	if( Which == NULL ) return 0;

	return Which->m_State;
}

void SE_Thread::Continue( Thread_ID Which )
{
	if( Which == NULL ) return;

	if( Which->m_State == Thread_State::STATE_PAUSED )
		Which->m_State = Thread_State::STATE_RUNNING;
}

void SE_Thread::Pause( Thread_ID Which )
{
	if( Which == NULL ) return;

	if( Which->m_State == Thread_State::STATE_RUNNING )
		Which->m_State = Thread_State::STATE_PAUSED;
}


Thread_ID SE_Thread::Get_Current()
{
#ifdef _WIN32
	uint Cur_ID = GetCurrentThreadId();
#else
	boost::thread::id Cur_ID = boost::this_thread::get_id();
#endif

	for( uint i = 0; i < g_Threads.size(); i++ )
		if( g_Threads[i]->m_ID == Cur_ID )
			return g_Threads[i];

	return NULL;
}

dt_precision SE_Thread::Get_DT( Thread_ID Which )
{
	if( Which == NULL )
		Which = Get_Current();

	return Which->m_Timer.Elapsed_Time;
}


/************    MUTEX CLASS   ************/

Mutex::Mutex()
{
	this->Thread_Using = NULL;//THREAD_ID_NONE;
}

void Mutex::Wait_For_Lock( uint Time ) // const char* F_Name,
{
	if( this->Is_Locked() == false )
		return;


#ifdef _DEBUG
/*	if( F_Name )
#ifdef _WIN32
		OutputDebugString( ("Waiting For: " + std::string(F_Name) + "\n").c_str() );
#else
		printf(("Waiting For: " + std::string(F_Name) + "\n").c_str());
#endif*/
#endif

	if( Time == INFINITE )
		;//Time = 999;

	while( Time != 0 && this->Is_Locked() )
	{
		Time--;
		if( Time == INFINITE )
			;
		else
			;

		//Sleep(0);
	}
}

bool Mutex::Is_Locked()
{
	if( Get_Current() != this->Thread_Using && this->Thread_Using != NULL )// in a seperate thread
		return true;

	else
		return false;
}

void Mutex::Unlock()
{
	this->Thread_Using = NULL;//THREAD_ID_NONE;
}

void Mutex::Lock()
{
	this->Thread_Using = Get_Current();
}





/**********   MODULE OPERATIONS    **************/

uint Initialize( const std::vector<std::string>& mArgs )
{


	return SE_SUCCESS;
}

uint Cleanup()
{
	for( uint i = 0; i < g_Threads.size(); i++ )
		End( g_Threads[i] );

	g_Threads.clear();
	delete g_Thread_Main;
	g_Thread_Main = NULL;

	return SE_SUCCESS;
}

void SE_Thread::Register_Module()
{
	Mod_Thread = S_Engine::Generate_Module( "thread-boost" );
	S_Engine::Register_Module_Engine( Mod_Thread, S_Engine::Reg_Mod_Time::LOW_LEVEL, Initialize, Cleanup );

	// Required for initialization to work
	g_Thread_Main = new Thread_Mgr;
	g_Thread_Main->Name = "engine";
	g_Thread_Main->m_State = Thread_State::STATE_RUNNING;
#ifdef _WIN32
	g_Thread_Main->m_ID = GetCurrentThreadId();
#else
	g_Thread_Main->m_ID = boost::this_thread::get_id();
#endif
	g_Threads.push_back( g_Thread_Main );
}
