#ifndef SURVIVOR_ENGINE_VIRTUAL_FILE_SYSTEM
#define SURVIVOR_ENGINE_VIRTUAL_FILE_SYSTEM

#include "../Core.hpp"
#include "../app/App.hpp"

#include <string>
#include <vector>

namespace SE_Virt_Filesys
{
	struct F_Node;

	/*struct VFS_Type_Register// registered file type
	{
		VFS_Type_Register();

		std::string Name;

		// general purpose
		virtual void* New() = 0;// function to create new instance, returns address
		virtual void Load( void** Data, std::string Name );// function to load entity
		virtual void Delete( void** Data );// function to delete entity


		virtual F_Node* Get_Child( std::string Name, bool Probe = false );
		virtual void* Get_Child_Ex( std::string Name, bool Probe = false );
	};*/

	struct F_Node
	{
		F_Node( F_Node* Parent );

		// needed to be defined for custom nodes
		virtual F_Node* Make_New( F_Node* Parent ) = 0;

		virtual bool Data_Load(); // load data if needed
		virtual void Cleanup() = 0;


		// default functions per node
//		VFS_Type_Register* Type_Get();
//		void Type_Set( VFS_Type_Register* nType );
		std::string Type_Get();
		void Type_Set( std::string nType );

		F_Node* Parent_Get();
		void Parent_Set( F_Node* nParent );

//		F_Node_Reference* Ref_Add();
//		void Ref_Remove( F_Node_Reference* Ref );
//		uchar Ref_Count_Get();

		void* Data_Get();

		std::string Name, Name_FS;
	private:
		std::string Type;
//		VFS_Type_Register* m_Type;
		std::string m_Type;
//		F_Node* m_Parent;
		bool m_Is_Pointer;

//		std::vector< F_Node_Reference* > References;

		uchar Perm_Set, Perm_Get; // permissions required
	};


	struct F_Folder:public F_Node
	{
		F_Folder( F_Node* Parent );
		void Cleanup();

		F_Node* Make_New( F_Node* Parent );
		static F_Node* Make_New_Static( F_Node* Parent );


		void Add_Child( F_Node* nChild );
		F_Folder* Add_Folder( std::string Name );

		void Remove_Child( std::string Name );
		F_Node* Get_Child( std::string Name, std::string Type = "", bool Probing = false );
		F_Node* Get_Child( std::string Name, bool Probing );


		std::vector< F_Node* > Children;
	};

/*	struct F_Resource:public F_Node
	{
		F_Resource( F_Node* Parent );

		void Add_Child( F_Node* nChild );
		F_Folder* Add_Folder( std::string Name );

		void Remove_Child( std::string Name );
		F_Node* Get_Child( std::string Name, uint Type = -1, bool Probing = false );

		void Load();
		uint ID, Res_Type;
		std::string File_Name;
	};


	struct F_File:public F_Node
	{
		std::string File_Name;



		F_File( F_Node* Parent );

		void Add_Child( F_Node* nChild );
		F_Folder* Add_Folder( std::string Name );

		void Remove_Child( std::string Name );
		F_Node* Get_Child( std::string Name, uint Type = -1, bool Probing = false );
	};
	/*struct F_Folder:public F_Node
	{
		void Clear();
		u_char Get_Type();

		void Add_Child( F_Node* nChild );
		F_Folder* Add_Folder( std::string Name );

		void Remove_Child( std::string Name );
		F_Node* Get_Child( std::string Name, uint Type = -1, bool Probing = false );

	private:
		std::vector< F_Node* > Children;
	};

	struct F_File: public F_Node// also a declaration of a file
	{
		virtual void Clear();
		virtual u_char Get_Type();

		virtual void Load();

		std::string File_Name;
	};

	struct F_Resource:public F_File
	{
		F_Resource();

		enum {
			RES_UNKNOWN = 0,
			RES_TEXTURE,
			RES_SOUND,
			RES_MUSIC
		};

		void Clear();
		u_char Get_Type();

		void Load();

		uint ID;
		u_char Res_Type;// resource type -> PNG, BMP, WAV
	};

	struct F_String:public F_File
	{
		void Clear();
		u_char Get_Type();

		void Load();

		std::string My_Text;
	};

	struct F_Binary:public F_File
	{
		void Clear();
		u_char Get_Type();

		void Load();

		E_Application::Raw_Buffer Buffer;
	};

	struct F_Mesh:public F_File
	{
		F_Mesh();
		void Clear();
		uchar Get_Type();

		void Load();

		uint Mesh_Ptr;
	};

	struct F_Extra:public F_File
	{
		F_Extra();

		void Clear();
		u_char Get_Type();

		void Load();

		void* Data;
		C_Ext_Register* Data_Type;
	};*/
}


#endif
