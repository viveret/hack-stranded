#ifndef SURVIVOR_ENGINE_MODULE_PLUGIN
#define SURVIVOR_ENGINE_MODULE_PLUGIN

#include <SE/Core.hpp>
#include <string>
#include <vector>


#ifdef __cplusplus
	extern "C" {
#endif

//namespace SE_Mod_Plugin
//{
	struct SE_Mod_Plugin_Interface
	{
		// must be YY.MM.dd.mm.ss, then any other info
		const char* m_Version;
		const char* m_Name;
		const char* m_Description;

		// return success, takes version #
		void (*Func_Init_Interface)();
		unsigned int (*Func_Mod_Hook)(const char*);
		void* (*Func_Mod_Intface)(const char*);
	};

	// extern SE_Mod_Plugin_Interface SE_Plugin
//}

#ifdef __cplusplus
	}
#endif


// Now for the engine's interface
namespace SE_Mod_Plugin
{
	uint Initialize( const std::vector<std::string>& mArgs );
//	uint Initialize_Hooks();
	uint Cleanup();

	std::vector< SE_Mod_Plugin_Interface* > Get_Plugin_List();

	SE_API void* Get_Plugin( const char* Name, const char* H_Ver );
	
}




#endif
