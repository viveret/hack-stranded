#ifndef SURVIVOR_ENGINE_HIEGHT_MAP
#define SURVIVOR_ENGINE_HIEGHT_MAP

#include "../Core.hpp"
#include "../math/Math.hpp"

#include "Map.hpp"

namespace SE_Map_Height_Namespace
{
	struct SE_Map_Vert
	{
		float Height;
		Vector3f Normal;
	};

	struct SE_Map_Height:public SE_Game::SE_Map::Map_Cell
	{
		SE_Map_Height();
		~SE_Map_Height();

		void Make( int w, int l, Vector3f Size, uint Flags );// size == bullet
		void Initialize( Vector3f Gen_Pos, Vector3f Gen_Scale );
		void Initialize( float* Data );

		void Bullet_Attatch();// attatch bullet physics to it

		void Gen_Normals();
		Vector3f Get_Normal( uint x, uint z );
		void Cleanup();

		void Draw_Base();
		virtual void Draw();

		float Get( uint x, uint z );
		float Get( Vector3f Pos ); // other

		// mutator operations
		void Set( uint x, uint z, float a );
		void Set_Box( uint x, uint z, uint w, uint l, float h );

		uint Get_W();
		uint Get_L();

		void Translate( Vector3f Trans );
		void Load_Mem( SE_Array<char>& C_Mem );

	private:

		//float m_H;// max +/-
		Vector3f m_Size;

		float* m_Array;
		uint m_W, m_L;
	};
}


#define SE_HEIGHTMAP_COLOR 0x01
#define SE_HEIGHTMAP_NORMAL 0x02
#define SE_HEIGHTMAP_TEXTURE 0x04
#define SE_HEIGHTMAP_MATERIAL 0x08

#endif
