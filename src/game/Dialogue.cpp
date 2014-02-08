#include "Dialogue.hpp"
using namespace SE_Game_Dialogue;

#include "../module/Module.hpp"

namespace SE_Game_Dialogue
{
	SE_Module_ID m_Mod;

	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
}

Dialogue_Manager::Dialogue_Manager()
{
	this->Dialogue_Speed = 0.01f;

	this->Letter_At = 0;	
	this->Letter_At_F = 0.f;
	this->Done_Scrolling = false;

	this->In_Dialogue = false;

//	this->Play_Tick_Noise = NULL;
}

#include "../engine/Engine.hpp"


void Dialogue_Manager::Update( dt_precision dt )
{				
	this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	this->Dialogue_Mutex.Lock();

	if( this->Cur_Dialogue.size() > 0 )
	{
		if( this->Done_Scrolling == false )
		{
			this->Letter_At_F += this->Dialogue_Speed * dt;
			if( this->Letter_At_F > this->Dialogue_Speed )
			{
				this->Letter_At_F = 0.f;
				this->Letter_At++;

//				if( this->Play_Tick_Noise )
					this->Play_Tick_Noise();

				if( this->Letter_At > this->Cur_Dialogue[0]->Text.length() )
					this->Done_Scrolling = true;
			}
		}
	}

	this->Dialogue_Mutex.Unlock();
}

std::string Dialogue_Manager::Get_Drawn_Text()
{		
	if( this->Cur_Dialogue.size() > 0 )
	{					
		this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->Dialogue_Mutex.Lock();


		std::string Text_Amount_Draw;
		uint Text_Amount = this->Letter_At;

		Text_Amount_Draw = this->Cur_Dialogue[0]->Text.substr( 0, Text_Amount );
								  

		this->Dialogue_Mutex.Unlock();
		return Text_Amount_Draw;
	}

	return std::string();
}


void Dialogue_Manager::Next_Dialogue()
{			  
	this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	this->Dialogue_Mutex.Lock();

	this->Cur_Dialogue.erase( this->Cur_Dialogue.begin() );

	if( this->Cur_Dialogue.size() == 0 )// done with conv
		this->In_Dialogue = false;

	this->Dialogue_Mutex.Lock();
								 
	this->Letter_At = 0;
	this->Done_Scrolling = false;
}
						
void Dialogue_Manager::Clean_Dialogue_Full( Dialogue_Full& Diag )
{
	for( uint i = 0;i < Diag.size(); i++ )
	{
#ifdef _TEST
		Engine.Mem_Remove( Diag[i] );
#endif
		delete Diag[i];
	}
}

void Dialogue_Manager::Add_Mem_Dialogue_Full( Dialogue_Full& Diag )
{		 
	for( uint i = 0;i < Diag.size(); i++ )
	{
#ifdef _TEST
		Engine.Mem_Add( Diag[i], "dialogue single" );
#endif
	}
}

uint SE_Game_Dialogue::Initialize( const std::vector<std::string>& mArgs )
{
	return SE_SUCCESS;
}

uint SE_Game_Dialogue::Cleanup()
{
	return SE_SUCCESS;
}


void SE_Game_Dialogue::Register_Module()
{
	m_Mod = S_Engine::Generate_Module( "stranded" );
	S_Engine::Register_Module_Engine( m_Mod, S_Engine::Reg_Mod_Time::MED_LEVEL + 60, Initialize, Cleanup );
}
