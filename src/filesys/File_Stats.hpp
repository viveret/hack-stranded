#ifndef SURVIVOR_ENGINE_FILESYS_STATS
#define SURVIVOR_ENGINE_FILESYS_STATS

#include "../Core.hpp"
#include <vector>
#include "../utility/Array.hpp"

namespace SE_Filesys
{
	struct FS_Folder; // Folder structure
	struct File_Type_Desc; // Describes the file type
	struct File_Stats_Active; // Data for interacting with OS

	struct FS_Stats // Details for any FS Item
	{
		std::string Name;
		FS_Folder* Parent;

		virtual uchar Type() = 0;
	};

	struct File_Stats: public FS_Stats
	{
		File_Stats();

		std::string* Name_Package; // For VFS, NULL = CWD
		File_Type_Desc* m_Type;
	};
	typedef File_Stats* File_ID;
	

	struct File_Type_Desc
	{
		File_Type_Desc();

		std::vector< std::string > Extension; // Without the '.'
		std::string Name;
//		std::vector< std::string > Name_Aliases; // Ie texture/image

		File_Stats* (*Func_New)(); // Create instance
		uchar (*Func_Init)( File_Stats* ); // Initialize instance -> Part + End / Whole
		uchar (*Func_Clean)( File_Stats* ); // Destroy instance
		uchar (*Func_Stream)( SE_Raw_Buffer, File_Stats* ); // For streaming
	};
}

//typedef SE_Filesys::File_ID File_ID;


#endif
