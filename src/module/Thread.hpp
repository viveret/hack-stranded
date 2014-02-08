#ifndef SURVIVIOR_ENGINE_THREAD
#define SURVIVIOR_ENGINE_THREAD

#include "../Core.hpp"

namespace SE_Thread
{
	struct Thread_Mgr;
	typedef Thread_Mgr* Thread_ID;

	struct Thread_Func_Data
	{
		Thread_Func_Data();

		void (*Func_Init)();
		void (*Callback)(void*);
		void (*Func_Clean)();
	};


//	Thread_ID Start( std::string Name, void (*Callback)() );
	Thread_ID Start( std::string Name, const Thread_Func_Data& Thread_Data, void* Arg = NULL );
	uint End( Thread_ID Which );

	void Continue( Thread_ID Which );
	void Pause( Thread_ID Which );

	dt_precision Get_DT( Thread_ID Which = NULL );

	Thread_ID Get_Current();
	uchar Get_State( Thread_ID Which );

	namespace Thread_State
	{enum{
		STATE_UNKNOWN = 0,

		STATE_INACTIVE,
		STATE_INITIALIZING,
		STATE_RUNNING,
		STATE_PAUSED,
		STATE_SHUTDOWN,
		STATE_FINISHED
	};}

	class Mutex
	{
		Thread_ID Thread_Using;
	public:
		Mutex();

		void Wait_For_Lock( uint Time = INFINITE );

		bool Is_Locked();
		void Unlock();
		void Lock();
	};


	void Register_Module();
}




#endif
