#include "../os/Threader.hpp"
using namespace E_Threads;

#include "../engine/Engine.hpp"

#ifdef _WIN32
DWORD WINAPI E_Threads::Draw_Thread(void*)
{
//	Engine.Thread_Draw.t_ID = GetCurrentThreadId();	  						 

//	wglMakeCurrent(Engine.Application.Window.hDC,Engine.Application.Window.hDraw_RC);
#else
void E_Threads::Draw_Thread()
{
#ifdef _TEST
	Engine.Thread_Draw.t_ID = boost::this_thread::get_id();
	glXMakeCurrent( Engine.Application.Window.m_dpy, Engine.Application.Window.m_win, Engine.Application.Window.m_glc);
#endif
#endif

#ifdef _TEST
	Engine.Thread_Draw.m_Timer.Start();

	Engine.Thread_Draw.Running = true;
	while( Engine.Is_Running )
	{							  
		Engine.Draw();	   					
		Engine.Thread_Draw.m_Timer.Update();
	}  
	
#ifdef _WIN32
	wglMakeCurrent(NULL,NULL);
#else
	//
#endif

	Engine.Thread_Draw.m_Timer.End();  
	Engine.Thread_Draw.Running = false;	 

	//while( true );
#endif

#ifdef _WIN32
    return (DWORD) 0;
#endif
}	



#ifdef _WIN32
DWORD WINAPI E_Threads::Logic_Thread(void*)
{
//	AttachThreadInput( Engine.Thread_Logic.t_ID, Engine.Thread_Engine, TRUE );
//	AttachThreadInput( Engine.Thread_Engine, Engine.Thread_Logic.t_ID, TRUE );
//	Engine.Thread_Logic.t_ID = GetCurrentThreadId();
#else
void E_Threads::Logic_Thread()
{
#ifdef _TEST
	Engine.Thread_Logic.t_ID = boost::this_thread::get_id();
#endif

#ifdef _TEST
	Engine.Thread_Logic.m_Timer.Start();

	Engine.Thread_Logic.Running = true;
	while( Engine.Is_Running )
	{							  
		Engine.Logic();	   					   
		Engine.Thread_Logic.m_Timer.Update();
	}  
#endif

#ifdef _WIN32
	AttachThreadInput( Engine.Thread_Logic.t_ID, Engine.Thread_Engine, FALSE );
	AttachThreadInput( Engine.Thread_Engine, Engine.Thread_Logic.t_ID, FALSE );  
#else
	//
#endif
#ifdef _TEST
	Engine.Thread_Logic.m_Timer.End();
	Engine.Thread_Logic.Running = false;	 
#endif
	//while( true );
#endif
#ifdef _WIN32
    return (DWORD) 0;
#endif
}

/*DWORD WINAPI E_Threads::Event_Thread(void*)
{
	Engine.Thread_Events.Running=true;
	while(Engine.Is_Running())
	{
		//Engine.E		   
	}	 
	Engine.Thread_Events.Running=false;
    ExitThread(0);
}

DWORD WINAPI E_Threads::Resource_Load_Thread(void*)
{					
	//get loading context working
	Engine.Resources.Stuff.hRC=wglCreateContext(Engine.Application.Window.hDC);//made
	wglShareLists(Engine.Application.Window.hRC,Engine.Resources.Stuff.hRC);//shared		   
	//GET LOADING CONTEXT
	wglMakeCurrent(Engine.Application.Window.hDC,Engine.Resources.Stuff.hRC);
	//ok to start loading
	Engine.Resources.Initialize();
//	Engine.Resources.Load_World_Resources_Thread(
	wglMakeCurrent(NULL,NULL);
	wglDeleteContext(Engine.Resources.Stuff.hRC);
    ExitThread(0);
}	*/

#ifdef USE_XBOX
DWORD WINAPI E_Threads::XBox_Thread(void*)
{
	Engine.Thread_XBox.m_Timer.Start();	
	Engine.Thread_XBox.t_ID=GetCurrentThreadId();
	while(Engine.Is_Running)
	{							    	
		Engine.Application.Input.XBOX.Check();
		Engine.Application.Input.XBOX.Do(0.f);	
		Engine.Thread_XBox.m_Timer.Update();	 
	}  
	Engine.Thread_XBox.m_Timer.End();
    return (DWORD) 0;
}
#endif

/*DWORD WINAPI E_Threads::Input_Thread(void*)
{
	Engine.Thread_Input.m_Timer.Start();	
	Engine.Thread_Input.t_ID = GetCurrentThreadId();
	//
	AttachThreadInput(Engine.Thread_Input.t_ID,Engine.Thread_Engine,TRUE);
	//
	while(Engine.Is_Running)
	{							  
		//Engine.Application.Input.Get();
//		GetKeyboardState(Engine.Application.Input.Keys); 
		Engine.Thread_Input.m_Timer.Update();
	}  
	Engine.Thread_Input.m_Timer.End();

    return (DWORD) 0;
}			 */




T_Thread::T_Thread()
{
	this->Running = false;

#ifdef _WIN32
	this->hThread=NULL;
	this->t_ID=NULL;
#endif
}

T_Thread::~T_Thread()
{
	this->Running = false;
	this->End();
}

void T_Thread::Start( void(*Func)() )
{
	this->Running = true;

#ifndef _WIN32
	this-> hThread = new boost::thread( Func );
#ifdef _TEST
	Engine.Mem_Add( this->hThread, "boost thread" );
#endif
#endif
}

void T_Thread::Start( void(*Func)(void*), void* Arg )
{
	this->Running = true;

#ifndef _WIN32
	this-> hThread = new boost::thread( Func, Arg );//, Arg );
#ifdef _TEST
	Engine.Mem_Add( this->hThread, "boost thread" );
#endif
#endif
}

void T_Thread::End()
{
	while( this->Running ); // wait to finish running
#ifdef _WIN32
	if(this->hThread)
	{
		//DWORD Result = STILL_ACTIVE;
		//while( Result == STILL_ACTIVE )
			//GetExitCodeThread( this->hThread, &Result );

		//while( this->Running );

		WaitForSingleObjectEx( this->hThread, INFINITE, TRUE );
		CloseHandle(this->hThread);

		this->hThread = NULL;
	}
#else

	if( this->hThread )
	{
		this->hThread->join();
#ifdef _TEST
		Engine.Mem_Remove( this->hThread );
#endif
		delete this->hThread;
		this->hThread = NULL;
	}

#endif
}

void T_Thread::Pause()
{
#ifdef _WIN32
	SuspendThread( this->hThread );
#endif
}

void T_Thread::Resume()
{
#ifdef _WIN32
	ResumeThread( this->hThread );
#endif
}

Thread_ID_Type E_Threads::Get_Thread()
{					
#ifdef _WIN32
	uint Cur_ID = GetCurrentThreadId();
#else
	boost::thread::id Cur_ID = boost::this_thread::get_id();
#endif

	return Cur_ID;
}

/*void T_Threader::Start(void(S_Engine::*func)())
{
	if(!this->Running)
	{
		this->m_Func=func;
		this->Running=true;
		this->hThread=CreateThread(NULL,0,&this->m_Thread,(void*)this,0,NULL);
	}
}

void T_Threader::End()
{
	this->Running=false;
}

DWORD WINAPI T_Threader::m_Thread(void* nThis)
{
	T_Threader* This=(nThis);
	while(this->Running)
		{
			this->Ping=0;
			(this->m_Func);
		}
}		  */

