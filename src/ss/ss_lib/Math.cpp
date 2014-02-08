#include "Math.hpp"
using namespace Survivor_Script_Math;
using namespace Survivor_Script;

#include <math.h>
#include <cmath>

#ifndef NAN
#define NAN 0.f/0.f
#endif


std::string Survivor_Script_Math::Lib_Add_Func( Survivor_Script::SS_Manager* SS_Man )
{
	SS_Var_Function* nFunc_Math = SS_Var_Function::Make_New( );
	nFunc_Math->Name = "math";
	nFunc_Math->m_Func = Func_Math;
	//nFunc_Math->Core_Print_Info_Local = Survivor_Script_Core::Func_Info_Print_Info;
	SS_Man->Lib_Add_Bin( nFunc_Math );


	return "math";
}


#include <stdlib.h>
float Solve_Part( std::string Part )
{
	Part += '\n';

	float Ret = 0.f;

	bool Is_Float = false;
	std::string Get_Num;

	std::vector< float > Vars;
	std::vector< uchar > Ops;

	for( uint i = 0;i < Part.length(); i++ )
	{
		if( Part[i] == '[' )// start part
		{
			int Depth = 1; uint l = 1;
			while( Depth != 0 && i + l < Part.length() )
			{
				if( Part[i + l] == '[' )
					Depth++;
				else if( Part[i + l] == ']' )
					Depth--;

				l++;
			}

			l -= 2;
			i++;

			if( Depth == 0 )// equalized
			{
				float nVar = Solve_Part( Part.substr( i, l ) );
				if( nVar == NAN )
					return NAN;
				else
					Vars.push_back( nVar );

				i += l;
				continue;
			}
			else if( Depth > 0 )// missing ]
			{
				printf("[EE] Missing terminating ']'\n");
				return NAN;
			}
			else if( Depth < 0 )// missing [
			{
				printf("[EE] Missing starting '['\n");
				return NAN;
			}
		}

		else if( isdigit(Part[i]) )
			Get_Num += Part[i];

		else // might be an operation
		{
			switch( Part[i] )
			{
			case ' ': // ignore whitespace
				break;

			case '+':case '-':case '*':case '/':
			{
				Ops.push_back( Part[i] );

				if( Get_Num.length() > 0 )
				{
					Vars.push_back( atof(Get_Num.c_str()) );
					Get_Num.clear();// = "";
				}
			}break;

			case '.':
			{
				if( Is_Float == false )
				{
					Get_Num += Part[i];
					Is_Float = true;
				}
				else // error
				{
					printf("[EE] Too many decimal points in number\n");
					return NAN;
				}
			}break;
			case '\n':
			{
				if( Get_Num.length() > 0 )
				{
					Vars.push_back( atof(Get_Num.c_str()) );
					Get_Num = "";
				}
			}break;
			}
		}
	}

	if( Vars.size() == 0 )
		return NAN;

	Ret = Vars[0]; Vars.erase( Vars.begin() );

	if( Vars.size() < Ops.size() )
	{
		printf( "[EE] Too many ops to vars\n" );
		return NAN;
	}
	else if( Vars.size() > Ops.size() )
	{
		printf( "[EE] Too many vars to ops %d/%d\n", Vars.size(), Ops.size() );
		return NAN;
	}


	for( uint i = 0;i < Ops.size(); i++ )
	{
		switch( Ops[i] )
		{
		case '+': Ret += Vars[i]; break;
		case '-': Ret -= Vars[i]; break;
		case '*': Ret *= Vars[i]; break;
		case '/': Ret /= Vars[i]; break;
		}
	}

	return Ret;
}
/* write 32bit n^bit math processor
 *
 * '12456'...'1222'
 * add single digits at a time
 * move up if >
 *
 */

std::vector<SS_Var*> Survivor_Script_Math::Func_Math( std::vector<SS_Var*>& Args )
{
	if( Args[0]->Name[0] == '\"' )
		Args[0]->Name.erase( Args[0]->Name.begin() );

	if( Args[0]->Name[Args[0]->Name.length() - 1] == '\"' )
			Args[0]->Name.pop_back();


	float End_Val = Solve_Part( Args[0]->Name );

	printf("Answer: %g\n", End_Val );
}
