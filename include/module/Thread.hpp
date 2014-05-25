#ifndef SURVIVIOR_ENGINE_THREAD
#define SURVIVIOR_ENGINE_THREAD

#include "../Core.hpp"
#include <vector>
#include "SE/setl/Timer.hpp"


namespace SE_Thread
{
	struct Thread_Mgr;
	typedef Thread_Mgr* Thread_ID;

	struct Thread_Func_Data
	{
		SE_API Thread_Func_Data();

		void (*Func_Init)();
		void (*Callback)(void*);
		void (*Func_Clean)();
	};


	SE_API Thread_ID Start( std::string Name, const Thread_Func_Data& Thread_Data, void* Arg = NULL );
	SE_API uint End( Thread_ID Which );

	SE_API void Continue( Thread_ID Which );
	SE_API void Pause( Thread_ID Which );

	SE_API void Cool_Off(); // Take a few to partition timeslice

	SE_API dt_precision Get_DT( Thread_ID Which = NULL );
	inline dt_precision DT( Thread_ID Which = NULL )
	{
		return Get_DT( Which );
	}

	SE_API uint Get_Ticks( Thread_ID Which = NULL );
	SE_API Timer* Get_Timer( Thread_ID Which = NULL );

	SE_API Thread_ID Get_Current();
	SE_API uchar Get_State( Thread_ID Which );

	uint Add_Ext( Thread_ID Which, void* Data, std::string Name );
	void* Get_Ext( Thread_ID Which, std::string Name, bool Silent = false );
	void* Get_Ext( Thread_ID Which, uint Ext_ID, bool Silent = false );
	const std::vector<void*>& Get_Ext_List( Thread_ID Which );

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
		SE_API Mutex();

		SE_API void Wait_For_Lock( uint Time = INFINITE );

		SE_API bool Is_Locked();
		SE_API void Unlock();
		SE_API void Lock();
	};


	void Register_Module();
}




#endif
