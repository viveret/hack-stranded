#include "Message_Log.hpp"
using namespace SE_Message_Sys;

#include "../engine/Engine.hpp"

#include "../utility/Functions.hpp"
#include "../utility/Draw_Utility.hpp"

#include "../gl/se_gl.hpp"


SE_Message::SE_Message()
{
	this->Timer = 0.f;
}



SE_Message_Event::SE_Message_Event() : SE_Events::Event_Base( NULL, 0 )
{

}

SE_Events::Event_Base* SE_Message_Event::Clone()
{
	SE_Message_Event* nEv = new SE_Message_Event;
	*nEv = *this;

	return nEv;
}

void SE_Message_Event::Set( SE_Message& Msg )
{
	this->Source = Msg.Source;
	this->Source_Color = Msg.Source_Color;

	this->What = Msg.What;
	this->What_Color = Msg.What_Color;
}



SE_Message_Manager::SE_Message_Manager()
{
	this->Show_Delay = 0.3f;
	this->Line_Height = 7;
	this->Max_W = 50;
	this->Remove_Style = Remove_Styles::STYLE_NONE;
}

void SE_Message_Manager::Update( double dt )
{
	this->m_Mutex.Wait_For_Lock( );// FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uchar i = 0;i < this->Messages.size(); i++ )
	{
		//this->Messages[i].Timer -= dt;

		if( this->Messages[i].Timer <= 0 )
		{
			this->Messages.erase( this->Messages.begin() + i );
			i--;
		}
	}

	this->m_Mutex.Unlock();
}

void SE_Message_Manager::Draw()
{
	glPushMatrix();

	glTranslatef( 100, 100, 0 );

	this->m_Mutex.Wait_For_Lock( );// FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uchar i = 0;i < this->Messages.size() && i < this->Line_Height; i++ )
	{
		Draw_Utility::String_Draw( this->Messages[i].Source, 0, i*16, this->Messages[i].Source_Color.r(), this->Messages[i].Source_Color.g(), this->Messages[i].Source_Color.b(), 1 );
#ifdef _TEST
		Vector2f Source_Size = Engine.Resources.Font_Manager.Get_Str_Size( this->Messages[i].Source );
		Draw_Utility::String_Draw( this->Messages[i].What, Source_Size.x, i*16, this->Messages[i].What_Color.x, this->Messages[i].What_Color.y, this->Messages[i].What_Color.z, 1 );
#endif
	}

	this->m_Mutex.Unlock();

	glPopMatrix();
}

void SE_Message_Manager::Print_Message( SE_Message& nMsg ) // from local player
{
#ifdef _TEST
	if( Engine.Net_Manager == NULL )
	{
		this->Add_Message( nMsg );
	}
	else if( Engine.Net_Manager->Is_Host == false )
	{
		SE_Message_Event* nEv = new SE_Message_Event;
		nEv->Code = SE_Net_Message::NET_CHAT;
		nEv->Set( nMsg );

		((SE_Network::SE_Net_Manager_CLIENT*)Engine.Net_Manager)->Main_Socket.Add_Event( nEv );

		// host will send back message
	}
	else // is host
	{
		SE_Message_Event* nEv = new SE_Message_Event;
		nEv->Code = SE_Net_Message::NET_CHAT;
		nEv->Set( nMsg );

		((SE_Network::SE_Net_Manager_HOST*)Engine.Net_Manager)->Global_Event( nEv );

		this->Add_Message( nMsg );
	}
#endif
}

void SE_Message_Manager::Add_Message( SE_Message& nMsg )
{
	if( nMsg.What.length() == 0 ) return;

	if( nMsg.Source.length() == 0 )
		nMsg.Source = "[N/A]";
	else
		this->Format_Message( &nMsg );

	nMsg.Timer = this->Show_Delay;

	this->m_Mutex.Wait_For_Lock( ); // FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	this->Messages.push_back( nMsg );

	this->m_Mutex.Unlock();

	sePrintf( SEPRINT_INFO, "%s%s\n", nMsg.Source.c_str(), nMsg.What.c_str() );
}

void SE_Message_Manager::Format_Message( SE_Message* Msg )
{
	Msg->Source = "[" + Msg->Source + "]: ";
}
