#ifndef SURVIVOR_ENGINE_MANAGER
#define SURVIVOR_ENGINE_MANAGER

#include "../Core.hpp"
#include "../module/Thread.hpp"

#include <string>
#include <vector>

namespace SE_Manager
{
	template< typename T >
	struct SE_Man_Resource
	{
		SE_Man_Resource();

		uint Handle_ID;
		std::string Handle_Str;

		T m_Data;
		uint Reference_Counter;
	};


	template< typename T >
	struct SE_Man_Manager
	{
		SE_Man_Manager();
		~SE_Man_Manager();

		void Cleanup()
		{
			//
		}

		uint Get_ID( SE_Man_Resource<T>* Which );
		SE_Man_Resource<T>* Get_Item( uint Which );

		virtual SE_Man_Resource<T>* Gen_Item() = 0;// generate new item

		uint Resource_Add_New( SE_Man_Resource<T>* What );
		void Resource_Remove( uint Which );
		void Resource_Remove( SE_Man_Resource<T>* Which );

	private:
		std::vector< SE_Man_Resource<T>* > m_Resources;
		SE_Thread::Mutex m_Mutex;

		uint Gen_ID();

		// Function to cleanup a resource
		void (*Func_Cleanup)( SE_Man_Resource<T>* Which );
	};
}

#endif
