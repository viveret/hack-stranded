#include "../os/Bass.hpp"
using namespace E_Bass;

#include "../engine/Engine.hpp"
#include "../utility/Functions.hpp"
#include "../app/Window.hpp"



B_Channel::B_Channel()
{
	this->Check_Done = 0.f;
	this->Channel = 0;
}
				
B_Channel::~B_Channel()
{
	//this->Destroy();
}

void B_Channel::Destroy()
{
	BASS_ChannelStop( this->Channel );

#ifdef _TEST
	if( Engine.BASS_Assert("Stopped Channel: ", 1) )
		return;
#endif
}

B_Sample::B_Sample()
{
	this->Sample = 0;
}

B_Sample::~B_Sample()
{
	//
}

void B_Sample::Destroy()
{
	BASS_SampleFree( this->Sample );

	this->Sample = 0;

	this->Data.Delete();
}

B_Stream::B_Stream()
{
	this->Stream = 0;
}

B_Stream::~B_Stream()
{
	//
}

void B_Stream::Destroy()
{					   
	BASS_StreamFree( this->Stream );
	this->Stream = 0;

	this->Data.Delete();
}




B_Bass_Manager::B_Bass_Manager()
{
	this->Running = false;
	this->Current_Volume = 0.f;
	this->Error_Sample = 0;
}

B_Bass_Manager::~B_Bass_Manager()
{
	this->Clean_Up();
}

bool B_Bass_Manager::Initialize()
{
	if(!
#ifdef _WIN32
	BASS_Init( -1, FREQ_DEFAULT, BASS_DEVICE_3D, SE_Window::Get_HWND(), NULL )
#else
	BASS_Init( -1, FREQ_DEFAULT, BASS_DEVICE_3D, NULL, NULL )//BASS_Init(-1,44100,0,NULL,NULL)//
#endif//&Engine.Application.Window.m_root
	)
	{
#ifdef _TEST
		Engine.Register_Error(S_Error( "Could not initialize BASS", 3));
#endif
		return false;
	}

	this->Running = true;

	//BASS_SetConfig( BASS_CONFIG_UPDATETHREADS, 2 );// only 1 update thread

	this->Error_Sample = this->Load_Sample( "engine/error.wav" );

#ifdef _TEST
	Engine.Log( "Initialized BASS Manager\n" );
#endif

	return true;
}

void B_Bass_Manager::Clean_Up()
{
	for( uint i = 0;i < this->Samples.size(); i++ )
		this->Samples[i].Destroy();

	for( uint i = 0;i < this->Streams.size(); i++ )
		this->Streams[i].Destroy();

	this->Samples.clear();
	this->Streams.clear();

	if( this->Running )
	{
		BASS_Free();
		this->Running = false;
#ifdef _TEST
		if( Engine.BASS_Assert("Clean Up", 3) )
			return;
#endif
	}
}

void B_Bass_Manager::Update( double dt )
{					
	this->m_Mutex.Wait_For_Lock();
	this->m_Mutex.Lock();

#ifdef BASS_NO_DEL
	for(uint i = 0;i < this->Channels.size(); i++)
	{	
		if( BASS_ChannelIsActive( this->Channels[i].Channel ) == BASS_ACTIVE_STOPPED )
		{						
			this->Channels[i].Destroy();
			this->Channels.erase( this->Channels.begin() + i ); 
			i--;
		}
	}  

	for(uint i = 0;i < this->Streams.size(); i++)
	{	
		if( BASS_ChannelIsActive( this->Streams[i].Stream ) == BASS_ACTIVE_STOPPED )
		{						
			this->Streams[i].Destroy();
			this->Streams.erase( this->Streams.begin() + i ); 
			i--;
		}
	}  
#endif

	this->m_Mutex.Unlock();
}				
				
uint B_Bass_Manager::Load_Sample( std::string File )
{
	B_Sample nSample;


	nSample.Data;
#ifdef _TEST
	= Engine.Application.Input.Load_File( File );
#endif
	nSample.Sample = BASS_SampleLoad( TRUE, nSample.Data.Data(), 0, nSample.Data.Size(), 1, 0 );//MAX_PLAYBACK, BASS_SAMPLE_3D );
#ifdef _TEST
	if( Engine.BASS_Assert("Load Sample: " + File, 1 ) )
		return 0;
#endif
	this->Samples.push_back( nSample );
	return this->Samples.size() - 1;
}

uint B_Bass_Manager::Load_Stream( std::string File )
{					
	B_Stream nStream;
	//this->Streams[ this->Streams.size() - 1 ].m_File.Open( File );
	//this->Streams[ this->Streams.size() - 1 ].Stream = BASS_StreamCreate(44100, 2, 0, &SE_BASS_Stream_Writer,
														//&this->Streams[this->Streams.size()-1]);
	
	nStream.Data;
#ifdef _TEST
	= Engine.Application.Input.Load_File( File );
#endif
	nStream.Stream = BASS_StreamCreateFile( TRUE, nStream.Data.Data(), 0, nStream.Data.Size(), 0 );
#ifdef _TEST
	if( Engine.BASS_Assert("Stream Create: " + File, 1 ) )
		return 0;
#endif
																				
	this->Streams.push_back( nStream );
	return this->Streams.size() - 1;
}

void B_Bass_Manager::Set_Volume( float Perc )
{
	this->Current_Volume = Perc;								

	BASS_SetConfig( BASS_CONFIG_GVOL_SAMPLE, (DWORD)(Perc * MAX_VOLUME) );
	BASS_SetConfig( BASS_CONFIG_GVOL_MUSIC, (DWORD)(Perc * MAX_VOLUME) );
	BASS_SetConfig( BASS_CONFIG_GVOL_STREAM, (DWORD)(Perc * MAX_VOLUME) );
}		

void B_Bass_Manager::Play_Sample( uint ID )
{
#ifdef _TEST
	if( E_Threads::Get_Thread() != Engine.Thread_Engine )
	{		
		SE_Events::Event_uint* nEvent = new SE_Events::Event_uint;
		nEvent->Type = SE_Events::Event_Types::TYPE_BASS;
		nEvent->Code = SE_Events::Events::EVENT_PLAY_SAMPLE;
		nEvent->Argument = ID;

		Engine.Event_Manager.Add_Event( nEvent, "BASS Play Sample" );	

		return;
	}

	if( ID < this->Samples.size() )
	{
		B_Channel nChan;
		nChan.Channel = BASS_SampleGetChannel( this->Samples[ID].Sample, FALSE); // get a sample channel
		if( Engine.BASS_Assert("Sample Get Channel: " + To_Str(ID), 1) )
			return;		

		BASS_ChannelPlay( nChan.Channel, FALSE ); // play it
		if( Engine.BASS_Assert("Played Sample: " + To_Str(ID), 1) )
			return;					 

		this->Channels.push_back( nChan );
	}
	else
		Engine.Register_Error(S_Error("BASS Sample out of range: " + To_Str(ID), 1) );
#endif
}

void B_Bass_Manager::Play_Stream( uint ID )
{
#ifdef _TEST
	if( E_Threads::Get_Thread() != Engine.Thread_Engine )
	{		
		SE_Events::Event_uint* nEvent = new SE_Events::Event_uint;
		nEvent->Type = SE_Events::Event_Types::TYPE_BASS;
		nEvent->Code = SE_Events::Events::EVENT_PLAY_STREAM;
		nEvent->Argument = ID;

		Engine.Event_Manager.Add_Event( nEvent, "BASS Play Stream" );	

		return;
	}

	if( ID < this->Streams.size() )
	{
		/*B_Channel nChan;
		nChan.Channel = BASS_StreamGetChannel( this->Samples[ID].Sample, FALSE); // get a sample channel
		if( Engine.BASS_Assert("Sample Get Channel: " + To_Str(ID), 1) )
			return;	*/	

		BASS_ChannelPlay( this->Streams[ID].Stream, FALSE ); // play it
		if( Engine.BASS_Assert("Played Sample: " + To_Str(ID), 1) )
			return;
	}
	else
		Engine.Register_Error(S_Error("BASS Sample out of range: " + To_Str(ID), 1) );
#endif
}

#ifdef _WIN32
DWORD CALLBACK E_Bass::SE_BASS_Stream_Writer( HSTREAM Handle, void *Buf, DWORD len, void *User )
{
	B_Stream* mStream = (B_Stream*)(User);
	if( mStream == NULL )
		return 0;

//	mStream->m_File.Read( (char*)Buf, (uint)len );

//	if( mStream->m_File.is_EOF() )
		return BASS_STREAMPROC_END;
	//else
		return len;
}
#endif








std::string BASS_Error_List[] = 						 
{
"BASS_OK",				//  0
"BASS_ERROR_MEM",		//  1
"BASS_ERROR_FILEOPEN",  //  2
"BASS_ERROR_DRIVER",	//  3
"BASS_ERROR_BUFLOST",	//  4
"BASS_ERROR_HANDLE",	//  5
"BASS_ERROR_FORMAT",	//  6
"BASS_ERROR_POSITION",  //  7
"BASS_ERROR_INIT",		//  8
"BASS_ERROR_START",		//  9
"",						// 10
"",						// 11
"",						// 12
"",						// 13
"BASS_ERROR_ALREADY",  	// 14
"",						// 15
"",						// 16
"",						// 17
"BASS_ERROR_NOCHAN",  	// 18
"BASS_ERROR_ILLTYPE",  	// 19
"BASS_ERROR_ILLPARAM",  // 20
"BASS_ERROR_NO3D",  	// 21
"BASS_ERROR_NOEAX",  	// 22
"BASS_ERROR_DEVICE",  	// 23
"BASS_ERROR_NOPLAY",  	// 24
"BASS_ERROR_FREQ",  	// 25
"",						// 26
"BASS_ERROR_NOTFILE",	// 27
"",						// 28
"BASS_ERROR_NOHW",		// 29
"",						// 30
"BASS_ERROR_EMPTY",		// 31
"BASS_ERROR_NONET",		// 32
"BASS_ERROR_CREATE",	// 33
"BASS_ERROR_NOFX",		// 34
"",						// 35
"",						// 36
"BASS_ERROR_NOTAVAIL",  // 37
"BASS_ERROR_DECODE",	// 38
"BASS_ERROR_DX",		// 39
"BASS_ERROR_TIMEOUT",	// 40
"BASS_ERROR_FILEFORM",  // 41
"BASS_ERROR_SPEAKER",	// 42
"BASS_ERROR_VERSION",	// 43
"BASS_ERROR_CODEC",		// 44
"BASS_ERROR_ENDED",		// 45
"BASS_ERROR_BUSY"		// 46
};

std::string E_Bass::BASS_Error_Str( uint Error )
{
	if( Error > 46 )
		return "BASS_ERROR_UNKNOWN";
	else
		return BASS_Error_List[ Error ];
}
