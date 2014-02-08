#include "Button.hpp"

#include "../math/Math.hpp"
#include "../gl/OpenGL.hpp"
#include "../utility/Draw_Utility.hpp"
#include "../os/Input.hpp"
#include "../os/Input_Codes.hpp"
#include "../module/Event_Defs.hpp"

namespace SE_GUI
{
	// Globals
	extern segl_ID Tex_Button;

	// Functions
	void Draw_Button( Button_Base* This );
}
using namespace SE_GUI;



void SE_GUI::Draw_Button( Button_Base* This )
{
	segl_Use_Texture( Tex_Button );//Engine.Resources.Resource_Req.GUI_Buttons );

	if( This->Hovering )
		glColor3f( 0.5f, 0.5f, 0.5f );//if in box

	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);			glVertex2i( This->Position.x()-This->Size.x(), This->Position.y()-This->Size.y() );
		glTexCoord2f(0.5f, 0 );		glVertex2i( This->Position.x()+This->Size.x(), This->Position.y()-This->Size.y() );
		glTexCoord2f(0.5f, 0.5f);	glVertex2i( This->Position.x()+This->Size.x(), This->Position.y()+This->Size.y() );
		glTexCoord2f(0, 0.5f);		glVertex2i( This->Position.x()-This->Size.x(), This->Position.y()+This->Size.y() );
	glEnd();

	String_Draw( This->Label_Text, This->Position.x()+18, This->Position.y(), 0,0,0,1 );
}



Button_Base::Button_Base()
{
	this->Hovering = false;
	this->Size.Set( 16, 16 );
}

uchar Button_Base::Type()
{
	return Widget_Types::TYPE_BUTTON;
}

void Button_Base::Update()
{
	// Is in box
	if( SE_Input::Get( SE_Input_Codes::MOUSE_X ) >= this->Position.x() - this->Size.x() + SE_Window::Get_W()/2.f && 
		SE_Input::Get( SE_Input_Codes::MOUSE_X ) <= this->Position.x() + this->Size.x() + SE_Window::Get_W()/2.f &&
		SE_Input::Get( SE_Input_Codes::MOUSE_Y ) >= this->Position.y() - this->Size.y() + SE_Window::Get_H()/2.f &&
		SE_Input::Get( SE_Input_Codes::MOUSE_Y ) <= this->Position.y() + this->Size.y() + SE_Window::Get_H()/2.f)
	{
		this->Hovering = true;
		if( SE_Input::Is_Down( SE_Input_Codes::MOUSE_LEFT ) )
			this->Click_Event();
	}
	else
		this->Hovering = false;
}



Button_Bool::Button_Bool( bool* Ptr )
{
	assert( Ptr != NULL );

	this->Value_Ptr = Ptr;
}

void Button_Bool::Click_Event()
{

}

void Button_Bool::Render()
{
	Draw_Button( this );

	// Is true
	if( *this->Value_Ptr )
		;// Draw as is
}




Button_Callback::Button_Callback()
{
	this->Func_Ptr = NULL;
	this->Data = NULL;
}

void Button_Callback::Click_Event()
{
	//
}

void Button_Callback::Render()
{
	Draw_Button( this );
}


Button_Menu::Button_Menu( const std::string& Text_Dpy, const std::string& Menu_Name, int x, int y )
{
	this->Label_Text = Text_Dpy;
	this->Position.Set( x, y );

	this->Size.Set( 16, 16 );

	this->Menu_Name = Menu_Name;
}

void Button_Menu::Click_Event()
{
	S_Engine::Event_Add_Engine( new SE_Events::Event_Var<std::string>( SE_Modules::Mod_GUI, Event_GUI::EVENT_MENU_LOAD, this->Menu_Name ) );	
}

void Button_Menu::Render()
{
	Draw_Button( this );
}
