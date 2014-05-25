#ifndef SURVIVOR_ENGINE_RENDER_INTERFACE
#define SURVIVOR_ENGINE_RENDER_INTERFACE

// This is used for cross-module functionality,
// Or as a general module interface.

#include "../Core.hpp"


namespace SE_Render
{
	void Set_Pos_Gravity( uchar Where );
	void Set_Pos_Unit( uchar What );

	void Set_Size_Unit( uchar What );

	void Translate2D_f( float x, float y );
	void Translate2D_i( int x, int y );
	void Translate2D_v( float* v );

	void Translate3D_f( float x, float y, float z );
	void Translate3D_i( int x, int y, int z );
	void Translate3D_v( float* v );
//	void Quad2D( Vector2& Pos, Vector2& Size, uchar Gravity );
}


#endif
