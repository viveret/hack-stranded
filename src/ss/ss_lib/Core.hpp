#ifndef SURVIVOR_SCRIPT_CORE
#define SURVIVOR_SCRIPT_CORE

#include "../Survivor_Script.hpp"


namespace Survivor_Script_Core
{
	std::string Lib_Add_Func( Survivor_Script::SS_Manager* SS_Man );

	struct SS_Var_Template: public SS_Var // "template"
	{
		SS_Var_Template();

		SS_Var* SS_Make_New( SS_Var* nParent = NULL );//cannot create new templates, return NULL
		static SS_Var_Template* Make_New( SS_Var* nParent = NULL );// only make used

		void Cleanup();

		//SS_Var* Get_Child( std::string Name, bool Probe = false ); // only return statics
		void* Get_Child_Ex( std::string Name, bool Probe = false ); // only return statics

		SS_Var* (*Var_New_Func)( SS_Var* ); // args, where
	};


	struct SS_Var_Container: public SS_Var // "container"
	{
		SS_Var_Container();

		SS_Var* SS_Make_New( SS_Var* nParent = NULL );
		static SS_Var_Container* Make_New( SS_Var* nParent = NULL );

		void Cleanup();

		SS_Var* Get_Child( std::string Name, bool Probe = false );
		void* Get_Child_Ex( std::string Name, bool Probe = false );

		std::vector< SS_Var* > Children;

		static void Core_List_Children( SS_Var* ThisVar );
		static void Core_Print_Info( SS_Var* ThisVar );
	};

	struct SS_Var_Pointer: public SS_Var // "pointer"
	{
		SS_Var_Pointer();

		SS_Var_Pointer* SS_Make_New( SS_Var* nParent = NULL );
		static SS_Var_Pointer* Make_New( SS_Var* nParent = NULL );

		void Cleanup();

		SS_Var* Pointing_To;
	};

	struct SS_Var_Function: public SS_Var // "func"
	{
		SS_Var_Function();

		SS_Var* SS_Make_New( SS_Var* nParent = NULL );
		static SS_Var_Function* Make_New( SS_Var* nParent = NULL );

		void Cleanup();

		//void (*m_Func)( std::vector<std::string> );
		std::vector< SS_Var* > (*m_Func)( std::vector<SS_Var*>& );

		void* Get_Child_Ex( std::string Name, bool Probe = false );

		static void Core_Print_Info( SS_Var* ThisVar );
		void (*Core_Print_Info_Local)( SS_Var* ThisVar );
	};

	struct SS_Var_String: public SS_Var // "string"
	{
		SS_Var_String();

		SS_Var* SS_Make_New( SS_Var* nParent = NULL );
		static SS_Var_String* Make_New( SS_Var* nParent = NULL );

		void Cleanup();

		void* Get_Child_Ex( std::string Name, bool Probe = false );

		static void Core_Print_Info( SS_Var* ThisVar );

		std::string m_String;
	};

	struct SS_Var_Int: public SS_Var // "int"
	{
		SS_Var_Int();

		SS_Var* SS_Make_New( SS_Var* nParent = NULL );
		static SS_Var_Int* Make_New( SS_Var* nParent = NULL );

		void Cleanup();

		void* Get_Child_Ex( std::string Name, bool Probe = false );

		static void Core_Print_Info( SS_Var* ThisVar );

		int m_Value;
	};

	struct SS_Var_Float: public SS_Var // "float"
	{
		SS_Var_Float();

		SS_Var* SS_Make_New( SS_Var* nParent = NULL );
		static SS_Var_Float* Make_New( SS_Var* nParent = NULL );

		void Cleanup();

		void* Get_Child_Ex( std::string Name, bool Probe = false );

		static void Core_Print_Info( SS_Var* ThisVar );

		float m_Value;
	};



	std::vector<SS_Var*> Func_Shutdown( std::vector<SS_Var*>& Args ); // restart / shutdown
		void Func_Shutdown_Print_Info( SS_Var* ThisVar );
	std::vector<SS_Var*> Func_Info( std::vector<SS_Var*>&  Args  ); // print info about variable (man, help, info)
		// calls void (*Info_Core)( SS_Var* ) that prints info if supported, name = "print_info"
		// calls void (*Info_Core_Local)( SS_Var* ) that prints local info if supported, name = "print_info_local"
		void Func_Info_Print_Info( SS_Var* ThisVar );

	std::vector<SS_Var*> Func_Exec( std::vector<SS_Var*>& Args );
	std::vector<SS_Var*> Func_CD( std::vector<SS_Var*>& Args ); // change locality (cd, goto, chdir)
		void Func_CD_Print_Info( SS_Var* ThisVar );
	std::vector<SS_Var*> Func_List( std::vector<SS_Var*>& Args  ); // list all children of object (list, ls)
		// calls void (*List_Core)( SS_Var* ) that prints children if supported, name = "list_children"
		void Func_List_Print_Info( SS_Var* ThisVar );

	std::vector<SS_Var*> Func_Var( std::vector<SS_Var*>& Args  ); // create new variable in cd (new, var)
		//   calls void (*Var_New_Core)( SS_Var* ) that creates a new var, name = "var_new"
		void Func_Var_Print_Info( SS_Var* ThisVar );

}



#endif
