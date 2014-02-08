#ifdef _WIN32_NOPE

#ifndef SURVIVOR_ENGINE_APP_WIN32
#define SURVIVOR_ENGINE_APP_WIN32

#include "App.hpp"


#define WIN32_WIN_CLASS_NAME "Survivor_Engine_Window"

namespace SE_App_Win32
{
	struct A_Win32_Win_Desc:public E_Application::A_Platform_Interface::AP_Win_Desc
	{
		A_Win32_Win_Desc();

		HINSTANCE nhInstance;
	};

	struct A_Win32_Window:public E_Application::A_Platform_Interface::AP_Window
	{
		A_Win32_Window();

		bool Create_Window( AP_Win_Desc* Desc );
		void Destroy_Window();


		HGLRC hEng_RC,hDraw_RC;//  rendering context, links opengl calls to engine and draw threads
		HDC hDC;//    device context         draw to the window
	    HWND hWnd;//  window handle
		HINSTANCE hInstance;//               instance of application
	};
}

#endif

#endif // win32
