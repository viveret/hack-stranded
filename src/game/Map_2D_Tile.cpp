#include "Map_2D_Tile.hpp"
using namespace SE_Map_2D;

#include "../engine/Engine.hpp"
#include "../module/Module.hpp"

#include "../gl/se_gl.hpp"
#include <memory.h>


Map::Map()
{
	this->w = 0;
	this->h = 0;

	this->Map_Array = NULL;

	this->Tex_Size = 1;
}

Map::~Map()
{
	this->Cleanup();
}

void Map::Initialize( uint x, uint y )
{
	if( x != 0 && y != 0 )
	{
		if( this->Map_Array == NULL )
		{
			this->w = x;
			this->h = y;

			this->Map_Array = new uchar[ x * y * 3 ];
#ifdef _TEST
			Engine.Mem_Add( this->Map_Array, "Map Array" );
#endif
			memset( this->Map_Array, 0, this->w * this->h * 3 );
		}
		else
			sePrintf( SEPRINT_ERROR, "Cannot Make Map When One Exists\n" );
	}
	else
		sePrintf( SEPRINT_ERROR, "Cannot Make Map With 0 Length/Width\n" );
}

void Map::Cleanup()
{
	if( this->Map_Array != NULL )
	{
#ifdef _TEST
		Engine.Mem_Remove( this->Map_Array );
#endif
		delete this->Map_Array;
		this->Map_Array = NULL;

		sePrintf( SEPRINT_DEBUG, "Cleaned Map\n" );
	}
}

void Map::Draw_Base( uchar Level )
{			  
	if( Level < 3 )
	{			
		float Tex_X = 0.f, Tex_Y = 0.f, Tex_S = 1.f/this->Tex_Size;

		glPushMatrix();

		glBegin( GL_QUADS );  
		for( uint y = 0;y < this->h; y++ )
		{
			for( uint x = 0;x < this->w; x++ )
			{
				Tex_X = this->Get(x, y, Level) % this->Tex_Size + 0.f, Tex_Y = this->Get(x, y, Level) / this->Tex_Size + 0.f;
									   
				Tex_X *= Tex_S;
				Tex_Y *= Tex_S;

				if( this->Get( x, y, Level ) != 0 )
				{
					glTexCoord2f(Tex_X,			Tex_Y		 );glVertex2f(-0.5f + x,-0.5f + y);
					glTexCoord2f(Tex_X + Tex_S,	Tex_Y		 );glVertex2f(+0.5f + x,-0.5f + y);
					glTexCoord2f(Tex_X + Tex_S,	Tex_Y + Tex_S);glVertex2f(+0.5f + x,+0.5f + y);
					glTexCoord2f(Tex_X,			Tex_Y + Tex_S);glVertex2f(-0.5f + x,+0.5f + y);
				}
			}			
		}
		glEnd();

		glPopMatrix();
	}
}

void Map::Draw( uchar Level )
{
	this->Draw_Base( Level );
}

uchar Map::Get( uint x, uint y, uchar Level )
{
	if( x < this->w && y < this->h && Level < 3 )
		return this->Map_Array[ x + y * this->h + (this->w * this->h * Level) ];

	else
		return 0;
}

// mutator operations
void Map::Set( uint x, uint y, uchar Level, uchar ID )
{				 
	if( x < this->w && y < this->h && Level < 3 )
		this->Map_Array[ x + y * this->h + (this->w * this->h * Level) ] = ID;
}

void Map::Set_Box( uint x, uint y, uint w, uint h, uchar Level, uchar ID )
{
	if( x + w <= this->w && y + h <= this->h && Level < 3 && this->Map_Array )
	{							
		for( uint i_y = 0;i_y < h; i_y++ ){
		for( uint i_x = 0;i_x < w; i_x++ )
		{
			this->Set( i_x + x, i_y + y, Level, ID );
		}}
	}
}
		  
uint Map::Get_W()
{				   
	return this->w;
}

uint Map::Get_H()
{
	return this->h;
}
