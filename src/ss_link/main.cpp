#include <stdio.h>
#include "Survivor_Script.h"

Survivor_Script::SS_Manager SS_Main;
#include "ss_lib/File_Sys.h"
#include "ss_lib/Math.h"


int main( int argc, char** argv )
{
	bool Running = true;

	SS_Main.G_Argc = argc;
	SS_Main.G_Argv = argv;

	SS_Main.Initialize();
	SS_Main.Lib_Add( Survivor_Script_File_Sys::Lib_Add_Func );
	SS_Main.Lib_Add( Survivor_Script_Math::Lib_Add_Func );

	printf( "~~ Welcome to Survivor Script ~~\n" );

	while( Running )
	{
		std::string Cur_Dir;
		Survivor_Script::SS_Var* Get_Cur_Dir = SS_Main.Current_Var;

		while( Get_Cur_Dir != NULL )
		{
			Cur_Dir = Get_Cur_Dir->Name + "/" + Cur_Dir;
			Get_Cur_Dir = Get_Cur_Dir->Parent;
		}

		Cur_Dir.pop_back();

		printf("\n%s>", Cur_Dir.c_str() );

		char Str_Buf[256];
		gets( Str_Buf );

		SS_Main.Handle_Statement( Str_Buf );

//		SS_Main.Update();
	}

	getchar();
	return 0;
}
