#include "Survivor_Script.h"
using namespace Survivor_Script;
								   


void Survivor_Script_Manager::Initialize( std::vector< SS_Library >& nLibs )
{					   
	for( ushort i = 0;i < nLibs.size(); i++)
		this->Libs.push_back( nLibs[i].SS_Mod_Func(this) );


	printf("Loaded Libraries: \n");
	for( ushort i = 0;i < this->Libs.size(); i++)
		printf( "%s\n", this->Libs[i].c_str() );

	printf("\n");
}




SS_Var::SS_Var()
{
	this->SS_Locality = 0;
	this->SS_Is_Pointer = false;
}
		   /*

SS_Var* SS_Var::SS_Get_Var( std::string Name )
{
	return this->Data->SS_Get_Var( Name );
}

SS_Var* SS_Var::SS_Get_Func( std::string Name )
{
	return this->Data->SS_Get_Func( Name );
}

void* SS_Var::SS_Make_New( SS_Mult_Vars& Args )
{
	return this->Data->SS_Make_New( Args );
}

void SS_Var::SS_Cleanup( void* This )
{
	this->Data->SS_Cleanup( This );
}
			 */


Survivor_Script_Manager::Survivor_Script_Manager()
{
	this->SS_Global.Children = new SS_Mult_Vars;
}


void Survivor_Script_Manager::Do_Chunk( std::string& Chunk )
{
	//
}

void Survivor_Script_Manager::Do_Line( std::string Line )
{
	std::vector<std::string> Tokens = this->Do_Tokens( Line );

	if( Tokens.size() == 0 )
		return;


	SS_Var* Start_Var = this->Get_Relative_Var( Tokens[0] );

	if( Start_Var == NULL )
	{
		printf( std::string("Unknown token '" + Tokens[0] + "'\n").c_str() );
		return;
	}



	if( Start_Var->SS_Type_Name == "function" )// is a function
		;//((SS_Lib_Core::SS_Func*)Start_Var)->Func( SS_Mult_Vars() );
}																

#include <sstream>
std::vector<std::string> Survivor_Script_Manager::Do_Tokens( std::string Line )
{
	std::vector<std::string> Ret_Tokens;	
	/*std::istringstream Token_Stream( Line );
	

	while( std::getline( Token_Stream, Line, ' ' ) )// for each branch
	{	
		if( Line.length() != 0 )
			Ret_Tokens.push_back( Line );
	} */

	
	Line += '\n';// to end the line

	bool In_Str = false;// ignore until end of string


	uint Start_Token = 0;
	for( uint l = 0;l < Line.length(); l++)
	{
		//Line[l] = tolower(Line[l]);
		if( In_Str == false )
		{	
			if( Line[l] == '\"' || Line[l] == '\'' )
				In_Str = true;

			else if( Line[l] == ' ' || Line[l] == ',' || Line[l] == '\n')// seperating op
			{
				std::string nToken = Line.substr(Start_Token, l - Start_Token );
				if( nToken.length() > 0 )// actually has something
				{
					Ret_Tokens.push_back( nToken );
					Start_Token = l + 1;
				}
			}
			else if( isalnum( Line[l] ) == false && Line[l] != '_' && Line[l] != '.' )// not a name, an op
			{					
				Ret_Tokens.push_back( Line.substr(Start_Token, l - Start_Token ) );
				Start_Token = l;
			}
		}
		else
		{
			if( Line[l] == '\"' || Line[l] == '\'' )// ending str
				In_Str = false;
		}
	}

	return Ret_Tokens;
}

SS_Var* Survivor_Script_Manager::Get_Relative_Var( std::string Name, SS_Var* This )
{
	if( This == NULL )
		This = &this->SS_Global;



	SS_Var* nVar = NULL;

	if( Name.find( '.' ) != Name.npos )// var exists and is a hierarchy
	{	
		std::string Whole_Name = Name;	

		u_int Start_Cur_Var_Name = Name.find_last_of('.') + 1;//find last var
		std::string Cur_Var_Name = Name.substr( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name );
		Name.erase( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name + 1 );


		std::istringstream Name_Stream( Name );
									
		SS_Var* Cur_Dir = nVar;
		SS_Var* Ret_Dir = NULL;

		std::getline( Name_Stream, Name, '.' );// get starting branch
		Cur_Dir = this->SS_Global.SS_Get_Var( Name );

		if( Cur_Dir == NULL )
			return NULL;

		// first get through directories
		while( std::getline( Name_Stream, Name, '.' ) )// for each branch
		{	
			Cur_Dir = Cur_Dir->SS_Get_Var( Name );
			if( Cur_Dir == NULL )// invalid var name
				return NULL;
		}


		return Cur_Dir->SS_Get_Var( Cur_Var_Name );// if it has a child with the same name
	}

	nVar = This->SS_Get_Var( Name );
	if( nVar )
		return nVar;

	// not a child, check global

	nVar = this->SS_Global.SS_Get_Var( Name );

	if( nVar != NULL )// is a global var
		return nVar;


	return NULL;// not found
}

SS_Var* Survivor_Script_Manager::Get_Type( std::string Name )
{
	for( uint i = 0;i < this->Types.size(); i++ )
	{
		if( this->Types[i]->SS_Type_Name == Name )
		{
			return this->Types[i];
		}
	}

	return NULL;
}

void Survivor_Script_Manager::Cleanup()
{
	//
}
	 
void Survivor_Script_Manager::Update()// update global stack
{
	this->Update_Stack( Stack_Pos::STACK_GLOBAL + 1 );
}

void Survivor_Script_Manager::Update_Stack( uint Level )
{
	for( uint i = 0;i < this->Var_Stack.size(); i++ )
	{
		if( this->Var_Stack[i]->SS_Locality >= Level )// out of stack bounds, needs to be deleted
		{
			this->Var_Stack[i]->SS_Cleanup();
			delete this->Var_Stack[i];

			this->Var_Stack.erase( this->Var_Stack.begin() + i );
			i--;
		}
	}
}



							   
/*void Survivor_Script_Manager::Parse_Line( std::string Line, std::vector<std::string> &Tokens )// single statement
{
	Line += '\n';// to end the line

	bool In_Str = false;// ignore until end of string

	SS_Variable* Cur_Var = NULL;

	uint Start_Token = 0;
	for( uint l = 0;l < Line.length(); l++)
	{
		//Line[l] = tolower(Line[l]);
		if( In_Str == false )
		{	
			if( Line[l] == '\"' || Line[l] == '\'' )
				In_Str = true;

			else if( Line[l] == ' ' || Line[l] == ',' || Line[l] == '\n')// seperating op
			{
				std::string nToken = Line.substr(Start_Token, l - Start_Token );
				if( nToken.length() > 0 )// actually has something
				{
					Tokens.push_back( nToken );
					Start_Token = l + 1;
				}
			}
			else if( isalnum( Line[l] ) == false && Line[l] != '_' && Line[l] != '.' )// not a name, an op
			{					
				Tokens.push_back( Line.substr(Start_Token, l - Start_Token ) );
				Start_Token = l;
			}
		}
		else
		{
			if( Line[l] == '\"' || Line[l] == '\'' )// ending str
				In_Str = false;
		}
	}
}

void Survivor_Script_Manager::Do_Line( std::string Line )// single statement
{									   
	std::vector< std::string > Tokens; 

	this->Parse_Line( Line, Tokens );

	// check first token, sets up rest of statement
	if( Tokens[0] == "+" )// add var to global
	{
		//
	}
	else if( Tokens[0] == "-" )// remove var from global
	{
		//
	}

	SS_Func tHandle_Func = this->Get_Func( Tokens[0], &this->SS_Global );
	if( tHandle_Func != NULL )// it exists
	{	
		Tokens.erase( Tokens.begin() );

		SS_MULT_VAR nRet_Vars = tHandle_Func( this->Make_Params( Tokens ) );
		return;
	}

	{// check if it is a variable
		
		SS_Variable* nVar = this->Get_Relative_Var( Tokens[0], &this->SS_Global );

		if( nVar )
		{		
			if( nVar->Type == "type_function" )// is a function
			{	
				//std::string Var_Child = Tokens[0].substr( 
				Tokens.erase( Tokens.begin() );

				SS_MULT_VAR nParams = this->Make_Params( Tokens );
			
				SS_Variable* nThis = new SS_Variable( NULL );
				nThis->Name = nVar->Parent->Name; nThis->Type = "this";
				nThis->Data = (void*)( nVar->Parent );
				nThis->Locality = uchar(-1);
				this->Temp_Stack.push_back( nThis );

				nParams.insert( nParams.begin(), nThis ); 

				((SS_Poly_Func*)(nVar->Data))->Call( nParams );
			}
			//printf( "%s Was found!\n\n", nVar->Name.c_str() );
			return;
		}
	}

	{// check if it is a type
		SS_Type* tHandle = this->Get_Type( Tokens[0] );
		if( tHandle != NULL )
			;
		else
			printf( "[EE] %s\n", Tokens[0].c_str() );

		return;
	}
	


	for( uchar t = 1;t < Tokens.size(); t++)
	{
		// check keywords
		if( "=" )
			;
		// check types

		// check ss commands
	}
}		 */

/*SS_MULT_VAR Survivor_Script_Manager::Make_Params( std::vector< std::string >& Tokens )
{
	SS_MULT_VAR nParams; 
	for( uint l = 0;l < Tokens.size(); l++)
	{
		SS_Variable* Nparam = this->Get_Relative_Var(Tokens[l], &this->SS_Global);	  
		if( Nparam )
			nParams.push_back( Nparam );
		else
		{
			bool Is_OK = false;
			//check all types
			SS_Type* nType = this->Get_Type( Tokens[l] );
			if( nType )// it was a type
			{
				Nparam = new SS_Variable( NULL );
				this->Temp_Stack.push_back( Nparam );

				Nparam->Type = nType->Name;// does not need any data, only type

				nParams.push_back( Nparam );
				Is_OK = true;
			}

			// check for string
			else if( Tokens[l][0] == '\"' && Tokens[l].find_last_of( '\"' ) != Tokens[l].npos )// it is a string
			{
				Nparam = new SS_Variable( NULL );
				this->Temp_Stack.push_back( Nparam );

				Nparam->Type = "string";
				Nparam->Data = this->Get_Type( "string" )->Op_New( SS_MULT_VAR() );
				((std::string*)(Nparam->Data))->assign( Tokens[l].substr(1, Tokens[l].find_last_of( '\"' )-1) );

				nParams.push_back( Nparam );  
				Is_OK = true;
			}

			// check for integer
			else if( isdigit( Tokens[l][0] ) )
			{
				bool All_Num = true;
				for( ushort p = 0;p < Tokens[l].length(); p++ )
					if( !isdigit( Tokens[l][p] ) )
					{
						All_Num = false;
						break;
					}

				if( All_Num )// they were all numbers
				{		   
					Nparam = new SS_Variable( NULL );
					this->Temp_Stack.push_back( Nparam );

					Nparam->Type = "int";
					Nparam->Data = this->Get_Type( "int" )->Op_New( SS_MULT_VAR() );
					*((int*)(Nparam->Data)) = atoi( Tokens[l].c_str() );

					nParams.push_back( Nparam );  
					Is_OK = true;
				}
			}
				

			if( Is_OK == false )
			{
				printf( "[EE] '%s' is not a valid param\n", Tokens[l].c_str() );
				return SS_MULT_VAR();
			}
		}
	}


	return nParams;
}		*/


			 
/*#include <sstream>
SS_Variable* Survivor_Script_Manager::Get_Relative_Var( std::string Name, SS_Variable* Where ) // get variable by local / relative
{							  
	SS_Variable* nVar = NULL;

	if( Name.find( '.' ) != Name.npos )// var exists and is a hierarchy
	{	
		std::string Whole_Name = Name;	

		u_int Start_Cur_Var_Name = Name.find_last_of('.') + 1;//find last var
		std::string Cur_Var_Name = Name.substr( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name );
		Name.erase( Start_Cur_Var_Name, Name.length() - Start_Cur_Var_Name + 1 );


		std::istringstream Name_Stream( Name );
									
		SS_Variable* Cur_Dir = nVar;
		SS_Variable* Ret_Dir = NULL;

		std::getline( Name_Stream, Name, '.' );// get starting branch
		Cur_Dir = this->SS_Global.SS_Var( Name );

		if( Cur_Dir == NULL )
			return NULL;

		// first get through directories
		while( std::getline( Name_Stream, Name, '.' ) )// for each branch
		{	
			Cur_Dir = Cur_Dir->SS_Var( Name );
			if( Cur_Dir == NULL )// invalid var name
				return NULL;
		}


		return Cur_Dir->SS_Var( Cur_Var_Name );// if it has a child with the same name
	}

	nVar = Where->SS_Var( Name );
	if( nVar )
		return nVar;

	// not a child, check global

	nVar = this->SS_Global.SS_Var( Name );

	if( nVar != NULL )// is a global var
		return nVar;


	return NULL;// not found
}	   */

/*void Survivor_Script_Manager::Clear_Stack()
{
	for( ushort i = 0; i < this->Temp_Stack.size(); i++ )
		this->Delete_Var( this->Temp_Stack[i] );

	this->Temp_Stack.clear();
} */
