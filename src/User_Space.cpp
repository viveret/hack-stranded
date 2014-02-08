/* The user should use these functions to hook their modules
 * or other data into the engine.
 *
 * This file is to hook modules into the engine to avoid
 * constructor fiasco. Just include the header, and call
 * its Register_Module function.
 *
 * Anything registered here has no module attatched.
 * Anything that required NO MODULE ID should be put here.
 */

#include "module/Module.hpp"
#include "module/Events.hpp"
#include "module/Event_Defs.hpp"

// Include Headers
#include "debug/Debug.hpp"
#include "module/Thread.hpp"
#include "app/Window.hpp"
#include "filesys/Filesystem.hpp"
#include "gl/OpenGL.hpp"
#include "gl/se_gl.hpp"
#include "menu/Menu.hpp"
#include "os/Input.hpp"
#include "render/Texture.hpp"
#include "render/Font.hpp"
#include "gui/GUI.hpp"
#include "physics/Physics.hpp"
#include "stranded/Stranded_Core.hpp"

SE_Menu::Menu_Base* User_Load_Menu( std::string Which );
void User_GUI_Background();

uint S_Engine::Module_Hook()
{
	SE_Debug::Register_Module();
	SE_Thread::Register_Module();
	SE_Window::Register_Module();
	SE_Filesys::Register_Module();
	SE_GL_Interfaces::Register_Module();
	segl::Register_Module();
	SE_Menu::Register_Module();
	SE_Input::Register_Module();
	SE_Texture::Register_Module();
	SE_Font::Register_Module();
	SE_GUI::Register_Module();
	SE_Physics::Register_Module();

	Stranded::Register_Module();

	SE_GUI::Set_GUI_Background( User_GUI_Background );

	SE_Physics::Set_Scene( SE_PHYS_SCENE_2 );

	return SE_SUCCESS;
}

#include "utility/Cipher.hpp"
uint S_Engine::User_Initialize( const std::vector<std::string>& mArgs )
{
	SE_Menu::Set_Load_Menu( User_Load_Menu );

	// Start on title screen
//	S_Engine::Event_Add_Engine( new SE_Events::Event_Var<std::string>( SE_Modules::Mod_GUI, Event_GUI::EVENT_MENU_LOAD, "title" ) );
			

	return SE_SUCCESS;
}

uint S_Engine::User_Cleanup()
{
	return SE_SUCCESS;
}


/************   MENU    ********************/
SE_Menu::Menu_Base* User_Load_Menu( std::string Which )
{
//	sePrintf( SEPRINT_DEBUG, "In user-set menu load\n" );
	if( Which == "title" )
	{
		SE_Menu::Menu_Base* nMenu = new SE_Menu::Menu_Base;

		nMenu->Title = "Stranded";

		nMenu->Make_Buttons(4);

		nMenu->Buttons[0] = new SE_GUI::Button_Menu("Singleplayer","singleplayer",-200,-100);
		nMenu->Buttons[1] = new SE_GUI::Button_Menu("Multiplayer","multiplayer",-200,-52);
		nMenu->Buttons[2] = new SE_GUI::Button_Menu("Options","options",-200,-4);
		nMenu->Buttons[3] = new SE_GUI::Button_Menu("Quit","quit",-200,92);

		return nMenu;
	}
/*	else if( Which == "singleplayer" )
	{
		SE_Menu::Menu_Base* nMenu = new SE_Menu::Menu_Base;

		nMenu->Title = "SURVIVOR";

		nMenu->Make_Buttons(2);


		nMenu->Buttons[0] = new Menu_Buttons::Button_Menu("Continue Story","single/continue",-200,-100);
		nMenu->Buttons[1] = new Menu_Buttons::Button_Menu("Back","back",-200,-52);


		return nMenu;
	}
	else if( Which == "single/continue" )
	{
		//
	}
*/
	return (SE_Menu::Menu_Base*)(-1);
}

uint Draw_Switch = 0;
void User_GUI_Background()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
	glOrtho( 0.f, SE_Window::Get_W(), SE_Window::Get_H(), 0, 0, 10 );
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT);
	segl_Disable(GL_DEPTH_TEST);
	segl_Disable( GL_BLEND );
	segl_Disable( GL_LIGHTING );

	if( Draw_Switch <= 60 )
		segl_Use_Texture( 1 );//Engine.Resources.Resource_Req.GUI_Buttons );
	else
		segl_Disable( SEGL_TEXTURE );

	segl_Set1p( SEGL_SHADER, NULL );

	glColor4f( 0.8f, 0.8f, 0.8f, 0.9f );
/* 
	glBegin(GL_QUADS);
 		glTexCoord2f(0, 0);			glVertex2d(0, 0 );
		glTexCoord2f(0.5f, 0);		glVertex2d(SE_Window::Get_W()/2.f,0 );
		glTexCoord2f(0.5f, 0.5f);	glVertex2d(SE_Window::Get_W()/2.f,SE_Window::Get_H()/2.f );
		glTexCoord2f(0, 0.5f);		glVertex2d(0,SE_Window::Get_H()/2.f );
	glEnd(); 
	glClearColor( 1, 1, 1, 1 );
*/
//	if( Draw_Switch++ > 120 )
//		Draw_Switch = 0;
}
