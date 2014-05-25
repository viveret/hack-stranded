#ifndef SURVIVOR_ENGINE_PROFILE
#define SURVIVOR_ENGINE_PROFILE

/* Profiling:
 * measuring performance by recording sections of code.
 * 
 * Register a section to a name,
 * then use that ID or function address
 * to interface with the profiler
 */

#include <string>
#include <SE/module/Thread.hpp>

namespace SE_D_Profile
{
	typedef void* SEDP_ID;

	void SEDP_Register_Thread( SE_Thread::Thread_ID nThread );

	void SEDP_Register( std::string Name, void* Func_Addr );
	void* SEDP_Gen( std::string Name ); // If without func addr

	void SEDP_Enter( void* Func_Addr );
	void SEDP_Exit();

	void SEDP_Sync_Thread( SE_Thread::Thread_ID ID );

	void SEDP_Register_Module();
}
using namespace SE_D_Profile;



#endif
