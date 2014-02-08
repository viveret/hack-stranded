#include "SS_Libs.h"
#include "..\Survivor_Script.h"
using namespace Survivor_Script;
using namespace SS_Lib_Core;



std::string SS_Lib_Core::SS_Add_Core( Survivor_Script_Manager* SS_Man )
{
	SS_Container* nContainer_Interface = new SS_Container;

	SS_Man->Types.push_back( nContainer_Interface );




	SS_Var* nFunc_Make = NULL;
	nFunc_Make->nFunc_Make->SS_Make_New();

	//SS_Man->SS_Global->SS_Get_Func





	//
	return "core";
}



SS_Container::SS_Container()
{
	this->SS_Type_Name = "container";
}
						  
std::vector< SS_Var* >* SS_Container::mDTT( SS_Var* This )
{
	return (std::vector< SS_Var* >*)This->Data;
}

SS_Var* SS_Container::Get_Index( uint i, SS_Var* This )
{
	return (*mDTT(This))[i];
}
			  
SS_Var* SS_Container::SS_Get_Var( std::string Name, SS_Var* This )
{
	for( uint i = 0;i < mDTT(This)->size(); i++ )
	{
		if( Get_Index(i, This)->Name == Name )
			return Get_Index( i, This );
	}

	return NULL;
}

SS_Var* SS_Container::SS_Get_Func( std::string Name, SS_Var* This )
{
	//

	return NULL;
}

void* SS_Container::SS_Make_New( SS_Mult_Vars& Args )
{
	return new std::vector< SS_Var* >;
}

void SS_Container::SS_Cleanup( void* This )
{					
	std::vector<SS_Var*>* nThis = (std::vector<SS_Var*>*)This;


	for( uint i = 0;i < nThis->size(); i++ )
	{
		
//		(*nThis)[i]->m_Type->SS_Cleanup( (*nThis)[i] );
	}

	delete This;
}














SS_Var* SS_Func::SS_Get_Var( std::string Name, SS_Var* This )
{
	return NULL;
}

SS_Var* SS_Func::SS_Get_Func( std::string Name, SS_Var* This )
{
	return NULL;
}

void* SS_Func::SS_Make_New( SS_Mult_Vars& Args )
{
	return new std::vector< SS_Var* >;
}

void SS_Func::SS_Cleanup( void* This )
{					
	delete This;
}
