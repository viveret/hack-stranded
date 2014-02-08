#include "Cipher.hpp"
using namespace SE_Cipher;

//#include <assert.h>

void SE_Cipher::Cipher( Cipher_Key mKey, void* Data, uint Size )
{
//	assert( Data != NULL );
//	assert( Size != 0 );

//	for( uint i = 0; i < Size / sizeof(mKey); i++ )
//		(*(((char*)Data) + i )) = (*(((uint*)Data) + i )) ^ mKey;

	for( uint i = 0; i < Size; i++ )
		*(((uchar*)Data)+i) = *(((uchar*)Data) + i) ^ *((uchar*)(&Size + i%sizeof(uint)));

//	for( uint i = 0; i < Size % sizeof(mKey); i++ )

}

Cipher_Key SE_Cipher::Gen_Key()
{
	return 0x1289ABEF;
}
