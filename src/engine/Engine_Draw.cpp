#include "../engine/Engine.hpp"

#include "../utility/Draw_Utility.hpp"
using namespace Draw_Utility;

#ifdef _TEST


void S_Engine::Draw()
{
#ifndef _WIN32
	XLockDisplay( this->Application.Window.m_dpy );
#endif

	this->Event_Manager.Draw_Thread.Update();
	this->Resources.Update();// update draw resources


	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glLoadIdentity();
    glClearColor(0,0,0,0);


	glPushMatrix();
	if(this->In_Game && this->Game)
	{
		this->Game->Draw();
		this->Game->Draw_HUD();
	}
	else
	{
		//DRAW SCREEN
		this->Menu.Draw();//Screen.Draw();
	}
	glPopMatrix();

	this->GUI_Manager.Draw();

	this->Console.Draw();


	String_Draw( this->Application.Input.Typing_What, ScreenW/2.f,0,1,1,1,1.f, SE_GUI::GUI_Gravity::Right_Bottom );


    glFlush();

#ifdef _WIN32
	SwapBuffers(this->Application.Window.hDC);
#else
    glXSwapBuffers(this->Application.Window.m_dpy, this->Application.Window.m_win);
	XUnlockDisplay( this->Application.Window.m_dpy );
#endif
}

#endif
