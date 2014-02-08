#include "Particle_System.hpp"	 
using namespace SE_Particle_System;

#include "../utility/Functions.hpp"
#include "../gl/se_gl.hpp"

#include <random>

			 
P_Particle::P_Particle()
{
	this->Duration = 0.f;
}

P_Particle::P_Particle( Vector3f Pos, Vector3f Velo )
{
	this->Duration = 0.f;

	this->Position = Pos;
	this->Velocity = Velo;
}

void P_Particle::Update( float dt )
{
	this->Duration += dt;
										   
	this->Position.x() += this->Velocity.x() * dt;	
	this->Position.y() += this->Velocity.y() * dt;
	this->Position.z() += this->Velocity.z() * dt;

	if( this->Position.y() < 0 )
	{
		this->Position.y() = 0;
		this->Velocity.y() = -this->Velocity.y() * PARTICLE_BOUNCE;
	}
																	   
	this->Velocity.x() -= Sign( this->Velocity.x() ) * PARTICLE_DRAG * dt; 
	this->Velocity.z() -= Sign( this->Velocity.z() ) * PARTICLE_DRAG * dt; 
}


P_Particle_System::P_Particle_System()
{
	this->Running = false;


	this->Particles_Per_Sec = 10;
	this->Add_Particle = 0.f;

	this->Particle_Duration = 5.f;

	this->Start_R = 255;
	this->Start_G = 255; 
	this->Start_B = 255; 
	this->Start_A = 255;
	
	this->End_R = 255; 
	this->End_G = 255; 
	this->End_B = 255; 
	this->End_A = 0;

	this->Start_Size = 0.1f;
	this->End_Size = 0.05f;

	this->Gravity = 0.5f;

	this->Rand_Noise = 0.1f;
}

void P_Particle_System::Update( float dt )
{
	this->m_Mutex.Wait_For_Lock( );//FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for( uint i = 0;i < this->Particles.size(); i++)
	{
		this->Particles[i].Velocity.y() -= this->Gravity * dt * PARTICLE_SPEED;

		this->Particles[i].Update( dt * PARTICLE_SPEED );

		if(this->Particles[i].Duration > this->Particle_Duration)
		{
			this->Particles.erase(this->Particles.begin() + i );
			i--;
		}
	}

	if( this->Running )
	{
		this->Add_Particle += dt * PARTICLE_SPEED;

		if( this->Add_Particle > 1.f/this->Particles_Per_Sec )
		{					
			this->Add_Particle = 0;

			P_Particle nPart( this->Position, this->Velocity );
				  
			nPart.Position.x() += ( rand()%200 - 100 )/100.f * this->Rand_Noise * 1.5f;
			nPart.Position.y() += ( rand()%200 - 100 )/100.f * this->Rand_Noise * 1.5f;
			nPart.Position.z() += ( rand()%200 - 100 )/100.f * this->Rand_Noise * 1.5f;

			nPart.Duration -= ( rand()%200 - 100 )/100.f * this->Rand_Noise * 8;


			this->Particles.push_back( nPart );
		}
	}	   

	this->m_Mutex.Unlock();
}

					   
#include "../S_Engine.hpp"

void P_Particle_System::Draw()
{					
	segl_Disable( GL_TEXTURE_2D );

	this->m_Mutex.Wait_For_Lock( );//FILE_LINE.c_str() );
	this->m_Mutex.Lock();

	for(uint i = 0;i < this->Particles.size(); i++)
	{
		float Perc = this->Particles[i].Duration / this->Particle_Duration;
		glColor4ub( u_char((this->End_R - this->Start_R) * Perc) + this->Start_R, 
				   u_char((this->End_G - this->Start_G) * Perc) + this->Start_G, 
				   u_char((this->End_B - this->Start_B) * Perc) + this->Start_B, 
				   u_char((this->End_A - this->Start_A) * Perc) + this->Start_A);

		float Size = Perc * (this->End_Size - this->Start_Size) + this->Start_Size;
						
		glPointSize( Size * 75.f );
																	 
		glBegin( GL_POINTS );											 
			glVertex3f( this->Particles[i].Position.x()-Size, this->Particles[i].Position.y()-Size, this->Particles[i].Position.z()-Size );
		glEnd();

		/*glVertex2f( this->Particles[i].Position.x-Size, this->Particles[i].Position.y-Size );
		glVertex2f( this->Particles[i].Position.x-Size, this->Particles[i].Position.y+Size );
		glVertex2f( this->Particles[i].Position.x+Size, this->Particles[i].Position.y+Size );
		glVertex2f( this->Particles[i].Position.x+Size, this->Particles[i].Position.y-Size );*/
	}
				
 	this->m_Mutex.Unlock();

	glColor4f( 1,1,1,1 );
}
