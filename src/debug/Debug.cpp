#include "../debug/Debug.hpp"
#include "../module/Module.hpp"

S_Error::S_Error()
{
	this->Level=0;
}

S_Error::S_Error(std::string nMessage,unsigned int nLevel)
{
	this->Message=nMessage;
	this->Level=nLevel;
}

namespace SE_Debug
{
	struct Watcher
	{
		void* Data;
		std::string Where;
		uint Size;

		Watcher()
		{
			this->Data = NULL;
			this->Size = 0;
		}
	};

/*********************   GLOBALS   ***********************/
	std::vector< Watcher > Watching_Vars;

	SE_Module_ID Mod_Debug = NULL;

/*********************   FUNCTIONS   ***********************/

	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
}
using namespace SE_Debug;





void SE_Debug::Watch( void* Data, std::string Where, uint Size )
{
	Watcher nWatch;
	nWatch.Data = Data;
	nWatch.Where = Where;
	nWatch.Size = Size;

	Watching_Vars.push_back( nWatch );
}

void SE_Debug::Unwatch( void* Data )
{
	for( uint i = 0; i < Watching_Vars.size(); i++ )
	{
		if( Watching_Vars[i].Data == Data )
		{
			Watching_Vars.erase( Watching_Vars.begin() + i );
			i--;
//			break; // Uncomment to remove first case (May result in false positives)
		}
	}
}



uint SE_Debug::Initialize( const std::vector<std::string>& mArgs )
{
	return SE_SUCCESS;
}

uint SE_Debug::Cleanup()
{
	if( Watching_Vars.size() > 0 )
	{
		sePrintf( SEPRINT_MODULE, "Unfreed memory:" );
		for( uint i = 0; i < Watching_Vars.size(); i++ )
			sePrintf( SEPRINT_WARN, "\n\t%p: %s", Watching_Vars[i].Data, Watching_Vars[i].Where.c_str() );

		sePrintf( SEPRINT_MODULE, "\n" );
	}
	else
		sePrintf( SEPRINT_MODULE, "All memory freed\n" );

	return SE_SUCCESS;
}

void SE_Debug::Register_Module()
{	
	Mod_Debug = S_Engine::Generate_Module( "debug" );
	S_Engine::Register_Module_Engine( Mod_Debug, S_Engine::Reg_Mod_Time::LOW_LEVEL, Initialize, Cleanup );
}
