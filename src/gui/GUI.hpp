#ifndef SE_GUI_CORE
#define SE_GUI_CORE

#include <vector>
#include <string>

#include "../Core.hpp"
#include "../math/Math.hpp"
#include "../os/Threader.hpp"

#include "Message_Log.hpp"
#include "Widget.hpp"


namespace SE_GUI
{
	void Register_Widget( Widget* nWidg, void* Parent );

	namespace GUI_Types
	{enum {
		Base = 0, // nothing else
		Container,
		Singleton,// nothing else, simple
		Attatchment
	};}

/*	struct SE_GUI_Base// by default is just a background
	{
		SE_GUI_Base();

		Vector2f Position, Size;
		uchar Gravity;

		virtual uchar Type();

		virtual void Update();
		virtual void Draw();
		virtual void Cleanup();
	};
*/
	typedef Widget SE_GUI_Base;


	struct SE_GUI_Button_Base: public SE_GUI_Base
	{
		// inherited but not defined: draw
		void Update();

		virtual void Clicking() = 0;// only used while mouse is down and in bounding box
	};

	namespace GUI_Attatches
	{
		struct GUI_Attatch_String: public SE_GUI_Base
		{
			GUI_Attatch_String();
			void Cleanup();

			void Render();

			std::string* To;
		};
		struct GUI_Attatch_CString: public SE_GUI_Base
		{
			GUI_Attatch_CString();
			//inherited but not defined: update, cleanup
			uchar Type();

			void Draw();

			char* To;
			uchar Str_Grav;
		};
	}


	void Draw_GUI_Box( SE_GUI_Base* Elem );// draw both background and border
	void Draw_GUI_Translate( SE_GUI_Base* Elem );// translate by gravity

	void GUI_Add( SE_GUI_Base* Elem );
	void GUI_Remove( SE_GUI_Base* Elem );

//		SE_Message_Sys::SE_Message_Manager Message_Log;

	void Set_GUI_Background( void (*Func)() );

	void Register_Module();
}



#endif
