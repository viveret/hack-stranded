#ifndef SURVIVOR_ENGINE_EVENTS
#define SURVIVOR_ENGINE_EVENTS

#include "Module.hpp"
#include <SE/setl/Array.hpp>
#include "Thread.hpp"
#include <queue>

namespace SE_Events
{
	// Each type has a seperate Code, found in its header
	namespace Types
	{enum{

		TYPE_UNKNOWN = 0,

		TYPE_GRAPHICS, // render thread
		TYPE_WINDOW, // engine thread
		TYPE_INPUT, // engine thread
		TYPE_GUI, // update thread
		TYPE_TIMER, // any thread
		TYPE_PHYSICS, // update thread
		TYPE_GAME, // update thread, user coded

		TYPE_ENGINE // engine thread, core events (including errors / notifs)
	};}

	struct Event_Base
	{
		SE_API Event_Base( SE_Module_ID nType, ushort nCode );
		
		virtual Event_Base* Clone();

		SE_Module_ID m_Type;
		ushort Code;
		std::string Backtrace;
//		SE_Array< SE_Module_ID > Modules_For; // if 0, global
	};


	template< class T >
	struct Event_Var:public Event_Base
	{
		Event_Var( SE_Module_ID nType, ushort ID, T Arg ) : Event_Base( nType, ID )
		{
			this->Argument = Arg;
		}

		Event_Base* Clone()
		{
			Event_Var<T>* nEv = new Event_Var<T>( this->m_Type, this->Code, this->Argument );
			return nEv;
		}
		//std::string To_String();
		T Argument;
	};

	template< typename T1, typename T2 >
	struct Event_Var2:public Event_Base
	{
		Event_Var2( SE_Module_ID nType, ushort ID, T1 Arg1, T2 Arg2 ) : Event_Base( nType, ID )
		{
			this->Argument1 = Arg1;
			this->Argument2 = Arg2;
		}

		Event_Base* Clone()
		{
			Event_Var2<T1, T2>* nEv = new Event_Var2<T1, T2>( this->m_Type, this->Code, this->Argument1, this->Argument2 );
			return nEv;
		}
		//std::string To_String();
		T1 Argument1;
		T2 Argument2;
	};

	struct Event_Manager_Base
	{
		SE_API Event_Manager_Base();
		SE_API ~Event_Manager_Base();

		SE_API void Cleanup();

		SE_API void Update();
		SE_API void Add_Event( Event_Base* Event );
		virtual bool Handle_Event( Event_Base* Event ) = 0; // return if it should be deleted


	private:
		SE_API void Pop_Event();

		SE_Thread::Mutex m_Mutex;
		std::queue<Event_Base*> Event_List;
	};

	struct Event_Manager: public Event_Manager_Base
	{
		SE_API Event_Manager();

		SE_API bool Handle_Event( Event_Base* Event );
		bool (*Processor)( Event_Base* Event );
	};
}


#endif
