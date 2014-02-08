#ifndef SURVIVOR_ENGINE_CONSOLE
#define SURVIVOR_ENGINE_CONSOLE
									 
#include "../Core.hpp"
#include "../math/Math.hpp"
#include "../os/Threader.hpp"
#include "../module/Thread.hpp"
				   
#include <vector>
#include <string>


namespace S_Console
{
	struct C_Line
	{
		C_Line( std::string Text, Vector3f Color );

		std::string Text;
		Vector3f Color;
	};


	struct C_Console
	{
		C_Console();

		void Initialize();

		void Draw();
		void Update( dt_precision dt );

		float Scroll_Line;// where to draw
		bool Is_Open;
		float Open_Anim;

		std::vector< C_Line > Lines;	
		SE_Thread::Mutex m_Mutex;

		void Add_Line( std::string Line );
		void Add_Line( std::string Line, Vector3f Color );
	};
}




#endif
