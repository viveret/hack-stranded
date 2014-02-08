#include "../menu/Menu.hpp"
using namespace SE_Menu;
#include "../menu/Menu_Buttons.hpp"
//using namespace Menu_Buttons;
//using namespace Button_Var;
#include "../engine/Engine.hpp"



/*void Menu_Manager::Load_Int_Menu( std::string Which )
{
	Engine.Application.Input.Mouse_L_Down=false;
	
	this->Menu_Trace.push_back(Which);

	this->Changing_Menu = true;		

	//while( this->Ready_Draw == false );
	
	if( E_Threads::Get_Thread() != THREAD_ENGINE )// wrong thread buddy
	{														
		E_Events::Event_Base* nEvent = new E_Events::Event_Base;
		nEvent->Type = E_Events::Event_Types::TYPE_MENU;
		nEvent->Code = E_Events::Events::EVENT_M_LOAD_INT;	

		Engine.Event_Manager.Add_Event( nEvent, "Load Int Menu" );	
	}
	else							 
		this->Load_Int_Base_Cont();
}


	// RECOMPILE ALL MENUS



//void Menu_Manager::Load_Int_Base_Cont()
//{		
	/*std::string Last = this->Menu_Trace[this->Menu_Trace.size()-1];


	Menu_Base* nMenu = this->Load_Menuinternal( this->Menu_Trace[this->Menu_Trace.size()-1] );
				 
		if( nMenu == (Menu_Base*)(-1) )    
			this->Load_Menu( "back" );

		else if( nMenu != NULL )// is a valid menu
		{
			this->Cur_Menu = nMenu;
			Engine.Mem_Add( nMenu, "new menu" );
		}

	

	this->Changing_Menu = false; 
	this->Wait_For_Threads( false );		  */
//}
