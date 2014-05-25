#include "Stranded_Core.hpp"
using namespace Stranded_Lib;

namespace Stranded_Lib
{
	extern SE_Physics_Interface* SE_Physics;
}


Grav_Node::Grav_Node()
{
	this->m_Obj = NULL;
	this->m_Force = NULL;

	this->Radius = this->Orbit_Speed = this->Orbit_Location = this->Mass = 0.f;
	this->Rotate = rand() % 360;
	this->Parent = NULL;
	this->Tex = 0;
}

Grav_Node::~Grav_Node()
{
	this->Cleanup();
}

void Grav_Node::Cleanup()
{
	while( this->Children.size() > 0 )
	{
		delete this->Children[0];
		this->Children.erase( this->Children.begin() );
	}

	if( this->m_Force )
	{
		SE_Physics->Force_Delete( this->m_Force );
		this->m_Force = NULL;
	}

	if( this->m_Obj )
	{
		SE_Physics->Body->Delete_Body( this->m_Obj );
		this->m_Obj = NULL;
	}

	this->Parent = NULL;
}
