#ifndef SURVIVOR_ENGINE_DRAW_UTILITY
#define SURVIVOR_ENGINE_DRAW_UTILITY

#include <string>
#include "../Core.hpp"


namespace Draw_Utility
{	
	void String_Draw_Core(std::string txt,int x,int y,float r,float g,float b,float a, uchar Pos = 0, float Scale = 1.f);
    void String_Draw(std::string txt,int x,int y,float r,float g,float b,float a, uchar Pos = 0, float Scale = 1.f);

	void Splash(std::string Info);
}
using namespace Draw_Utility;

												
//extern uint Engine.Application.Window.Wind_w
//extern uint Engine.Application.Window.Wind_h
#include "../app/Window.hpp"
#define ScreenW SE_Window::Get_W()
#define ScreenH SE_Window::Get_H()

#endif
