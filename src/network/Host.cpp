#include "Net.hpp"
using namespace SE_Network;

#include "../engine/Engine.hpp"

#include "../utility/Functions.hpp"

#include <stdio.h>
#include <errno.h>

SE_Net_Manager_HOST::SE_Net_Manager_HOST()
{
	this->Is_Host = true;
}

void SE_Net_Manager_HOST::Initialize()
{
#ifdef _WIN32
	this->Initialize_WSA();
#endif

	this->Main_Socket.Display_Name = "Host";
	this->Main_Socket.Initialize();
}

void SE_Net_Manager_HOST::Update( double dt )
{
	for( char p = 0; p < this->Clients.size(); p++ )
	{
		if( this->Clients[p]->Alive )
		{
			if( this->Clients[p]->Ping > SE_NET_PING_LOST)//lost connection
			{
				this->Clients[p]->Cleanup();
#ifdef _TEST
				Engine.Mem_Remove( this->Clients[p] );
#endif
				delete this->Clients[p];
				this->Clients.erase( this->Clients.begin() + p );
				p--;
				continue;
			}
			else
				this->Clients[p]->Update( dt );
		}
    }
}

void Recursive_Print_VFS( SE_Net_Events::Event_VFS* What, uint Depth )
{
	assert( What != NULL );

	for( uint i = 0; i < Depth; i++ )
		printf("\t");

	if( What->Children.size() > 0 )
		printf( "Folder:");
	else
		printf( "File:");
	printf( " %s, FS: %s\n", What->Name.c_str(), What->Name_FS.c_str() );

	for( uint i = 0; i < What->Children.size(); i++ )
		Recursive_Print_VFS( &What->Children[i], Depth + 1 );
}

void SE_Net_Manager_HOST::Event_Process( SE_Socket* This, SE_Events::Event_Base* nEv )
{
	switch( nEv->Code )
	{
	case SE_Net_Message::ETC_VERSION:
	{
#ifdef _TEST
		Engine.Printf( "[%s] version: %s\n", Log_Type::TYPE_NTWK, This->Get_IP().c_str(),
				((SE_Events::Event_Var<S_Engine_Version_Desc>*)nEv)->Argument.As_String().c_str() );
#endif
	}break;

	case SE_Net_Message::FILE_VERIFY:
	{
#ifdef _TEST
		Engine.Printf( "Get VFS:\n" );
#endif
		Recursive_Print_VFS( &((SE_Events::Event_Var< SE_Net_Events::Event_VFS >*)nEv)->Argument, 0 );
	}break;
	}
}

void SE_Net_Manager_HOST::Start_Listening_For_Clients()
{
	if( this->Con_Port == 0 )
	{
#ifdef _TEST
		Engine.Log( "Cannot Host Without Port\n", Log_Type::TYPE_EROR );
#endif
		return;
	}

	this->Main_Socket.Address.sin_family = AF_INET;
	this->Main_Socket.Address.sin_addr.s_addr = inet_addr("0.0.0.0");
	this->Main_Socket.Address.sin_port = htons( this->Con_Port );

	if( bind(this->Main_Socket.Sock_h, (const sockaddr*)&this->Main_Socket.Address, sizeof(this->Main_Socket.Address)) == -1 )
	{
		this->Assert( "Could not bind" );
		this->Cleanup();
		return;
	}

	if( listen(this->Main_Socket.Sock_h, SOMAXCONN) == -1 )//acceptance queue max
	{
		this->Assert( "Could not listen" );
		this->Cleanup();
		return;
	}

	this->Main_Socket.m_Thread.Start( this->Listen_For_Clients );// start listening thread
}

void SE_Net_Manager_HOST::Cleanup()
{
	this->Main_Socket.Alive = false;

	for( uint i = 0;i < this->Clients.size(); i++ )
	{
		this->Clients[i]->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( this->Clients[i] );
#endif
		delete this->Clients[i];
	}

	this->Clients.clear();

	this->Main_Socket.Cleanup();
}

void SE_Net_Manager_HOST::Listen_For_Clients()
{
#ifdef _TEST
	if( Engine.Net_Manager->Main_Socket.Alive == false ) return;
	Engine.Printf( "Started listening for clients\n", 0 );
	while( Engine.Net_Manager->Main_Socket.Alive )
	{
		SE_Socket* Temp_Sock = new SE_Socket;
#ifdef _TEST
		Engine.Mem_Add( Temp_Sock, "client socket" );

		socklen_t nLength = sizeof( Temp_Sock->Address );

		Temp_Sock->Initialize();
		Temp_Sock->Sock_h = accept( Engine.Net_Manager->Main_Socket.Sock_h, (sockaddr*)&Temp_Sock->Address, &nLength );
		Engine.Printf( "Client %s connected\n", 0, Temp_Sock->Get_IP().c_str() );
#endif
		((SE_Net_Manager_HOST*)Engine.Net_Manager)->Clients.push_back( Temp_Sock );

		Temp_Sock->m_Thread.Start( &SE_Net_Manager_HOST::Update_Client, Temp_Sock );

	#ifdef _WIN32
		if( this->Main_Socket.Sock_h )// already listening
		{
			Engine.Log( "Tried to listen to Host Socket when already existing\n", Log_Type::TYPE_EROR );
			return;
		}

		// string have been set, start listening

		this->Target.sin_family = AF_INET;
		this->Target.sin_port = htons( atoi(this->Con_Port.c_str()) );//assign port to this socket
		this->Target.sin_addr.s_addr = inet_addr("0.0.0.0");


		bind( this->Main_Socket.Sock_h, (LPSOCKADDR)&this->Target, sizeof(this->Target) );
		if( Engine.WSA_Assert( "Bound HOST Socket", 2 ) )
		{
			this->Cleanup();
			return;
		}

		listen( this->Main_Socket.Sock_h, 1 );
		if( Engine.WSA_Assert( "Listen to HOST Socket", 2 ) )
		{
			this->Cleanup();
			return;
		}

		Engine.Log( "Starting HOST Listen Thread\n" );
		this->Main_Socket.Thread_h = CreateThread( NULL, 0, HOST_Listen_For_New, (void*)NULL, 0, NULL );
#endif
	}
#endif
}

void SE_Net_Manager_HOST::Update_Client( void* This_Void )
{
	SE_Socket* This = (SE_Socket*)This_Void;
	assert( This != NULL );
	assert( This->Update_Send != 0 );

	This->Add_Event( new SE_Events::Event_Base(NULL, SE_Net_Message::ETC_VERSION) );


	while( This->Alive )
	{
		This->Update_Send( This );
		This->Send_Flush();// send all

//		usleep( 5000 );
//		sleep(1);sleep(1);sleep(1);

		This->Recieve();
		This->Recieve_Process();
		This->Buffer.Clear();
	}
}

using namespace Socket_States;
void SE_Network::Update_Host_Send( SE_Socket* This )
{
	switch( This->m_State )
	{
	case STATE_INIT:
	{

		This->m_State++;
	}break;
	}
}

/*void SE_Network::Update_Host_Recieve( SE_Socket* This )
{
	switch( This->m_State )
	{
	case STATE_INIT:
	{
	}break;

	case STATE_NORMAL:
	{
		for( uint i = 0;i < This->Buffer.Get_Size(); i++ )
		{
			if( This->Buffer[i] == SE_Net_Message::NET_CHAT )
			{
				SE_Message_Sys::SE_Message nMsg;
				nMsg.Source = (char*)(This->Buffer.Get_Data() + i + 1); i += nMsg.Source.length() + 1;
				nMsg.What = (char*)(This->Buffer.Get_Data() + i + 1);
				nMsg.What_Color.Set( 1, 1, 1 );

				//nMsg.Source = This->Display_Name;

				Engine.GUI_Manager.Message_Log.Print_Message( nMsg );
				break;
			}
		}
	}break;
	}
}*/

void SE_Net_Manager_HOST::Global_Event( SE_Events::Event_Base* nEv )
{
	this->m_Mutex.Wait_For_Lock( );// FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Clients.size(); i++ )
	{
		this->Clients[i]->Add_Event( nEv->Clone() );
	}

	this->m_Mutex.Unlock();
}
