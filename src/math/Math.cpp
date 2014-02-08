#include <math.h>
#include "Math.hpp"
#include "Vector.hpp"
/*
Vector2D::Vector2D()
{
	this->Set( 0, 0 );
}

Vector2D::Vector2D( float x, float y )
{
	this->Set( x, y );
}			
									 
Vector2D Vector2D::operator+( Vector2D A )
{
	return Vector2D( this->x + A.x, this->y + A.y );
}

Vector2D Vector2D::operator+( float A )
{													
	return Vector2D( this->x + A, this->y + A );
}	 

Vector2D Vector2D::operator-( Vector2D A )
{											 
	return Vector2D( this->x - A.x, this->y - A.y );
}

Vector2D Vector2D::operator-( float A )
{											 
	return Vector2D( this->x - A, this->y - A );
}
								  
Vector2D Vector2D::operator*( float A )
{											 
	return Vector2D( this->x * A, this->y * A );
}

Vector2D Vector2D::operator/( float A )
{											 
	return Vector2D( this->x / A, this->y / A );
}		 

Vector2D::operator btVector3()
{
	return btVector3( this->x, this->y, 0 );
}

Vector2D::operator float* ()// float array, use wisely
{
	return &this->x;
}

void Vector2D::Set( float x, float y )
{				
	this->x = x;
	this->y = y;
}

void Vector2D::Set( Vector2D& A )
{
	this->x = A.x;
	this->y = A.y;
}

void Vector2D::Normalize()
{
	float Mag = this->Magnitude();
				 
	if(Mag!=0.f)
	{
		this->x/=Mag;
		this->y/=Mag;
	}
}

float Vector2D::Magnitude()
{
	return sqrt( this->x * this->x + this->y + this->y );
}

float Vector2D::Length()
{
	return this->Magnitude();
}		




Vector3D::Vector3D()
{
	this->Set( 0, 0, 0 );
}

Vector3D::Vector3D( float x, float y, float z )
{
	this->Set( x, y, z );
}	   	
									 
Vector3D Vector3D::operator+( Vector3D A )
{
	return Vector3D( this->x + A.x, this->y + A.y, this->z + A.z );
}

Vector3D Vector3D::operator+( float A )
{													
	return Vector3D( this->x + A, this->y + A, this->z + A );
}	 

Vector3D Vector3D::operator-( Vector3D A )
{											 
	return Vector3D( this->x - A.x, this->y - A.y, this->z - A.z );
}

Vector3D Vector3D::operator-( float A )
{											 
	return Vector3D( this->x - A, this->y - A, this->z - A );
}
								  
Vector3D Vector3D::operator*( float A )
{											 
	return Vector3D( this->x * A, this->y * A, this->z * A );
}

Vector3D Vector3D::operator/( float A )
{											 
	return Vector3D( this->x / A, this->y / A, this->z / A );
}				  

Vector3D::operator btVector3()
{
	return btVector3( this->x, this->y, this->z );
}

Vector3D::operator float* ()// float array, use wisely
{
	return &this->x;
}

void Vector3D::Set( float x, float y, float z )
{				
	this->x = x;
	this->y = y;
	this->z = z;
}

void Vector3D::Set( Vector3D& A )
{
	this->x = A.x;
	this->y = A.y;
	this->z = A.z;
}

void Vector3D::Normalize()
{
	float Mag = this->Magnitude();
				 
	if(Mag!=0.f)
	{
		this->x/=Mag;
		this->y/=Mag;
		this->z/=Mag;
	}
}

float Vector3D::Magnitude()
{
	return sqrt( this->x * this->x + this->y + this->y + this->z * this->z );
}

float Vector3D::Length()
{
	return this->Magnitude();
}




Vector4D::Vector4D()
{
	this->Set( 0, 0, 0, 0 );
}

Vector4D::Vector4D( float x, float y, float z, float a )
{
	this->Set( x, y, z, a );
}		  	
									 
Vector4D Vector4D::operator+( Vector4D A )
{
	return Vector4D( this->x + A.x, this->y + A.y, this->z + A.z, this->a + A.a );
}

Vector4D Vector4D::operator+( float A )
{													
	return Vector4D( this->x + A, this->y + A, this->z + A, this->a + A );
}	 

Vector4D Vector4D::operator-( Vector4D A )
{											 
	return Vector4D( this->x - A.x, this->y - A.y, this->z - A.z, this->a - A.a );
}

Vector4D Vector4D::operator-( float A )
{											 
	return Vector4D( this->x - A, this->y - A, this->z - A, this->a - A );
}
								  
Vector4D Vector4D::operator*( float A )
{											 
	return Vector4D( this->x * A, this->y * A, this->z * A, this->a * A );
}

Vector4D Vector4D::operator/( float A )
{											 
	return Vector4D( this->x / A, this->y / A, this->z / A, this->a / A );
}

Vector4D::operator float* ()// float array, use wisely
{
	return &this->x;
}

void Vector4D::Set( float x, float y, float z, float a )
{				
	this->x = x;
	this->y = y;
	this->z = z;
	this->a = a;
}

void Vector4D::Normalize()
{
	float Mag = this->Magnitude();
				 
	if(Mag!=0.f)
	{
		this->x/=Mag;
		this->y/=Mag;
		this->z/=Mag;
		this->a/=Mag;
	}
}

float Vector4D::Magnitude()
{
	return sqrt( this->x * this->x + this->y + this->y + this->z * this->z + this->a * this->a );
}

float Vector4D::Length()
{
	return this->Magnitude();
}
*/


			
														
float Dist_2D( Vector2f A, Vector2f B )
{
	Vector2f C = Vector2f( B.x() - A.x(), B.y() - A.y() );
	return sqrt( C.x() * C.x() + C.y() * C.y() );
}

float Dist_2D( float x1, float y1, float x2, float y2 )
{
	return Dist_2D( Vector2f( x1, y1 ), Vector2f( x2, y2 ) );
}

float Dist_3D( Vector3f A, Vector3f B )
{															 
	Vector3f C = Vector3f( B.x() - A.x(), B.y() - A.y(), B.z() - A.z() );
	return sqrt( C.x() * C.x() + C.y() * C.y() + C.z() * C.z() );
}

float Dist_3D( float x1, float y1, float z1, float x2, float y2, float z2 )
{
	return Dist_3D( Vector3f( x1, y1, z1 ), Vector3f( x2, y2, z2 ) );
}


		
signed char Sign( float A )
{
	if( A > 0 )
		return 1;
	else if( A < 0 )
		return -1;
	else
		return 0;
}

signed char Sign( int A )
{				 
	if( A > 0 )
		return 1;
	else if( A < 0 )
		return -1;
	else
		return 0;
}




bool Is_In_Box(float boxx,float boxy,float boxw,float boxh,float mx,float my)
{
     if((mx>=boxx) && (mx<=boxx+boxw) && (my>=boxy) && (my<=boxy+boxh))
     {
         return true;
     }
     else{return false;}
}

bool Is_In_Box(int boxx,int boxy,int boxw,int boxh,int mx,int my)
{
	return Is_In_Box(float(boxx),float(boxy),float(boxw),float(boxh),float(mx),float(my));
}				 

void Clamp_Angle(float* Angle)//clamp between 0 and 360
{
	while(*Angle>360)
		*Angle-=360;

	while(*Angle<0)
		*Angle+=360;
}

float Clamp_Angle(float Angle)//clamp between 0 and 360
{
	Clamp_Angle(&Angle);
	return Angle;
}

#ifdef _WIN32
int log2(double n)
{
	return (int)(log(n)/log(2.0));
}
#endif


bool Check_Flag(int flags,int flag)
{
	if((flags>>(int)log2(flag))&1)
		return true;

	return false;
}
