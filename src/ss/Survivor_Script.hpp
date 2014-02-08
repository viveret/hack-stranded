#ifndef SURVIVOR_SCRIPT_MAIN
#define SURVIVOR_SCRIPT_MAIN

#include <string>
#include <vector>
#include <stack>
#include "../Core.hpp"
//#include "ss_lib/SS_Libs.h"


#define SS_MAN SS_Main

#define SS_PRINT_ERRORS 1

#define SS_ADD_MEM(X) X;
#define SS_REM_MEM(X) X;


#define SS_PRINTF printf

namespace Survivor_Script
{
	enum{
		SS_OK = 0,
		SS_ERROR,
		SS_UNKNOWN = SS_ERROR
	};

	extern bool g_Verbose;

	struct SS_Var
	{
		SS_Var();

		virtual SS_Var* SS_Make_New( SS_Var* nParent = NULL ) = 0;
		//virtual SS_Var* Clone() = 0;
		virtual void Cleanup() = 0;

		std::string Name;

		std::string Type();
		void Set_Type( std::string Type ); // can only be set once

		virtual SS_Var* Get_Child( std::string Name, bool Probe = false );
		virtual void* Get_Child_Ex( std::string Name, bool Probe = false );

		SS_Var* Parent;

		bool Is_Pointer();
		void Set_Pointer( bool Is );
	private:
		std::string m_Type;
		bool Pointer; // do not delete
	};

	struct SS_Manager;
	extern Survivor_Script::SS_Manager SS_Main;

#include "ss_lib/Core.hpp"
	using namespace Survivor_Script_Core;

	typedef SS_Var* Ext_Var_Link;

	struct SS_Manager
	{
		SS_Manager();
		void Initialize();

		void Lib_Add( std::string(*Lib_Func)(SS_Manager*) ); // call function to add library
		void Lib_Add_Bin( SS_Var* What );
		SS_Var_Template* Lib_Add_Template( SS_Var* (*Func)( SS_Var* ), std::string Name );

		void Update( float dt );

		int G_Argc;
		char** G_Argv;

		void Cleanup();
		void Delete( SS_Var* What );

		uchar Handle_Statement( const std::string& Line );
		static uchar Handle_Tokens( const std::string& Line, std::vector< SS_Var* >* Buffer );

		/*static SS_Var* Token_To_Var( std::string& Token );
		static std::vector< SS_Var* > Tokens_To_Vars( std::vector< std::string >& Tokens );*/

		SS_Var* Get_Var( std::string Name, bool Probe = false );

		SS_Var_Pointer* Create_Link( std::string Name, SS_Var* To, SS_Var* Where = NULL );
		void Delete_Link( SS_Var_Pointer* Ptr );
		void Delete_Link_Source( SS_Var* To );// delete all with pointing to

		static void Print_Err_Exist( std::string Child, std::string Parent );


		SS_Var_Container Global_Space;
		SS_Var* Current_Var;

		void Stack_Add( SS_Var* Var );
	private:
		std::stack< SS_Var* > Stack_Heap;
		uint Stack_Depth;

		void Stack_Push();
		void Stack_Pop();

		std::vector< SS_Var_Pointer* > Active_Links;


		std::vector<SS_Var*> Handle_Part( std::vector<SS_Var*>& Tokens );
	};

	/* TODO: functions
	 *
	 * call / exec -> external functions
	 * color -> change colors
	 * cls  -> clear screen
	 * copy -> files or variables
	 * del -> delete files or variables
	 * deltree -> recursive
	 * echo / print
	 * ipconfig / connect -> set network variables
	 * rename
	 * set
	 * version
	 * find
	 * ln / bind -> symbolic link, also for key input
	 * protect / readonly: set to readonly
	 * setprm -> set permisions (read / write for levels)
	 * sleep
	 * memstat -> memory statistics
	 * enable / disable -> any proccesses / usages
	 */

	/* VARIABLES
	 *
	 * timescale
	 * closed captions
	 * allow download / upload
	 * show hud / fps / position / etc
	 * preload
	 * mouse enabled
	 * user data
	 */
}


#endif
