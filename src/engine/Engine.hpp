/*
	This is the survivor engine.
	It holds and controls everything.

	the app is what is needed to run, the core

	the game is what you play in, changing alot
	the screen is also something you move around in, is overlayed on the game, if one.

	the resources can be app && screen: never change
	or game content: always changing

*/
#ifdef _TEST
#ifndef SURVIVOR_ENGINE
#define SURVIVOR_ENGINE

#include "../Core.h"

#include "../module/Events.h"
#include "../engine/Console.h"
#include "../app/App.h"
#include "../game/Game.h"
#include "../network/Net.h"
#include "../menu/Menu.h"
#include "../gui/GUI.h"
#include "../utility/Lua.h"
#include "../content/Resources.h"
#include "../os/Bass.h"
#include "../content/Content.h"
#include "../os/Threader.h"
#include "../os/Input.h"
#include "../debug/Debug.h"
#include "../engine/Timer.h"
#include "../app/File_Stream.h"

#include <stdio.h>
#include <stdarg.h>

struct Engine_Descriptor
{
	std::string Name;

#ifdef _WIN32
	HINSTANCE hInstance;
#endif
	bool Single_Inst;

	E_Resources::R_Other_Resources* nRes;
	void (*Input_Event_Handler)(SE_Input::Input_Event*);

	std::string Def_Font;uint Def_Font_Size;
	std::string Pause_Name;
};


struct S_Engine_Version_Desc
{
	std::string OS, OS_Version;

	std::string Build_Target_OS;
	std::string Build_Time;
	std::string Build_Date;
	std::string Build_Name;

	std::string As_String();
};

struct S_Engine
{
	S_Engine();
	~S_Engine();

	void Start( Engine_Descriptor& nDesc, char argc, char** argv );
	void Start_Handle_Args( char argc, char** argv );
	bool Start_Is_Single_Instance();// check if already running, single instance

	void Shutdown();
	void Exit();//actual destructor
	void Register_Error(S_Error nError);//do error stuff

#ifdef _WIN32
	bool Win32_Assert( std::string Info, u_char Level );
	bool WSA_Assert( std::string Info, u_char Level );
#else
	int X11_Error( Display* dpy, XErrorEvent* xev );
#endif

	bool OpenGL_Assert( std::string Info, u_char Level );
	bool BASS_Assert( std::string Info, u_char Level );
	bool Assert_IL( std::string Info, u_char Level );

	void Wind_Command(unsigned int Com);

	//void Thread_Handle();

	/*    ENGINE COMPONENTS    */

	SE_Events::Event_Manager Event_Manager;

	static void Event_Draw_Processor( SE_Events::Event_Base* Event );
	static void Event_Logic_Processor( SE_Events::Event_Base* Event );
	static void Event_Main_Processor( SE_Events::Event_Base* Event );

	void Handle_Logic_Event_Ex( SE_Events::Event_Base* Event );

	S_Console::C_Console Console;
	void Log( std::string Message, uchar Type = 0 );
	void Printf( const char* format, uchar L_Type = 0, ... );
	std::string Log_Buf;
	void Log_Color( uchar Type );
	bool Log_No_Color;

	E_Application::A_Application Application;
	SE_Input::Input_Manager Input;
	SE_Network::SE_Net_Manager_Base* Net_Manager;

	E_Menu::Menu_Manager Menu;
	SE_GUI::SE_GUI_Manager GUI_Manager;
	E_Lua::L_Lua_Handler Lua_Handler;

	E_Game::G_Game* Game;

	E_Resources::R_Resources Resources;
	E_Bass::B_Bass_Manager Bass_Manager;
	E_Content::C_Content_Manager Content;
	SE_File_Stream::File_Stream_Manager Stream_Manager;
	//E_Debug::D_Debug Debug;

	/*   ENGINE THREADS   */
	E_Threads::Thread_ID_Type Thread_Engine;
	Timer E_Timer;

	E_Threads::T_Thread Thread_Draw;
	E_Threads::T_Thread Thread_Logic;
	//E_Threads::T_Thread Thread_Events;
	//E_Threads::T_Thread Thread_Resource;
	E_Threads::T_Thread Thread_XBox;
	E_Threads::T_Thread Thread_Input;

	/*   DEFAULT ENGINE FUNCTIONS   */

#ifdef _WIN32
	LRESULT CALLBACK MainWindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#else
	void Win_X11_Events();
#endif


	void Draw();//called by draw thread
	void Logic();//math, etc, called by logic thread
	void Main_Loop();//in main

	/*    ENGINE FUNCTIONS    */
	void Focus( bool Gained );

	void Make_Game( E_Game::G_Game* n_Game );//make a new-default level to access
	void Enter_Game();//go into game
	void Enter_Screen();//go into menu

	void Pause_Game();//get ready to enter menu
	void Resume_Game();//get ready to enter game

	void Make_Net_Manager( bool Be_Host );// make net manager, if should be host / client

	/*    ENGINE VARIABLES    */

	void Get_Options();


	bool In_Game, Is_Running, Is_Paused;


	void Mem_Add( void* What, std::string Name );
	void Mem_Remove( void* What );
	void Mem_List();// list current memory allocs


	DTX::dtx_font* SE_Font;


	S_Engine_Version_Desc* Get_Version();
	std::string Get_CWD();
	std::string Get_EXE_Name();

private:
	E_Threads::Mutex Mem_Mutex;
	std::vector< void* > Memory_Allocs;
	std::vector< std::string > Memory_Names;

	bool Ready_Exit;

	S_Engine_Version_Desc m_Version;

	// engine user-def details
	std::string Menu_Load_Pause;
};

//extern S_Engine Engine;

#endif
#endif
