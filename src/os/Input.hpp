#ifndef SURVIVOR_ENGINE_INPUT
#define SURVIVOR_ENGINE_INPUT

#include "../Core.hpp"
#include "../utility/Timer.hpp"
#include "../utility/Array.hpp"

#include "../module/Events.hpp"

#include "Input_Codes.hpp"

#include <vector>

namespace SE_Input
{
	bool Load_Button_Bindings();

	void Poll( bool Wait = false );
	void Wait_For_Input();

	void New_Input( const Input_Event& nEv );
	inline void New_Input( ushort Code, sshort Data )
	{
		New_Input( Input_Event( NULL, Code, Data) );
	}

#ifdef _WIN32
	void New_Virtualkey( uint Code, sshort Data );
#else
	void New_Keysym( uint Code, sshort Data );// new scancode was found
#endif

	bool Is_Down( ushort Incode );
	inline bool Is_Up( ushort Incode )
	{
		return !Is_Down( Incode );
	}


	sshort Get( ushort Incode ); // get data
	void Set( ushort Incode, sshort Data ); // set data

	bool Bind_Key( ushort Code, void (*Input_Handler)( sshort* ), std::string Name, bool Overload = false );
	void Unbind_Key( ushort Code );
//	private:

//		void (*Input_Event_Handler)(Input_Event*);

	namespace Default_Input_Commands
	{
		void Input_Quit( sshort* Data );
		void Input_Pause_Toggle( sshort* Data );
		void Input_Debug_Toggle( sshort* Data );
		void Input_Chat( sshort* Data );
	}

	void Register_Module();
}


#endif
