#include <stdio.h>
#include <string>
#include <vector>




/*
	type: template
		var a,b		 // no actual data

	object:
		type*
		var a,b
*/
		




typedef unsigned int uint;


namespace SS_Script
{	
	struct SS_Type;
	struct SS_Var_Base;
	typedef std::vector< SS_Var_Base* > SS_Mult_Vars;

	typedef SS_Mult_Vars(*SS_Func_Call)(SS_Mult_Vars);

	struct SS_Var_Interface
	{																			 
		virtual SS_Var_Base* SS_Get( std::string Name, SS_Type* Type = NULL ) = 0;
		SS_Var_Base* SS_Get( std::string Name, std::string Type = "" );

		std::string Name;
	};


	struct SS_Type//:public SS_Var_Interface
	{
		virtual SS_Var_Base* Make_New() = 0;
		std::string Type_Name;
	};


	struct SS_Var_Base:public SS_Var_Interface// basic variable
	{
		SS_Var_Base();

		virtual void* Get_Data() = 0;// actual data

		SS_Type* Type;
		uint Locality;// describes deletion properties
	};


	namespace Def_Types
	{			  
		struct SS_Function:public SS_Var_Base, SS_Type
		{						
			SS_Function();

			SS_Var_Base* Make_New();
			SS_Var_Base* SS_Get( std::string Name, SS_Type* Type = NULL );// returns function address	
									
			using SS_Var_Interface::Name;

			void* Get_Data();
			SS_Func_Call Morphs;
		};

		struct SS_Container:public SS_Var_Base, SS_Type
		{						
			SS_Var_Base* Make_New();	
			SS_Var_Base* SS_Get( std::string Name, SS_Type* Type = NULL );
			void* Get_Data();
										 
			using SS_Var_Interface::Name;

			SS_Mult_Vars Objects;
			SS_Function m_Printf;
		};
	}



	struct SS_Type_Manager
	{
		SS_Type* Get_Type( std::string Type_Name );
		std::vector< SS_Type* > Types;
	};

	struct SS_Obj_Manager
	{
		void Cleanup();

		void Add( SS_Var_Base* What );
		void Delete( SS_Var_Base* What );
											
		std::vector< SS_Var_Base* > Objects;
		std::vector< SS_Var_Base* > Stack;
	};


	struct SS_Manager
	{
		SS_Manager();


		SS_Type_Manager Type_Manager;

		Def_Types::SS_Container SS_Global;
	};
}


SS_Script::SS_Manager SS_Man;
#define SS_MAN SS_Man


int main()
{
	SS_Script::SS_Var_Base* nCont = SS_Man.Type_Manager.Get_Type( "container" )->Make_New();

	SS_Script::SS_Func_Call* nFunc = (SS_Script::SS_Func_Call*)nCont->SS_Get( "printf", "function" )->Get_Data();


	(*nFunc)( SS_Script::SS_Mult_Vars() );




	getchar();

	return 0;
}








SS_Script::SS_Mult_Vars mmmPrintf( SS_Script::SS_Mult_Vars Vars )
{
	printf("awd awd awd awda");


	return SS_Script::SS_Mult_Vars();
}










using namespace SS_Script;

SS_Var_Base* SS_Var_Interface::SS_Get( std::string Name, std::string Type )
{
	return this->SS_Get( Name, SS_MAN.Type_Manager.Get_Type( Type ) );
}

SS_Var_Base::SS_Var_Base()
{
	this->Type = NULL;
	this->Locality = 0;
}


SS_Type* SS_Type_Manager::Get_Type( std::string Type_Name )
{
	for( uint i = 0;i < this->Types.size(); i++ )
	{
		if( this->Types[i]->Type_Name == Type_Name )
			return this->Types[i];
	}

	return NULL;
}


SS_Var_Base* Def_Types::SS_Container::SS_Get( std::string Name, SS_Type* Type )
{
	if( Name == "printf" )
		return &this->m_Printf;


	return NULL;
}
			 
SS_Var_Base* Def_Types::SS_Container::Make_New()
{
	Def_Types::SS_Container* nCont = new Def_Types::SS_Container;
	memcpy( nCont, this, sizeof(Def_Types::SS_Container) );


	return (SS_Var_Base*)nCont;
}

void* Def_Types::SS_Container::Get_Data()
{
	return NULL;
}




Def_Types::SS_Function::SS_Function()
{
	this->Morphs = NULL;
}

SS_Var_Base* Def_Types::SS_Function::SS_Get( std::string Name, SS_Type* Type )
{
	return NULL;
}
			 
SS_Var_Base* Def_Types::SS_Function::Make_New()
{
	return (SS_Var_Base*)new Def_Types::SS_Function;
}

void* Def_Types::SS_Function::Get_Data()
{
	return this->Morphs;
}






SS_Manager::SS_Manager()
{
	// add def types								

	Def_Types::SS_Container* Type_Cont = new Def_Types::SS_Container;
	Type_Cont->Name = "container";
	Type_Cont->Type_Name = "container";
	Type_Cont->m_Printf.Morphs = mmmPrintf;							 

	this->Type_Manager.Types.push_back( Type_Cont );				 

	

	Def_Types::SS_Function* Type_Func = new Def_Types::SS_Function;
	Type_Func->Name = "function";
	Type_Func->Type_Name = "function";

	this->Type_Manager.Types.push_back( Type_Func );
}