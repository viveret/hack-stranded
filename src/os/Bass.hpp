#ifndef SURVIVOR_ENGINE_BASS_WRAPPER
#define SURVIVOR_ENGINE_BASS_WRAPPER

#include <bass.h> 
#include "../Core.hpp"
#include "../utility/Array.hpp"
#include "../module/Thread.hpp"


#define MAX_PLAYBACK 65535
#define FREQ_DEFAULT 44100
#define MAX_VOLUME 10000

#include <vector>
#include <string>



namespace E_Bass
{
	struct B_Channel
	{
		B_Channel();
		~B_Channel();  
		void Destroy();

		void Update( double dt );

		HCHANNEL Channel;

		float Check_Done;
	};					 

	struct B_Sample
	{
		B_Sample();
		~B_Sample();	
		void Destroy();

		HSAMPLE Sample;
		SE_Raw_Buffer Data;
	};

	struct B_Stream
	{
		B_Stream();
		~B_Stream();	
		void Destroy();

		HSTREAM Stream;					
		SE_Raw_Buffer Data;
	};


	struct B_Bass_Manager
	{
		B_Bass_Manager();
		~B_Bass_Manager();

		bool Running;

		bool Initialize();
		void Clean_Up();

		void Update( double dt );
											  
		uint Load_Sample( std::string File );
		uint Load_Stream( std::string File );

		void Set_Volume( float Perc );
		float Current_Volume;
									 
		void Play_Sample( uint ID );
		void Play_Stream( uint ID );

		uint Error_Sample;

	private:							

		std::vector< B_Channel > Channels;
		std::vector< B_Sample > Samples;
		std::vector< B_Stream > Streams;

		SE_Thread::Mutex m_Mutex;
	};

	std::string BASS_Error_Str( uint Error );
#ifdef _WIN32
	DWORD CALLBACK SE_BASS_Stream_Writer( HSTREAM Handle, void *Buf, DWORD len, void *User );
#endif
}





#endif
