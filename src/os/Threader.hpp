#ifndef SURVIVOR_ENGINE_THREADER
#define SURVIVOR_ENGINE_THREADER

#include <vector>

#include "../Core.hpp"
#include "../utility/Timer.hpp"


#ifndef _WIN32
#include <boost/thread.hpp>
#endif


namespace E_Threads
{
#ifdef _WIN32
	DWORD WINAPI Draw_Thread(void*);
	DWORD WINAPI Logic_Thread(void*);
	//DWORD WINAPI Event_Thread(void*);
	//DWORD WINAPI Resource_Load_Thread(void*);
	DWORD WINAPI Input_Thread(void*);
#ifdef USE_XBOX
	DWORD WINAPI XBox_Thread(void*);
#endif

	typedef DWORD Thread_ID_Type;
	typedef HANDLE Thread_Handle_Type;

#else
	void Draw_Thread();
	void Logic_Thread();

	typedef boost::thread::id Thread_ID_Type;
	typedef boost::thread* Thread_Handle_Type;
#define THREAD_ID_NONE boost::thread::id()
#endif


	struct T_Thread
	{
		T_Thread();
		~T_Thread();

		void Start( void(*Func)() );
		void Start( void(*Func)(void*), void* Arg = NULL );
		void End();

		void Pause();
		void Resume();

		Thread_Handle_Type hThread;
		Thread_ID_Type t_ID;

		bool Running;
		Timer m_Timer;
	};

	Thread_ID_Type Get_Thread();// returns which thread it is in
}
	//


/*struct T_Threader
{
	unsigned int Ping;

	void Start(void(S_Engine::*func)());
	void End();
private:
	bool Running;
	HANDLE hThread;
	DWORD WINAPI m_Thread(void*);
	void(S_Engine::*m_Func)();
};
	   */
#endif
