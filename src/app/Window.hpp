#ifndef SURVIVOR_ENGINE_APP_WINDOW
#define SURVIVOR_ENGINE_APP_WINDOW


#include "../Types.hpp"
#include <string>


namespace SE_Window
{
	void Reshape( uint w, uint h );
	void Move( uint x, uint y );
	void Set_Name( std::string Name );

	uint Get_W();
	uint Get_H();

	void Register_Module();

	void Frame_Start();
	void Frame_End();

#ifdef _WIN32
	HWND Get_HWND();
	LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
	bool Win32_Assert( std::string Info, uint Level );
#endif
}




#endif
