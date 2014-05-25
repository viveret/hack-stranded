#ifndef SURVIVOR_ENGINE_RENDER_SPRITE_SHEET
#define SURVIVOR_ENGINE_RENDER_SPRITE_SHEET

#include "SE/Types.hpp"

namespace SE_Render_Sprite_Sheet
{
	struct Sprite_Sheet;
	typedef Sprite_Sheet* Sprite_Sheet_ID;


	Sprite_Sheet_ID Load( std::string Name );
//	Sprite_Sheet_ID New( uint Tex_ID, 

	uint& Sequence( Sprite_Sheet_ID This );
	uchar& Frame( Sprite_Sheet_ID This );
	float& Time( Sprite_Sheet_ID This );

	uint Add_Sequence( Sprite_Sheet_ID This );
	
}



#endif
