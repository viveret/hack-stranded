#ifndef SURVIVOR_ENGINE_MESH_CORE
#define SURVIVOR_ENGINE_MESH_CORE

#include <vector>


#include "../Core.hpp"
#include "../utility/Math.hpp"
#include "../app/App.hpp"

#include "../content/Manager.hpp"

#include <assimp/cimport.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>


namespace SE_Render_Mesh
{
	namespace Types
	{enum {
		TYPE_UNKNOWN = 0,
		TYPE_BASIC, // CUBE / SPHERE
		TYPE_SMDL, // SURVIVOR ENGINE MODEL
		TYPE_VBO
	};}


	typedef struct SE_Mesh_Interface
	{
		virtual uchar Get_Type() = 0;
		virtual void Cleanup() = 0;
		virtual void Draw() = 0;
	} SE_Mesh_Base;



	struct Mesh_Vertex
	{
		Mesh_Vertex();
		Point3D Position;
		Vector3D Normal;
		Vector3D Tangent;
		Vector3D Color;
		float U,V;
	};
	struct Mesh_Texture
	{
		Mesh_Texture();
		uint ID;
		uint Type;
	};

	struct SE_Mesh_VBO;
	typedef std::vector< SE_Mesh_VBO* > SE_Mesh_VBO_List;
	struct SE_Mesh_VBO: public SE_Mesh_Interface
	{
		SE_Mesh_VBO();

		uchar Get_Type();

		static void Recursive_Process( aiNode* node, const aiScene* scene, SE_Mesh_VBO_List* M_List );
		static void Process_Mesh( aiMesh* mesh, const aiScene* scene, SE_Mesh_VBO_List* M_List );

		void Initialize( std::vector<Mesh_Vertex>* n_Data, std::vector<uint>* n_id, std::vector<Mesh_Texture>* n_td  );
		void Cleanup();

		void Draw();

		std::vector< Mesh_Vertex > Data;
		std::vector< Mesh_Texture > Textures;
		std::vector< uint > Indices;
		uint VBO;
		uint IND;
	};


	struct SE_Mesh_Scene// group of meshes, container
	{
		SE_Mesh_Scene();

		void Cleanup();
		void Draw();
	};


	namespace SE_Mesh_SMDL
	{
		struct SMDL_Texture_Map_Info
		{
			SMDL_Texture_Map_Info(float nx,float ny, float nw, float nh);
			SMDL_Texture_Map_Info();
			void Reset();
			void Set(float nx,float ny, float nw, float nh);

			float m_x,m_y;
			float m_w,m_h;

			float x();
			float y();
			float w();//x+w
			float h();//y+h
		};
		struct Mesh_SMDL_Base//cube, no physics
		{
			Mesh_SMDL_Base();
			virtual void Draw();
			Vector3 Size,Position;
			SMDL_Texture_Map_Info Tex_Maps[6];// back front left right down up	6
			u_char r,g,b;

			std::string Name;
		};
		struct Mesh_SMDL_Pivot
		{
			Vector3 Pos;
			Vector3 Rotate;

			std::string Name;
		};
		struct SMDL_Pivot_Attachment
		{
			SMDL_Pivot_Attachment();
			void Attach();// attach to mesh pivot

			float* Attach_To;
			float Rotation;// our values
		};

		typedef struct Mesh_SMDL_Bone: public SE_Mesh_Interface
		{
			Mesh_SMDL_Bone();

			uchar Get_Type();

			void Make(u_char nNum_Meshes, u_char nNum_Pivots, u_char nNum_Bones);
			void Cleanup();

			std::string my_Name;

			void Draw();

			Mesh_SMDL_Base* Get_Mesh(std::string Name);
			Mesh_SMDL_Bone* Get_Bone(std::string Name);
			Mesh_SMDL_Pivot* Get_Pivot(std::string Name);

			void Attatch_Rotation(Vector3* Rots);// update rotations


			Mesh_SMDL_Base* Meshes;
			u_char Num_Meshes;

			Mesh_SMDL_Bone** Bones;
			u_char Num_Bones;

			Mesh_SMDL_Pivot* my_Pivots;
			u_char Num_my_Pivots;

			Vector3 Position, Size;// simple stuff, overidable

			// registered with contents
			static void* Make_New();
			static void Load( void** Obj, std::string File );
			static void Delete( void** Obj );
		} Mesh_SMDL_Container;

		namespace Mesh_SMDL_Loaders
		{
			Mesh_SMDL_Container* Load_Mesh(std::string File);
			Mesh_SMDL_Container* Load_Noncompiled_Mesh(std::string File);
			Mesh_SMDL_Container* Load_Compiled_Mesh( std::string File );

			Mesh_SMDL_Container* Load_XML_Mesh(std::string File);
			Mesh_SMDL_Bone* Gen_XML_Bone( SE_Application::XML_Node Bone );// create bone from xml bone

			void Save_XML_Mesh( Mesh_SMDL_Container* Mesh, std::string File );
			void Create_XML_Mesh( SE_Application::XML_File* File, uint Num_Parts );
			void Create_XML_Bone( SE_Application::XML_Node* At, Mesh_SMDL_Bone* Bone );// create XML string from bone

			void Compile_Mesh(std::string Source,std::string Dest);
		}
	}
#ifdef _DEPRECATED
	struct SE_Mesh_Man_Item: public SE_Manager::SE_Man_Res
	{
		SE_Mesh_Man_Item();
		void Cleanup();

		SE_Mesh_Base* m_Mesh;
	};

	struct SE_Mesh_Manager: public SE_Manager::SE_Man_Manager
	{
		SE_Mesh_Manager();
		~SE_Mesh_Manager();

		bool Initialize();
		void Cleanup();

		SE_Manager::SE_Man_Item* Gen_Item();// generate new item

		uint Load_Mesh( std::string File );
		uint Load_Mesh_Assimp( std::string File );
		uint Load_Mesh_SurvivorMDL( std::string File );

		void Use_Mesh( uint Which );
		void Draw( uint Which );

		SE_Mesh_Base* Get_Mesh( uint Which );

	private:

		//std::vector< SE_Mesh_Base* > Meshes;
		aiLogStream Assimp_Stream;

		void Recursive_Process( aiNode* node, const aiScene* scene );
		void Process_Mesh( aiMesh* mesh, const aiScene* scene );
	};
	void recursive_render(const aiScene *sc, const aiNode* nd);
#endif
}




#endif
