#ifndef SURVIVOR_ENGINE_HUMAN_INTERFACE_LANGUAGE
#define SURVIVOR_ENGINE_HUMAN_INTERFACE_LANGUAGE


/* This is used to translate words
 * or phrases into data that can be
 * used by different modules.
 */

#include "../Types.hpp"
#include <string>

namespace SE_HIL
{
	std::string Get_Str( uint ID );
	uint Get_ID( std::string Name );
	
	std::string Get_Type_Str( uint ID );
	uint Get_Type_ID( std::string Name );

	void* Get( std::string Name, std::string Attr, std::string Type = "" );
	void* Get( uint ID );

	uint Load_Library( std::string File, void** Ret_ID );
	void Add_Type( std::string Name, void*(*Func_New_XML)( XML_Node ), void(*Func_Del)(void*) );

	namespace Render
	{
		void Apply_Color3f( uint ID );
		void Apply_Color3f( std::string Name );

		void Apply_Color4f( uint ID );
		void Apply_Color4f( std::string Name );
	}

	// Module
	void Register_Module();
}








#endif
