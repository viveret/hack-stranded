#ifndef SURVIVOR_ENGINE_GUI_BUTTON
#define SURVIVOR_ENGINE_GUI_BUTTON

#include "Widget.hpp"

namespace SE_GUI
{
	struct Button_Base: public Widget
	{
		Button_Base();
		uchar Type();

		Vector2ub Size;
		bool Hovering;

		void Update();

		virtual void Click_Event() = 0; // Called by Event_Click_Base
		// Render must also be declared
	};

	struct Button_Bool: public Button_Base
	{
		Button_Bool( bool* Ptr );

		void Click_Event();
		void Render();

		private:
		bool* Value_Ptr;
	};

	struct Button_Callback: public Button_Base
	{
		Button_Callback();

		void Click_Event();
		void Render();

		void (*Func_Ptr)( void* );
		void* Data;
	};

	struct Button_Menu: public Button_Base
	{
		Button_Menu( const std::string& Text_Dpy, const std::string& Menu_Name, int x, int y );

		void Click_Event();
		void Render();

		std::string Menu_Name;
	};
}



#endif
