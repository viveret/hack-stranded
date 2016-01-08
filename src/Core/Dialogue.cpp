#include "Dialogue.hpp"
using namespace SE_Game_Dialogue;

#include <SE/module/Module.hpp>
#include <SE/Filesys/Filesystem.hpp>
#include <SE/Logger/Logger.hpp>

namespace SE_Game_Dialogue
{
	void Initialize();
	void Cleanup();
}using namespace SE_Game_Dialogue;

namespace Stranded_Lib
{
	extern SE_Log_Interface* seLog;
	extern SE_Filesys_Interface* SE_Filesys;
}using namespace Stranded_Lib;

Dialogue_Manager::Dialogue_Manager()
{
	this->Dialogue_Speed = 0.01f;

	this->Letter_At = 0;	
	this->Letter_At_F = 0.f;
	this->Done_Scrolling = false;

	this->In_Dialogue = false;

//	this->Play_Tick_Noise = NULL;
}

void Dialogue_Manager::Update( dt_precision dt )
{			
	if( this->Cur_Dialogue.size() == 0 )
		return;
	
//	this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
	this->Dialogue_Mutex.lock();

	if( this->Done_Scrolling == false )
	{
		this->Letter_At_F += this->Dialogue_Speed * dt;
		if( this->Letter_At_F > this->Dialogue_Speed )
		{
			this->Letter_At_F = 0.f;
			this->Letter_At++;

//			if( this->Play_Tick_Noise )
				this->Play_Tick_Noise();

			if( this->Letter_At > this->Cur_Dialogue[0].Text.length() )
				this->Done_Scrolling = true;
		}
	}

	this->Dialogue_Mutex.unlock();
}

std::string Dialogue_Manager::Get_Drawn_Text()
{		
	if( this->Cur_Dialogue.size() > 0 )
	{					
//		this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->Dialogue_Mutex.lock();


		std::string Text_Amount_Draw;
		uint Text_Amount = this->Letter_At;

		Text_Amount_Draw = this->Cur_Dialogue[0].Text.substr( 0, Text_Amount );
								  

		this->Dialogue_Mutex.unlock();
		return Text_Amount_Draw;
	}

	return std::string();
}


void Dialogue_Manager::Next_Dialogue()
{
	if( this->Cur_Dialogue.size() > 0 )
	{
//		this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->Dialogue_Mutex.lock();

		this->Cur_Dialogue.erase( this->Cur_Dialogue.begin() );
		this->Dialogue_Mutex.unlock();

		if( this->Cur_Dialogue.size() == 0 )// done with conv
			this->In_Dialogue = false;
	}
								 
	this->Letter_At = 0;
	this->Done_Scrolling = false;
}

void Dialogue_Manager::End_Dialogue()
{
	if( this->Cur_Dialogue.size() > 0 )
	{
//		this->Dialogue_Mutex.Wait_For_Lock( ); // __FUNCTION__ );
		this->Dialogue_Mutex.lock();

		this->Cur_Dialogue.clear();
		this->Dialogue_Mutex.unlock();

		this->In_Dialogue = false;
	}
								 
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
//		delete Diag[i];
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

void Dialogue_Manager::Load( const std::string& File )
{
	Dialogue_File( &this->Cur_Dialogue, File );
	this->In_Dialogue = true;
}

void SE_Game_Dialogue::Dialogue_File( Dialogue_Full* Ret, const std::string& Name )
{
	SE_Raw_Buffer Data = SE_Filesys->File_Load_Instant( Name.c_str() );

	if( Data.Size() == 0 )
	{
		seLog->Printf( "Dialogue", SELOG_ERROR, "$(e)Cannot Make Conv W/ Data\n" );
		return;
	}

	bool In_Text = false;

	Dialogue_Single_Base nConv;
	std::string Temp_Str;

	for( uint l = 0;l < Data.Size(); l++ )
	{
		if( In_Text == false )
		{
			if( Data[l] == '\"' )
				In_Text = true;
			else if( Data[l] == ':' )
			{
				nConv.Who = atoi( Temp_Str.c_str() );
				Temp_Str.clear();
			}
			else
				Temp_Str += Data[l];
		}
		else
		{
			if( Data[l] == '\"' )
			{
				nConv.Text = Temp_Str;
				In_Text = false;
				Ret->push_back( nConv );

				Temp_Str.clear();
			}
			else
				Temp_Str += Data[l];
		}
	}

	Data.Delete();	

	seLog->Printf( "Dialogue", SELOG_DEBUG - 10, "$(d)Successfully loaded dialogue (%d)\n",
			Ret->size() );
}

void SE_Game_Dialogue::Initialize()
{
	//return SE_SUCCESS;
}

void SE_Game_Dialogue::Cleanup()
{
	//return SE_SUCCESS;
}


void SE_Game_Dialogue::Register_Module()
{
//	m_Mod = S_Engine::Generate_Module( "stranded" );
	S_Engine::Register_Module_Engine( S_Engine::Reg_Mod_Time::MED_LEVEL + 60, Initialize, Cleanup );
}
