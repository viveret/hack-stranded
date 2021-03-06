#ifndef SURVIVOR_ENGINE_DIALOGUE
#define SURVIVOR_ENGINE_DIALOGUE

#include <SE/Core.hpp>
#include <SE/module/Thread.hpp>
#include <string>
#include <vector>
#include <mutex>

namespace SE_Game_Dialogue
{
	struct Dialogue_Single_Base
	{
		std::string Text;
		uint Who;
	};

	typedef std::vector< Dialogue_Single_Base > Dialogue_Full;
	void Dialogue_File( Dialogue_Full* Ret, const std::string& Name );

	struct Dialogue_Manager
	{
		Dialogue_Manager();

		void Update( dt_precision dt );
		virtual void Draw() = 0; // defined by user

		virtual void Play_Tick_Noise() = 0; // defined by user, in game constructor

		std::string Get_Drawn_Text();// get text to draw

		void Next_Dialogue();
		void End_Dialogue();

<<<<<<< HEAD:src/Core/Dialogue_OLD.hpp
		std::recursive_timed_mutex Dialogue_Mutex;
=======
		std::recursive_mutex Dialogue_Mutex;
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1:src/Core/Dialogue.hpp
		Dialogue_Full Cur_Dialogue;

		float Dialogue_Speed;
		float Letter_At_F;// for scrolling, can also be sped up
		uint Letter_At;
		bool In_Dialogue, Done_Scrolling;

		void Clean_Dialogue_Full( Dialogue_Full& Diag );
		void Add_Mem_Dialogue_Full( Dialogue_Full& Diag );

		void Load( const std::string& File );
	};

	void Register_Module();
}



#endif
