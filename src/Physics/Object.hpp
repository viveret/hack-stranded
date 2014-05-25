#ifndef SURVIVOR_ENGINE_PHYS_OBJECT
#define SURVIVOR_ENGINE_PHYS_OBJECT

#include <vector>
#include <string>


//#include <S_Engine\Object.h>

//#include "Functions.h"

//#include "myMath.h"

//#include "Bullet\btBulletDynamicsCommon.h"


/*#define DRAW_NORM 0x1
#define DRAW_GLOW 0x2
#define DRAW_SHADOWS 0x4
#define DRAW_NOTEX 0x8
#define DRAW_OUTLINE 0x10
//#define P_FLAG_NORM 0x2


class Part
{
public:
	Part();
	~Part();


    std::string name;
    bool draw,ShowDots,usetex,rep;//rep = repeat texture, objt = use object (parent) texture

	int flags;
    //Vector3 Position;
    Vector3 Size;
    //std::vector<rotate3D>rots;//Point on which to rotate, local to object
	btRigidBody* Body;
	Vector3 Get_Position();//get position from body
	Vector3 Get_hSize();//get half size from body

    Vector4 Color;
	void reColor(Vector4 nColer);
	void reColor(float r,float g,float b,float a)
	{
		this->reColor(Vector4(r,g,b,a));
	}

	void Texture(unsigned int nt0,unsigned int nt1,unsigned int nt2,unsigned int nt3,unsigned int nt4,unsigned int nt5,bool nRep);
	unsigned int t0,t1,t2,t3,t4,t5;//textures to use
    float tx0,ty0,tw0,th0;//Texturing 0
    float tx1,ty1,tw1,th1;//Texturing 1
    float tx2,ty2,tw2,th2;//Texturing 2
    float tx3,ty3,tw3,th3;//Texturing 3
    float tx4,ty4,tw4,th4;//Texturing 4
    float tx5,ty5,tw5,th5;//Texturing 5

	void DefTx();//set to tex pos defaults

    void New(Vector3 nPos,Vector3 nSize,std::string Nname,float Mass);
	void New(Vector3 nPos,Vector3 nSize,std::string Nname)
	{
		this->New(nPos,nSize,Nname,0);
	}
	void New(float x,float y,float z,float l,float h,float w,std::string Nname,float Mass)
	{
		this->New(Vector3(x,y,z),Vector3(l,h,w),Nname,Mass);
	}
	void New(float x,float y,float z,float l,float h,float w,std::string Nname)
	{
		this->New(Vector3(x,y,z),Vector3(l,h,w),Nname,0);
	}

	void Draw(Vector3 nPos,int flags);
	void Draw(float x,float y,float z,int flags)
	{
		this->Draw(Vector3(x,y,z),flags);
	}

	void RepProp();//makes it
};

class Object
{
    public:
	std::string Name;
    std::vector<Part>Parts;

	//btAlignedObjectArray<btRigidBody*>Bodys;



	void Draw(Vector3 Pos,int flags);
    void Draw(float nx,float ny,float nz,int nflags)
	{
		this->Draw(Vector3(nx,ny,nz),flags);
	}
    Vector3 Position;
    std::vector<rotate3D>rots;

    unsigned int tex;
    bool usetex,ShowDots;

	int flags;

    void New(std::string nName,Vector3 nPos,unsigned int ntex,bool nusetex,bool texrep);// pass ntex to children
	void New(std::string nName,float x,float y,float z,unsigned int ntex,bool nusetex,bool texrep)
	{
		this->New(nName,Vector3(x,y,z),ntex,nusetex,texrep);
	}
    void Make(Object What);
};

#include "Resources.h"


namespace Object_Namespace
{
	struct Base_Object;

	struct Draw_Component
	{
		Draw_Component(Base_Object* Parent);
		Draw_Component();//same but with NULL
		~Draw_Component();
		void Defaults(Base_Object* Parent);//clears everything

		void Draw();

		void Apply_Texture(unsigned int Tex);//only 1
		void Apply_Texture(unsigned int* Texs);//only 6
		void Apply_Texture_Map(Vector2 Pos,Vector2 Size);//only 1 for all faces
		void Apply_Texture_Map(Vector2* Pos,Vector2* Size);//only 6
		void Remove_Texture();
		void Texture_Defaults();//just sets values to defaults if there

		void Apply_Color(Vector4 nColor);//only 1 color
		void Apply_Color(Vector4* nColor);//only 6
		void Remove_Color();
		void Color_Defaults();//just sets values to defaults if there

	private:
		/*   IF USES TEXTURE   *//*
		unsigned int* Texture;//1 or 6
		bool Multi_Texture;//if 1 or 6
		Vector2* Tex_Map_Pos;//1 or 6
		Vector2* Tex_Map_Size;//1 or 6
		//COLORSDevelopment
		Vector4* Color;//1 or 6, not needed if has texture
		bool Multi_Color;
		//
		bool Use_Lighting;

		Base_Object* Parent;//additional info

		void Do_Nulls();//put down nulls
	};

	struct Physics_Component
	{
		//default
		//bounding box(s)
		//Matrix3 Rotation;

		//dynamic (moving)
		Vector3 Velocity;
		std::vector<Vector3>Forces;
	};

	struct Base_Object
	{
		Draw_Component* Drawing;
		Physics_Component* Physics;

		//default
		Vector3 Position;
		//functions
		void Update(double dt);
		void Draw();
	};
}*/
		 /*struct Base_Draw_Component
	{
		Base_Draw_Component(Base_Object* Parent);
		Base_Draw_Component();//same but with NULL
		~Base_Draw_Component();

		void Defaults(Base_Object* Parent);//clears everything

		void Draw();

		void Apply_Texture(unsigned int Tex);//only 1
		void Apply_Texture(unsigned int* Texs);//only 6
		void Apply_Texture_Map(Vector2 Pos,Vector2 Size);//only 1 for all faces
		void Apply_Texture_Map(Vector2* Pos,Vector2* Size);//only 6
		void Remove_Texture();
		void Texture_Defaults();//just sets values to defaults if there

		void Apply_Color(Vector4 nColor);//only 1 color
		void Apply_Color(Vector4* nColor);//only 6
		void Remove_Color();
		void Color_Defaults();//just sets values to defaults if there

	private:
		//   IF USES TEXTURE
		unsigned int* Texture;//1 or 6
		bool Multi_Texture;//if 1 or 6
		Vector2* Tex_Map_Pos;//1 or 6
		Vector2* Tex_Map_Size;//1 or 6
		//COLORS
		Vector4* Color;//1 or 6, not needed if has texture
		bool Multi_Color;
		//
		bool Use_Lighting;

		Base_Object* Parent;//additional info

		void Do_Nulls();//put down nulls
	};*/


#include "../Core.hpp"
#include <vector>

//#include "Object_Mesh.h"


#include "Physics.hpp"


//#define PROJ_LIFE_TIME 20000.

namespace SE_OPhys_bjects
{
	struct Object_Base_Draw
	{
		virtual void Draw() = 0;
		virtual void Cleanup() = 0;


		uint Draw_Type;
		enum {
			DRAW_BOX = 0,
			DRAW_CYLINDER,
			DRAW_MESH,
			DRAW_MESH_VBO
		};
	};

	struct Bullet_Data
	{
		//Bullet_Data();

		Bullet_Data( uint nID, btRigidBody* nBody, void* Own, std::string nName );

		virtual void Update();


		enum{
			ID_UNKNOWN = 0,
			ID_OBJ,
			ID_TERRAIN,
			ID_AI,
			ID_PROJECTILE
		};

		uint id;
		bool Hit;
		void* Own_Obj;

		btRigidBody* Body;

		std::string Name;
	};

	struct Projectile_Descriptor
	{
		Vector3f Color;
		//uint Tex;
		Vector3f Size;

		std::string Name;
		std::vector<std::string> Traits;
	};

	struct Object_Projectile
	{
		Object_Projectile();
		void Delete();
		void Make( Vector3f Position, Vector3f Velocity, Projectile_Descriptor* Proj_Desc );

		void Update(double dt);
		void Draw();

		double Trail_Time, Life_Time;
		std::vector<Vector3f> Trail;
		void Update_Trail();


		btRigidBody* Body;

		void Get_Matrix();
		btScalar GLMatrix[16];

		Projectile_Descriptor* Draw_Desc;
	};

	struct Object_Base;

	struct Object_Base_Lean
	{
		Object_Base_Lean();
		~Object_Base_Lean();
		virtual void Reset();
		virtual void Delete_Body();
		virtual void Make_From(Vector3f Position,Vector3f nSize,float Mass,uint Flags);//center of mass and half size
		virtual void Draw();

		Vector3f Size,Position;//bounding box
		btRigidBody* Body;
		uint Tex_ID;
	};

	struct Object_Base:public Object_Base_Lean
	{
		Object_Base();
		~Object_Base();
		void Reset();

		void Make_From(Vector3f Position,Vector3f nSize,float Mass,uint Flags);//center of mass and half size
		void Make_From(Vector3f Position,Vector3f nSize,uint Flags);//center of mass and half size

		uint Flags;

		Vector3f Get_Position();//get position from body
		Vector3f Get_hSize();//get half size from body
		//physics
		Vector3f Acceleration;
		void Update(double dt);
		void Get_Matrix();

		btScalar GLMatrix[16];


		void Set_Rotation(Vector3f Angles);
		void Set_RotationX(float Angle);
		void Set_RotationY(float Angle);
		void Set_RotationZ(float Angle);

		//drawing
		virtual void Draw();//clamped
		bool Draw_Bounding_Box;

		//O_Mesh::Mesh_Container* Mesh;
	};

   struct Object_Container
   {
	   void Delete();
	   void Add_To(Object_Container* Other);
	   void Draw();

	   std::vector<Object_Base*> Objects;
   };
}


namespace SE_Phys_Objects
{
	struct SE_Object_Dynamic // most interested in
	{

	};
}



//#define SE_PHYS_CHAN_SIZE 8// # different channels of gravity
namespace SE_Physics
{
//	typedef uchar Physics_Channel[ SE_PHYS_CHAN_SIZE / sizeof(uchar) ];
	struct Phys_Object;
	typedef Phys_Object* Obj_ID;

	Obj_ID Gen_Object( Vector3f Pos, Vector3f Size, float Mass, uchar mShape, void* Ext_Info = NULL );
	Obj_ID Gen_Object( Vector2f Pos, Vector2f Size, float Mass, uchar mShape, void* Ext_Info = NULL );

	bool Del_Object( Obj_ID What );

	Vector2f& Pos2D( Obj_ID What );
	Vector3f& Pos3D( Obj_ID What );

	Vector2f& Size2D( Obj_ID What );
	Vector3f& Size3D( Obj_ID What );

	float& Attract( Obj_ID What );

	btRigidBody* btBody( Obj_ID What );
	void MultGL( Obj_ID What ); // Translate GL by object

	void* Get_Ext_Info( Obj_ID What );

	bool& Renderable( Obj_ID What );
	Vector4f& Color( Obj_ID What );
	uint& Obj_Tex( Obj_ID What );
	uchar& Obj_Tex_Type( Obj_ID What );

	void Apply_Force( Vector2f v, Obj_ID What );
	void Apply_Force( Vector3f v, Obj_ID What );

	Vector2f Get_Velocity2D( Obj_ID What );
	Vector3f Get_Velocity3D( Obj_ID What );

	void Set_Pos( Vector2f p, Obj_ID What );
	void Set_Pos( Vector3f p, Obj_ID What );
}


#define DRAW_NORM 0

//creation flags

#define OBJ_STAYUP 0x01//stay upright
#define OBJ_NOROT  0x02//do not rotate

#endif // OBJECT_H_INCLUDED
