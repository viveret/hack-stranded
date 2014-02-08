#ifndef SURVIVOR_ENGINE_GAME
#define SURVIVOR_ENGINE_GAME

#include "../math/Math.hpp"
#include "../physics/Physics.hpp"

#include "../gl/OpenGL.hpp"
#include "../render/Material.hpp"

namespace E_Game
{
	struct G_Game
	{
		G_Game();
		~G_Game();

		bool Initialize_Base( bool Make_2D );
		virtual bool Initialize() = 0;
		void Destroy_Base();
		virtual void Destroy() = 0;

		void Do_Commands();//from inside update

		virtual void Draw() = 0;
		virtual void Draw_HUD() = 0;


		void Update_Base( double dt );
		virtual void Update(double dt) = 0;//logic
		virtual void Update_Input( double dt ) = 0;

		virtual void Clean() = 0;//clean entire game-> Safe For Work

#ifdef _TEST
		SE_Physics::SE_Phys_Manager Phys_Manager;
#endif
		SE_Render_Material::Material_Base Map_Material, Light_Material;
	};


	//bool Obj_Callback( btManifoldPoint& cp, const btCollisionObject* Obj1, int id1, int index1, const btCollisionObject* Obj2, int id2, int index2 );
}

#endif
