#include "GUI.hpp"
using namespace SE_GUI;

#include "../engine/Engine.hpp"

#include "../utility/Draw_Utility.hpp"
using namespace Draw_Utility;

#include "../gl/se_gl.hpp"

namespace SE_Modules
{
	SE_Module_ID Mod_GUI = NULL;
}

namespace SE_GUI
{
	struct Reged_Widg
	{
		Widget* Widg;
		void* Parent;
	};

	// Globals
	segl_ID Tex_Button = 0;

	std::vector< Reged_Widg > Widget_List;
	std::vector< SE_GUI_Base* > Elements;
	SE_Thread::Mutex Elem_Mutex;

	void (*m_GUI_Background)() = NULL;

	// Functions
	
	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();

	uint Get_Resources( const std::vector<std::string>& mArgs );

	void Thread_Render();
	void Thread_Update();
}



Widget::Widget()
{
	this->Size.Set( 10, 10 );
	this->Gravity = SE_Gravity::Left_Top;
	this->Enabled = true;
	this->m_Texture = 0;
	this->Label_Gravity = SE_Gravity::Left_Top;
}



uint SE_GUI::Initialize( const std::vector<std::string>& mArgs )
{
	SE_GUI::GUI_Attatches::GUI_Attatch_CString* GUI_Bennett_S = new SE_GUI::GUI_Attatches::GUI_Attatch_CString;
	GUI_Bennett_S->To = "S_ENGINE BY BENNETT STEELE, DO NOT DISTRIBUTE";
	GUI_Bennett_S->Position.Set( 0, 0 );GUI_Bennett_S->Gravity = SE_Gravity::Left_Bottom;
	GUI_Bennett_S->Size = Vector2i( 640, 32 );//Resources.Font_Manager.Get_Str_Size( GUI_Bennett_S->To );//GUI_Bennett_S->Size.x += 4.f;
	GUI_Add( GUI_Bennett_S );

	return SE_SUCCESS;
}

uint SE_GUI::Cleanup()
{
	for( uint i = 0;i < Elements.size(); i++ )
	{
		Elements[i]->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( Elements[i] );
#endif
		delete Elements[i];
	}

	return SE_SUCCESS;
}

uint SE_GUI::Get_Resources( const std::vector<std::string>& mArgs )
{
	Tex_Button = Texture_Load( "gui/buttons.png" );

	return SE_SUCCESS;
}

void SE_GUI::Thread_Render()
{
	if( m_GUI_Background )
		m_GUI_Background();

	Elem_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	Elem_Mutex.Lock();

	for( uint i = 0;i < Elements.size(); i++ )
	{
		Elements[i]->Render();
	}

	Elem_Mutex.Unlock();

//	Message_Log.Draw();
}

void SE_GUI::Thread_Update()
{

}


void SE_GUI::Register_Widget( Widget* nWidg, void* Parent )
{
	assert( nWidg != NULL );

	Reged_Widg nWidg_Reg = { nWidg, Parent };
	Widget_List.push_back( nWidg_Reg );
}



/*SE_GUI_Base::SE_GUI_Base()
{
	Gravity = SE_Gravity::Left_Bottom;// default top left
}

uchar SE_GUI_Base::Type()
{
	return GUI_Types::Base;
}

void SE_GUI_Base::Update()
{
	Elem_Mutex.Wait_For_Lock( );// __FUNCTION__ );
	Elem_Mutex.Lock();

	for( uint i = 0;i < Elements.size(); i++ )
	{
		Elements[i]->Update();
	}

	Elem_Mutex.Unlock();

//	Message_Log.Update( dt );
}

void SE_GUI_Base::Draw()
{
	Draw_GUI_Box( this );
}

void SE_GUI_Base::Cleanup()
{

}*/
void SE_GUI::Set_GUI_Background( void (*Func)() )
{
	m_GUI_Background = Func;
}

using namespace GUI_Attatches;
GUI_Attatch_CString::GUI_Attatch_CString()
{
	To = NULL;
	Str_Grav = SE_Gravity::Left_Top;
}

uchar GUI_Attatch_CString::Type()
{
	return GUI_Types::Attatchment;
}

void GUI_Attatch_CString::Draw()
{
	Draw_GUI_Box( this );

	glPushMatrix();
	Draw_GUI_Translate( this );
	String_Draw( To, 0,4, 0,0,0,1, Str_Grav );
	glPopMatrix();
}



void SE_GUI::GUI_Add( SE_GUI_Base* Elem )
{
#ifdef _TEST
	Engine.Mem_Add( Elem, "gui" );
#endif
	Elem_Mutex.Wait_For_Lock( );// __FUNCTION__ );
	Elem_Mutex.Lock();

	Elements.push_back( Elem );

	Elem_Mutex.Unlock();
}

void SE_GUI::GUI_Remove( SE_GUI_Base* Elem )
{
	Elem_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	Elem_Mutex.Lock();

	for( uint i = 0;i < Elements.size(); i++ )
	{
		if( Elements[i] == Elem )
		{
			Elem->Cleanup();
#ifdef _TEST
			Engine.Mem_Remove( Elem );
#endif
			delete Elem;

			Elements.erase( Elements.begin() + i );
			i--;
		}
	}

	Elem_Mutex.Unlock();
}

#include "../utility/Draw_Utility.hpp"
#define IMG_S 256
void SE_GUI::Draw_GUI_Box( SE_GUI_Base* Elem )
{
	if( Elem == NULL ) return;

	glPushMatrix();

	Draw_GUI_Translate( Elem );
	segl_Use_Texture( Elem->m_Texture );//Engine.Resources.Resource_Req.GUI_Background );

	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
	glBegin( GL_QUADS );
		glTexCoord2f(0.f,				   Elem->Size.y() / IMG_S);glVertex2f( 0, Elem->Size.y() );
		glTexCoord2f(Elem->Size.x() / IMG_S, Elem->Size.y() / IMG_S);glVertex2f( Elem->Size.x(), Elem->Size.y() );
		glTexCoord2f(Elem->Size.x() / IMG_S, 					0.f);glVertex2f( Elem->Size.x(), 0 );
		glTexCoord2f(0.f, 									0.f);glVertex2f( 0, 0 );
	glEnd();


	glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
	glColor3f( 0, 0, 0 );
	glLineWidth( 3 );

	glBegin( GL_QUADS );
		glVertex2f( 0, 0 );
		glVertex2f( 0, Elem->Size.y() );
		glVertex2f( Elem->Size.x(), Elem->Size.y() );
		glVertex2f( Elem->Size.x(), 0 );
	glEnd();

	glColor3f( 1, 1, 1 );
	glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );

	glPopMatrix();
}

void SE_GUI::Draw_GUI_Translate( SE_GUI_Base* Elem )// translate by gravity
{
	if( Elem == NULL ) return;

	switch( Elem->Gravity )
	{
	case SE_Gravity::Left_Top:		glTranslatef( Elem->Position.x(), Elem->Position.y(), 0 );break;
	case SE_Gravity::Left_Mid:		glTranslatef( Elem->Position.x(), Elem->Position.y() + SE_Window::Get_H()/2.f, 0 );break;
	case SE_Gravity::Left_Bottom:	glTranslatef( Elem->Position.x(), SE_Window::Get_H() - Elem->Position.y() - Elem->Size.y(), 0 );break;
	case SE_Gravity::Mid_Top:		glTranslatef( Elem->Position.x() + SE_Window::Get_W()/2.f, Elem->Position.y(), 0 );break;
	case SE_Gravity::Mid_Mid:		glTranslatef( Elem->Position.x() + SE_Window::Get_W()/2.f, Elem->Position.y() + SE_Window::Get_H()/2.f, 0 );break;
	case SE_Gravity::Mid_Bottom:	glTranslatef( Elem->Position.x() + SE_Window::Get_W()/2.f, SE_Window::Get_H() - Elem->Position.y() - Elem->Size.y(), 0 );break;
	case SE_Gravity::Right_Top:		glTranslatef( SE_Window::Get_W() - Elem->Position.x() - Elem->Size.x(), Elem->Position.y(), 0 );break;
	case SE_Gravity::Right_Mid:		glTranslatef( SE_Window::Get_W() - Elem->Position.x() - Elem->Size.x(), Elem->Position.y() + SE_Window::Get_H()/2.f, 0 );break;
	case SE_Gravity::Right_Bottom:	glTranslatef( SE_Window::Get_W() - Elem->Position.x() - Elem->Size.x(), SE_Window::Get_H() - Elem->Position.y() - Elem->Size.y(), 0 );break;
	}
}


/*******************   MODULE OPERATIONS   ********************/
void SE_GUI::Register_Module()
{
	SE_Modules::Mod_GUI = S_Engine::Generate_Module( "gui" );
	S_Engine::Register_Module_Engine( SE_Modules::Mod_GUI, S_Engine::Reg_Mod_Time::MED_LEVEL, Initialize, Cleanup );
	S_Engine::Register_Module_Render( SE_Modules::Mod_GUI, S_Engine::Reg_Mod_Time::MED_LEVEL, Get_Resources, NULL );

	S_Engine::Register_Module_Thread_Render( SE_GUI::Thread_Render, S_Engine::Module_Positions::POS_MED, 32 );
	S_Engine::Register_Module_Thread_Logic( SE_GUI::Thread_Update, S_Engine::Module_Positions::POS_END, 255 );
}
