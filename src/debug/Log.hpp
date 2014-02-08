#ifndef SURVIVOR_ENGINE_DEBUG_LOG
#define SURVIVOR_ENGINE_DEBUG_LOG

#include "../Core.hpp"
#include "../module/Thread.hpp"
#include <fstream>

namespace SE_Debug_Log
{
	struct Log_File
	{
		Log_File();
		~Log_File();

		bool Open( std::string Where );
		void Close();

		void Write( std::string What, uchar Type );

	private:
		SE_Thread::Mutex m_Mutex;
		std::ofstream F_Stream;
		bool is_Open;

		std::string Line_Buffer;
		uchar Line_Type;

		void Proccess_Buffer();

		void Format_And_Write( std::string& m_Line );
	};
}

#endif
