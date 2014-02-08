#include "../Survivor_Script.h"
//using namespace Survivor_Script_Core;
using namespace Survivor_Script;









std::string Survivor_Script_Core::Lib_Add_Func( Survivor_Script::SS_Manager* SS_Man )
{
	SS_Var_Function* nFunc_Quit = SS_Var_Function::Make_New();
	nFunc_Quit->Name = "quit";
	nFunc_Quit->m_Func = Survivor_Script_Core::Func_Shutdown;
	nFunc_Quit->Core_Print_Info_Local = Survivor_Script_Core::Func_Shutdown_Print_Info;
	SS_Man->Lib_Add_Bin( nFunc_Quit );


	SS_Var_Function* nFunc_CD = SS_Var_Function::Make_New( );
	nFunc_CD->Name = "cd";
	nFunc_CD->m_Func = Survivor_Script_Core::Func_CD;
	nFunc_CD->Core_Print_Info_Local = Survivor_Script_Core::Func_CD_Print_Info;
	SS_Man->Lib_Add_Bin( nFunc_CD );


	SS_Var_Function* nFunc_List = SS_Var_Function::Make_New( );
	nFunc_List->Name = "list";
	nFunc_List->m_Func = Survivor_Script_Core::Func_List;
	nFunc_List->Core_Print_Info_Local = Survivor_Script_Core::Func_List_Print_Info;
	SS_Man->Lib_Add_Bin( nFunc_List );


	SS_Var_Function* nFunc_Info = SS_Var_Function::Make_New( );
	nFunc_Info->Name = "info";
	nFunc_Info->m_Func = Survivor_Script_Core::Func_Info;
	nFunc_Info->Core_Print_Info_Local = Survivor_Script_Core::Func_Info_Print_Info;
	SS_Man->Lib_Add_Bin( nFunc_Info );


	SS_Var_Function* nFunc_Var = SS_Var_Function::Make_New( );
	nFunc_Var->Name = "var";
	nFunc_Var->m_Func = Survivor_Script_Core::Func_Var;
	nFunc_Var->Core_Print_Info_Local = Survivor_Script_Core::Func_Var_Print_Info;
	SS_Man->Lib_Add_Bin( nFunc_Var );


	/*   VAR TYPES   */
	SS_Man->Lib_Add_Template( (SS_Var*(*)(SS_Var*))(void*)SS_Var_String::Make_New, "string" );


	return "core";
}




#ifdef _ALREADY_DEFED



SS_Var_Template::SS_Var_Template()
{
	this->Var_New_Func = NULL;
}

SS_Var* SS_Var_Template::SS_Make_New( SS_Var* nParent )//cannot create new templates, return NULL
{
	return NULL;
}

SS_Var_Template* SS_Var_Template::Make_New( SS_Var* nParent )// only make used
{
	SS_Var_Template* Ret = new SS_Var_Template;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_Template::Cleanup()
{
	this->Var_New_Func = NULL;
}

/*SS_Var* SS_Var_Template::Get_Child( std::string Name, bool Probe = false ) // only return statics
{
	return NULL;
}*/

void* SS_Var_Template::Get_Child_Ex( std::string Name, bool Probe ) // only return statics
{
	if( Name == "var_new" )
		return (void*)this->Var_New_Func;

	/*else if( Name == "print_info" )
		return (void*)this->Core_Print_Info;*/

	else
	{
		if( Probe == false )
			SS_MAN.Print_Err_Exist( Name, this->Name );

		return NULL;
	}
}




SS_Var_Container::SS_Var_Container()
{
	this->Set_Type( "container" );
}

SS_Var* SS_Var_Container::SS_Make_New( SS_Var* nParent )
{
	return SS_Var_Container::Make_New( nParent );
}

SS_Var_Container* SS_Var_Container::Make_New( SS_Var* nParent )
{
	SS_Var_Container* Ret = new SS_Var_Container;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_Container::Cleanup()
{
	for( uint i = 0;i < this->Children.size(); i++ )
	{
		SS_MAN.Delete( this->Children[i] );
	}

	this->Children.clear();
}

SS_Var* SS_Var_Container::Get_Child( std::string Name, bool Probe )
{
	for( uint i = 0;i < this->Children.size(); i++ )
	{
		if( this->Children[i]->Name == Name )
		{
			if( this->Children[i]->Type() == "pointer" )
				return ((SS_Var_Pointer*)this->Children[i])->Pointing_To;
			else
				return this->Children[i];
		}
	}

	// not listed so far, last try
	if( Name == ".." )
		return this->Parent;

	if( Probe == false )
		SS_MAN.Print_Err_Exist( Name, this->Name );

	return NULL;
}

void* SS_Var_Container::Get_Child_Ex( std::string Name, bool Probe )
{
	if( Name == "list_children" )
		return (void*)this->Core_List_Children;

	else if( Name == "print_info" )
		return (void*)this->Core_Print_Info;

	else
	{
		if( Probe == false )
			SS_MAN.Print_Err_Exist( Name, this->Name );

		return NULL;
	}
}

void SS_Var_Container::Core_List_Children( SS_Var* ThisVar )
{
	if( ThisVar == NULL ) return;
	if( ThisVar->Type() != "container" ) return;

	SS_Var_Container* This = (SS_Var_Container*)ThisVar;

	for( uint i = 0;i < This->Children.size(); i++ )
		printf( "%s\n", This->Children[i]->Name.c_str() );
}

void SS_Var_Container::Core_Print_Info( SS_Var* ThisVar )
{
	if( ThisVar == NULL ) return;
	if( ThisVar->Type() != "container" ) return;

	//SS_Var_Container* This = (SS_Var_Container*)ThisVar;

	printf( "Manages variables. Using [list] would provide more information.\n" );
}




SS_Var_Pointer::SS_Var_Pointer()
{
	this->Set_Type( "pointer" );
	this->Pointing_To = NULL;
}

SS_Var_Pointer* SS_Var_Pointer::SS_Make_New( SS_Var* nParent )
{
	return this->Make_New( nParent );
}

SS_Var_Pointer* SS_Var_Pointer::Make_New( SS_Var* nParent )
{
	SS_Var_Pointer* Ret = new SS_Var_Pointer;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_Pointer::Cleanup()
{
	this->Pointing_To = NULL;
}



SS_Var_Function::SS_Var_Function()
{
	this->m_Func = NULL;
	this->Set_Type( "func" );
}

SS_Var* SS_Var_Function::SS_Make_New( SS_Var* nParent )
{
	return SS_Var_Function::Make_New( nParent );
}

SS_Var_Function* SS_Var_Function::Make_New( SS_Var* nParent )
{
	SS_Var_Function* Ret = new SS_Var_Function;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_Function::Cleanup()
{
	if( this->m_Func )
		this->m_Func = NULL;
}

void* SS_Var_Function::Get_Child_Ex( std::string Name, bool Probe )
{
	/*if( Name == "list_children" )
		return (void*)this->Core_List_Children;

	else */if( Name == "print_info" )
		return (void*)this->Core_Print_Info;

	else if( Name == "print_info_local" )
		return (void*)this->Core_Print_Info_Local;

	else
	{
		if( Probe == false )
			SS_MAN.Print_Err_Exist( Name, this->Name );

		return NULL;
	}
}

void SS_Var_Function::Core_Print_Info( SS_Var* ThisVar )
{
	if( ThisVar == NULL ) return;
	if( ThisVar->Type() != "func" ) return;

	//SS_Var_Container* This = (SS_Var_Container*)ThisVar;

	printf( "Executes a non ss function.\n" );
}






SS_Var_String::SS_Var_String()
{
	this->Set_Type( "string" );
}

SS_Var* SS_Var_String::SS_Make_New( SS_Var* nParent )
{
	return SS_Var_String::Make_New( nParent );
}

SS_Var_String* SS_Var_String::Make_New( SS_Var* nParent )
{
	SS_Var_String* Ret = new SS_Var_String;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_String::Cleanup()
{

}

void* SS_Var_String::Get_Child_Ex( std::string Name, bool Probe )
{
	if( Name == "print_info" )
		return (void*)this->Core_Print_Info;

	else
	{
		if( Probe == false )
			SS_MAN.Print_Err_Exist( Name, this->Name );

		return NULL;
	}
}

void SS_Var_String::Core_Print_Info( SS_Var* ThisVar )
{
	if( ThisVar == NULL ) return;
	if( ThisVar->Type() != "string" ) return;

	printf( "Contains an array of characters.\n" );
}






SS_Var_Int::SS_Var_Int()
{
	this->Set_Type( "int" );
	this->m_Value = 0;
}

SS_Var* SS_Var_Int::SS_Make_New( SS_Var* nParent )
{
	return SS_Var_Int::Make_New( nParent );
}

SS_Var_Int* SS_Var_Int::Make_New( SS_Var* nParent )
{
	SS_Var_Int* Ret = new SS_Var_Int;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_Int::Cleanup()
{

}

void* SS_Var_Int::Get_Child_Ex( std::string Name, bool Probe )
{
	if( Name == "print_info" )
		return (void*)this->Core_Print_Info;

	else
	{
		if( Probe == false )
			SS_MAN.Print_Err_Exist( Name, this->Name );

		return NULL;
	}
}

void SS_Var_Int::Core_Print_Info( SS_Var* ThisVar )
{
	if( ThisVar == NULL ) return;
	if( ThisVar->Type() != "int" ) return;

	printf( "A whole-number digit from -2147483648 to 2147483647.\n" );
}






SS_Var_Float::SS_Var_Float()
{
	this->Set_Type( "float" );
	this->m_Value = 0;
}

SS_Var* SS_Var_Float::SS_Make_New( SS_Var* nParent )
{
	return SS_Var_Float::Make_New( nParent );
}

SS_Var_Float* SS_Var_Float::Make_New( SS_Var* nParent )
{
	SS_Var_Float* Ret = new SS_Var_Float;

	if( nParent )
		Ret->Parent = nParent;
	else
		Ret->Parent = SS_MAN.Current_Var;

	return Ret;
}

void SS_Var_Float::Cleanup()
{

}

void* SS_Var_Float::Get_Child_Ex( std::string Name, bool Probe )
{
	if( Name == "print_info" )
		return (void*)this->Core_Print_Info;

	else
	{
		if( Probe == false )
			SS_MAN.Print_Err_Exist( Name, this->Name );

		return NULL;
	}
}

void SS_Var_Float::Core_Print_Info( SS_Var* ThisVar )
{
	if( ThisVar == NULL ) return;
	if( ThisVar->Type() != "float" ) return;

	printf( "A floating point numder.\n" );
}







#include <cstdlib>
#include <unistd.h>

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
		Where = SS_MAN.Get_Var( Args[0]->Name );

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
		;

	if( nDir == NULL )
	{
		printf( "[EE] argument must be of type 'string' or 'container', not '%s'\n", Args[0]->Type().c_str() );
		return std::vector<SS_Var*>();
	}
	/*{
		if( Args[0] == ".." )
		{
			if( SS_MAN.Current_Var->Parent )
				SS_MAN.Current_Var = SS_MAN.Current_Var->Parent;
			else
				printf("[EE] \"%s\" has no parent\n", SS_MAN.Current_Var->Name.c_str() );
		}
		else
			SS_MAN.Print_Err_Exist( Args[0], SS_MAN.Current_Var->Name );

		return;
	}*/

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
	/*Where = SS_MAN.Get_Var( Args[0] );

	if( Where == NULL )
		return;*/

	bool Long = false, Core_Type_Show = false;

	for( uint i = 0;i < Args.size(); i++ )
	{
		if( Args[i]->Name == "l" || Args[i]->Name == "long" )
			Long = true;

		else if( Args[i]->Name == "ex" || Args[i]->Name == "x" )
			Core_Type_Show = true;
	}

	void* List_Func = Where->Get_Child_Ex( "list_children" );
	if( List_Func == NULL ) return std::vector<SS_Var*>();

	void (*Func_Call)( SS_Var* ) = (void (*)( SS_Var* )) List_Func;

//	printf("Children in %s:\n", Where->Name.c_str() );
	Func_Call( Where );

	return std::vector<SS_Var*>();
}
void Survivor_Script_Core::Func_List_Print_Info( SS_Var* ThisVar )
{
	printf( "Prints a list of all children in a variable\n" );
}

#endif
