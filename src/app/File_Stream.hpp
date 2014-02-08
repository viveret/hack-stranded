#ifndef SURVIVOR_ENGINE_FILE_STREAM
#define SURVIVOR_ENGINE_FILE_STREAM

#include "../Core.hpp"
#include "App.hpp"
#include <vector>
#include <fstream>

#define FSTREAM_BUF_SIZE 512

namespace SE_File_Stream
{
	namespace Load_State
	{enum{
		
		LOAD_STATE_NO_ERROR = 0,
		LOAD_STATE_NO_FILE,
		LOAD_STATE_NO_DATA,
		LOAD_STATE_INVALID_DATA,
		LOAD_STATE_EOF
	};}


	struct File_Base
	{
		File_Base();

		void Cleanup();
		void Load( std::string File, void* To_Data );// to data = variable to stream to
		virtual void Init_Data_Buf() = 0;// for custom types

		uint Total_Bytes, Cur_Byte;// percents
		uint Bytes_Per_Load;// num to load each update
		
		void Load_Partial();
		uchar Load_State;

		virtual void Update_Data() = 0;
													 
		// main buffer data
		// update data buffer( if needed )
		SE_Raw_Buffer File_Buf, Data_Buf;

		uint Data_State;// own method
		void* Data;
		
		std::ifstream File_Reader;
		std::string File_Name;
	};


	namespace Def_Files// default file types
	{
		struct File_TXT:public File_Base
		{
			void Init_Data_Buf();
			void Update_Data();

			std::string* mData();
		};
	}



	struct File_Stream_Manager
	{
		File_Stream_Manager();
		~File_Stream_Manager();


		void Cleanup();
		void Update();

										 
		void Add_File( File_Base* File );
		//void Rem_File( File_Base* File );

		SE_Thread::Mutex File_Mutex;
		std::vector< File_Base* > Files;
	};
}



#endif
