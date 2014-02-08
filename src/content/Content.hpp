#ifndef E_CONTENT
#define E_CONTENT

#include "VFS.hpp"

#include "../Core.hpp"
#include "../app/App.hpp"
#include "../render/Mesh.hpp"

#include <string>

#define NUM_CHUNKS 9


namespace E_Content
{
#ifdef _TEST
	struct C_Content_Package
	{
		C_Content_Package( std::string nName );

		std::string Name;
		std::vector< SE_Virt_Filesys::F_Node* > Items; // for replace / remove to load right
	};

	struct C_Registered_Type
	{
		C_Registered_Type();

		std::string Name;
		SE_Virt_Filesys::F_Node* (*Make_New)(SE_Virt_Filesys::F_Node*);
	};

	struct C_Content_Manager
	{
		C_Content_Manager();

		bool Initialize();
		bool Cleanup();

		bool Get_Packages();
		bool Load_Packages();

		bool Get_Pre_Req();
		bool Get_Resources();


		void Register_Type( std::string Name, SE_Virt_Filesys::F_Node* (*Make_New)(SE_Virt_Filesys::F_Node*) );

		SE_Virt_Filesys::F_Folder VFS;
		SE_Render_Mesh::SE_Mesh_Manager Mesh_Manager;
		void VFS_Write();

		bool Log_Unfound_Files;

		uint Percent, Perc_Max;// for the load screen

	private:
		bool Package_Get_List(); // get list of packages
		bool Package_Handle_Script( const std::string& Script, C_Content_Package* Pack ); // perform script
		bool Package_Load( C_Content_Package* Pack );

		std::vector< C_Content_Package > Packages;


		std::vector< C_Registered_Type > Registered_Types;
		C_Registered_Type* Reg_Type_Get( std::string Name , bool Probe = false );
	};

	/*struct C_Content//main container for content
	{
		C_Content();

		SE_Render_Mesh::SE_Mesh_Manager Mesh_Manager;


		C_Content_Manager nContentMgr;


		void Load_Stuff();
		void Load_Package( std::string File );// from content/, for virtual file space
		void Load_Files( SE_Virt_Filesys::F_Folder* nFolder );// load files from virtual filesystem
		uint Percent, Perc_Max;// for the load screen

		std::string Write_Filesystem( SE_Virt_Filesys::F_Folder* Folder );
		void Write_Folder( E_Application::XML_Node* At, SE_Virt_Filesys::F_Folder* Folder );// create filesystem at node

		SE_Virt_Filesys::F_Folder Virtual_Folder;

		uint Get_Res( std::string File );// includes meshes
		std::string* Get_String( std::string File );
		E_Application::Raw_Buffer* Get_Raw( std::string File );

//		SE_Virt_Filesys::F_Extra* Get_Ext( std::string File );
		void* Get_Ext( std::string File, std::string Type );

		bool Log_Unfound_Files;


		void Register_Class_File( void* (*Make_New)(), void (*Load)(void**,std::string), void (*Delete)(void**), std::string Name );// function that makes new instance and name

	private:
		// registered file types
//		std::vector< SE_Virt_Filesys::VFS_Type_Register* > Registered_Types;
		std::vector< std::string > Registered_Types;
	};
	// example of getting an ext mesh: #define Engine.Content.Get_Mesh(X) ((Mesh*)Engine.Content.Get_Ext(X))
	 * */
#endif
}



#endif
