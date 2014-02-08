#ifndef SURVIVOR_ENGINE_MAP_2D_TILE
#define SURVIVOR_ENGINE_MAP_2D_TILE

#include "../Core.hpp"



namespace SE_Map_2D
{
	struct Map
	{
		Map();
		~Map();

		void Initialize( uint x, uint y );
		void Cleanup();		

		void Draw_Base( uchar Level );
		virtual void Draw( uchar Level );

		uchar Get( uint x, uint y, uchar Level );

		// mutator operations
		void Set( uint x, uint y, uchar Level, uchar ID );
		void Set_Box( uint x, uint y, uint w, uint h, uchar Level, uchar ID );

		uint Get_W();
		uint Get_H();

		uchar Tex_Size; // 8 x 8

	private:
		uchar* Map_Array;

		uint w,h;
	};
}







#endif
