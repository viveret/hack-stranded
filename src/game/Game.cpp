#include "../game/Game.hpp"
using namespace E_Game;

//#include "GL/glew.hpp"
//#include "GL/gl.hpp"

#include "../engine/Engine.hpp"


G_Game::G_Game()
{	   
}

G_Game::~G_Game()
{
	this->Destroy_Base();
}		 

bool G_Game::Initialize_Base( bool Make_2D )
{
#ifdef _TEST
	if( this->Phys_Manager.Initialize( Make_2D ) == false )
		return false;
#endif
	
	return true;
}

void G_Game::Destroy_Base()
{
#ifdef _TEST
	this->Phys_Manager.Cleanup();
#endif
}
