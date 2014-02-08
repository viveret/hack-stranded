#include "Net.hpp"
using namespace SE_Network;


#include "../engine/Engine.hpp"


#include <errno.h>
#include <stdio.h>

SE_Net_Manager_Base::SE_Net_Manager_Base()
{
	this->Is_Host = false;
}

bool SE_Net_Manager_Base::Assert( std::string Message )
{
	if( errno != 0 )// something happened
	{
		std::string Enum_Str = "unknown";
		switch( errno )
		{
		case EACCES:Enum_Str = "EACCES";break;
		case EPERM:Enum_Str = "EPERM";break;
		case EADDRINUSE:Enum_Str = "EADDRINUSE";break;
		case EAFNOSUPPORT:Enum_Str = "EAFNOSUPPORT";break;
		case EAGAIN:Enum_Str = "EAGAIN";break;
		case EBADF:Enum_Str = "EBADF";break;
		case EINVAL:Enum_Str = "EINVAL";break;
		case EALREADY:Enum_Str = "EALREADY";break;
		case ECONNREFUSED:Enum_Str = "ECONNREFUSED";break;
		case EFAULT:Enum_Str = "EFAULT";break;
		case EINPROGRESS:Enum_Str = "EINPROGRESS";break;
		case EINTR:Enum_Str = "EINTR";break;
		case EISCONN:Enum_Str = "EISCONN";break;
		case ENETUNREACH:Enum_Str = "ENETUNREACH";break;
		case ETIMEDOUT:Enum_Str = "ETIMEDOUT";break;
		case ENOTSOCK:Enum_Str = "ENOTSOCK";break;
		}
#ifdef _TEST
		Engine.Printf( "Socket error: %s - %d, %s\n", Log_Type::TYPE_EROR, Enum_Str.c_str(), errno, Message.c_str() );
#endif
		return true;
	}
	else
		return false;
}




void SE_Network::Input_Start_Host( sshort* Data )
{
#ifdef _TEST
	if( Engine.Net_Manager ) return;

	Engine.Make_Net_Manager( true );
	Engine.Net_Manager->Con_Port = 7777;
	((SE_Network::SE_Net_Manager_HOST*)Engine.Net_Manager)->Start_Listening_For_Clients();
#endif
}

void SE_Network::Input_Start_Client( sshort* Data )
{
#ifdef _TEST
	if( Engine.Net_Manager ) return;

	Engine.Make_Net_Manager( false );
	((SE_Network::SE_Net_Manager_CLIENT*)Engine.Net_Manager)->Con_IP = "127.0.0.1";
	Engine.Net_Manager->Con_Port = 7777;
	((SE_Network::SE_Net_Manager_CLIENT*)Engine.Net_Manager)->Connect();
#endif
}




#ifdef _WIN32
#include <sstream>
void SE_Net_Manager_Base::Initialize_WSA()// the network library
{
    uint nErr = WSAStartup( MAKEWORD(2,2), &this->WSA_Data );

	if( nErr != 0 )
	{
		LPTSTR Error_Str = NULL;

        FormatMessage( FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL,
					   nErr, 0, (LPTSTR)&Error_Str, 0, NULL );

        std::stringstream nError_Str; nError_Str << "WSA Startup Error: " << nErr << ", " << Error_Str;
		sePrintf( SEPRINT_ERROR, nError_Str.str().c_str() );

		LocalFree( Error_Str );

		return;
	}

	sePrintf( SEPRINT_MODULE, "Initialized WSA Library\n" );
}

void SE_Net_Manager_Base::Cleanup_WSA()
{
	this->Main_Socket.Cleanup();

#ifdef _WIN32
    WSACleanup();
#endif
}



DWORD WINAPI SE_Network::HOST_Listen_For_New(void*)
{
	//
	return (DWORD)0;
}

DWORD WINAPI SE_Network::HOST_Handle_Client(void*)
{
	//
	return (DWORD)0;
}

DWORD WINAPI SE_Network::CLIENT_Connect_To_Host(void*)
{
	//
	return (DWORD)0;
}
#endif








using namespace SE_Net_Events;


/*
 * The format is:
 * Name
 * FS_Name
 * # Children sent
 * 		Name
 * 		FS_NAme
 * 		#chidlren sent
 * 		end
 * end
 */
void Event_VFS::Recursive_Add( uint* Amount, SE_Network::SE_Socket_Packet* Where )
{
	assert( Amount != NULL );
	assert( Where != NULL );

	Where->Buf_Index_Push();
	if( Where->Buf_Add( this->Name ) == false )
	{
		Where->Buf_Index_Pop();
		return;
	}
	if( Where->Buf_Add( this->Name_FS ) == false ) // first is the current directory
	{
		Where->Buf_Revert();
		return;
	}

	uint Children_Amount = 0;
	uchar* Child_Sent_Buf = Where->Data + Where->Buf_Index; // get where children sent is
	if( Where->Buf_Add( &Children_Amount, sizeof(uint) ) == false )
	{
		Where->Buf_Revert();
		return;
	}


	uint Child_i = 0;
	while( Child_i < this->Children.size() && *Amount < SE_NET_FS_AVG ) //&& *Length < Where->Size() - Where->Buf_Index )
	{
		uint Amount_Before = *Amount;
		this->Children[Child_i].Recursive_Add( Amount, Where );

		if( Amount_Before == *Amount ) // did not fit
			Child_i++;

		else // was added
		{
			this->Children.erase( this->Children.begin() + Child_i );
			Children_Amount++;
		}
	}

	memcpy( Child_Sent_Buf, &Children_Amount, sizeof(uint) );
	(*Amount)++;
	Where->Buf_Index_Pop();
}

void Event_VFS::Recursive_Get( SE_Network::SE_Socket_Packet* Where )
{
	assert( Where != NULL );

	Where->Buf_Get( &this->Name );
	Where->Buf_Get( &this->Name_FS );

	uint Num_Children = 0;
	Where->Buf_Get( &Num_Children, sizeof(uint) );

	for( uint i = 0; i < Num_Children; i++ )
	{
		this->Children.push_back( Event_VFS() );
		this->Children[ i ].Recursive_Get( Where );
	}
}

void Event_VFS::Create_From_VFS( Event_VFS* Where, SE_Virt_Filesys::F_Folder* Folder )
{
#ifdef _TEST
	Where->Name = Folder->Name;
	Where->Name_FS = Folder->Name_FS;

	for(uint i = 0;i < Folder->Children.size(); i++)
	{
		Where->Children.push_back( Event_VFS() );
		if( Folder->Children[i]->Type_Get() ==	"folder" )
			Event_VFS::Create_From_VFS( &Where->Children[Where->Children.size() - 1], (SE_Virt_Filesys::F_Folder*)Folder->Children[i] );

		else
		{
			Where->Children[Where->Children.size() - 1].Name = Folder->Children[i]->Name;
			Where->Children[Where->Children.size() - 1].Name_FS = Folder->Children[i]->Name_FS;
		}
	}
#endif
}





Event_Download_File::Event_Download_File()
{
	this->Length = 0;
	this->Data = NULL;
}
