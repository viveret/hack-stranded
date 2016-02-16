#ifndef STRANDED_CORE
#define STRANDED_CORE

#include <vector>
#include <SE/physics/Physics.hpp>
#include <SE/utility/Lua.hpp>
#include <SE/gui/GUI.hpp>
//#include "Dialogue.hpp"

namespace Stranded_Lib
{
	struct Grav_Node
	{
		Grav_Node();
		~Grav_Node();
		void Cleanup();

		std::vector< Grav_Node* > Children;

		SE_Physics_Lib::Body_ID m_Obj;
		SE_Physics_Lib::Force_ID m_Force;
		float Radius, Orbit_Speed, Orbit_Location, Mass;
		Vec2 Orbit_Origin;

		uint Tex; // within player, so render
		float Rotate;
		std::string Name;
		Grav_Node* Parent;
	};

	struct Trigger_Spot
	{
		Vec2 Pos;
		float Radius_Sqrd;
		std::string Conv;
	};

<<<<<<< HEAD
/*	struct m_Dialogue_Man: public SE_Game_Dialogue::Dialogue_Manager
=======
	/*struct m_Dialogue_Man: public SE_Game_Dialogue::Dialogue_Manager
>>>>>>> 3071c16e89baa5ddd386dd11b642f44815affce1
	{
		void Draw();
		void Play_Tick_Noise();
	};*/

	extern luaL_Reg Stranded_Lualib[];

	SE_GUI_Lib::Widget_Container* User_Load_Menu( std::string Which );
	void User_GUI_Background();

	void Start_Game();

	void Register_Gravnode( Grav_Node* );
	void Clear_Solar_System(); // Just start fresh
	void Escaped_Solar_System(); // Next time!
}


#endif
