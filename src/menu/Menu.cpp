#include "Menu.hpp"   
#include "../gl/se_gl.hpp"
#include "../module/Thread.hpp"
#include "../module/Events.hpp"
#include "../module/Event_Defs.hpp"
#include "../app/App.hpp"

#include "../os/Input.hpp"
#include "../os/Input_Codes.hpp"

namespace SE_Menu
{
/**********   GLOBALS   ************/
	Vector3f Splash_Color( 0, 0, 0 );

	Menu_Base* Cur_Menu = NULL;
	bool Changing_Menu = false;//prep for new menu
	
	bool Ready_Draw = false;
	bool Ready_Logic = false;

	std::vector< std::string > Menu_Trace;
/**********    FUNCTIONS   *********/
	void Wait_For_Threads( bool State );

	// METHODS TO LOAD MENUS  
	void Load_Menu( std::string Which );

	Menu_Base* Load_Menu_XML( std::string Which );
	Menu_Base* Load_Menu_TXT( std::string Which );
	Menu_Base* Load_Menu_BIN( std::string Which );

	// USER SET
	Menu_Base* (*Load_Menuinternal)( std::string ); // No suffix

	// Modules
	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();

	void Thread_Engine();
	void Thread_Update();
	void Thread_Render();

	bool Event_Engine( SE_Events::Event_Base* nEv );
}
using namespace SE_Menu;



void SE_Menu::Thread_Update()
{
	if( Changing_Menu == false )
	{
		Ready_Logic = false;

		if( Cur_Menu )
			Cur_Menu->Update();
	}
	else if( Changing_Menu == true ) 	   
		Ready_Logic = true;
}

void SE_Menu::Thread_Render()
{
	if( Changing_Menu == false )
	{		  
		Ready_Draw = false; 

		glColor4f( 1, 1, 1, 1 );
		if( Cur_Menu )
			Cur_Menu->Draw();
	}
	else if( Changing_Menu == true )
		Ready_Draw = true;
}

bool SE_Menu::Event_Engine( SE_Events::Event_Base* nEv )
{
	if( nEv->m_Type == SE_Modules::Mod_GUI )
	{
		switch( nEv->Code )
		{
		case Event_GUI::EVENT_MENU_LOAD:
			{
				Load_Menu( ((SE_Events::Event_Var<std::string>*)nEv)->Argument );
			}break;
		}
	}
	return true;
}


// User set function
void SE_Menu::Set_Load_Menu( Menu_Base*(*Func)(std::string) )
{
	Load_Menuinternal = Func;
}

/*void Menu_Manager::Load_Ext_Menu(std::string Which)
{
	Engine.Application.Input.Mouse_L_Down = false;
													
	this->Menu_Trace.push_back( Which );

	this->Changing_Menu = true;//to let us know its external 	
	Engine.Application.Error_Log.Write_Log("Ext Menu Start");

	//while( this->Ready_Draw == false );
	
	if( E_Threads::Get_Thread() != THREAD_ENGINE )// wrong thread buddy
	{												
		E_Events::Event_Base* nEvent = new E_Events::Event_Base;
		nEvent->Type = E_Events::Event_Types::TYPE_MENU;
		nEvent->Code = E_Events::Events::EVENT_M_LOAD_EXT;

		Engine.Event_Manager.Add_Event( nEvent, "Ext Menu Load" );
	}
	else
		this->Load_Ext_Menu_Cont();//this->Load_Int_Base_Cont();
}			*/

/*#include "../utility/Functions.h"
void SE_Menu_Manager::Cleanup()
{			
	if( SE_Thread::Get_Thread() == Engine.Thread_Engine )// can just go to the next stage
		this->Cleanup_Cont();			 
	else
	{		 
		Engine.Log( "Cleanup Menu Event", Log_Type::TYPE_DBUG );												

		SE_Events::Event_Base* nEvent = new SE_Events::Event_Base;
		nEvent->Type = SE_Events::Event_Types::TYPE_MENU;
		nEvent->Code = SE_Events::Events::EVENT_M_CLEANUP;
			
		Engine.Event_Manager.Add_Event( nEvent, "Cleanup Menu" );
	}
}*/

/****************   LOAD MENU OPERATIONS   ********************/
void SE_Menu::Load_Menu( std::string Which )
{		   	  					
	Changing_Menu = true;	

/* 	if( E_Threads::Get_Thread() != Engine.Thread_Engine )// wrong thread buddy
	{						
		Engine.Application.Input.Mouse_L_Down = false;

		Engine.Log( "Load Menu Event\n", Log_Type::TYPE_DBUG );
									
		SE_Events::Event_Str* nEvent = new SE_Events::Event_Str;
		nEvent->Type = SE_Events::Event_Types::TYPE_MENU;
		nEvent->Code = SE_Events::Events::EVENT_M_LOAD;
		nEvent->Argument = Which;

		Engine.Event_Manager.Add_Event( nEvent, "Load Int Menu" );	

		return;
	}
*/		

	SE_Input::Set( SE_Input_Codes::MOUSE_LEFT, false );

	//this->Changing_Menu = true;
	Wait_For_Threads( true );

	if( Cur_Menu )
	{	
//		sePrintf( SEPRINT_DEBUG, "Load Menu Cleaning\n" );

		Cur_Menu->Clean_Up();
//		Engine.Mem_Remove( Cur_Menu );

		delete Cur_Menu;
		Cur_Menu = NULL;
	}

	sePrintf( SEPRINT_MODULE, "Load Menu Start: %s\n", Which.c_str() );

	// check menu commands first
	if( Which == "back" )
	{
		if( Menu_Trace.size() <= 1 )
			sePrintf( SEPRINT_ERROR, "Must Have Menu To Go Back To\n" );

		Menu_Trace.pop_back();//remove current menu

		std::string Last = Menu_Trace[Menu_Trace.size()-1];//get last menu		   
		Menu_Trace.pop_back();//remove last menu

		Load_Menu( Last ); 
		return;
	}
	else if( Which == "quit" )
	{			  
		sePrintf( SEPRINT_INFO, "Shutdown By Menu\n" );
		S_Engine::Event_Add_Engine( new SE_Events::Event_Base( NULL, Event_Module::EVENT_SHUTDOWN ) );
		return;
	}
	else if( Which == "resume" )
	{	  
//		Engine.Enter_Game();
		return;
	}


	Menu_Base* nMenu = (Menu_Base*)(-1);

	// check load file format
	if( Which.find( ".xml" ) != Which.npos )// is a xml file								
		nMenu = Load_Menu_XML( Which );

	else if( Which.find( ".txt" ) != Which.npos )// is a txt file		
		nMenu = Load_Menu_TXT( Which );

	else if( Which.find( ".ss" ) != Which.npos )// is a survivor script					
		;//Menu_Base* nMenu = this->Load_Menu_SS( Which );

	else if( Which.find( ".bin" ) != Which.npos )// is a compiled file
		nMenu = Load_Menu_BIN( Which );

	else if( Load_Menuinternal )// use internal
		nMenu = Load_Menuinternal( Which ); // can also include a command, if NULL

	
	if( nMenu == (Menu_Base*)(-1) ) // error   
	{
		sePrintf( SEPRINT_ERROR, "Failed To Load Menu: %s\n", Which.c_str() );

		Menu_Trace.push_back("ERROR");
		//this->Menu_Trace.pop_back();
		Load_Menu( "back" );
		return;
	}
	else if( nMenu != NULL )// is a valid menu
	{
		Cur_Menu = nMenu;
//		Engine.Mem_Add( nMenu, "new menu" );
		Cur_Menu->Add_Buttons_Mem();// add buttons to memory manager

		Menu_Trace.push_back( Which );
	}
	

	Changing_Menu = false; 
	Wait_For_Threads( false );

	sePrintf( SEPRINT_MODULE, "Finished Menu: %s\n", Which.c_str() );
}



// METHODS TO LOAD MENUS   
Menu_Base* SE_Menu::Load_Menu_XML( std::string Which )
{	
	SE_Application::XML_File F_Data; 
#ifdef _TEST
	if( Engine.Application.Input.Load_XML_File(&F_Data, "screen/menus/" + Which)==false )
	{
		Engine.Register_Error(S_Error("Could not load XML menu: " + Which, 2 ));
		return (Menu_Base*)(-1);
	}

	E_Application::XML_Node Data = F_Data.child("menu");

	Menu_Base* nMenu = new Menu_Base;

	nMenu->Title = Data.attribute("title").as_string();

	uchar Num_Buttons = 0;
	// get number of parts
	for(pugi::xml_node nButt = Data.child("button"); nButt; nButt = nButt.next_sibling("button"))
		Num_Buttons++;

	nMenu->Make_Buttons( Num_Buttons );
	
	
	// get parts						   
	uint i = 0; // keep track of what part at
	for (pugi::xml_node nButt = Data.child("button"); nButt; nButt = nButt.next_sibling("button"))
	{	
		std::string bType = nButt.attribute("type").as_string();
		std::string bShow = nButt.attribute("show").as_string();

		if( bType == "menu" )
			nMenu->Buttons.Data()[i] = new Button_Menu( bShow, nButt.attribute("goto").as_string(),
												 nButt.attribute("x").as_int(), nButt.attribute("y").as_int() );

		i++;
	}


	return nMenu;
#endif
	return NULL;
}

Menu_Base* SE_Menu::Load_Menu_TXT( std::string Which )
{
	//						
	return (Menu_Base*)(-1);
}

Menu_Base* SE_Menu::Load_Menu_BIN( std::string Which )
{
	//						
	return (Menu_Base*)(-1);
}


/***********************   MODULE OPERATIONS    ****************/

uint SE_Menu::Initialize( const std::vector<std::string>& mArgs )
{

	return SE_SUCCESS;
}

uint SE_Menu::Cleanup()
{
	if( Cur_Menu )
	{				
		sePrintf( SEPRINT_DEBUG, "Cleanup Menu Waiting\n" );
		Wait_For_Threads( true );

		sePrintf( SEPRINT_DEBUG, "Cleaning Menu\n" );
		Cur_Menu->Clean_Up();

//		Engine.Mem_Remove( this->Cur_Menu );
		delete Cur_Menu;
		Cur_Menu = NULL;

		Menu_Trace.clear();

		sePrintf( SEPRINT_MODULE, "Cleaned up Menu\n" );
	}

	Changing_Menu = false;
	Wait_For_Threads( false );

	return SE_SUCCESS;
}

#ifndef _WIN32
#include <unistd.h>
#endif

void SE_Menu::Wait_For_Threads( bool State )
{
	// Engine.Is_Running == false || 
	if( Cur_Menu == NULL || Changing_Menu != State )// dont wait for memory not there
		return;

	sePrintf( SEPRINT_DEBUG, "Load Menu Waiting\n" );

	// && Engine.Thread_Draw.Running    && Engine.Thread_Logic.Running
	while( Ready_Draw != State || Ready_Logic != State )
#ifdef _WIN32
		Sleep(0);
#else
		usleep(0);
#endif
}

namespace SE_Modules
{
	SE_Module_ID Mod_Menu = NULL;
}

/**************    MODULE OPERATION    ********************/
void SE_Menu::Register_Module()
{	
	SE_Modules::Mod_Menu = S_Engine::Generate_Module( "menu" );
	S_Engine::Register_Module_Engine( SE_Modules::Mod_Menu, S_Engine::Reg_Mod_Time::MED_LEVEL, SE_Menu::Initialize, SE_Menu::Cleanup );

	S_Engine::Register_Module_Thread_Render( Thread_Render, S_Engine::Module_Positions::POS_LATE, 5 );
	S_Engine::Register_Module_Thread_Logic( Thread_Update, S_Engine::Module_Positions::POS_LATE, 5 );

	S_Engine::Register_Module_Event_Engine( SE_Modules::Mod_Menu, Event_Engine );
}
