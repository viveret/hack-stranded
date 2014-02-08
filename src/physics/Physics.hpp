#ifndef SURVIVOR_ENGINE_PHYSICS
#define SURVIVOR_ENGINE_PHYSICS

#ifdef _WIN32
#include <btBulletDynamicsCommon.h>
#else
#include <bullet/btBulletDynamicsCommon.h>
#endif

#include <string>
#include <vector>
#include "../math/Math.hpp"
#include "../module/Thread.hpp"

// Scene types
#define SE_PHYS_SCENE_3	0x00 // 3D
#define SE_PHYS_SCENE_2 0x01 // 2D

#define SE_PHYS_SHAPE_RECT		0x00
#define SE_PHYS_SHAPE_ELIPSE	0x01


namespace SE_Physics
{
	struct Phys_Object;
	typedef Phys_Object* Obj_ID;

	struct Object_Interface;
	struct Object_Effect;

	struct Effect_Interface
	{
		Effect_Interface();

		virtual Object_Effect* New_Effect( SE_Physics::Object_Interface* This, SE_Physics::Object_Interface* Other ) = 0;
		std::string Name;// when drawing, first letter is capital
		std::string Description;

		// base effect stats
		uint Timer;// amount of seconds ot lasts (0 = instant, -1 = infinite)
		virtual bool Contagious( SE_Physics::Object_Interface* This, SE_Physics::Object_Interface* Other ) = 0;// does it spread?

		virtual void Draw( Object_Interface* Obj ) = 0;// centered on object
		virtual void Apply( Object_Interface* Obj, double dt ) = 0;// like update
	};

	struct Object_Effect
	{
		Object_Effect();

		Effect_Interface* Desc;
		//std::string Desc_Name;// prevent memory errors
		float Timer;
	};

	struct Object_Interface
	{
		virtual uchar Get_Type() = 0;// simple type of object
		virtual void Destroy() = 0;
		virtual void Collide_With( Object_Interface* Other ) = 0;
		virtual void Update( double dt ) = 0;
		virtual void Draw() = 0;

		uint Flags;
	};

/*	struct SE_Phys_Manager
	{
		SE_Phys_Manager();
		~SE_Phys_Manager();

		void Cleanup();
		bool Initialize( bool Make_2D );

		void Update( double dt );
		void Draw();

		void Set_Body_Props_Func( void(*Func)(btRigidBody*, uint) );// additionaqwl settings


		//BULLET STUFF
		float Phys_Speed_Mod;

		//BULLET FUNCTIONS
		void Delete_Body(btRigidBody* What);//delete single body
		void Delete_All_Bodies();//delete all objects
		void Delete_btOther();

		void Set_Gravity( Vector3f Direction );
		void Set_World_Bounds( Vector3f Abs_Size );//set world size, make boundaries
		void Set_Obj_Bounds( Vector3f* Pos, Vector3f Size );

		btRigidBody* Make_Body(float Mass,Vector3f Position,btCollisionShape* Shape,uint Flags, std::string Where );//add rigid body

		void Add_Body( btRigidBody* Body );//add rigid body


		//Obj_Efect_Descriptor_Base* Get_Obj_Eff_Desc_From_VFS( std::string Name );
		void Effect_Add( Effect_Interface* Eff );
		Effect_Interface* Effect_Get( std::string Name );
		Object_Effect* Effect_Create( std::string Name, Object_Interface* This, Object_Interface* Other );

	private:

		bool Is_2D;
		std::vector< Object_Interface* > Objects;
		SE_Thread::Mutex Obj_Mutex;

		std::vector< Effect_Interface* > Effects;
		void Effect_Add_Default();
		void Effect_Cleanup();// cleanup all effects

		Vector3f m_Bounds;

		btAlignedObjectArray<btCollisionShape*>	Collision_Shapes;//hold shapes
		btDynamicsWorld* Dynamics_World;//holds world stuff

		btBroadphaseInterface*	BT_Broad_Phase;
		btCollisionDispatcher*	BT_Dispatcher;
		btConstraintSolver*	BT_Solver;
		btDefaultCollisionConfiguration* BT_Collision_Configuration;

		void(*Add_Body_Props)(btRigidBody*, uint);
	};
*/
	uchar Get_Scene();
	void Set_Scene( uchar Type );
	void Set_World_Scale( Vector3f Size );
	void Set_World_Scale( Vector2f Size );
	void Mult_World_Scale();

	inline bool Is_2D()
	{
		return Get_Scene() == SE_PHYS_SCENE_2;
	}
	inline bool Is_3D()
	{
		return Get_Scene() == SE_PHYS_SCENE_3;
	}

	btRigidBody* Make_Body(float Mass,Vector3f Position,btCollisionShape* Shape,uint Flags, std::string Where );//add rigid body
	void Add_Body( btRigidBody* Body );//add rigid body

	void Delete_Body( btRigidBody* What );//delete single body
	void Delete_All_Bodies();//delete all objects

	bool SE_Phys_Callback( btManifoldPoint& cp, const btCollisionObject* Obj1, int id1, int index1, const btCollisionObject* Obj2, int id2, int index2 );

	void Register_Module();
}


#endif
