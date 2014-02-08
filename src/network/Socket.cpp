#include "Net.hpp"
using namespace SE_Network;


#include "../engine/Engine.hpp"


#include <stdio.h>
#include <memory.h>
#include "../utility/Functions.hpp"



SE_Socket_Packet::SE_Socket_Packet()
{
	this->Signature = SE_SIGNATURE;
	memset( this->Data, 0, this->Size() );
	this->Buf_Index = 0;
}

uint SE_Socket_Packet::Size()
{
	return SE_NET_PACKET_SIZE - sizeof(uint);
}

uchar& SE_Socket_Packet::operator[] ( const uint nIndex )
{
	assert( nIndex < this->Size() );
	return this->Data[nIndex];
}

void SE_Socket_Packet::Clear()
{
	memset( Data, 0, this->Size() );//this->Buf_Index > this->Buf_Amount ? this->Buf_Index : this->Buf_Amount );
	this->Signature = SE_SIGNATURE;
	this->Buf_Index = 0;
	this->Buf_Index_Stack.clear();
}

bool SE_Socket_Packet::Buf_Add( const std::string& What )
{
	if( this->Buf_Index + What.length() >= this->Size() )
	{
#ifdef _TEST
		Engine.Log( "Netwk did not add to buf\n" );
#endif
		return false;
	}

	strcpy( (char*)this->Data + this->Buf_Index, What.c_str() );
	this->Buf_Index += What.length() + 1;

	return true;
}

bool SE_Socket_Packet::Buf_Add( void* What, uchar Size )
{
	if( this->Buf_Index + Size >= this->Size() )
	{
#ifdef _TEST
		Engine.Log( "Netwk did not add to buf\n" );
#endif
		return false;
	}

	memcpy( (char*)this->Data + this->Buf_Index, What, Size );
	this->Buf_Index += Size;

	return true;
}

bool SE_Socket_Packet::Buf_Get( std::string* Where )
{
	if( this->Buf_Index >= this->Size() )
		return false;

	Where->append( (char*)this->Data + this->Buf_Index );
	this->Buf_Index += Where->length() + 1;

	return true;
}

bool SE_Socket_Packet::Buf_Get( void* Where, uchar Size )
{
	if( this->Buf_Index >= this->Size() )
		return false;

	memcpy( Where, (char*)this->Data + this->Buf_Index, Size );
	this->Buf_Index += Size;

	return true;
}

void SE_Socket_Packet::Buf_Revert()
{
	if( this->Buf_Index_Stack.size() > 0 )
	{
		memset( this->Data + this->Buf_Index_Stack[ this->Buf_Index_Stack.size() - 1 ], 0,
				this->Buf_Index - this->Buf_Index_Stack[ this->Buf_Index_Stack.size() - 1 ] );

		this->Buf_Index = this->Buf_Index_Stack[ this->Buf_Index_Stack.size() - 1 ];
		this->Buf_Index_Pop();
	}
	else
	{
		this->Clear();
	}
}

void SE_Socket_Packet::Buf_Index_Push()
{
	this->Buf_Index_Stack.push_back( this->Buf_Index );
}

void SE_Socket_Packet::Buf_Index_Pop()
{
	this->Buf_Index_Stack.pop_back();
}





SE_Socket::SE_Socket()
{
#ifdef _WIN32
	this->Thread_h = 0;
#endif

	this->Sock_h = 0;
	this->Ping = 0.f;
	this->Alive = false;

	this->Display_Name = "Bob";
#ifdef _TEST
	if( Engine.Net_Manager && Engine.Net_Manager->Is_Host )
		this->Update_Send = Update_Host_Send;

	else
		this->Update_Send = 0;
#endif

	this->Buf_Amount = 0;
}

void SE_Socket::Initialize()
{
	this->Sock_h = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );
	if( this->Sock_h < 0 )
	{
#ifdef _TEST
		Engine.Register_Error(S_Error( "Could not make socket", 2 ));
#endif
//		printf("Could not make socket\n");
		return;
	}

#ifdef _WIN32
	if( WSA_Assert("Socket Make", 2) )
	{
		this->Cleanup();
		return;
	}
#endif

	this->Alive = true;
}

void SE_Socket::Cleanup()
{
	this->Alive = false;
	this->m_Thread.Running = false;
#ifdef _TEST
	Engine.Log( "Waiting for socket...\n", Log_Type::TYPE_DBUG );
#ifdef _WIN32
	if( this->Thread_h )
		WaitForSingleObject( this->Thread_h, INFINITE );

	closesocket( this->Sock_h );
#else
	this->m_Thread.End();

	close( this->Sock_h );
#endif

	Engine.Log( "Cleaned Socket\n", Log_Type::TYPE_DBUG );
#endif
}

std::string SE_Socket::Get_IP()
{
	return inet_ntoa( this->Address.sin_addr );
}

bool Already_Printed_Rec = false;
void SE_Socket::Recieve()
{
	this->Buffer.Clear();

	this->Buffer.Signature = 0; // check if still alive
	this->Buf_Amount = recv( this->Sock_h, (NET_BUF_TYPE*)&this->Buffer, SE_NET_PACKET_SIZE, 0 );

/*	printf( "Recieved:\n");
	for( uint i = 0; i < this->Buf_Amount; i++ )
	{
		if(this->Buffer[i] < 32)
			printf("\\%d", this->Buffer[i] );
		else
			printf("%c", this->Buffer[i] );
	}
	printf("\n");
*/


	if( this->Buf_Amount == -1 )
	{
		if( Already_Printed_Rec == false )
		{
#ifdef _TEST
			Engine.Printf( "Could not recieve\n", Log_Type::TYPE_NTWK );
#endif
			Already_Printed_Rec = true;
		}
		//this->Alive = false;
	}
	else if( this->Buffer.Signature != SE_SIGNATURE )
	{
#ifdef _TEST
		Engine.Printf( "Signature mismatch\n", Log_Type::TYPE_EROR );
#endif
		this->Alive = false;
	}
}

bool Already_Printed_Send = false;
void SE_Socket::Send()
{
	this->Buf_Amount = send( this->Sock_h, (NET_BUF_TYPE*)&this->Buffer, this->Buffer.Buf_Index + sizeof(uint), 0);

/*	printf( "Sent:\n");
	for( uint i = 0; i < this->Buf_Amount; i++ )
	{
		if(this->Buffer[i] < 32)
			printf("\\%d", this->Buffer[i] );
		else
			printf("%c", this->Buffer[i] );
	}
	printf("\n");
*/

	this->Buffer.Clear();

	if( this->Buf_Amount == -1 )
	{
		if( Already_Printed_Send == false )
		{
#ifdef _TEST
			Engine.Printf( "Could not send\n", Log_Type::TYPE_NTWK );
#endif
			Already_Printed_Send = true;
		}
		//this->Alive = false;
	}
}

void SE_Socket::Send_Flush() // send all
{
	this->m_Mutex.Wait_For_Lock( ); //FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	uint Num_Skipped = 0; // incase it does not fit
	while( this->Event_Out_List.size() > Num_Skipped )
	{
		bool Remove = true;

		switch( this->Event_Out_List.front()->Code )
		{
		case SE_Net_Message::FILE_DOWNLOAD:
		{
			//this->Buffer[this->Buffer.Buf_Index] = SE_Net_Message::FILE_DOWNLOAD; this->Buf_Index++;



		}break;

		case SE_Net_Message::NET_CHAT:
		{
#ifdef _TEST
			if( this->Buffer.Buf_Index + 1 + ((SE_Message_Sys::SE_Message_Event*)this->Event_Out_List.front())->Source.length() +
			    ((SE_Message_Sys::SE_Message_Event*)this->Event_Out_List.front())->What.length() > this->Buffer.Size() )
			{
				Remove = false;
				break;
			}


			this->Buffer[this->Buffer.Buf_Index] = SE_Net_Message::NET_CHAT; this->Buffer.Buf_Index++;
			this->Buffer.Buf_Add( ((SE_Message_Sys::SE_Message_Event*)this->Event_Out_List.front())->Source );
			this->Buffer.Buf_Add( ((SE_Message_Sys::SE_Message_Event*)this->Event_Out_List.front())->What );
#endif
		}break;

		case SE_Net_Message::FILE_VERIFY:
		{
#ifdef _TEST
			this->Buffer[this->Buffer.Buf_Index] = SE_Net_Message::FILE_VERIFY; this->Buffer.Buf_Index++;
			SE_Net_Events::Event_VFS& VFS_Add = ((SE_Events::Event_Var< SE_Net_Events::Event_VFS >*)this->Event_Out_List.front())->Argument;

			uint Amount = 0;
			VFS_Add.Recursive_Add( &Amount, &this->Buffer );

			if( VFS_Add.Children.size() > 0 ) // there is still some to send
				Remove = false;
#endif
		}break;

		case SE_Net_Message::ETC_VERSION:
		{
#ifdef _TEST
			if( this->Buffer.Buf_Index + 1 + Engine.Get_Version()->OS.length() + Engine.Get_Version()->OS_Version.length() +
				Engine.Get_Version()->Build_Target_OS.length() + Engine.Get_Version()->Build_Time.length() +
				Engine.Get_Version()->Build_Date.length() + Engine.Get_Version()->Build_Name.length() > this->Buffer.Size() )
			{
				Remove = false;
				break;
			}

			this->Buffer[this->Buffer.Buf_Index] = SE_Net_Message::ETC_VERSION; this->Buffer.Buf_Index++;
			this->Buffer.Buf_Add( Engine.Get_Version()->OS );
			this->Buffer.Buf_Add( Engine.Get_Version()->OS_Version );
			this->Buffer.Buf_Add( Engine.Get_Version()->Build_Target_OS );
			this->Buffer.Buf_Add( Engine.Get_Version()->Build_Time );
			this->Buffer.Buf_Add( Engine.Get_Version()->Build_Date );
			this->Buffer.Buf_Add( Engine.Get_Version()->Build_Name );
#endif
		}break;
		}

		if( Remove )
		{
#ifdef _TEST
			Engine.Mem_Remove( this->Event_Out_List.front() );
#endif
			delete this->Event_Out_List.front();
			this->Event_Out_List.pop();
		}
		else
		{
			this->Event_Out_List.push( this->Event_Out_List.front() );
			this->Event_Out_List.pop();
			Num_Skipped++;
		}
	}

	this->m_Mutex.Unlock();

	this->Send();
}

void SE_Socket::Add_Event( SE_Events::Event_Base* nEv )
{
	this->m_Mutex.Wait_For_Lock( );// FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	this->Event_Out_List.push( nEv );

	this->m_Mutex.Unlock();
}

void SE_Socket::Recieve_Process()
{
	SE_Events::Event_Base* Cur_Event = NULL;

	// bud_index = 0 since buf is cleared
	for( ; this->Buffer.Buf_Index < this->Buffer.Size(); )//this->Buf_Amount - sizeof(uint)
	{
		this->Buffer.Buf_Index++;
		switch( this->Buffer[this->Buffer.Buf_Index - 1] )
		{
		case SE_Net_Message::ETC_VERSION:
		{
#ifdef _TEST
			SE_Events::Event_Var<S_Engine_Version_Desc>* Get_Ver = new SE_Events::Event_Var<S_Engine_Version_Desc>;

			Get_Ver->Code = SE_Net_Message::ETC_VERSION;

			this->Buffer.Buf_Get( &Get_Ver->Argument.OS );
			this->Buffer.Buf_Get( &Get_Ver->Argument.OS_Version );
			this->Buffer.Buf_Get( &Get_Ver->Argument.Build_Target_OS );
			this->Buffer.Buf_Get( &Get_Ver->Argument.Build_Time );
			this->Buffer.Buf_Get( &Get_Ver->Argument.Build_Date );
			this->Buffer.Buf_Get( &Get_Ver->Argument.Build_Name );

			Cur_Event = Get_Ver;
#endif
		}break;

		case SE_Net_Message::FILE_VERIFY:
		{
			SE_Events::Event_Var< SE_Net_Events::Event_VFS >* Get_VFS = new SE_Events::Event_Var< SE_Net_Events::Event_VFS >(NULL, 0, SE_Net_Events::Event_VFS());

			Get_VFS->Code = SE_Net_Message::FILE_VERIFY;
			Get_VFS->Argument.Recursive_Get( &this->Buffer );

			Cur_Event = Get_VFS;
		}break;

		case 0:		return;
		}

		if( Cur_Event ) // new event to process
		{
#ifdef _TEST
			Engine.Net_Manager->Event_Process( this, Cur_Event );
#endif
			delete Cur_Event;
			Cur_Event = NULL;
		}
	}
}

void SE_Socket::Update( double dt )
{
	this->Ping = 0.f;
}



#ifdef _WIN32

bool SE_Network::WSA_Assert( std::string Info, int Level )
{
	return false;
}

#endif