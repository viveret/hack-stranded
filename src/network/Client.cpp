#include "Net.hpp"
using namespace SE_Network;


#include "../engine/Engine.hpp"



SE_Net_Manager_CLIENT::SE_Net_Manager_CLIENT()
{
	this->Is_Host = false;
}

void SE_Net_Manager_CLIENT::Initialize()
{
#ifdef _WIN32
	this->Initialize_WSA();
#endif

	this->Main_Socket.Display_Name = "Client";
	this->Main_Socket.Initialize();

	this->Main_Socket.Update_Send = Update_Client_Send;
}

void SE_Net_Manager_CLIENT::Update( double dt )
{
	//
}

void SE_Net_Manager_CLIENT::Event_Process( SE_Socket* This, SE_Events::Event_Base* nEv )
{
	switch( nEv->Code )
	{
	case SE_Net_Message::ETC_VERSION:
	{
#ifdef _TEST
		Engine.Printf( "Host version: %s\n", Log_Type::TYPE_NTWK,
				((SE_Events::Event_Var<S_Engine_Version_Desc>*)nEv)->Argument.As_String().c_str() );
#endif
	}break;
	}
}

void SE_Net_Manager_CLIENT::Cleanup()
{
	this->Main_Socket.Cleanup();
}

void SE_Net_Manager_CLIENT::Connect()
{
	if( this->Con_IP.length() == 0 )
	{
#ifdef _TEST
		Engine.Log( "Cannot Connect Without IP\n", Log_Type::TYPE_EROR );
#endif
		return;
	}

	if( this->Con_Port == 0 )
	{
#ifdef _TEST
		Engine.Log( "Cannot Connect Without Port\n", Log_Type::TYPE_EROR );
#endif
		return;
	}

	// string have been set, start connecting

#ifdef _WIN32
	sePrintf( SEPRINT_DEBUG, "Starting CLIENT Listen Thread\n" );
	this->Main_Socket.Thread_h = CreateThread( NULL, 0, CLIENT_Connect_To_Host, (void*)NULL, 0, NULL );
#else
	this->Main_Socket.Address.sin_family = AF_INET;
	this->Main_Socket.Address.sin_addr.s_addr = inet_addr(this->Con_IP.c_str());
	this->Main_Socket.Address.sin_port = htons( this->Con_Port );

	if( connect(this->Main_Socket.Sock_h,(const sockaddr*)&this->Main_Socket.Address, sizeof(this->Main_Socket.Address)) == -1 )
	{
		this->Assert( "Could not connect" );
		this->Cleanup();
		return;
	}

	this->Main_Socket.m_Thread.Start( this->Listen_To_Host );// start listening thread
#endif
}

void SE_Net_Manager_CLIENT::Listen_To_Host() // start listening thread
{
	SE_Socket* This;
#ifdef _TEST
	= &Engine.Net_Manager->Main_Socket;
#endif
	assert( This != NULL );
	assert( This->Update_Send != 0 );

	This->Add_Event( new SE_Events::Event_Base( NULL, SE_Net_Message::ETC_VERSION ) );

	SE_Events::Event_Var< SE_Net_Events::Event_VFS >* Get_VFS = new SE_Events::Event_Var< SE_Net_Events::Event_VFS >(NULL, 0, SE_Net_Events::Event_VFS() );
	Get_VFS->Code = SE_Net_Message::FILE_VERIFY;
#ifdef _TEST
	SE_Net_Events::Event_VFS::Create_From_VFS( &Get_VFS->Argument, &Engine.Content.VFS );
#endif
	This->Add_Event( Get_VFS );
#ifdef _TEST
	while( Engine.Net_Manager->Main_Socket.Alive )
	{
		This->Recieve();
		This->Recieve_Process();
		This->Buffer.Clear();

//		usleep( 5000 );
		sleep(1);

		This->Update_Send( This );
		This->Send_Flush();// send all
	}
#endif
}

using namespace Socket_States;
void SE_Network::Update_Client_Send( SE_Socket* This )
{
	switch( This->m_State )
	{
	case STATE_INIT:
	{

		This->m_State++;
	}break;
	}
}
/*
void SE_Network::Update_Client_Recieve( SE_Socket* This )
{
	switch( This->m_State )
	{
	case STATE_INIT:
	{
		This->m_State++;
	}break;
	case STATE_NORMAL:
	{
		for( uint i = 0;i < This->Buffer.Get_Size(); i++ )
		{
			if( This->Buffer[i] == SE_Net_Message::NET_CHAT )
			{
				SE_Message_Sys::SE_Message nMsg;
				nMsg.Source = (char*)(This->Buffer.Get_Data() + i + 1); i += nMsg.Source.length() + 1;
				nMsg.What = (char*)(This->Buffer.Get_Data() + i + 1); i += nMsg.What.length() + 1;
				nMsg.What_Color.Set( 1, 1, 1 );

				//nMsg.Source = This->Display_Name;

				Engine.GUI_Manager.Message_Log.Add_Message( nMsg );
				break;
			}
		}
	}break;
	}
}*/
