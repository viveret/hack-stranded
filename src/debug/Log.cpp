#include "Log.hpp"
using namespace SE_Debug_Log;

#include "../engine/Engine.hpp"

#include <memory.h>


Log_File::Log_File()
{
	this->is_Open = false;
	this->Line_Type = 0;
}

Log_File::~Log_File()
{
	if(this->is_Open)
	{
		this->Write("Was Not Explicitly Closed, ", 0);
		this->Close();
	}
}

bool Log_File::Open( std::string Where )
{
	if(!this->is_Open)//if not already open
	{
		this->m_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->m_Mutex.Lock();

		this->F_Stream.open(Where.c_str());
		if( this->F_Stream.is_open() )
		{
			this->is_Open = true;
//			this->Write( "Opened Successfully.", false );
			return true;
		}

		this->m_Mutex.Unlock();
	}
	return false;
}

void Log_File::Write( std::string What, uchar Type )
{
	if( this->is_Open && What.length() > 0 )
	{
		this->m_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->m_Mutex.Lock();

//		this->F_Stream << What;
//		this->F_Stream.flush();
		this->Line_Buffer += What;
		this->Line_Type = Type;

		this->Proccess_Buffer();

		this->m_Mutex.Unlock();
	}
}

void Log_File::Proccess_Buffer()
{
	size_t Line_End = this->Line_Buffer.find("\n");
	while( (Line_End = this->Line_Buffer.find("\n")) != std::string::npos )
	{
		std::string nLine = this->Line_Buffer.substr(0, Line_End + 1);
		this->Format_And_Write( nLine );
		this->Line_Buffer.erase( 0, Line_End + 1 );
	}
}

#include <time.h>
void Log_File::Format_And_Write( std::string& m_Line )
{
	std::string Time_Str;
	{
		time_t Get_Time;
		time( &Get_Time );

		char Time_Buf[32];memset( Time_Buf, 0, 32 );
		tm *Time_Info = localtime( &Get_Time );
		strftime( Time_Buf, 32, "%j:%H:%M:%S", Time_Info );

		Time_Str.append( Time_Buf );
	}


//+ (char)(this->Line_Type + '0')
	m_Line = "["  + Time_Str  +  "]\t" + m_Line;

	this->F_Stream << m_Line;
	this->F_Stream.flush();
}

/*void A_Log::Write_No_Format( std::string What, bool To_Console )
{
	if( this->is_Open && What.length() > 0 )
	{
		while( this->Mutex.Is_Locked() );
		this->Mutex.Lock();

		if( To_Console )
		{
			this->Console_Buffer += What;
			this->Process_Buf();
		}

		this->F_Stream << What;
		this->F_Stream.flush();

		this->Mutex.Unlock();
	}
}
					 /*
void A_Log::Write(std::string What)
{
	if( this->is_Open && Engine.Application.Time && What.length() > 0 )
	{
		while( this->Mutex.Is_Locked() );
		this->Mutex.Lock();

		this->F_Stream << What;
		this->F_Stream.flush();

		if(this->Record_History)
			this->Log_History.push_back( What );

		this->Mutex.Unlock();
	}
}		  			*/

/*void A_Log::Write_Log( std::string What, bool To_Console )
{
	this->Print_Time();
	this->Write( What + "\n", To_Console );
}*/

/*void A_Log::Write_Line( std::string What, bool To_Console )
{
	if( this->is_Open && What.length() > 0 )
	{
		std::string Writer = "/t";

		for(uint l = 0;l < What.length(); l++)
			if( What[l] == '\n' )
				Writer += "\n/t";
			else
				Writer += What[l];

		Writer += "\n";


		while( this->Mutex.Is_Locked() );
		this->Mutex.Lock();

		if( To_Console )
		{
			this->Console_Buffer += "\n" + What ;
			this->Process_Buf();
		}

		this->F_Stream << Writer;
		this->F_Stream.flush();

		this->Mutex.Unlock();
	}
}		   */
					 /*
void A_Log_File::Print_Time()
{
	if( this->is_Open )
	{
		while( this->Mutex.Is_Locked() );
		this->Mutex.Lock();

		std::stringstream Writer;
		Writer << ">" << Engine.Application.Time.wMinute << ':' << Engine.Application.Time.wSecond << ':' << Engine.Application.Time.wMilliseconds << "--\n";

		this->F_Stream << Writer.str();
		this->F_Stream.flush();

		this->Mutex.Unlock();
	}
}					   */

void Log_File::Close()
{
	if(this->is_Open)
	{
		this->m_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->m_Mutex.Lock();

		this->Write("Closing...\n", 0);
		this->F_Stream.close();
		this->is_Open = false;

		this->m_Mutex.Unlock();
	}
}
