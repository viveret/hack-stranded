#ifndef SURVIVOR_ENGINE_SERVICE_MANAGER
#define SURVIVOR_ENGINE_SERVICE_MANAGER

#include <SE/Types.hpp>

namespace SE_Service_Lib
{
	void Register_Module();

	struct Service
	{
		const char* Name;
		void (*Func)();
		uchar Priority;
		float Interval;
	};

	struct Service_Manager_Type;
	typedef Service_Manager_Type* Servicer_ID;

	Servicer_ID New_Servicer_Lean( const char* Name );
	Servicer_ID New_Servicer( const char* Name ); // Includes timer

	void Call( Servicer_ID This );
	void Call_Reverse( Servicer_ID This );

	void Add( Service* What, Servicer_ID Where );

	Service* Get_Service( void* Func, Servicer_ID Where );
	Service* Get_Service_Name( const char* Name, Servicer_ID Where );

	void Set_State( void* Func, bool What, Servicer_ID Where );
	void Set_State_Name( const char* Name, bool What, Servicer_ID Where );

// To make our lives easier
	inline void Enable( void* Func, Servicer_ID Where )
	{
		Set_State( Func, true, Where );
	}

	inline void Enable_Name( const char* Name, Servicer_ID Where )
	{
		Set_State_Name( Name, true, Where );
	}

	inline void Disable( void* Func, Servicer_ID Where )
	{
		Set_State( Func, false, Where );
	}

	inline void Disable_Name( const char* Name, Servicer_ID Where )
	{
		Set_State_Name( Name, false, Where );
	}
}



#endif
