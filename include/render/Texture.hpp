#ifndef SURVIVOR_ENGINE_TEXTURE
#define SURVIVOR_ENGINE_TEXTURE

#include "../Core.hpp"

namespace SE_Texture
{
	struct Tex_Stats;
	typedef Tex_Stats* Tex_ID;

	Tex_ID Retrieve_Texture( const std::string& VFS_Name );
	void Use_Texture( Tex_ID Which );


	void Register_Module();
}



#endif
