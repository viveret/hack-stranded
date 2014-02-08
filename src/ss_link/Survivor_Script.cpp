#include "Survivor_Script.h"
using namespace Survivor_Script;
#include <assert.h>

namespace Survivor_Script
{
	bool g_Verbose = false;
}

SS_Var::SS_Var()
{
	this->Parent = NULL;
	this->Pointer = -1;
}

SS_Var* SS_Var::Get_Child( std::string Name, bool Probe )
{
	if( Name == ".." )
		return this->Parent;

	else if( Probe == false )
		printf("[EE] Variable \"%s\" does not have child \"%s\"\n", this->Name.c_str(), Name.c_str() );

	return NULL;
}

void* SS_Var::Get_Child_Ex( std::string Name, bool Probe )
{
	if( Probe == false )
		printf("[EE] Variable \"%s\" does not have child \"%s\"\n", this->Name.c_str(), Name.c_str() );

	return NULL;
}

std::string SS_Var::Type()
{
	return this->m_Type;
}

void SS_Var::Set_Type( std::string Type )
{
	if( this->m_Type.length() == 0 )
		this->m_Type = Type;
}

bool SS_Var::Is_Pointer()
{
	if( this->Pointer != -1 )
		return this->Pointer;
	else
		return false; // just in case
}

void SS_Var::Set_Pointer( bool Is )
{
	if( this->Pointer == -1 )
		this->Pointer = Is;
}




struct SS_Var_Syntax: public SS_Var// name - syntax
{
	SS_Var_Syntax()
	{
		this->Set_Type( "_syntax" );
	}

	SS_Var* SS_Make_New( SS_Var* nParent )
	{
		return NULL;
	}

	void Cleanup()
	{
		// none
	}
};







SS_Manager::SS_Manager()
{
	this->Current_Var = &this->Global_Space;
	this->Stack_Depth = 0;
}

void SS_Manager::Initialize()
{
	this->Global_Space.Name = "Global";

	SS_Var_Container* Template_Dir = SS_Var_Container::Make_New();
	Template_Dir->Name = "temps";
	this->Global_Space.Children.push_back( Template_Dir );

	SS_Var_Container* Lib_Dir = SS_Var_Container::Make_New();
	Lib_Dir->Name = "bin";
	this->Global_Space.Children.push_back( Lib_Dir );

	this->Current_Var = Lib_Dir;

	this->Lib_Add( Survivor_Script_Core::Lib_Add_Func );
}

void SS_Manager::Lib_Add( std::string (*Lib_Func)(SS_Manager*) ) // call function to add library
{
	std::string Ret = Lib_Func( this );

	if( Ret.length() > 0 )
		printf( "Library \"%s\" successfully added\n", Ret.c_str() );
	else
		printf( "Library \"%s\" was NOT successfully added\n", Ret.c_str() );
}

void SS_Manager::Lib_Add_Bin( SS_Var* What )
{
	if( What == NULL ) return;

	SS_Var* Get_Bin = this->Global_Space.Get_Child( "bin" );

	if( Get_Bin == NULL ) return;
	if( Get_Bin->Type() != "container" ) return;

	((SS_Var_Container*)Get_Bin)->Children.push_back( What );
	What->Parent = Get_Bin;
}

SS_Var_Template* SS_Manager::Lib_Add_Template( SS_Var* (*Func)( SS_Var* ), std::string Name )
{
	SS_Var* Get_Templ = this->Global_Space.Get_Child( "temps" );

	if( Get_Templ == NULL ) return NULL;
	if( Get_Templ->Type() != "container" ) return NULL;


	SS_Var_Template* nTempl = SS_Var_Template::Make_New();
	nTempl->Name = Name;
	nTempl->Var_New_Func = Func;


	((SS_Var_Container*)Get_Templ)->Children.push_back( nTempl );
	nTempl->Parent = Get_Templ;

	return nTempl;
}

void SS_Manager::Cleanup()
{
	this->Global_Space.Cleanup();
}

void SS_Manager::Delete( SS_Var* What )
{
	if( What == NULL ) return;

	What->Cleanup();

	if( What->Type() == "pointer" )
		;//this->Delete_Link( What );

	// remove from engine mem
	delete What;
}

std::vector<SS_Var*> SS_Manager::Handle_Part( std::vector<SS_Var*>& Tokens )
{
	this->Stack_Push();

	if( Tokens.size() == 0 )
		return std::vector<SS_Var*>();

	if( Tokens[0]->Type() != "func" )
	{
		printf( "[EE] Statement must start with an input object, not \"%s\"\n", Tokens[0]->Type().c_str() );
		printf( "Name: %s\n", Tokens[0]->Name.c_str() );
		return std::vector<SS_Var*>();
	}

	SS_Var_Function* nFunc = (SS_Var_Function*)Tokens[0];

	Tokens.erase( Tokens.begin() );

	if( nFunc->m_Func )
		return nFunc->m_Func( Tokens );
	else
	{
		printf( "[EE] Input object has no data\n");
		return std::vector<SS_Var*>();
	}


	return std::vector<SS_Var*>();
	this->Stack_Pop();
}

std::vector< SS_Var* > Do_Statement( std::vector< SS_Var* >& Args )
{
	std::vector< SS_Var* > Ret;


	for( uint i = 0;i < Args.size(); i++ )
	{
		//if(// Args[i]->Type == add)
	}

}

uchar SS_Manager::Handle_Statement( const std::string& Line )
{
	this->Stack_Push();

	std::vector< SS_Var* > Tokens;

	uchar nErr = this->Handle_Tokens( Line, &Tokens );
	if( nErr != SS_OK )
		return nErr;

	if( Tokens.size() == 0 )
	{
		printf( "[EE] Need at least 1 token\n" );
		return SS_ERROR;
	}

	this->Handle_Part( Tokens );

	this->Stack_Pop();

	return SS_OK;
}



#define TOKEN_UNKNOWN 0
#define TOKEN_ERROR unsigned(-1)
#define TOKEN_STRING 1
#define TOKEN_FLOAT 2
#define TOKEN_INT 3

uchar SS_Manager::Handle_Tokens( const std::string& Line, std::vector< SS_Var* >* Buffer )
{
	if( g_Verbose )
		printf( "Tokens:" );
	uchar Type = 0;

	uint Start_Token = 0;
	for( uint l = 0;l < Line.length(); l++)
	{
		bool End_Token = false;
		//Line[l] = tolower(Line[l]);
		if( Type == 0 )
		{
			if( Line[l] == '\"' || Line[l] == '\'' )
				Type = TOKEN_STRING;

			else if( isdigit(Line[l]) )
				Type = TOKEN_INT;

			else if( isalnum( Line[l] ) == false && Line[l] != '_' && Line[l] != '/' && Line[l] != '.' )// not a name, an op
			{
				End_Token = true;
			}
			else if( Line[l] == '[' || Line[l] == ']' )
				End_Token = true;
		}
		else if( Type == TOKEN_STRING )
		{
			if( Line[l] == '\"' || Line[l] == '\'' )// ending str
			{
				End_Token = true;
			}
		}
		else if( Type == TOKEN_INT || Type == TOKEN_FLOAT )
		{
			if( Line[l] == '.' )
				Type = TOKEN_FLOAT;

			else if( isdigit(Line[l]) == false )
			{
				End_Token = true;
				Type = TOKEN_ERROR;
			}
		}


		// END TOKEN CONDITIONS
		if( Line[l] == ' ' || Line[l] == ',' || Line[l] == '\n' )// seperating op
		{
			End_Token = true;
		}
		if( l == Line.length() - 1 )
		{
			l++;
			End_Token = true;
		}

		// HANDLE TOKEN
		if( End_Token )
		{
			End_Token = false;
			std::string nToken = Line.substr(Start_Token, l - Start_Token );

			if( g_Verbose )
				printf( "\n\t%s", nToken.c_str() );

			if( nToken == "verbose" )
			{
				g_Verbose = !g_Verbose;
				if( g_Verbose )
					printf( "Verbose ON\n" );
				else
					printf( "Verbose OFF\n" );
			}
			else if( nToken.length() > 0 )// actually has something
			{
				SS_Var* nVar = NULL;
				switch( Type )
				{
				default: case TOKEN_UNKNOWN:
				{
					if( nToken == "/" ) // root
						nVar = &SS_MAN.Global_Space;

					else if( nToken[0] == '/' ) // global
						nVar = SS_MAN.Global_Space.Get_Child( nToken );

					else
					{
						nVar = SS_MAN.Get_Var( nToken, true );
						if( nVar == NULL )
							nVar = SS_MAN.Global_Space.Get_Child( "bin" )->Get_Child( nToken, true );
					}
				}break;
				case TOKEN_STRING:
				{
					nToken.pop_back();
					nToken.erase( nToken.begin() );

					nVar = SS_Var_String::Make_New();
					((SS_Var_String*)nVar)->m_String = nToken;
					SS_MAN.Stack_Add( nVar );
				}break;
				case TOKEN_INT:
				{
					nVar = SS_Var_String::Make_New();
					((SS_Var_String*)nVar)->m_String = nToken;
					SS_MAN.Stack_Add( nVar );
				}break;
				case TOKEN_FLOAT:
				{
					nVar = SS_Var_String::Make_New();
					((SS_Var_String*)nVar)->m_String = nToken;
					SS_MAN.Stack_Add( nVar );
				}break;
				}

				if( nVar ) // was successful
					Buffer->push_back( nVar );

				else // also TOKEN_ERROR/
				{
					printf( "\n[EE] Token \"%s\" not recognized\n", nToken.c_str() );

					SS_MAN.Stack_Pop();
					Buffer->clear();
					return SS_ERROR;
				}

				Type = TOKEN_UNKNOWN;
				Start_Token = l + 1;
			}
		}
	}

	if( g_Verbose )
		printf( "\n" );

	return SS_OK;
}

SS_Var* SS_Manager::Get_Var( std::string Name, bool Probe )
{
	SS_Var* nVar = NULL;

	if( (nVar = this->Current_Var->Get_Child( Name, true )) != NULL )
		return nVar;

	else if( (nVar = this->Global_Space.Get_Child( Name, true )) != NULL )
		return nVar;

	else
	{
		if( Probe == false )
			printf( "[EE] Variable \"%s\" does not exist\n", Name.c_str() );

		return NULL;
	}
}

SS_Var_Pointer* SS_Manager::Create_Link( std::string Name, SS_Var* To, SS_Var* Where )
{
	SS_Var_Pointer* nPtr = SS_Var_Pointer::Make_New( Where );
	nPtr->Pointing_To = To;
	nPtr->Name = Name;

	this->Active_Links.push_back( nPtr );

	return NULL;
}

void SS_Manager::Delete_Link( SS_Var_Pointer* Ptr )
{
	for( uint i = 0;i < this->Active_Links.size(); i++ )
	{
		if( this->Active_Links[i] == Ptr )
		{
			//delete
			this->Active_Links.erase( this->Active_Links.begin() + i );
			return;
		}
	}
}

void SS_Manager::Delete_Link_Source( SS_Var* To )// delete all with pointing to
{
	for( uint i = 0;i < this->Active_Links.size(); i++ )
	{
		if( this->Active_Links[i]->Pointing_To == To )
			this->Delete_Link( this->Active_Links[i] );
	}
}

void SS_Manager::Print_Err_Exist( std::string Child, std::string Parent )
{
	printf("[EE] Variable \"%s\" does not exist in \"%s\"\n", Child.c_str(), Parent.c_str() );
}

void SS_Manager::Stack_Add( SS_Var* Var )
{
	this->Stack_Heap.push( Var );
}

void SS_Manager::Stack_Push()
{
	// Check for overflow
	assert( this->Stack_Depth != unsigned(-1) );

	this->Stack_Heap.push( NULL );
	this->Stack_Depth++;
}

void SS_Manager::Stack_Pop()
{
	// Check for underflow
	assert( this->Stack_Depth != 0 );

	while( this->Stack_Heap.size() > 0 && this->Stack_Heap.top() != NULL )
	{
		if( g_Verbose )
			printf("[DD] Deleting %p\n", this->Stack_Heap.top() );

		delete this->Stack_Heap.top();
		this->Stack_Heap.pop();
	}

	this->Stack_Heap.pop();
	this->Stack_Depth--;
}

/*void SS_Manager::Printf( const char* format, ... )
{
	char Str[512];memset( Str, 0, 512 );

    va_list args;
    va_start( args, format );
    vsprintf( Str, format, args );
    va_end( args );

	this->Log( Str, L_Type );
}*/
