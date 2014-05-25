#ifndef SURVIVOR_ENGINE_OBJECT_EFFECTS
#define SURVIVOR_ENGINE_OBJECT_EFFECTS

/*   DEFAULT OBJECT EFFECTS FOR S_ENGINE   */
/*
			EFF_POISON,// includes burn. corrosion, wet, bleeding, anything happening over time
			EFF_PUSH_HIT,// push by parents velocity, only when touched
			EFF_PUSH_NEAR,// push away when near, radius
 */

// drawing and physical effects
#include "../Core.hpp"
#include "Physics.hpp"

namespace SE_Object_Effects
{
	//  FIRE, "fire"
	struct Effect_Fire_Data: public SE_Physics::Object_Effect
	{
		// inherited but preset: timer, desc
	};
	struct Obj_Effect_Fire: public SE_Physics::Effect_Interface
	{
		Obj_Effect_Fire();

		// inherited
		SE_Physics::Object_Effect* New_Effect( SE_Physics::Object_Interface* This, SE_Physics::Object_Interface* Other );
		void Draw( SE_Physics::Object_Interface* Obj );// centered on object
		void Apply( SE_Physics::Object_Interface* Obj, double dt );// like update

		bool Contagious( SE_Physics::Object_Interface* This, SE_Physics::Object_Interface* Other );// does it spread?
	};
}


// bool  -> fire


#endif
