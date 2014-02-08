#include "Physics.hpp"
#include "Object.hpp"
using namespace SE_Physics;

#include "../utility/Functions.hpp"
#include "../module/Module.hpp"

#include "../gl/OpenGL.hpp"

namespace SE_Physics
{
	// Structures
	struct Phys_Object3D;
	struct Phys_Object2D;

	// Functions
	uint Initialize( const std::vector< std::string >& mArgs );
	uint Cleanup();

	void Thread_Update();
	void Thread_Render();

	void Render_Obj2D( Obj_ID What );
	void Render_Obj3D( Obj_ID What );

	void Update_Obj2D( Obj_ID What );
	void Update_Obj3D( Obj_ID What );

	void Sync_Bullet_Pos2D( Obj_ID What );
	void Sync_Bullet_Pos3D( Obj_ID What );

	void Delete_btOther();
	void Effect_Add( Effect_Interface* Eff );
	Effect_Interface* Effect_Get( std::string Name );
	Object_Effect* Effect_Create( std::string Name, Object_Interface* This, Object_Interface* Other );

	void Set_Gravity( Vector3f Direction );
	void Set_World_Bounds( Vector3f Abs_Size );//set world size, make boundaries
	void Set_Obj_Bounds( Vector3f* Pos, Vector3f Size );

	void Set_Body_Props_Func( void(*Func)(btRigidBody*, uint) );// additionaqwl settings

	// Globals
	Vector3f World_Scale( 16, 16, 16 );
	
	uchar Scene_Type = uchar(-1);
	void (*Render_Obj)( Obj_ID What ) = NULL; // Must be set by scene
	void (*Update_Obj)( Obj_ID What ) = NULL; // Must be set by scene
	void (*Sync_Bullet_Pos)( Obj_ID What ) = NULL;

	float Phys_Speed_Mod = 1.f;

//	std::vector< Object_Interface* > Objects;
	std::vector< Obj_ID > Objects;
	SE_Thread::Mutex Obj_Mutex;

	std::vector< Effect_Interface* > Effects;
	void Effect_Add_Default();
	void Effect_Cleanup();// cleanup all effects

	Vector3f m_Bounds( 100, 100, 100 );

	// Bullet
	btAlignedObjectArray< btCollisionShape* > Collision_Shapes;//hold shapes
	btDynamicsWorld* Dynamics_World = NULL;//holds world stuff

	btBroadphaseInterface*	BT_Broad_Phase = NULL;
	btCollisionDispatcher*	BT_Dispatcher = NULL;
	btConstraintSolver*	BT_Solver = NULL;
	btDefaultCollisionConfiguration* BT_Collision_Configuration = NULL;
	
	void(*Add_Body_Props)(btRigidBody*, uint) = NULL;
}

// Structures
struct SE_Physics::Phys_Object
{
	Vector4f Color;
	uchar Tex_Type;
	uint Texture;
	bool Renderable;

	btRigidBody* mbt_Body;
	void* Ext_Info;

	uchar Shape;
	float m_Attract;

	Phys_Object()
	{
		this->Tex_Type = 0;
		this->Texture = 0;
		this->mbt_Body = NULL;
		this->Shape = 0;
		this->m_Attract = 0.f;

		this->Ext_Info = NULL;

		this->Renderable = true;
		this->Color.Set( 1, 1, 1, 1 );
	}
};

struct SE_Physics::Phys_Object2D: public Phys_Object
{
	Vector2f Pos, Size;
};

struct SE_Physics::Phys_Object3D: public Phys_Object
{
	Vector3f Pos, Size;
};

Phys_Object2D* mObj;
uint SE_Physics::Initialize( const std::vector< std::string >& mArgs )
{
	switch( Scene_Type )
	{
	case SE_PHYS_SCENE_2:
			Render_Obj = Render_Obj2D;
			Update_Obj = Update_Obj2D;
			Sync_Bullet_Pos = Sync_Bullet_Pos2D;
		break;
	case SE_PHYS_SCENE_3:
		Render_Obj = Render_Obj3D;
		Update_Obj = Update_Obj3D;
		Sync_Bullet_Pos = Sync_Bullet_Pos3D;
		break;
	default:
		sePrintf( SEPRINT_ERROR, "No scene type selected\n" );
		return SE_FAILURE;
		break; // Just in case
	}

	assert( Render_Obj != NULL );
	assert( Update_Obj != NULL );
	assert( Sync_Bullet_Pos != NULL );


	if(!Dynamics_World)// no physics world
	{
//		sePrintf( SEPRINT_MODULE, "Initializing Bullet Phys Initialization\n" );
		///collision configuration contains default setup for memory, collision setup
		BT_Collision_Configuration = new btDefaultCollisionConfiguration();
//		Engine.Mem_Add( BT_Collision_Configuration, "bt collision config" );
		sePrintf( SEPRINT_MODULE, "Made Bullet Collision Config\n" );

		//use the default collision dispatcher. For parallel processing you can use a diffent dispatcher (see Extras/BulletMultiThreaded)
		BT_Dispatcher = new	btCollisionDispatcher(BT_Collision_Configuration);
//		Engine.Mem_Add( BT_Dispatcher, "bt collision dispatcher" );
		sePrintf( SEPRINT_MODULE, "Made Bullet Dispatcher\n" );

		BT_Broad_Phase = new btDbvtBroadphase();
//		Engine.Mem_Add( BT_Broad_Phase, "bt broad phase" );
		sePrintf( SEPRINT_MODULE, "Made Bullet Broad Phase\n" );

		///the default constraint solver. For parallel processing you can use a different solver (see Extras/BulletMultiThreaded)
		BT_Solver = new btSequentialImpulseConstraintSolver;
//		Engine.Mem_Add( BT_Solver, "bt solver" );
		sePrintf( SEPRINT_MODULE, "Made Bullet Solver\n" );

		Dynamics_World = new btDiscreteDynamicsWorld(BT_Dispatcher,BT_Broad_Phase,BT_Solver,BT_Collision_Configuration);
//		Engine.Mem_Add( Dynamics_World, "bt dynamic world" );
		sePrintf( SEPRINT_MODULE, "Made Bullet Dynamic World\n" );

#ifndef _WIN32
#pragma GCC diagnostic ignored "-fpermissive"
#endif
		gContactAddedCallback = SE_Phys_Callback;
	}

	if( BT_Collision_Configuration == NULL || BT_Dispatcher == NULL ||
		BT_Broad_Phase == NULL || BT_Solver == NULL || Dynamics_World == NULL )
	{
//		sePrintf( SEPRINT_ERROR, "Failed To Initialize Bullet\n" );
		Cleanup();
		return SE_FAILURE;
	}

//	sePrintf( SEPRINT_MODULE, "Finished Bullet Initialization\n" );

	// Create some garbage stuff
	assert( Scene_Type == SE_PHYS_SCENE_2 );

	Set_Gravity( Vector3f( 0, 0, 0 ) );

	mObj = Gen_Object( Vector2f(0, 0), Vector2f(1, 2), -1, SE_PHYS_SHAPE_RECT );
	mObj->Tex_Type = 0;
	mObj->Color.Set( 0, 1, 0, 1 );

//	mObj->mbt_Body->setDamping( mObj->mbt_Body->getLinearDamping() / 1000.f,
//			mObj->mbt_Body->getAngularDamping() / 1000.f );


	return SE_SUCCESS;
}

uint SE_Physics::Cleanup()
{
	//remove the rigidbodies from the dynamics world and delete them
	if(Dynamics_World)
	{
		//cleanup in the reverse order of creation/initialization
		//remove the rigidbodies from the dynamics world and delete them

		Delete_All_Bodies();
		Delete_btOther();

//		Engine.Mem_Remove( Dynamics_World );
		delete Dynamics_World;
		Dynamics_World=NULL;

//		Engine.Mem_Remove( BT_Solver );
		delete BT_Solver;
		BT_Solver=NULL;

//		Engine.Mem_Remove( BT_Broad_Phase );
		delete BT_Broad_Phase;
		BT_Broad_Phase=NULL;

//		Engine.Mem_Remove( BT_Dispatcher );
		delete BT_Dispatcher;
		BT_Dispatcher=NULL;

//		Engine.Mem_Remove( BT_Collision_Configuration );
		delete BT_Collision_Configuration;
		BT_Collision_Configuration=NULL;
	}

	return SE_SUCCESS;
}

void SE_Physics::Thread_Update()
{
	if( Dynamics_World )
		Dynamics_World->stepSimulation((btScalar)( SE_Thread::Get_DT()/1000. * Phys_Speed_Mod ));

	Obj_Mutex.Wait_For_Lock( );//FILE_LINE.c_str() );
	Obj_Mutex.Lock();

//	mObj->mbt_Body->applyCentralImpulse( btVector3( 0.5f, 0.5f, 0 ) );

	for( uint i = 0;i < Objects.size(); i++ )
		Sync_Bullet_Pos( Objects[i] );

	for( uint i = 0;i < Objects.size(); i++ )
	{
		Update_Obj( Objects[i] );
	}

	Obj_Mutex.Unlock();
}

void SE_Physics::Mult_World_Scale()
{
	glScalef( World_Scale.x(), World_Scale.y(), World_Scale.z() );
}

void SE_Physics::Thread_Render()
{
	glPushMatrix();

	Mult_World_Scale();

	Obj_Mutex.Wait_For_Lock( );//FILE_LINE.c_str() );
	Obj_Mutex.Lock();

	for( uint i = 0;i < Objects.size(); i++ )
	{
		if( Objects[i]->Renderable )
			Render_Obj( Objects[i] );
	}

	Obj_Mutex.Unlock();

	glPopMatrix();
}

void SE_Physics::MultGL( Obj_ID What ) // Translate GL by object
{
	btScalar GLMatrix[16];
	btTransform trans3;
	What->mbt_Body->getMotionState()->getWorldTransform(trans3);
	trans3.getOpenGLMatrix( GLMatrix );
	glMultMatrixf( (GLfloat*)GLMatrix );
}

void SE_Physics::Render_Obj2D( Obj_ID What )
{
	Phys_Object2D* Obj = (Phys_Object2D*)(What);

	glPushMatrix();

	segl_Use_Texture( Obj->Texture );
	segl_Set1i( SEGL_TEXTURE, Obj->Tex_Type );

	// If use static pos
//	glTranslatef( Obj->Pos.x(), Obj->Pos.y(), 0 );

	// Use bullet
	MultGL( What );
	// Rest of render

	glScalef( Obj->Size.x(), Obj->Size.y(), 1 );

	switch( What->Shape )
	{
	case SE_PHYS_SHAPE_RECT:
		{
			glColor4fv( Obj->Color ); 
			glBegin(GL_QUADS);
 				glTexCoord2f(0, 0);		glVertex2d( -0.5f, -0.5f );
				glTexCoord2f(1, 0);		glVertex2d( +0.5f, -0.5f );
				glTexCoord2f(1, 1);		glVertex2d( +0.5f, +0.5f );
				glTexCoord2f(0, 1);		glVertex2d( -0.5f, +0.5f );
			glEnd();
		}break;
		case SE_PHYS_SHAPE_ELIPSE:
			break;
	}

	glPopMatrix();
}

void SE_Physics::Render_Obj3D( Obj_ID What )
{
	Phys_Object3D* Obj = (Phys_Object3D*)(What);
	switch( What->Shape )
	{

	}
}

void SE_Physics::Update_Obj2D( Obj_ID What )
{
	Phys_Object2D* Obj = (Phys_Object2D*)(What);

	if( Obj->m_Attract != 0.f ) // Pull all others closer
	{
		for( uint i = 0; i < Objects.size(); i++ )
		{
			if( Objects[i] != What && Objects[i]->m_Attract == 0.f )
			{
				Vector2f Pull_Dir = Obj->Pos - ((Phys_Object2D*)Objects[i])->Pos;
//				if( abs(Pull_Dir.Magnitude()) < Obj->Size.x() * 100.f * Obj->m_Attract )
				{
					float Strength = Obj->Size.x() - Pull_Dir.Magnitude();
					if( Strength < 0 )
						continue;

					Strength *= 0.005f * Strength;

					Pull_Dir.Normalize();
//					Pull_Dir = (100 - Pull_Dir) / 100;				
					//sePrintf( SEPRINT_DEBUG, "%f,%f | ", Pull_Dir.x(), Pull_Dir.y() );
					Objects[i]->mbt_Body->applyCentralImpulse( btVector3(
						Pull_Dir.x() * What->m_Attract * Strength,
						Pull_Dir.y() * What->m_Attract * Strength, 0 ) );
				}
			}
		}
	}
}

void SE_Physics::Update_Obj3D( Obj_ID What )
{
	Phys_Object3D* Obj = (Phys_Object3D*)(What);
	
}

void SE_Physics::Sync_Bullet_Pos2D( Obj_ID What )
{
	Phys_Object2D* Obj = (Phys_Object2D*)(What);

	btTransform trans3;
	What->mbt_Body->getMotionState()->getWorldTransform(trans3);

	Obj->Pos.Set( trans3.getOrigin().x(), trans3.getOrigin().y() );
}

void SE_Physics::Sync_Bullet_Pos3D( Obj_ID What )
{
	Phys_Object3D* Obj = (Phys_Object3D*)(What);


}



void SE_Physics::Delete_All_Bodies()
{
	for (int i=0;i<Dynamics_World->getNumCollisionObjects();i++)
	{
		btCollisionObject* obj = Dynamics_World->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if( body && body->getMotionState() )
		{
#ifdef _TEST
			Engine.Mem_Remove( body->getMotionState() );
#endif
			delete body->getMotionState();
		}
		Dynamics_World->removeCollisionObject( obj );
#ifdef _TEST
		Engine.Mem_Remove( obj );
#endif
		delete obj;
	}
}

void SE_Physics::Delete_btOther()
{
	//delete collision shapes
	for (int j=0;j<Collision_Shapes.size();j++)
	{
		btCollisionShape* shape = Collision_Shapes[j];
#ifdef _TEST
		Engine.Mem_Remove( shape );
#endif
		delete shape;
	}
	Collision_Shapes.clear();
}

/*void SE_Physics::Set_Callback( bool(*Func)(btManifoldPoint&, btCollisionObject const*, int, int, btCollisionObject const*, int, int))
{
	gContactAddedCallback = Func;
}*/
bool SE_Physics::SE_Phys_Callback( btManifoldPoint& cp, const btCollisionObject* Obj1, int id1, int index1, const btCollisionObject* Obj2, int id2, int index2 )
{
	Object_Interface* Obj1_Data = (Object_Interface*)Obj1->getUserPointer();
	Object_Interface* Obj2_Data = (Object_Interface*)Obj2->getUserPointer();


	if( Obj1_Data && Obj2_Data )
	{
		Obj1_Data->Collide_With( Obj2_Data );
		Obj2_Data->Collide_With( Obj1_Data );
	}


	return true;
}

void SE_Physics::Set_Body_Props_Func( void(*Func)(btRigidBody*, uint) )// additional settings
{
	Add_Body_Props = Func;
}

void SE_Physics::Set_Gravity( Vector3f Direction )
{
	Dynamics_World->setGravity( Direction );
}

void SE_Physics::Set_World_Bounds( Vector3f Abs_Size )//set world size, make boundaries
{
	m_Bounds = Abs_Size;

	if( Abs_Size.x() != 0.f )// is a bound
	{
		btCollisionShape* Plane_X_Pos = new btStaticPlaneShape(btVector3(-1,0,0),1);
		Make_Body( 0.f, Vector3f(Abs_Size.x(), 0, 0), Plane_X_Pos, 0, FILE_LINE);

		btCollisionShape* Plane_X_Neg = new btStaticPlaneShape(btVector3(1,0,0),1);
		Make_Body( 0.f, Vector3f(-Abs_Size.x(), 0, 0), Plane_X_Neg, 0, FILE_LINE);
	}

	if( Abs_Size.y() != 0.f )// is a bound
	{
		btCollisionShape* Plane_Y_Pos = new btStaticPlaneShape(btVector3(0,-1,0),1);
		Make_Body( 0.f, Vector3f(0, Abs_Size.y(), 0), Plane_Y_Pos, 0, FILE_LINE);

		btCollisionShape* Plane_Y_Neg = new btStaticPlaneShape(btVector3(0,1,0),1);
		Make_Body( 0.f, Vector3f(0, -Abs_Size.y(), 0), Plane_Y_Neg, 0, FILE_LINE);
	}

	if( Abs_Size.z() != 0.f )// is a bound
	{
		btCollisionShape* Plane_Z_Pos = new btStaticPlaneShape(btVector3(0,0,-1),1);
		Make_Body( 0.f, Vector3f(0, 0, Abs_Size.z()), Plane_Z_Pos, 0, FILE_LINE);

		btCollisionShape* Plane_Z_Neg = new btStaticPlaneShape(btVector3(0,0,1),1);
		Make_Body( 0.f, Vector3f(0, 0, -Abs_Size.z()), Plane_Z_Neg, 0, FILE_LINE);
	}
}

void SE_Physics::Set_Obj_Bounds( Vector3f* Pos, Vector3f Size )
{
	if( m_Bounds.x() != 0.f )// is a bound
	{
		if( Pos->x() + Size.x()/2.f > m_Bounds.x() )
			Pos->x() = m_Bounds.x() - Size.x()/2.f;

		else if( Pos->x() - Size.x()/2.f < -m_Bounds.x() )
			Pos->x() = -m_Bounds.x() + Size.x()/2.f;
	}

	if( m_Bounds.y() != 0.f )// is a bound
	{
		if( Pos->y() + Size.y()/2.f > m_Bounds.y() )
			Pos->y() = m_Bounds.y() - Size.y()/2.f;

		else if( Pos->y() - Size.y()/2.f < -m_Bounds.y() )
			Pos->y() = -m_Bounds.y() + Size.y()/2.f;
	}

	if( m_Bounds.z() != 0.f )// is a bound
	{
		if( Pos->z() + Size.z()/2.f > m_Bounds.z() )
			Pos->z() = m_Bounds.z() - Size.z()/2.f;

		else if( Pos->z() - Size.z()/2.f < -m_Bounds.z() )
			Pos->z() = -m_Bounds.z() + Size.z()/2.f;
	}
}

btRigidBody* SE_Physics::Make_Body( float Mass, Vector3f Position, btCollisionShape* Shape, uint Flags, std::string Where )
{
	//btShape_List.push_back( Shape );
	//Engine.Mem_Add( Shape, "bt shape" );

	if( Mass != 0 )// will be moving
	{
		/*btVector3 Sphere_Pos;
		btScalar Sphere_Size;

		Shape->getAabb()*/

		Set_Obj_Bounds( &Position, Vector3f() );// size irrelevant for now
	}

	btTransform Trans;Trans.setIdentity();
	Trans.setOrigin( Position );

	btVector3 Local_Inertia(0,0,0);

	if(Mass != 0 )
		Shape->calculateLocalInertia(Mass,Local_Inertia);

	btDefaultMotionState* Motion_State = new btDefaultMotionState(Trans);
#ifdef _TEST
	Engine.Mem_Add( Motion_State, "motion state: " + Where );
#endif
	btRigidBody::btRigidBodyConstructionInfo Body_Info(Mass,Motion_State,Shape,Local_Inertia);
	btRigidBody* Body = new btRigidBody(Body_Info);
#ifdef _TEST
	Engine.Mem_Add( Body, "body: " + Where );
#endif

	if( Add_Body_Props )
		Add_Body_Props( Body, Flags );

	//add the body to the dynamics world
	Dynamics_World->addRigidBody(Body);

	return Body;
}

void SE_Physics::Add_Body(btRigidBody* Body)//add rigid body
{
	Dynamics_World->addRigidBody(Body);
}

void SE_Physics::Delete_Body(btRigidBody* Body)//remove rigid body
{
//	Engine.Mem_Remove( Body->getMotionState() );
	delete Body->getMotionState();

	Dynamics_World->removeRigidBody(Body);
//	Engine.Mem_Remove( Body );
	delete Body;
}

Effect_Interface* SE_Physics::Effect_Get( std::string Name )
{
	for( uint i = 0;i < Effects.size(); i++ )
	{
		if( Effects[i]->Name == Name )
			return Effects[i];
	}

	return NULL;
}

Object_Effect* SE_Physics::Effect_Create( std::string Name, Object_Interface* This, Object_Interface* Other  )
{
	Object_Effect* Ret = NULL;

	Effect_Interface* nEff = Effect_Get( Name );
	if( nEff == NULL ) return NULL;
#ifdef _TEST
	Engine.Mem_Add( nEff, "obj eff" );
#endif

	Ret = nEff->New_Effect( This, Other );;

	Ret->Desc = nEff;
	Ret->Timer = nEff->Timer;

	return Ret;
}

void SE_Physics::Effect_Add( Effect_Interface* Eff )
{
	if( Eff == NULL ) return;

	Effects.push_back( Eff );
#ifdef _TEST
	Engine.Mem_Add( Eff, "obj eff int" );
#endif
}

void SE_Physics::Effect_Cleanup()
{
	for( uint i = 0;i < Effects.size(); i++ )
	{
#ifdef _TEST
		Engine.Mem_Remove( Effects[i] );
#endif
		delete Effects[i];
	}

	Effects.clear();
}



#include "Object_Effects.hpp"
void SE_Physics::Effect_Add_Default()
{
	//Effect_Add( new SE_Object_Effects::Obj_Effect_Fire );
}

uchar SE_Physics::Get_Scene()
{
	return Scene_Type;
}

void SE_Physics::Set_Scene( uchar Type )
{
	if( Scene_Type == uchar(-1) ) // not set yet
		Scene_Type = Type;

	else
		sePrintf( SEPRINT_ERROR, "Scene type already set\n" );
}

void SE_Physics::Set_World_Scale( Vector3f Size )
{
	assert( Size.Magnitude() != 0.f );

	World_Scale = Size;
}

void SE_Physics::Set_World_Scale( Vector2f Size )
{
	assert( Size.Magnitude() != 0.f );

	World_Scale = Vector3f( Size.x(), Size.y(), 1 );
}


Obj_ID SE_Physics::Gen_Object( Vector3f Pos, Vector3f Size, float Mass, uchar mShape, void* Ext_Info )
{
	assert( Is_3D() );

	Phys_Object3D* nObj = new Phys_Object3D;
	nObj->Pos = Pos;
	nObj->Size = Size;
	nObj->Color.Set( 1, 1, 0, 1 );

	Objects.push_back( nObj );

	return (Obj_ID)(nObj);
}

Obj_ID SE_Physics::Gen_Object( Vector2f Pos, Vector2f Size, float Mass, uchar mShape, void* Ext_Info )
{
	assert( Is_2D() );

	Phys_Object2D* nObj = new Phys_Object2D;
	nObj->Pos = Pos;
	nObj->Size = Size;
	nObj->Color.Set( 1, 1, 0, 1 );
	nObj->Ext_Info = Ext_Info;
	nObj->Shape = mShape;

	// Bullet initialization
	btCollisionShape* Shape = NULL;//btBoxShape* 
	switch( mShape )
	{
	case SE_PHYS_SHAPE_ELIPSE:
		Shape = new btSphereShape( Size.x() / 4.f );
		break;
	case SE_PHYS_SHAPE_RECT:
		Shape = new btBoxShape(btVector3( Size.x() / 2.f, Size.y() / 2.f, 1 ));
		break;
	}

	assert( Shape != NULL );

	btTransform Trans;Trans.setIdentity();
	Trans.setOrigin( btVector3(Pos.x(), Pos.y(), -0.5f) );

	btVector3 Local_Inertia(0,0,0);

	if( Mass < 0 )
		Mass = Size.x() * Size.y() * 2.f;

	if(Mass != 0 )
		Shape->calculateLocalInertia( Mass, Local_Inertia );

	btDefaultMotionState* Motion_State = new btDefaultMotionState(Trans);

	btRigidBody::btRigidBodyConstructionInfo Body_Info(Mass,Motion_State,Shape,Local_Inertia);
	nObj->mbt_Body = new btRigidBody(Body_Info);

	nObj->mbt_Body->setLinearFactor( btVector3( 1, 1, 0 ) );
	nObj->mbt_Body->setAngularFactor( btVector3( 0, 0, 1 ) );

	nObj->mbt_Body->setSleepingThresholds( 0, 0 );

	//add the body to the dynamics world
	Dynamics_World->addRigidBody( nObj->mbt_Body );

	// Finished, now add to SE Phys
	Objects.push_back( nObj );

	return (Obj_ID)(nObj);
}

bool SE_Physics::Del_Object( Obj_ID What )
{
	Obj_Mutex.Wait_For_Lock( );//FILE_LINE.c_str() );
	Obj_Mutex.Lock();

	for( uint i = 0;i < Objects.size(); i++ )
	{
		if( Objects[i] == What )
		{
			Objects.erase( Objects.begin() + i );
			i--;
		}
	}

	Obj_Mutex.Unlock();

	delete What->mbt_Body->getMotionState();
	Dynamics_World->removeRigidBody( What->mbt_Body );
	delete What->mbt_Body;
	delete What;

	return true;
}

Vector2f& SE_Physics::Pos2D( Obj_ID What )
{
	return ((Phys_Object2D*)What)->Pos;
}

Vector3f& SE_Physics::Pos3D( Obj_ID What )
{
	return ((Phys_Object3D*)What)->Pos;
}

Vector2f& SE_Physics::Size2D( Obj_ID What )
{
	return ((Phys_Object2D*)What)->Size;
}

Vector3f& SE_Physics::Size3D( Obj_ID What )
{
	return ((Phys_Object3D*)What)->Size;
}

void SE_Physics::Apply_Force( Vector2f v, Obj_ID What )
{
	What->mbt_Body->applyCentralImpulse(btVector3( v.x(), v.y(), 0 ));
}

void SE_Physics::Apply_Force( Vector3f v, Obj_ID What )
{
	What->mbt_Body->applyCentralImpulse(btVector3( v.x(), v.y(), v.z() ));
}

Vector2f SE_Physics::Get_Velocity2D( Obj_ID What )
{
	return Vector2f( What->mbt_Body->getLinearVelocity().x(), What->mbt_Body->getLinearVelocity().y() );
}

Vector3f SE_Physics::Get_Velocity3D( Obj_ID What )
{
	return Vector3f( What->mbt_Body->getLinearVelocity().x(), What->mbt_Body->getLinearVelocity().y(), What->mbt_Body->getLinearVelocity().z() );
}

float& SE_Physics::Attract( Obj_ID What )
{
	return What->m_Attract;
}

btRigidBody* SE_Physics::btBody( Obj_ID What )
{
	return What->mbt_Body;
}

void* SE_Physics::Get_Ext_Info( Obj_ID What )
{
	return What->Ext_Info;
}

bool& SE_Physics::Renderable( Obj_ID What )
{
	return What->Renderable;
}

Vector4f& SE_Physics::Color( Obj_ID What )
{
	return What->Color;
}

uint& SE_Physics::Obj_Tex( Obj_ID What )
{
	What->Tex_Type = SEGL_TEXTURE_ALL;
	return What->Texture;
}

uchar& SE_Physics::Obj_Tex_Type( Obj_ID What )
{
	return What->Tex_Type;
}

void SE_Physics::Set_Pos( Vector2f p, Obj_ID What )
{
	btTransform nTrans = What->mbt_Body->getCenterOfMassTransform();
	nTrans.setOrigin( btVector3( p.x(), p.y(), 0 ));
	What->mbt_Body->setCenterOfMassTransform( nTrans );

	((Phys_Object2D*)What)->Pos = p;
}

void SE_Physics::Set_Pos( Vector3f p, Obj_ID What )
{

}








/* *********   MODULE OPERATIONS  ****** */
namespace SE_Modules
{
	SE_Module_ID Mod_Physics = NULL;
}using namespace SE_Modules;

void SE_Physics::Register_Module()
{
	Mod_Physics = S_Engine::Generate_Module( "bullet-physics" );
	S_Engine::Register_Module_Engine( Mod_Physics, S_Engine::Reg_Mod_Time::LOW_LEVEL + 20, Initialize, Cleanup );

	S_Engine::Register_Module_Thread_Render( Thread_Render, S_Engine::Module_Positions::POS_MED, 128 );
	S_Engine::Register_Module_Thread_Logic( Thread_Update, S_Engine::Module_Positions::POS_MED, 64 );
}
