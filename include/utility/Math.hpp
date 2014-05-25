#ifndef SURVIVOR_ENGINE_MATH
#define SURVIVOR_ENGINE_MATH

#include <math.h>
#include <cmath>

#include "../Core.hpp"
#include <LinearMath/btVector3.h>

typedef struct Vector2D
{
	Vector2D();
	Vector2D( float x, float y );

	Vector2D operator+( Vector2D A );
	Vector2D operator+( float A );

	Vector2D operator-( Vector2D A );
	Vector2D operator-( float A );

	Vector2D operator*( float A );
	Vector2D operator/( float A );

	operator btVector3();
	operator float* ();// float array, use wisely

	void Set( float x, float y );
	void Set( Vector2D& A );

	void Normalize();
	float Magnitude();
	float Length();

	float x, y;

}Point2,Point2D,Vector2,Vertex2D;

typedef struct Vector3D
{
	Vector3D();
	Vector3D( float x, float y, float z );

	Vector3D operator+( Vector3D A );
	Vector3D operator+( float A );

	Vector3D operator-( Vector3D A );
	Vector3D operator-( float A );

	Vector3D operator*( float A );
	Vector3D operator/( float A );

	operator btVector3();
	operator float* ();// float array, use wisely

	void Set( float x, float y, float z );
	void Set( Vector3D& A );

	void Normalize();
	float Magnitude();
	float Length();

	float x, y, z;

}Point3,Point3D,Vector3,Vertex3D;

typedef struct Vector4D
{
	Vector4D();
	Vector4D( float x, float y, float z, float a );

	Vector4D operator+( Vector4D A );
	Vector4D operator+( float A );

	Vector4D operator-( Vector4D A );
	Vector4D operator-( float A );

	Vector4D operator*( float A );
	Vector4D operator/( float A );

	operator float* ();// float array, use wisely

	void Set( float x, float y, float z, float a );
	void Normalize();
	float Magnitude();
	float Length();

	float x, y, z, a;

}Point4,Point4D,Vector4;

float Dot_Product( Vector3 A,Vector3 B );
float Dot( Vector3 A,Vector3 B );


float Dist_2D( Vector2D A, Vector2D B );
float Dist_2D( float x1, float y1, float x2, float y2 );

float Dist_3D( Vector3D A, Vector3D B );
float Dist_3D( float x1, float y1, float z1, float x2, float y2, float z2 );


struct Box2D
{
	Vector2D Pos, Size;
};


struct RECTID
{
    float x,y,w,h,Val;
    int ID;
};

class rotate3D
{
    public:
    Vector3 Position;
    Vector3 Rot;
    bool local;
    void New(float nx,float ny,float nz);
};


signed char Sign( float A );
signed char Sign( int A );


class NearObj
{
    public:
    int top,bottom,right,left,front,back;
    NearObj(int atop,int abottom,int aright,int aleft,int afront,int aback)
    {
        NearObj::top=atop;
        NearObj::bottom=abottom;
        NearObj::right=aright;
        NearObj::left=aleft;
        NearObj::front=afront;
        NearObj::back=aback;
    }
};

class Box
{
    public:
    int x,w,y,h;
};

void Clamp_Angle(float* Angle);//clamp between 0 and 360
float Clamp_Angle(float Angle);//clamp between 0 and 360

//int log2(double n);


/*float DistForm2d(float x1,float y1,float x2,float y2);
float DistForm2d(Vector2 A,Vector2 B);
float DistForm3d(float x1,float y1,float z1,float x2,float y2,float z2);
float DistForm3d(Vector3 A,Vector3 B);	  */

bool Is_In_Box(float boxx,float boxy,float boxw,float boxh,float mx,float my);
bool Is_In_Box(int boxx,int boxy,int boxw,int boxh,int mx,int my);

//int IntToBarPos(int h);


bool Check_Flag(int flags,int flag);




#ifndef _WIN32
struct POINT
{
	int x, y, w, h;
};
#endif



#endif // MATH_H_INCLUDED
