#ifndef SURVIVOR_ENGINE_MEMORY
#define SURVIVOR_ENGINE_MEMORY

#include "../Types.hpp"

#include <string>

namespace SE_Memory
{
	void* Alloc( uint Size );
	void Free( void* What );

	void Tag( void* What, std::string Name );
}



#endif
