#include <stdio.h>
#include "survivor_script.h"

Survivor_Script::Survivor_Script_Manager SS_Main;





#include "ss_lib\SS_Libs.h"
int main( int argc, char** argv )
{
	std::vector< Survivor_Script::SS_Library > Libs;

	Survivor_Script::SS_Library Lib_Core = {
		Survivor_Script::SS_Lib_Core::SS_Add_Core
	};

	Libs.push_back( Lib_Core );
	//Libs.push_back( SS_Lib_Core::SS_Lib_Core );  
	//Libs.push_back( SS_Lib_Math::SS_Lib_Math ); 
	//Libs.push_back( SS_Lib_Debug::SS_Lib_Debug );
															 
	SS_Main.Initialize( Libs );

	bool Running = true;

	printf("~~ Welcome to Survivor Script ~~\n");

	while( Running )
	{
		printf("\n>");

		char Str_Buf[256];
		gets( Str_Buf );

		SS_Main.Do_Line( Str_Buf );

		SS_Main.Update();
	}

	getchar();
	return 0;
}