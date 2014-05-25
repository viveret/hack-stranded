#ifndef SURVIVOR_ENGINE_COMPONENT_BASED_SYSTEM
#define SURVIVOR_ENGINE_COMPONENT_BASED_SYSTEM


// Individual CBS interfaces

namespace SE_CBS
{
	struct CBS_Manager
	{
		CBS_Manager();
		~CBS_MAnager();

		uint Initialize(  );
		uint Cleanup();

		void* m_Data;

		private:

		
	}


}



#endif
