#ifndef SURVIVOR_ENGINE_FILE_SYSTEM
#define SURVIVOR_ENGINE_FILE_SYSTEM


#include "../Core.hpp"
#include "../module/Module.hpp"
#include "../utility/Array.hpp"

#include "File_Stats.hpp"

namespace SE_Filesys
{
/* **********************    WRITING OPERATIONS    ************************* */

	// Just open and register, nothing else
	File_ID Open_For_Write_Stream( const std::string& File );

/**********************   READING OPERATIONS    ********************************/

	// Read and update by the chunk, initialize when done
	File_ID Open_For_Read_Chunky( const std::string& File, std::string Type = "" );

	// Read by chunk, initialize when done
	File_ID Open_For_Read_Whole( const std::string& File, std::string Type = "" );

	// Read a file instantly (bypass streamer)
	SE_Raw_Buffer File_Load_Instant( const std::string& File );

/******************************    VFS OPERATIONS    *******************************/

	File_ID VFS_Get( std::string Name );

	uchar Register_File_Type( File_Type_Desc& nType );
	File_Type_Desc* File_Type_Get( std::string Name );
	File_Type_Desc* File_Type_Get_Ext( const std::string& F_Name );// By file name

	// To start the loading of a file. if read_func == null, read whole
//	File_ID Open_File_Read( std::string File, void* Func_Data, void (*Func_Read)(SE_Raw_Buffer, void* ), void (*Func_End)(void*), uint Buffer_Size = 512, uint Bytes_Per_Second = 0 );
//	File_ID Open_File_Read_Part( std::string File, void* Func_Data, void (*Func_End)(void*), uint Buffer_Size = 512, uint Bytes_Per_Second = 0 );

	// Write to open file
	void Write_To( const void* Data, uint Size, File_ID F_ID );

	// Load in VFS struture
	void VFS_Load( const std::string& Where );
	void VFS_Load_Script( const std::string& Script ); 
	void VFS_Load_XML( void* XML_File );

/****************************    FILE OPERATIONS    *****************************************/

	uchar Get_State( File_ID F_ID ); // Get state of file
	std::string Get_FS_Name( File_ID F_ID ); // Get name
	SE_Raw_Buffer& Get_Data( File_ID F_ID ); // Get data, only when loading

	namespace File_State
	{enum{

		FILE_NO_FILE = 0,
		FILE_NO_ERROR,

		// Applicable to reading
		FILE_NO_DATA,
		FILE_INVALID_DATA,
		FILE_EOF

		// Applicable to writing
	};}

	namespace File_Type
	{enum{
		TYPE_UNKNOWN = 0,
		TYPE_FOLDER,

		TYPE_READ_INSTANT,
		TYPE_READ_STREAM,
		TYPE_WRITE_INSTANT,
		TYPE_WRITE_STREAM
	};}


/* **********************    ETC / MODULE OPERATIONS    ************************* */

	void Register_Module();
}







#endif
