#ifndef SURVIVOR_ENGINE_DIALOGUE
#define SURVIVOR_ENGINE_DIALOGUE

#include "../Core.hpp"
#include "../os/Threader.hpp"
#include "../module/Thread.hpp"
#include <string>
#include <vector>

namespace SE_Game_Dialogue
{
	struct Dialogue_Single_Base
	{
		std::string Text;
		uint Who;
	};

	typedef std::vector< Dialogue_Single_Base* > Dialogue_Full;

	struct Dialogue_Manager
	{
		Dialogue_Manager();

		void Update( dt_precision dt );
		virtual void Draw() = 0; // defined by user

		virtual void Play_Tick_Noise() = 0; // defined by user, in game constructor

		std::string Get_Drawn_Text();// get text to draw

		void Next_Dialogue();

		SE_Thread::Mutex Dialogue_Mutex;
		Dialogue_Full Cur_Dialogue;

		float Dialogue_Speed;
		float Letter_At_F;// for scrolling, can also be sped up
		uint Letter_At;
		bool In_Dialogue, Done_Scrolling;

		void Clean_Dialogue_Full( Dialogue_Full& Diag );
		void Add_Mem_Dialogue_Full( Dialogue_Full& Diag );
	};

	void Register_Module();
}



#endif
