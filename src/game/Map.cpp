#include "Map.hpp"
using namespace SE_Game::SE_Map;






Map_Cell_Manager::Map_Cell_Manager()
{					 
	this->GPos_X = 0;
	this->GPos_Y = 0;

	this->Orig_X = 1;
	this->Orig_Y = 1;

	this->Wait_Switch = 0.f;
}

void Map_Cell_Manager::Initialize()
{
	//
}

void Map_Cell_Manager::Destroy()
{
	//
}

void Map_Cell_Manager::Update( dt_precision dt )
{
	if( this->Wait_Switch < CELL_WAIT_TIME )
		this->Wait_Switch += dt;
	//
}

void Map_Cell_Manager::Draw()
{
	//
}

void Map_Cell_Manager::Generate_Map() // whole map
{
	//
}

void Map_Cell_Manager::Generate_Cell( uchar x, uchar y ) // single cell
{
	//
}

Map_Cell* Map_Cell_Manager::Get( uchar x, uchar y )
{
	//return &this->Cells[ x % 3 ][ y % 3 ];
	return NULL;
}
												   
void Map_Cell_Manager::Move_X( schar Dir ) // move in X axis
{
	if( this->Wait_Switch >= CELL_WAIT_TIME )
	{
		this->Orig_X += Dir;
		this->Wait_Switch = 0.f;
	}
}

void Map_Cell_Manager::Move_Y( schar Dir ) // move in Y axis
{						
	if( this->Wait_Switch >= CELL_WAIT_TIME )
	{
		this->Orig_Y += Dir;
		this->Wait_Switch = 0.f;
	}
}
