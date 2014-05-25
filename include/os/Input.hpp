#ifndef SURVIVOR_ENGINE_INPUT
#define SURVIVOR_ENGINE_INPUT

#include <SE/Core.hpp>
#include <SE/setl/Timer.hpp>
#include <SE/setl/Array.hpp>

#include "../module/Events.hpp"

#include "Input_Codes.hpp"

#include <vector>

namespace SE_Input
{
	// Contexts
//	void* Gen_Context( std::string Name );
	SE_API void Recieve_String( std::string* Where, void* Data, void (*Func_Callback)(void*) );
	SE_API void Recieve_String_Cancel();

	// Buttons
	bool Load_Button_Bindings();

	void Poll( bool Wait = false );
	void Wait_For_Input();

	SE_API void New_Input( const Input_Event& nEv );
	inline void New_Input( ushort Code, sshort Data )
	{
		New_Input( Input_Event( NULL, Code, Data) );
	}

#ifdef _WIN32
	SE_API void New_Virtualkey( uint Code, sshort Data );
#elif defined(_UBUNTU)
	void New_Keysym( long unsigned int Code, sshort Data );// new scancode was found
#endif


	SE_API sshort Get( ushort Incode ); // get data
	SE_API void Set( ushort Incode, sshort Data ); // set data

	inline bool Is_Down( ushort Incode )
	{
		return Get( Incode ) != 0;
	}
	inline bool Is_Up( ushort Incode )
	{
		return Get( Incode ) == 0;
	}

	SE_API bool Bind_Key( ushort Code, void (*Input_Handler)( sshort* ), std::string Name, bool Overload = false );
	void Unbind_Key( ushort Code );
//	private:

//		void (*Input_Event_Handler)(Input_Event*);

	namespace Default_Input_Commands
	{
		void Input_Quit( sshort* Data );
		void Input_Pause_Toggle( sshort* Data );
		void Input_Chat( sshort* Data );

		void Input_Finished_Typing( sshort* Data );
	}

	void Register_Module();
}


#endif
