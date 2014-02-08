#include "Interactive_Text.hpp"
#include "../Core.hpp"
#include "../module/Module.hpp"

namespace SE_Interactive_Text
{
/*****************    GLOBALS    **************************/
	IT_Obj Global_Space;

	// Convert alternates to standards
	std::map< std::string, std::string > Word_Map;

/*****************    FUNCTIONS    ************************/
	void Initialize();
	void Cleanup();

	void State_Save( std::string F_Name );
	void State_Load( std::string F_Name );

	// Checks
	bool Is_Filler( const std::string& What );
}
using namespace SE_Interactive_Text;


namespace Parse_States
{enum{
	NORMAL = 0,
	STRING,
	EOT // END OF TOKEN, BREAK
};}

void SE_Interactive_Text::Input_Str( const std::string& Text )
{
	std::vector< std::string > Tokens;

	uchar Parse_State = Parse_States::NORMAL;
	ssize_t Sep_Last = 0;
	for( uint l = 0; l <= Text.size(); l++ )
	{
		switch( Parse_State )
		{
		case Parse_States::NORMAL:
			{
				switch( Text[l] )
				{
				case '\n': case '\r': case ' ': case 0:
					{
						Parse_State = Parse_States::EOT;
						l--;
					}break;
				}
			}break;
		case Parse_States::STRING:
			{
				if( Text[l] == '\'' || Text[l] == '\"' )
					Parse_State = Parse_States::NORMAL;
			}break;
		case Parse_States::EOT: // Break, new token
			{
				Tokens.push_back( Text.substr(Sep_Last, l - Sep_Last) );
				Sep_Last = l;
			}break;
		}
	}

	// Proccess tokens
	// First remove excess data

	if( Tokens.size() == 0 )
	{
		sePrintf( SEPRINT_ERROR, "SE_Interactive needs at least 1 token\n" );
		return;
	}	
	else if( Tokens.size() == 1 ) // Just an action
	{
		sePrintf( SEPRINT_MODULE, "SE_Interactive surrently only supports simple statements\n" );
		return;
	}
	else if( Tokens.size() == 2 ) // simple statement
	{
		IT_Obj* Obj_Now = Global_Space.Get_Child( Tokens[2] );
	}
	else
	{
		sePrintf( SEPRINT_WARN, "SE_Interactive does not support %u tokens\n", Tokens.size() );
		return;
	}
}


void SE_Interactive_Text::Initialize()
{

}

void SE_Interactive_Text::Cleanup()
{

}



/********************    IT OBJ FUNCTIONS    ***********************/
void IT_Obj::Cleanup()
{
	for( uint i = 0; i < this->Children.size(); i++ )
	{
		this->Children[i]->Cleanup();
		delete this->Children[i];
	}

	this->Children.clear();
	this->Details.clear();
}

IT_Obj* IT_Obj::Get_Child( const std::string& Name )
{
	for( uint i = 0; i < this->Children.size(); i++ )
	{
		if( this->Children[i]->Name == Name )
			return this->Children[i];
	}

	return NULL;
}

void* IT_Obj::Get_Detail( const std::string& Name )
{
	for( uint i = 0; i < this->Details.size(); i++ )
	{
#ifdef _TEST
		if( this->Details[i]->Name == Name )
			return (void*)this->Details[i];
#endif
	}

	return NULL;
}
