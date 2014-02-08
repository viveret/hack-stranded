#ifndef SURVIVOR_ENGINE_MENU
#define SURVIVOR_ENGINE_MENU
					
#include <vector>
#include "../Core.hpp"
#include "../math/Math.hpp"
#include "../utility/Array.hpp"

#include "../menu/Menu_Buttons.hpp"
#include "../gui/Button.hpp"


namespace SE_Menu
{
	struct Menu_Base
	{
		Menu_Base();
		~Menu_Base();

		void Clean_Up_Base();
		virtual void Clean_Up();

		void Update_Base();
		void Draw_Base();

		virtual void Update();
		virtual void Draw();

		void Make_Buttons(u_char Num);
		void Add_Buttons_Mem();// add to engine memory manager

		SE_Array< SE_GUI::Button_Base* > Buttons;

		std::string Title;
	};

	// Set user load menu function
	void Set_Load_Menu( Menu_Base* (*Func)( std::string ) );

	void Register_Module();
}
	

#endif
