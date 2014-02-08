#ifndef SURVIVOR_ENGINE_MESSAGE_SYSTEM
#define SURVIVOR_ENGINE_MESSAGE_SYSTEM

#include "../Core.hpp"
#include "../math/Math.hpp"
#include "../module/Thread.hpp"
#include "../module/Events.hpp"
#include <vector>
#include <string>

namespace SE_Message_Sys
{
	struct SE_Message
	{
		SE_Message();

		std::string Source;
		Vector3f Source_Color;

		std::string What;
		Vector3f What_Color;

		float Timer;
	};

	struct SE_Message_Event:public SE_Events::Event_Base, public SE_Message
	{
		SE_Message_Event();

		SE_Events::Event_Base* Clone();
		void Set( SE_Message& Msg );
	};

	struct SE_Message_Manager
	{
		SE_Message_Manager();

		void Update( double dt );
		void Draw();

		void Print_Message( SE_Message& nMsg );
		void Add_Message( SE_Message& nMsg );

		void Format_Message( SE_Message* Msg );

		// attributes
		float Show_Delay;
		uchar Line_Height, Max_W;

		uchar Remove_Style; //

	private:

		std::vector< SE_Message > Messages;
		SE_Thread::Mutex m_Mutex;
	};

	namespace Remove_Styles
	{enum {
		STYLE_NONE  = 0,
		STYLE_FADE_DYNAMIC, // fade away removed messages
		STYLE_FADE_STATIC // fade as it passes height
	};}
}



#endif
