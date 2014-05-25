#ifndef SURVIVOR_ENGINE_ARRAY
#define SURVIVOR_ENGINE_ARRAY

#include "../Core.hpp"
#include <SE/debug/Debug.hpp>

#include <string>
#include <memory.h>
#include <cassert>

// protected array for memory safety


template< typename T >
class SE_Array
{
	T* m_Data;
	uint m_Size;

public:
	SE_Array( uint nSize, std::string Mem_Name )
	{
		this->Make( nSize, Mem_Name );
	}

	SE_Array()
	{
		this->m_Size = 0;
		this->m_Data = NULL;
	}

	void Make( uint nSize, std::string Mem_Name )
	{
		this->Delete();

		assert( nSize != 0 );// make sure its an actual size

		this->m_Size = nSize;
		this->m_Data = new T[nSize];

		assert( this->m_Data != NULL );
		memset( this->m_Data, 0, sizeof(T) * this->m_Size );

		SE_Debug::Watch( this->m_Data, Mem_Name, this->m_Size );

		Mem_Name.clear();
	}

	void Delete()
	{
		if( this->m_Data )// there is data
		{
			SE_Debug::Unwatch( this->m_Data );

			delete[] this->m_Data;
			this->m_Data = NULL;
		}
	}



	T& operator[] ( const uint nIndex )
	{
		if( nIndex < this->m_Size )
			return this->m_Data[nIndex];

		else
		{
			assert( false );// should not be here
			return this->m_Data[0];
		}
	}

	uint Size()
	{
		return this->m_Size;
	}
	T* Data()
	{
		return this->m_Data;
	}
};

//#undef ADD_TO_MEM
//#undef REM_TO_MEM

typedef SE_Array< char > SE_Raw_Buffer;


#endif

