#ifndef SURVIVOR_ENGINE_CIPHER
#define SURVIVOR_ENGINE_CIPHER


//#include "../Types.hpp"
#include "../Core.hpp"


namespace SE_Cipher
{
	typedef uint32 Cipher_Key;

	void Cipher( Cipher_Key mKey, void* Data, uint Size );

	inline void Encrypt( Cipher_Key mKey, void* Data, uint Size )
	{
		Cipher( mKey, Data, Size );
	}
	inline void Decrypt( Cipher_Key mKey, void* Data, uint Size )
	{
		Cipher( mKey, Data, Size );
	}

	Cipher_Key Gen_Key();
}


#endif
