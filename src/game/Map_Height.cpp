#include "Map_Height.hpp"
using namespace SE_Map_Height_Namespace;

#include "../S_Engine.hpp"


SE_Map_Height::SE_Map_Height()
{
	this->m_W = 0;
	this->m_L = 0;

	this->m_Array = NULL;
}

SE_Map_Height::~SE_Map_Height()
{
	this->Cleanup();
}

#include <memory.h>
void SE_Map_Height::Make( int w, int l, Vector3f Size, uint Flags )
{
	this->Cleanup();

	this->m_W = w;
	this->m_L = l;

	//this->m_Scale = Vector3( Size.x / w, Size.y / h, Size.z / l );
	this->m_Size = Size;

	this->m_Array = new float[ w * l ];
#ifdef _TEST
	Engine.Mem_Add( this->m_Array, "height map" );
#endif

	memset( this->m_Array, 0, sizeof(float) * w * l );// set all to 0.f
}

#include <libnoise/noise.h>
void SE_Map_Height::Initialize( Vector3f Gen_Pos, Vector3f Gen_Scale )
{
	if( this->m_Array == NULL ) return;

	noise::module::Perlin myModule;

	for( uint z = 0;z < this->m_L; z++ ){
	for( uint x = 0;x < this->m_W; x++ )
	{
		this->Set( x, z, myModule.GetValue(Gen_Pos[0] + Gen_Scale[0] * x, Gen_Pos[1], Gen_Pos[0] + Gen_Scale[0] * z));
	}}
}

void SE_Map_Height::Initialize( float* Data )
{
	if( Data && this->m_Array )// copy data
		memcpy( this->m_Array, Data, sizeof(float) * this->m_W * this->m_L );
}

#include <BulletCollision/CollisionShapes/btHeightfieldTerrainShape.h>
void SE_Map_Height::Bullet_Attatch()// attatch bullet physics to it
{
#ifdef _TEST
	if( Engine.Game == NULL ) return;// need game to use bullet

	btHeightfieldTerrainShape *heightFieldShape = new btHeightfieldTerrainShape(
								this->m_W, this->m_L, this->m_Array,
								1, 0, this->m_Size.y * 2, 1, PHY_FLOAT, false);

	Engine.Mem_Add( heightFieldShape, "bullet height map" );

	heightFieldShape->setLocalScaling( Vector3( this->m_Size.x / this->m_W, 1, this->m_Size.z / this->m_L) * 2 );
	//heightFieldShape->setMargin(0.1);
	btTransform Trans;Trans.setIdentity();
	//Trans.setOrigin(btVector3( 0, 0, 0));

	btDefaultMotionState* Motion_State = new btDefaultMotionState(Trans);
	btRigidBody* nBody = new btRigidBody(btScalar(0), Motion_State, heightFieldShape, btVector3(0,0,0));
	Engine.Game->Phys_Manager.Add_Body( nBody );
#endif
}

void SE_Map_Height::Gen_Normals()
{

}

void SE_Map_Height::Cleanup()
{
	this->m_W = 0;
	this->m_L = 0;
	this->m_Size.Set( 0, 0, 0 );

	if( this->m_Array )
	{
#ifdef _TEST
		Engine.Mem_Remove( this->m_Array );
#endif
		delete[] this->m_Array;
		this->m_Array = NULL;
	}
}

#define NUM_SAMPS 3
Vector3f SE_Map_Height::Get_Normal( uint x, uint z )
{
//	return Vector3( 0, 1, 0 );

	Vector3f Vecs[NUM_SAMPS];

	Vector3f mVec( x, this->Get(x,z), z );

	Vecs[0] = Vector3f( x - 1, this->Get(x - 1, z), z ) - mVec;// back
	Vecs[1] = Vector3f( x + 1, this->Get(x + 1, z), z ) - mVec;// front
	Vecs[2] = Vector3f( x, this->Get(x, z - 1), z - 1 ) - mVec;// left
//	Vecs[3] = Vector3( x, this->Get(x, z + 1), z + 1 ) - mVec;// right
	// got vert/lats, now diags
//	Vecs[4] = Vector3( x - 1, this->Get(x - 1, z - 1), z - 1 ) - mVec;// back right
//	Vecs[5] = Vector3( x + 1, this->Get(x + 1, z + 1), z + 1 ) - mVec;// front left

//	Vecs[6] = Vector3( x - 1, this->Get(x - 1, z + 1), z + 1 ) - mVec;// back right
//	Vecs[7] = Vector3( x + 1, this->Get(x + 1, z - 1), z - 1 ) - mVec;// front left

	Vector3f Trigs[NUM_SAMPS];
	for( uchar i = 0;i < NUM_SAMPS; i++ )
	{
		Trigs[i].x() = Vecs[i].y() * Vecs[i + 1].z() - Vecs[i].z() * Vecs[i + 1].y();
		Trigs[i].y() = Vecs[i].z() * Vecs[i + 1].x() - Vecs[i].x() * Vecs[i + 1].z();
		Trigs[i].z() = Vecs[i].x() * Vecs[i + 1].y() - Vecs[i].y() * Vecs[i + 1].x();
		//Trigs[i].Normalize();
	}

	Trigs[NUM_SAMPS - 1].x() = Vecs[NUM_SAMPS - 1].y() * Vecs[0].z() - Vecs[NUM_SAMPS - 1].z() * Vecs[0].y();
	Trigs[NUM_SAMPS - 1].y() = Vecs[NUM_SAMPS - 1].z() * Vecs[0].x() - Vecs[NUM_SAMPS - 1].x() * Vecs[0].z();
	Trigs[NUM_SAMPS - 1].z() = Vecs[NUM_SAMPS - 1].x() * Vecs[0].y() - Vecs[NUM_SAMPS - 1].y() * Vecs[0].x();
	Trigs[NUM_SAMPS - 1].Normalize();

	Vector3f Average = Trigs[0];
	for( uchar i = 1;i < NUM_SAMPS; i++ )
		Average = Average + Trigs[i];

	Average.Normalize();

	return Average;
}

void SE_Map_Height::Draw_Base()
{
	glPushMatrix();
	//glScalef( this->m_Scale.x, this->m_Scale.y, this->m_Scale.z );
	//glTranslatef( -this->m_W, -15, -this->m_L );//-this->m_H / 2.f
	//glScalef( 2, 2, 2 );


	glScalef( this->m_Size.x() / this->m_W, 1, this->m_Size.z() / this->m_L );
	glTranslatef( -this->m_W/2.f, 0, -this->m_L/2.f );

	glScalef( 2, 1, 2 );

	glBegin( GL_TRIANGLE_STRIP );
	bool Side_Switch = false;
	uint Num_Row = 0, Num_Col = 0;
	for ( uint x = 0; x < m_W - 1; x++)
	{
		Num_Row++;
		if( Side_Switch )
		{
			Num_Col = 0;
			glColor3f( 0, 0, 0 );
			for( uint z = m_L - 1; z<=m_L-1; z--)
			{
				Vector3f Norm = this->Get_Normal( x, z );
				glNormal3f( Norm.x(), Norm.y(), Norm.z() );
				glVertex3f(x, this->Get(x,z), z);

				Norm = this->Get_Normal( x+1, z );
				glNormal3f( Norm.x(), Norm.y(), Norm.z() );
				glVertex3f(x+1, this->Get(x+1,z), z);

				Num_Col++;
			}
	    }
	    else
	    {
			Num_Col = 0;
	    	glColor3f( 0, 1, 0 );
	    	for( uint z=0; z<this->m_L; z++)
	    	{
				Vector3f Norm = this->Get_Normal( x+1, z );
				glNormal3f( Norm.x(), Norm.y(), Norm.z() );
	    		glVertex3f(x+1, this->Get(x+1,z), z);

				Norm = this->Get_Normal( x, z );
				glNormal3f( Norm.x(), Norm.y(), Norm.z() );
	    		glVertex3f(x, this->Get(x,z), z);

				Num_Col++;
	    	}
	    }
		Side_Switch = !Side_Switch;
	}
	glEnd();


	glPopMatrix();
}

void SE_Map_Height::Draw()
{
	this->Draw_Base();
}

float SE_Map_Height::Get( uint x, uint z )
{
	if( x < this->m_W  && z < this->m_L )
		return this->m_Array[ x + z * this->m_W ];

	else
		return 900.f;
}

float SE_Map_Height::Get( Vector3f Pos ) // other
{
	return 0.f;
}

// mutator operations
void SE_Map_Height::Set( uint x, uint z, float h )
{
	if( x < this->m_W  && z < this->m_L )
		this->m_Array[ x + z * this->m_W ] = h;
}

void SE_Map_Height::Set_Box( uint x, uint z, uint w, uint l, float h )
{
}

uint SE_Map_Height::Get_W()
{
	return this->m_W;
}

uint SE_Map_Height::Get_L()
{
	return this->m_L;
}

void SE_Map_Height::Translate( Vector3f Trans )
{

}

void SE_Map_Height::Load_Mem( SE_Array<char>& C_Mem )
{

}
