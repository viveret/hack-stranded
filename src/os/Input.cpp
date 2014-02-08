#include "Input.hpp"
#include "Input_Codes.hpp"

//#include "../engine/Engine.h"

#ifndef _WIN32
#include<X11/X.h>
#include<X11/Xlib.h>
#include <X11/keysym.h>
#endif

namespace SE_Input
{
	struct Bound_Input;

/* *********    GLOBALS     ******** */
	sshort Button_Buffer[ SE_Input_Codes::BUTTON_LAST + 1 ];
	Timer Poll_Timer;

	std::vector< Bound_Input > Bound_Buttons;

/********    INPUT OPERATIONS    ********/


/******    MODULE OPERATIONS    **********/
	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();

	bool Load_Button_Bindings();
	void Poll( bool Wait );
	void Wait_For_Input();

	bool Handle_Input_Events( SE_Events::Event_Base* nEv );
}
using namespace SE_Input;
using namespace SE_Input_Codes;

struct SE_Input::Bound_Input
{
	Bound_Input();
	Bound_Input( void (*Input_Handler)( sshort* ), ushort Code );

	void (*Input_Handler)( sshort* ); // sshort* Data
	ushort Code;
};



Bound_Input::Bound_Input()
{
	this->Code = 0;
	this->Input_Handler = NULL;
}

Bound_Input::Bound_Input( void (*Input_Handler)( sshort* ), ushort Code )
{
	this->Code = Code;
	this->Input_Handler = Input_Handler;
}




/*SE_Input::SE_Input()
{
	this->Input_Event_Handler = NULL;
}*/

/*void SE_Input::Initialize( void (*Input_Event_Handler)(Input_Event*) )
{
//	assert( Input_Event_Handler != NULL );
//	this->Input_Event_Handler = Input_Event_Handler;

#ifdef _TEST
	Engine.Log( "Initialized Input System\n", Log_Type::TYPE_DBUG );
#endif
}*/

bool SE_Input::Load_Button_Bindings()
{
	std::string Bindings;
#ifdef _TEST
	= Engine.Application.Input.Load_File_Str("data/button_bindings.txt");
#endif

	if( Bindings.length() == 0 )
	{
#ifdef _TEST
		Engine.Register_Error(S_Error("Cannot get button bindings, no data",2));
#endif
		return false;
	}

	std::vector< std::string > Tokens;

	size_t Sep_Pos = 0;
	while( (Sep_Pos = Bindings.find_first_of("\t\n\r ")) != std::string::npos )
	{
		if( Sep_Pos == 0 )
		{
			Bindings.erase( 0, Sep_Pos + 1 );
			continue;
		}

		Tokens.push_back( Bindings.substr( 0, Sep_Pos ) );
		Bindings.erase( 0, Sep_Pos + 1 );
	}

	if( Tokens.size() % 2 != 0 )
	{
#ifdef _TEST
		Engine.Register_Error(S_Error("Cannot get button bindings, there must be 1 key and 1 command (uneven list)",2));
#endif
		return false;
	}

	for( uint i = 0; i < Tokens.size(); i++ )
	{
		Bound_Input nBind;
		nBind.Code = Convert::Str_To_ID( Tokens[i] );

		i++;

		if( Tokens[i] == "quit" )
			nBind.Input_Handler = Default_Input_Commands::Input_Quit;

		else if( Tokens[i] == "pause-toggle" )
			nBind.Input_Handler = Default_Input_Commands::Input_Pause_Toggle;

		else if( Tokens[i] == "debug-toggle" )
			nBind.Input_Handler = Default_Input_Commands::Input_Debug_Toggle;

		else if( Tokens[i] == "chat" )
			nBind.Input_Handler = Default_Input_Commands::Input_Chat;

		if( nBind.Code == 0 || nBind.Input_Handler == NULL )
			sePrintf( SEPRINT_WARN, "Key command '%s' not recognized\n", Tokens[i].c_str() );

		else
			Bind_Key( nBind.Code, nBind.Input_Handler, Tokens[i], true );
	}


	return true;
}

void SE_Input::Poll( bool Wait )
{

}

void SE_Input::Wait_For_Input()
{

}

void SE_Input::New_Input( const Input_Event& nEv )
{
	if( Is_Valid( nEv.Code ) == false )
	{
		sePrintf( SEPRINT_DEBUG, "Key 0x%x is not valid\n", nEv.Code );
		return;
	}
	else
	{
//		sePrintf( SEPRINT_DEBUG, "Key 0x%x changed\n", nEv.Code );
		Button_Buffer[ nEv.Code ] = nEv.Argument;
		if( nEv.Code == MOUSE_WHEEL_UP )
			Button_Buffer[ MOUSE_WHEEL_POS ]--;
		else if( nEv.Code == MOUSE_WHEEL_DOWN )
			Button_Buffer[ MOUSE_WHEEL_POS ]++;

		for( uint i = 0; i < Bound_Buttons.size(); i++ )
			if( Bound_Buttons[i].Code == nEv.Code && Bound_Buttons[i].Input_Handler != NULL )
				Bound_Buttons[i].Input_Handler( &Button_Buffer[ nEv.Code ] );
	}
}

#ifdef _WIN32
void SE_Input::New_Virtualkey( uint Keysym, sshort Data )
{
	New_Input( Convert::From_Virtual_Key( Keysym ), Data );
}
#else
void SE_Input::New_Keysym( uint Keysym, sshort Data )
{
	New_Input( Convert::From_Keysym( Keysym ), Data );
}
#endif

bool SE_Input::Is_Down( ushort Incode )
{
	if( Is_Valid(Incode) )
		return Button_Buffer[ Incode ];

	else
		return false;
}


sshort SE_Input::Get( ushort Incode ) // get data
{
	if( Is_Valid(Incode) )
		return Button_Buffer[ Incode ];

	else
		return 0;
}

void SE_Input::Set( ushort Incode, sshort Data ) // set data
{
	if( Is_Valid(Incode) )
		Button_Buffer[ Incode ] = Data;
}

bool SE_Input::Bind_Key( ushort Code, void (*Input_Handler)( sshort* ), std::string Name, bool Overload )
{
	// make sure it does not exist
	if( Overload == false )
	{
		for( uint i = 0; i < Bound_Buttons.size(); i++ )
		{
			if( Bound_Buttons[i].Code == Code )
				return false;
		}
	}

	if( Is_Valid( Code ) == false )
		return false;

	assert( Input_Handler != NULL );

	// ready to add key

	Bound_Buttons.push_back( Bound_Input( Input_Handler, Code ) );

	sePrintf( SEPRINT_CONFIG, "Bound %s to %s: %p\n", Convert::ID_To_Str( Code ).c_str(), Name.c_str(), (void*)Input_Handler );

	return true;
}

void Unbind_Key( ushort Code )
{

}




#include "../module/Event_Defs.hpp"
void Default_Input_Commands::Input_Quit( sshort* Data )
{
	if( *Data == false ) // up
		return;

	sePrintf( SEPRINT_INFO, "Shutdown By Input\n" );
	S_Engine::Event_Add_Engine( new SE_Events::Event_Base( NULL, Event_Module::EVENT_SHUTDOWN ) );
//	S_Engine::Shutdown();

}

void Default_Input_Commands::Input_Pause_Toggle( sshort* Data )
{
#ifdef _TEST
	if( Engine.Game && !Engine.Application.Input.Typing && *Data )
	{
		if(Engine.In_Game)
			Engine.Pause_Game();
		else
			Engine.Resume_Game();
	}
#endif
}

void Default_Input_Commands::Input_Debug_Toggle( sshort* Data )
{
#ifdef _TEST
	if( !Engine.Application.Input.Typing && *Data )
		Engine.Console.Is_Open = !Engine.Console.Is_Open;
#endif
}

void Default_Input_Commands::Input_Chat( sshort* Data )
{
#ifdef _TEST
	if( !Engine.Application.Input.Typing && *Data )
		Engine.Application.Input.Typing = true;
#endif
}

bool SE_Input::Handle_Input_Events( SE_Events::Event_Base* nEv )
{
	return true;
}

/* ***********    MODULE OPERATIONS    *************** */

uint SE_Input::Initialize( const std::vector<std::string>& mArgs )
{	
	memset( Button_Buffer, 0, sizeof(sshort) * (SE_Input_Codes::BUTTON_LAST + 1) );
	
	Load_Button_Bindings();

	return SE_SUCCESS;
}

uint SE_Input::Cleanup()
{
	return SE_SUCCESS;
}

namespace SE_Modules
{
	SE_Module_ID Mod_Input = NULL;
}

void SE_Input::Register_Module()
{
	SE_Modules::Mod_Input = S_Engine::Generate_Module( "input" );

	S_Engine::Register_Module_Engine( SE_Modules::Mod_Input, S_Engine::Reg_Mod_Time::LOW_LEVEL + 3, Initialize, Cleanup );

	S_Engine::Register_Module_Event_Engine( SE_Modules::Mod_Input, Handle_Input_Events );

	Bind_Key( KEY_ESCAPE, Default_Input_Commands::Input_Quit, "quit-esc", true );
}
