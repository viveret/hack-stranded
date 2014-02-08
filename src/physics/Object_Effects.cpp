/*   DEFAULT OBJECT EFFECTS FOR S_ENGINE   */
#include "Object_Effects.hpp"
using namespace SE_Physics;
using namespace SE_Object_Effects;

#include "../gl/se_gl.hpp"

/*
Obj_Effect_Base::Obj_Effect_Base()
{
	this->Make_From( this->EFF_NONE );
}

void Obj_Effect_Base::Make_From( uint nID )
{
	this->ID = nID;

	switch( nID )
	{
	case Obj_Effect_Base::EFF_NONE:
		this->Value = 0;
		break;

	case Obj_Effect_Base::EFF_POISON:// includes burn. corrosion, wet, bleeding, anything happening over time
		this->Value = 10;
		break;

	case Obj_Effect_Base::EFF_PUSH_HIT:// push by parents velocity, only when touched
		this->Value = 10;
		break;

	case Obj_Effect_Base::EFF_PUSH_NEAR:// push away when near, radius
		this->Value = 10;
		break;

	case Obj_Effect_Base::EFF_NOISE:
		this->Value = 10;
		break;

	default:
		this->Make_From( this->EFF_NONE );
		break;
	}
}*/



Object_Effect::Object_Effect()
{
	this->Timer = 0.f;
	this->Desc = NULL;
}

Effect_Interface::Effect_Interface()
{
	this->Timer = 0;
}





/*
			EFF_POISON,// includes burn. corrosion, wet, bleeding, anything happening over time
			EFF_PUSH_HIT,// push by parents velocity, only when touched
			EFF_PUSH_NEAR,// push away when near, radius
 */


/*   FIRE   */

Obj_Effect_Fire::Obj_Effect_Fire()
{
	this->Name = "fire";
	this->Description = "A warm but urgent sensation";
	this->Timer = -1;
}

bool Obj_Effect_Fire::Contagious( SE_Physics::Object_Interface* This, SE_Physics::Object_Interface* Other )// does it spread?
{
	return true;
}

Object_Effect* Obj_Effect_Fire::New_Effect( Object_Interface* This, Object_Interface* Other )
{
	if( This == NULL || Other == NULL ) return NULL;

	return new Effect_Fire_Data;
}

void Obj_Effect_Fire::Draw( Object_Interface* Obj )// centered on object
{
	if( Obj == NULL ) return;


	glPushMatrix();

	glPopMatrix();
}

void Obj_Effect_Fire::Apply( Object_Interface* Obj, double dt )// like update
{
	if( Obj == NULL ) return;

}
