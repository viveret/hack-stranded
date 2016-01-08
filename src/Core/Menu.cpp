#include <SE/gl/segl.hpp>
#include <SE/Window.hpp>

#include "Stranded_Core.hpp"

namespace Stranded_Lib
{
	extern SE_GUI_Interface* SE_GUI;
	extern segl_Interface* segl;
	extern SE_Window_Interface* SE_Window;
}

/************   MENU    ********************/
using namespace SE_GUI_Lib;
SE_GUI_Lib::Widget_Container* Stranded_Lib::User_Load_Menu( std::string Which )
{
//	sePrintf( SEPRINT_DEBUG, "In user-set menu load\n" );
	if( Which == "title" )
	{
		SE_GUI_Lib::Widget_Container* nMenu = (SE_GUI_Lib::Widget_Container*) SE_GUI->Generate_Widget( "menu", NULL );

		nMenu->Label_Text = "Stranded";
		nMenu->Label_Color.set( 0.2, 0.4, 1 );

		Widget_Menu_Button* nButton = (Widget_Menu_Button*)SE_GUI->Generate_Widget( "menu_button", nMenu );
		nButton->Label_Text = "Singleplayer"; nButton->Goto = "singleplayer";
		nButton->Position.set( -106, -100 );
		nButton->Size.set( 16, 16 );
		nButton->Label_Color.set( 0.2, 0.4, 1 );

		nButton = (Widget_Menu_Button*)SE_GUI->Generate_Widget( "menu_button", nMenu );
		nButton->Label_Text = "Quit"; nButton->Goto = "quit";
		nButton->Position.set( -106, 92 );
		nButton->Size.set( 16, 16 );
		nButton->Label_Color.set( 0.2, 0.4, 1 );

		return nMenu;
	}
	else if( Which == "singleplayer" )
	{
		Stranded_Lib::Start_Game();
		return NULL;
	}
/*	else if( Which == "single/continue" )
	{
		//
	}
*/
	return (SE_GUI_Lib::Widget_Container*)(-1);
}

void Stranded_Lib::User_GUI_Background()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho( 0.f, SE_Window->Get_W(), SE_Window->Get_H(), 0, 0, 10 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT);
	segl_Disable(GL_DEPTH_TEST);
	segl_Disable( GL_BLEND );
	segl_Disable( GL_LIGHTING );
}
