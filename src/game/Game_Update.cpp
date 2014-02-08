#include "../game/Game.hpp"
using namespace E_Game;

#include "../engine/Engine.hpp"

#include <limits>



void G_Game::Update_Base( double dt )
{
#ifdef _TEST
	if( Engine.Is_Paused == false )
		this->Phys_Manager.Update( dt );
#endif
}
