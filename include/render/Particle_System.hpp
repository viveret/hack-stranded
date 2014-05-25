#ifndef SURVIVOR_ENGINE_PARTICLE_SYSTEM
#define SURVIVOR_ENGINE_PARTICLE_SYSTEM

#include "../math/Math.hpp"
#include "../Core.hpp"
#include "../module/Thread.hpp"
#include <vector>


#define PARTICLE_DRAG 0.1f
#define PARTICLE_BOUNCE 0.5f
#define PARTICLE_SPEED 0.005f



namespace SE_Particle_System
{
	struct P_Particle
	{
		P_Particle();
		P_Particle( Vector3f Pos, Vector3f Velo );
		void Update( float dt );

		Vector3f Position;
		Vector3f Velocity;

		float Duration;
	};


	struct P_Particle_System
	{
		P_Particle_System();

		void Update( float dt );
		void Draw();

		bool Running;

		uint Particles_Per_Sec;
												  
		u_char Start_R, Start_G, Start_B, Start_A;
		u_char End_R, End_G, End_B, End_A;

		float Start_Size, End_Size;

		Vector3f Position, Velocity;
		float Gravity, Particle_Duration, Rand_Noise;

	private:
		std::vector< P_Particle > Particles;
		SE_Thread::Mutex m_Mutex;

		float Add_Particle;
	};
}




#endif
