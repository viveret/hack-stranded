#ifndef SURVIVOR_ENGINE_CONTENT_FILE_TYPES
#define SURVIVOR_ENGINE_CONTENT_FILE_TYPES


#include "../Core.hpp"
#include "VFS.hpp"

namespace SE_Content_File_Types
{
	struct Type_File: public SE_Virt_Filesys::F_Node // n/a
	{
		Type_File( F_Node* Parent );

		void Cleanup();

		F_Node* Make_New( F_Node* Parent );
		static F_Node* Make_New_Static( F_Node* Parent );
	};

	struct Type_Text: public SE_Virt_Filesys::F_Node // n/a
	{
		Type_Text( F_Node* Parent );

		bool Data_Load();
		void Cleanup();

		F_Node* Make_New( F_Node* Parent );
		static F_Node* Make_New_Static( F_Node* Parent );

		static std::string* Get_From_VFS( std::string Name );
		std::string m_Text;
	};

	struct Type_Texture: public SE_Virt_Filesys::F_Node // images
	{
		Type_Texture( F_Node* Parent );

		bool Data_Load();
		void Cleanup();

		F_Node* Make_New( F_Node* Parent );
		static F_Node* Make_New_Static( F_Node* Parent );

		static uint* Get_From_VFS( std::string Name );
		uint ID;
	};

	struct Type_Sound: public SE_Virt_Filesys::F_Node // n/a
	{
		Type_Sound( F_Node* Parent );

		bool Data_Load();
		void Cleanup();

		F_Node* Make_New( F_Node* Parent );
		static F_Node* Make_New_Static( F_Node* Parent );

		static uint* Get_From_VFS( std::string Name );
		uint ID;
	};
}


#endif
