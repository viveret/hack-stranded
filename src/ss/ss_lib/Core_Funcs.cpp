#include "../Survivor_Script.hpp"
//using namespace Survivor_Script_Core;
using namespace Survivor_Script;


#include <cstdlib>
#ifndef _WIN32
#include <unistd.h>
#endif

std::vector<SS_Var*> Survivor_Script_Core::Func_Shutdown( std::vector<SS_Var*>& Args )
{
	for( uint i = 0;i < Args.size(); i++ )
	{
		if( Args[i]->Type() == "string" )
		{
			if( ((SS_Var_String*)Args[i])->m_String == "-restart" || ((SS_Var_String*)Args[i])->m_String == "-r" )
			{
				//execv( SS_MAN.G_Argv[0], SS_MAN.G_Argv );
				//return;
			}
		}
	}

	exit(0);

	return std::vector<SS_Var*>();
}
void Survivor_Script_Core::Func_Shutdown_Print_Info( SS_Var* ThisVar )
{
	printf( "Shutdowns the current Survivor Script session\n" );
}

std::vector<SS_Var*> Survivor_Script_Core::Func_Info( std::vector<SS_Var*>& Args  ) // print info about variable
{
	// calls void (*Info_Core)( SS_Var* ) that prints info if supported, name = "print_info"
	SS_Var* Where = SS_MAN.Current_Var;

	if( Args.size() >= 1 )
	{
		if( Args[0]->Type() == "string" )
			Where = SS_MAN.Get_Var( ((SS_Var_String*)Args[0])->m_String );
		else
			Where = Args[0];

		if( Where == NULL )
			return std::vector<SS_Var*>();
	}

	void* Info_Func = Where->Get_Child_Ex( "print_info" );
	if( Info_Func == NULL ) return std::vector<SS_Var*>();

	void (*Func_Call)( SS_Var* ) = (void (*)( SS_Var* )) Info_Func;

	printf( "Type: %s, pointer: %s\n", Where->Type().c_str(), Where->Is_Pointer() ? "true" : "false" );
	Func_Call( Where );
//	printf( "\n" );

	// try to get local information about that instance
	Info_Func = Where->Get_Child_Ex( "print_info_local" );
	if( Info_Func == NULL ) return std::vector<SS_Var*>();

	Func_Call = (void (*)( SS_Var* )) Info_Func;

	printf( "Local info: " );
	Func_Call( Where );

	return std::vector<SS_Var*>();
}
void Survivor_Script_Core::Func_Info_Print_Info( SS_Var* ThisVar )
{
	printf( "Prints info about a variable\n" );
}

std::vector<SS_Var*> Survivor_Script_Core::Func_Exec( std::vector<SS_Var*>& Args )
{
	return std::vector<SS_Var*>();
}

std::vector<SS_Var*> Survivor_Script_Core::Func_CD( std::vector<SS_Var*>& Args ) // change locality
{
	if( Args.size() == 0 )
	{
		printf("[EE] Need at least 1 arg: directory to move to\n");
		return std::vector<SS_Var*>();
	}

	SS_Var* nDir = NULL;

	if( Args[0]->Type() == "string" )
		nDir = SS_MAN.Get_Var( ((SS_Var_String*)Args[0])->m_String );
	else if( Args[0]->Type() == "container" )
		nDir = Args[0];
	else
	{
		printf( "[EE] argument must be of type 'string' or 'container', not '%s'\n", Args[0]->Type().c_str() );
		return std::vector<SS_Var*>();
	}

	SS_MAN.Current_Var = nDir;

	return std::vector<SS_Var*>();
}

void Survivor_Script_Core::Func_CD_Print_Info( SS_Var* ThisVar )
{
	printf( "Changes locality (current directory)\n" );
}

std::vector<SS_Var*> Survivor_Script_Core::Func_List( std::vector<SS_Var*>& Args  ) // list all children of object
{
	SS_Var* Where = SS_MAN.Current_Var;
	if( Args.size() > 0 )
		Where = Args[0];

	bool Long = false, Core_Type_Show = false;

	/*for( uint i = 0;i < Args.size(); i++ )
	{
		if( Args[i]->Name == "l" || Args[i]->Name == "long" )
			Long = true;

		else if( Args[i]->Name == "ex" || Args[i]->Name == "x" )
			Core_Type_Show = true;
	}*/

	void* List_Func = Where->Get_Child_Ex( "list_children" );
	if( List_Func == NULL ) return std::vector<SS_Var*>();

	void (*Func_Call)( SS_Var* ) = (void (*)( SS_Var* )) List_Func;

	Func_Call( Where );

	return std::vector<SS_Var*>();
}
void Survivor_Script_Core::Func_List_Print_Info( SS_Var* ThisVar )
{
	printf( "Prints a list of all children in a variable\n" );
}



std::vector<SS_Var*> Survivor_Script_Core::Func_Var( std::vector<SS_Var*>& Args  )
{
	// create new variable in cd (new, var)
	//   calls SS_Var* (*Var_New_Core)( SS_Var* ) that creates a new var, name = "var_new"

	if( Args.size() < 2 )
	{
		printf("[EE] Need at least 2 args: template name and new var name\n");
		return std::vector<SS_Var*>();
	}

	//SS_Var* Temp = SS_MAN.Current_Var;

	if( Args[0]->Type() != "template" )
	{
		printf( "[EE] template name must be of type 'template' , not '%s'\n", Args[0]->Type().c_str() );
		return std::vector<SS_Var*>();
	}
	else if( Args[1]->Type() != "string" )
	{
		printf( "[EE] var name must be of type 'string' , not '%s'\n", Args[1]->Type().c_str() );
		return std::vector<SS_Var*>();
	}


	SS_Var_Template* nTemp = (SS_Var_Template*)SS_MAN.Global_Space.Get_Child("temps");
	nTemp = (SS_Var_Template*)nTemp->Get_Child( ((SS_Var_String*)Args[0])->m_String );

/*	for( uint i = 0;i < Args.size(); i++ )
	{
		if( Args[i]->Name == "l" || Args[i]->Name == "long" )
			Long = true;

		else if( Args[i]->Name == "ex" || Args[i]->Name == "x" )
			Core_Type_Show = true;
	}
*/





	void* Var_New_Func = nTemp->Get_Child_Ex( "var_new" );
	if( Var_New_Func == NULL ) return std::vector<SS_Var*>();

	SS_Var* (*Func_Call)( SS_Var* ) = (SS_Var* (*)( SS_Var* )) Var_New_Func;

	std::vector<SS_Var*> Ret;
	Ret.push_back( Func_Call( SS_MAN.Current_Var ) );

	return Ret;
}

void Survivor_Script_Core::Func_Var_Print_Info( SS_Var* ThisVar )
{
	printf( "Creates new variables within Survivor Script\n" );
}
