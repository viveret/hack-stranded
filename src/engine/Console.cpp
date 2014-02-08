#include "Console.hpp"
using namespace S_Console;

#include "../S_Engine.hpp"

#include "../utility/Draw_Utility.hpp"
using namespace Draw_Utility;


C_Line::C_Line( std::string Text, Vector3f Color )
{
	this->Text = Text;
	this->Color = Color;
}

C_Console::C_Console()
{
	this->Scroll_Line = 0.f;// where to draw
	this->Is_Open = false;
	this->Open_Anim = 0.f;
}

void C_Console::Initialize()
{
	//
}


#define CONSOLE_SIZE 480
void C_Console::Draw()
{
	if( this->Open_Anim != 0.f )
	{
		glPushMatrix();

		glTranslatef( 0, CONSOLE_SIZE * this->Open_Anim - CONSOLE_SIZE, 0 );

		segl_Disable( GL_TEXTURE_2D );

		glColor4f( 0, 0, 0, 1.f );
		glBegin( GL_QUADS );							   
			glVertex2f( 0,				 	 0 );
			glVertex2f( SE_Window::Get_W(),  0 );
			glColor4f( 0, 0, 0, 0.75f );
			glVertex2f( SE_Window::Get_W(), CONSOLE_SIZE );
			glVertex2f( 0,				    CONSOLE_SIZE );
		glEnd();
		glColor4f( 1, 1, 1, 1 );

	
#ifdef _TEST
		std::stringstream Engine_Timer;Engine_Timer<<"Engine Ticks: "<<Engine.E_Timer.Average;
		String_Draw(Engine_Timer.str(),ScreenW,0,1,1,1,1.f, SE_GUI::GUI_Gravity::Right_Bottom);

		std::stringstream Logic_Timer;Logic_Timer<<"Logic Ticks: "<<Engine.Thread_Logic.m_Timer.Average;
		String_Draw(Logic_Timer.str(),ScreenW,16,1,1,1,1, SE_GUI::GUI_Gravity::Right_Bottom);

		std::stringstream Draw_Timer;Draw_Timer<<"Draw Ticks: "<<Engine.Thread_Draw.m_Timer.Average;
		String_Draw(Draw_Timer.str(),ScreenW,32,1,1,1,1, SE_GUI::GUI_Gravity::Right_Bottom);
#endif
							 
		this->m_Mutex.Wait_For_Lock( );// __FUNCTION__ );
		this->m_Mutex.Lock();
					   // this->Scroll_Line - CONSOLE_SIZE / 16    // && (this->Lines.size() - 1 - i + this->Scroll_Line) < this->Lines.size()
		for( uint i = 0;i < CONSOLE_SIZE / 16; i++ )
		{
			uint nI = (uint)this->Scroll_Line + i;// + CONSOLE_SIZE / 16;
			if( nI < this->Lines.size() )
				String_Draw( this->Lines[nI].Text, 0, i*16, this->Lines[nI].Color[0], this->Lines[nI].Color[1], this->Lines[nI].Color[2], 1 );
		}

		this->m_Mutex.Unlock();


		glColor4f( 1, 1, 1, 1 );

		glPopMatrix();
	}
}

#define CONSOLE_SPEED 0.0075f
#define CONSOLE_SCROLL 3
void C_Console::Update( dt_precision dt )
{								  
	if( this->Is_Open && this->Open_Anim < 1.f )
	{
		this->Open_Anim += dt * CONSOLE_SPEED;
		if( this->Open_Anim > 1.f )
			this->Open_Anim = 1.f;
	}
	else if( this->Is_Open == false && this->Open_Anim > 0.f )
	{
		this->Open_Anim -= dt * CONSOLE_SPEED;
		if( this->Open_Anim < 0.f )
			this->Open_Anim = 0.f;
	}

	if( this->Is_Open )
	{
#ifdef _TEST
		this->Scroll_Line -= Engine.Application.Input.Mouse_Scroll * CONSOLE_SCROLL;
#endif
		if( this->Scroll_Line < 0.f )
			this->Scroll_Line = 0.f;

#ifdef _TEST
		Engine.Application.Input.Mouse_Scroll = 0;
#endif
	}
}


void C_Console::Add_Line( std::string Line )
{
	float nCol[] = {1.f, 1.f, 1.f};
	this->Add_Line( Line, Vector3f( nCol ) );
}

#define CONSOLE_MAX_LINES 256

void C_Console::Add_Line( std::string Line, Vector3f Color )
{
	if( Line.length() > 0 )
	{
		this->m_Mutex.Wait_For_Lock( );// __FUNCTION__ );
		this->m_Mutex.Lock();

		this->Lines.push_back( C_Line( Line, Color ) );
		//this->Scroll_Line += 1.f;

		while( Line.size() > CONSOLE_MAX_LINES )
		{
			Line.erase( Line.begin() );
			//this->Scroll_Line -= 1.f;
		}

		this->m_Mutex.Unlock();
	}
}
