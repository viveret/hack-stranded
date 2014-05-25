#ifndef SURVIVOR_ENGINE_UNIQUE_ID
#define SURVIVOR_ENGINE_UNIQUE_ID


#include "../Types.hpp"
#include <string>

namespace SE_UID
{
	struct UID_Manager;
	
	uint New( UID_Manager* This );
	void Delete( UID_Manager* This, uint ID );

	uint Get_ID( UID_Manager* This, void* Ptr );

	void* Get_Ptr( UID_Manager* This, uint ID );
	void Set_Ptr( UID_Manager* This, uint ID, void* What );

	UID_Manager* Create_Manager( std::string Name, void*(*Func_New)(), void(*Func_Del)(void*), bool(*Func_Compare)(void*,void*) );
	void Delete_Manager( UID_Manager* This );

	extern UID_Manager* Global_UID;

	// Module
	void Register_Module();
}


#endif
