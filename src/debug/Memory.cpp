#include "Memory.hpp"
#include <vector>
#include <cassert>


namespace SE_Memory
{
struct Mem_Tag
{
	Mem_Tag();

	std::string Name;
	void* Data;
	uint Size;

	uchar Delete;
};
/***********    GLOBALS    **********/

std::vector< Mem_Tag > Memory_Tags;

}
using namespace SE_Memory;



/*************    MEMORY OPERATIONS    *********/

void* SE_Memory::Alloc( uint Size )
{
	assert( Size != 0 );

	Memory_Tags.push_back( Mem_Tag() );
	Memory_Tags[ Memory_Tags.size() - 1 ].Data = new uchar[ Size ];
	Memory_Tags[ Memory_Tags.size() - 1 ].Size = Size;

	return	Memory_Tags[ Memory_Tags.size() - 1 ].Data;
}

void SE_Memory::Free( void* What )
{
	for( uint i = Memory_Tags.size() - 1; i <= Memory_Tags.size() - 1; i-- )
	{
		if( Memory_Tags[i].Data == What )
		{
			if( Memory_Tags[i].Delete )
				delete[] Memory_Tags[i].Data;

			Memory_Tags.erase( Memory_Tags.begin() + i );
			return;
		}
	}
}

void SE_Memory::Tag( void* What, std::string Name )
{
	assert( What != NULL );

	Memory_Tags.push_back( Mem_Tag() );
	Memory_Tags[ Memory_Tags.size() - 1 ].Data = What;
	Memory_Tags[ Memory_Tags.size() - 1 ].Size = 0;
	Memory_Tags[ Memory_Tags.size() - 1 ].Name = Name;
	Memory_Tags[ Memory_Tags.size() - 1 ].Delete = false;
}


/**********   MEM TAG   **************/


Mem_Tag::Mem_Tag()
{
	this->Data = NULL;
	this->Size = 0;
	this->Delete = true;
}

