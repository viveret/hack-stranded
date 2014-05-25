#ifndef SURVIVOR_ENGINE_MATH
#define SURVIVOR_ENGINE_MATH

#include <math.h>
#include <cmath>

#include "../Core.hpp"
#ifndef _NO_BULLET
#include <bullet/LinearMath/btVector3.h>
#endif

#include "Vector.hpp"


float Dot_Product( Vector3f A,Vector3f B );
float Dot( Vector3f A,Vector3f B );


float Dist_2D( Vector2f A, Vector2f B );
float Dist_2D( float x1, float y1, float x2, float y2 );

float Dist_3D( Vector3f A, Vector3f B );
float Dist_3D( float x1, float y1, float z1, float x2, float y2, float z2 );


struct Box2D
{
	Vector2f Pos, Size;
};


struct RECTID
{
    float x,y,w,h,Val;
    int ID;
};

class rotate3D
{
    public:
    Vector3f Position;
    Vector3f Rot;
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

const float SE_DEG_TO_RAD = 0.01745329251994329576923690768f;
const float SE_RAD_TO_DEG = 57.2957795130823208767981548141f;

inline float ToRadian( const float Degree )
{
  return ( Degree * SE_DEG_TO_RAD );
}

inline float ToDegree( const float Radian )
{
  return ( Radian * SE_RAD_TO_DEG );
}

#endif // MATH_H_INCLUDED
