#include "SS_Libs.h"
#include "../Survivor_Script.h"
using namespace Survivor_Script;
using namespace SS_Lib_Core;



std::string SS_Lib_Core::SS_Add_Core( Survivor_Script_Manager* SS_Man )
{															  
	SS_Container* nContainer_Interface = new SS_Container;
	SS_Man->Types.push_back( nContainer_Interface );
	
	SS_Func* nFunc_Interface = new SS_Func;
	SS_Man->Types.push_back( nFunc_Interface );




	SS_Func* nFunc_Make = NULL;//(SS_Func*)SS_Man->Get_Type( "function" )->SS_Make_New( SS_Mult_Vars() );
	nFunc_Make->SS_Name = "make";
	nFunc_Make->Func = SS_Lib_Core::Func_Make;

	SS_Man->SS_Global.Children->push_back( nFunc_Make );





	//
	return "core";
}			 


SS_Mult_Vars SS_Lib_Core::Func_Make( SS_Mult_Vars& Args )
{				
	if( Args.size() > 1 )// type + name
	{
		/*SS_Var* nVar = SS_MAN.Get_Type( Args[0]-> );
		SS_Func* nFunc_Make = new SS_Func;
		nFunc_Make->SS_Name = "make";
		nFunc_Make->Func = SS_Lib_Core::Func_Make;

		SS_MAN.SS_Global.Children->push_back( nFunc_Make );*/
	}
	else
		printf( "Cannot make without at least 2 args\n" );

	return SS_Mult_Vars();
}



SS_Container::SS_Container()
{
	this->SS_Type_Name = "container";
	this->Children = NULL;
}

Survivor_Script::SS_Var* SS_Lib_Core::SS_Container::SS_Make_New( void* )
{
	//
}

void* SS_Container::SS_Get_Data()
{
	return this->Children;
}	

SS_Var* SS_Container::Get_Index( uint i )
{
	return (*this->Children)[i];
}
			  
SS_Var* SS_Container::SS_Get_Var( std::string Name )
{
	for( uint i = 0;i < this->Children->size(); i++ )
	{
		if( Get_Index(i)->SS_Name == Name )
			return Get_Index( i );
	}

	return NULL;
}

SS_Var* SS_Container::SS_Get_Func( std::string Name )
{
	//

	return NULL;
}

SS_Var* SS_Container::SS_Make_New( SS_Mult_Vars& Args )
{
	SS_Container* Ret = new SS_Container;

	if( this->SS_Is_Pointer == false )
		Ret->Children = new std::vector< SS_Var* >;

	return Ret;
}

void SS_Container::SS_Cleanup()
{					
	for( uint i = 0;i < this->Children->size(); i++ )
	{
		
//		(*nThis)[i]->m_Type->SS_Cleanup( (*nThis)[i] );
	}

	delete this->Children;
}











SS_Func::SS_Func()
{
	this->SS_Type_Name = "function";
	this->Func = NULL;
}

void* SS_Func::SS_Get_Data()
{
	return (void*)this->Func;
}

SS_Var* SS_Func::SS_Get_Var( std::string Name )
{
	return NULL;
}

SS_Var* SS_Func::SS_Get_Func( std::string Name )
{
	return NULL;
}

SS_Var* SS_Func::SS_Make_New( SS_Mult_Vars& Args )
{
	return new SS_Func;
}

void SS_Func::SS_Cleanup()
{					
	//delete this->;
}








using namespace SS_C_Lib;


SS_Char::SS_Char()
{
	this->m_Char = NULL;
}

void* SS_Char::SS_Get_Data()
{
	return this->m_Char;
}

SS_Var* SS_Char::SS_Get_Var( std::string Name )
{
	return NULL;
}

SS_Var* SS_Char::SS_Get_Func( std::string Name )
{				
	return NULL;
}

SS_Var* SS_Char::SS_Make_New( Survivor_Script::SS_Mult_Vars& Args )
{
	SS_Char* Ret = new SS_Char;
	if( this->SS_Is_Pointer == false )
		;//Ret->

	return NULL;//new
}

void SS_Char::SS_Cleanup()
{
	//
}




					
SS_Short::SS_Short()
{
	this->m_Short = NULL;
}

void* SS_Short::SS_Get_Data()
{
	return this->m_Short;
}

SS_Var* SS_Short::SS_Get_Var( std::string Name )
{
	//
}

SS_Var* SS_Short::SS_Get_Func( std::string Name )
{
	//
}

SS_Var* SS_Short::SS_Make_New( Survivor_Script::SS_Mult_Vars& Args )
{
	//
}

void SS_Short::SS_Cleanup()
{
	//
}





SS_Int::SS_Int()
{
	this->m_Int = NULL;
}

void* SS_Int::SS_Get_Data()
{
	return this->m_Int;
}

SS_Var* SS_Int::SS_Get_Var( std::string Name )
{
	//
}

SS_Var* SS_Int::SS_Get_Func( std::string Name )
{
	//
}

SS_Var* SS_Int::SS_Make_New( Survivor_Script::SS_Mult_Vars& Args )
{
	//
}

void SS_Int::SS_Cleanup()
{
	//
}





SS_Float::SS_Float()
{
	this->m_Float = NULL;
}

void* SS_Float::SS_Get_Data()
{
	return this->m_Float;
}

SS_Var* SS_Float::SS_Get_Var( std::string Name )
{
	//
}

SS_Var* SS_Float::SS_Get_Func( std::string Name )
{
	//
}

SS_Var* SS_Float::SS_Make_New( Survivor_Script::SS_Mult_Vars& Args )
{
	//
}

void SS_Float::SS_Cleanup()
{
	//
}





SS_Double::SS_Double()
{
	this->m_Double = NULL;
}

void* SS_Double::SS_Get_Data()
{
	return this->m_Double;
}

SS_Var* SS_Double::SS_Get_Var( std::string Name )
{
	//
}

SS_Var* SS_Double::SS_Get_Func( std::string Name )
{
	//
}

SS_Var* SS_Double::SS_Make_New( Survivor_Script::SS_Mult_Vars& Args )
{
	//
}

void SS_Double::SS_Cleanup()
{
	//
}





SS_String::SS_String()
{
	this->m_String = NULL;
}

void* SS_String::SS_Get_Data()
{
	return this->m_String;
}

SS_Var* SS_String::SS_Get_Var( std::string Name )
{
	//
}

SS_Var* SS_String::SS_Get_Func( std::string Name )
{
	//
}

SS_Var* SS_String::SS_Make_New( Survivor_Script::SS_Mult_Vars& Args )
{
	//
}

void SS_String::SS_Cleanup()
{
	//
}

