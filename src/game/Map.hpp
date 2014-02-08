#ifndef SURVIVOR_ENGINE_GAME_MAP
#define SURVIVOR_ENGINE_GAME_MAP

#include "../math/Math.hpp"
#include "../utility/Array.hpp"

#define MAP_SIZE 128.f // +/- x,y,z

#define CELL_WAIT_TIME 2.f // 2 seconds



namespace SE_Game{ namespace SE_Map
{
	struct Map_Cell // base cell, single chunk
	{
		//virtual void Initialize( void* Data ) = 0;
		virtual void Bullet_Attatch() = 0;// attatch bullet physics to it
		virtual void Cleanup() = 0;

		virtual void Translate( Vector3f Trans ) = 0;

		virtual void Load_Mem( SE_Array<char>& C_Mem ) = 0;
	};

 
	struct Map_Cell_Manager
	{
		Map_Cell_Manager();

		void Initialize();
		void Destroy();

		void Update( dt_precision dt );
		void Draw();

		void Generate_Map(); // whole map
		void Generate_Cell( uchar x, uchar y ); // single cell

		schar GPos_X, GPos_Y;

		Map_Cell* Get( uchar x, uchar y );
												   
		void Move_X( schar Dir ); // move in X axis
		void Move_Y( schar Dir ); // move in Y axis

	private:
		Map_Cell* Cells;
		uchar Orig_X, Orig_Y; // where the player is at

		float Wait_Switch;// time to wait between switching
	};
}}


					  


#endif
