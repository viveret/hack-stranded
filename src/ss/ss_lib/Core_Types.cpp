#include "../Survivor_Script.hpp"
//using namespace Survivor_Script_Core;
using namespace Survivor_Script;






SS_Var_Template::SS_Var_Template()
{
	this->Set_Type( "template" );
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
	Name += "/";
	// get tokens (ie heirarchy)
	std::vector< std::string > Name_Hier;
	ssize_t Sep_Pos = 0;
	while( (Sep_Pos = Name.find("/")) != std::string::npos )
	{
		Name_Hier.push_back( Name.substr( 0, Sep_Pos ) );
		Name.erase( 0, Sep_Pos + 1 );

		if( Survivor_Script::g_Verbose )
			printf( "\tSep name: %s\n", Name_Hier[Name_Hier.size()-1].c_str() );
	}


	SS_Var* Child = this, *Last_Child = this;


	uint n = 0;
	for( ;n < Name_Hier.size() && Child != NULL; n++ )
	{
		if( Name_Hier[n].length() <= 1 )// weird error
			continue;

		Last_Child = Child;
		if( Name_Hier[n] == ".." ) // aka ".."
		{
			if( Child->Parent == NULL && Probe == false )
			{
				printf( "[EE] %s has no parent\n", Child->Name.c_str() );
				Child = NULL;
				break;
			}
			else
				Child = Child->Parent;
		}

		else if( Child->Type() == "container" )
		{
			SS_Var* Got_Child = NULL;
			for( uint i = 0;i < ((SS_Var_Container*)Child)->Children.size(); i++ )
			{
				if( ((SS_Var_Container*)Child)->Children[i]->Name == Name_Hier[n] )
				{
					Got_Child = ((SS_Var_Container*)Child)->Children[i];
					break;
				}
			}

			if( Got_Child )
				Child = Got_Child;
			else
				Child = NULL;
		}
		else
			Child = Child->Get_Child( Name_Hier[n] );
	}

	if( Child == NULL || Child == this )// || n != Name_Hier.size() )
	{
		if( Probe == false )
		{
			if( Last_Child != this )
				printf( "[EE] %s has no child \"%s\"\n", Last_Child->Name.c_str(), Child->Name.c_str() );//SS_MAN.Print_Err_Exist( Name, t );
			else if( Name_Hier.size() > 0 )
				printf( "[EE] %s has no child \"%s\"\n", this->Name.c_str(), Name_Hier[0].c_str() );
		}

		return NULL;
	}
	else
		return Child; // not found
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
