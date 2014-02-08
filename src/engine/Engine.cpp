/*#include "../engine/Engine.h"
#include "../utility/Functions.h"


#define SE_INCLUDE_LIBS
#include "../S_Engine.h"*/
#ifdef _TEST

std::string S_Engine_Version_Desc::As_String()
{
	char Buf[256]; memset( Buf, 0, 256 );

	sprintf( Buf, "%s (%s) Pre-Alpha, %s : %s, %s  %s", this->Build_Name.c_str(), this->Build_Target_OS.c_str(), this->OS.c_str(),
														this->OS_Version.c_str(), this->Build_Date.c_str(), this->Build_Time.c_str() );

	return std::string( Buf );

//return std::string( this->Build_Name + "Pre-Alpha, ") + this->OS + ":" + this->OS_Version + ", " + this->Build_Data + ", " this->Build_Time;
}


S_Engine::S_Engine()
{
	this->Game = NULL;
	this->Net_Manager = NULL;

	this->Is_Running = true;
	this->In_Game = false;
	this->Is_Paused = false;

	this->Ready_Exit = false;

	this->m_Version.Build_Target_OS = SE_OS;
	this->m_Version.Build_Date = __DATE__;
	this->m_Version.Build_Time = __TIME__;

	this->Log_No_Color = false;
}

S_Engine::~S_Engine()
{
	if( this->Is_Running )// probablu not, but to be safe...
	{
		this->Is_Running = false;
		this->Exit();
	}
}

#include <sys/time.h>
#include <sys/utsname.h>
void S_Engine::Start( Engine_Descriptor& nDesc, char argc, char** argv )
{
	Timer Startup_Timer;
	Startup_Timer.Start();

	this->m_Version.Build_Name = nDesc.Name;

	// Get system info
#ifdef _WIN32
#else
	utsname Get_OS_Buf;
	uname( &Get_OS_Buf );

	this->m_Version.OS.append( Get_OS_Buf.version );
	this->m_Version.OS_Version.append( Get_OS_Buf.release );

	system( "cat /proc/version > data/log_system.log" );
	system( "cat /proc/vmstat >> data/log_system.log" );
	system( "cat /proc/stat >> data/log_system.log" );
	system( "cat /proc/misc >> data/log_system.log" );
	system( "cat /proc/loadavg >> data/log_system.log" );
	system( "cat /proc/meminfo >> data/log_system.log" );
	system( "lscpu >> data/log_system.log" );
	system( "cat /proc/cpuinfo >> data/log_system.log" );
#endif

	this->Start_Handle_Args( argc, argv );

//	this->Printf( "CWD: \"%s\"\n", Log_Type::TYPE_DBUG, this->Get_CWD().c_str() );
	// SET OS SPECIFIC APPLICATION IN S_ENGINE();

#ifdef _WIN32
	this->Thread_Engine = GetCurrentThreadId();
#else
	this->Thread_Engine = boost::this_thread::get_id();
#endif


	this->Event_Manager.Draw_Thread.Processer = this->Event_Draw_Processor;
	this->Event_Manager.Logic_Thread.Processer = this->Event_Logic_Processor;
	this->Event_Manager.Engine_Thread.Processer = this->Event_Main_Processor;


	// set engine modules
	this->Resources.Stuff = nDesc.nRes;
	this->Mem_Add( nDesc.nRes, "resources" );

	this->Menu_Load_Pause = nDesc.Pause_Name;

#ifdef _WIN32
	this->Application.Initialize( nDesc.hInstance );
#else
	this->Application.Initialize();
#endif

	this->Printf( "Args:\n", Log_Type::TYPE_DBUG );
	for( uchar i = 0;i < argc; i++ )
		this->Printf("%s\n", Log_Type::TYPE_DBUG, argv[i] );

	this->Log( this->Get_Version()->As_String() + "\n", Log_Type::TYPE_INFO );

	if( nDesc.Single_Inst )// only 1 instance
	{
		if( this->Start_Is_Single_Instance() == false )// not first
		{
			this->Log( "Not first instance, exiting.\n", Log_Type::TYPE_EROR );
			this->Shutdown();
		}
	}

	this->Input.Initialize( nDesc.Input_Event_Handler );

	this->Get_Options();

	E_Application::AP_Win_Desc nWin;
	nWin.Name = "Survivor Engine Test";
	nWin.x = 0; nWin.y = 0; nWin.w = 640; nWin.h = 480; nWin.Fullscreen = false;

	this->Application.Window.Create_Window( nWin );
	E_Application::Set_OpenGL_Settings();
	this->Log( "Set OpenGL Settings\n" );

	this->Content.Mesh_Manager.Initialize();

	this->Resources.Font_Manager.Initialize();
	this->SE_Font = DTX::dtx_open_font( (std::string("fonts/") + nDesc.Def_Font).c_str(), nDesc.Def_Font_Size );
	//this->Resources.Font_Manager.Open_Font((std::string("stuff/fonts/") + nDesc.Def_Font).c_str(), nDesc.Def_Font_Size);

	this->Lua_Handler.Initialize();

	this->Bass_Manager.Initialize();

	//this->Resources.Initialize();
	this->Content.Initialize();
//	this->Resources.Initialize();


	// thread caps
	this->E_Timer.Frame_Time = 1000.f/ENGINE_TPS/1000.f;
	this->Thread_Logic.m_Timer.Frame_Time = 1000.f/LOGIC_TPS/1000.f;
	this->Thread_Draw.m_Timer.Frame_Time = 1000.f/DRAW_TPS/1000.f;

#ifdef _WIN32
	wglMakeCurrent(NULL,NULL);

	this->Thread_Logic.hThread = CreateThread(NULL,0,E_Threads::Logic_Thread,(void*)NULL,0,NULL);
	this->Thread_Draw.hThread = CreateThread(NULL,0,E_Threads::Draw_Thread,(void*)NULL,0,NULL);
	//this->Thread_Input.hThread = CreateThread(NULL,0,E_Threads::Input_Thread,(void*)NULL,0,NULL);
#else
    //glXMakeCurrent(this->Application.Window.m_dpy, None, NULL);

    this->Thread_Logic.Start( E_Threads::Logic_Thread );
    //this->Thread_Draw.hThread = boost::thread( E_Threads::Draw_Thread );
#endif



#ifdef USE_XBOX
	this->Thread_XBox.hThread = CreateThread(NULL,0,E_Threads::XBox_Thread,(void*)NULL,0,NULL);
#endif

	this->Log( "Started Threads\n" );

	this->E_Timer.Start();

	//AttachThreadInput(this->Thread_Engine,this->Thread_Logic.t_ID,TRUE);


	SE_GUI::GUI_Attatches::GUI_Attatch_CString* GUI_Bennett_S = new SE_GUI::GUI_Attatches::GUI_Attatch_CString;
	GUI_Bennett_S->To = "S_ENGINE BY BENNETT STEELE, DO NOT DISTRIBUTE";
	GUI_Bennett_S->Position.Set( 0, 0 );GUI_Bennett_S->Gravity = SE_GUI::GUI_Gravity::Left_Bottom;
	GUI_Bennett_S->Size = this->Resources.Font_Manager.Get_Str_Size( GUI_Bennett_S->To );//GUI_Bennett_S->Size.x += 4.f;
	this->GUI_Manager.GUI_Add( GUI_Bennett_S );


	// display time it took to start up
	Startup_Timer.End();
	this->Printf( "Initialized Engine, Time: %g s\n\n", Log_Type::TYPE_INFO, Startup_Timer.Elapsed_Time );
}

void S_Engine::Start_Handle_Args( char argc, char** argv )
{
	for( uchar i = 0;i < argc; i++ )
	{
		if( strcmp(argv[i], "--no-color") == 0 )
			this->Log_No_Color = true;

		else if( strcmp(argv[i], "--color") == 0 )
			this->Log_No_Color = false;

		else
			printf( "Arg \'%s\' not recognized\n", argv[i] );
	}
}

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
bool S_Engine::Start_Is_Single_Instance()// check if already running, single instance
{
	int fdlock;

	flock fl;

	fl.l_type = F_WRLCK;
	fl.l_whence = SEEK_SET;
	fl.l_start = 0;
	fl.l_len = 1;

	if((fdlock = open("oneproc.lock", O_WRONLY|O_CREAT, 0666)) == -1)
	return false;

	if(fcntl(fdlock, F_SETLK, &fl) == -1)
	return false;

	return true;
}

void S_Engine::Shutdown()
{
	if(this->Is_Running)
	{
		this->Is_Running = false;
	}
}

void S_Engine::Exit()
{
	if( this->Is_Running == false )
	{
		if( this->Net_Manager )
		{
			this->Net_Manager->Cleanup();
		}
		//this->Thread_XBox.End();
		//this->Thread_Input.End();
		this->Thread_Draw.End();
		this->Thread_Logic.End();

		while( this->Thread_Draw.Running || this->Thread_Logic.Running );//|| this->Thread_Input.Running );

#ifdef _WIN32
		wglMakeCurrent( this->Application.Window.hDC, this->Application.Window.hDraw_RC );
#else
		this->Win_X11_Events();
#endif

		this->Log( "Successfully Ended Threads\n" );

		this->GUI_Manager.Cleanup();
		this->Stream_Manager.Cleanup();

		//stuff
		if( this->Game )
		{
			this->Game->Destroy();
			this->Mem_Remove( this->Game );

			delete this->Game;
			this->Game = NULL;
		}

		this->Lua_Handler.Destroy();


		this->Menu.Ready_Draw = true;
		this->Menu.Ready_Logic = true;
		this->Menu.Cleanup();

		this->Content.Cleanup();
		this->Bass_Manager.Clean_Up();


		/* finish events */
		this->Event_Manager.Engine_Thread.Update();

		this->Input.Cleanup();

		printf( "About to shutdown app... " );
		this->Application.Shutdown();
		printf( " Success\n" );


		this->Mem_Remove( this->Resources.Stuff );
		delete this->Resources.Stuff;
		this->Resources.Stuff = NULL;

		this->Mem_List();

		this->Log( "Finished S_Engine\n" );
	}
}

void S_Engine::Register_Error(S_Error nError)
{
	if(nError.Level>0)//is somewhat important
	{
		std::string Error_Rep; uint L_Type = 0;
		switch(nError.Level)
		{
		case 1:Error_Rep="Warning";L_Type = Log_Type::TYPE_WARN;break;
		case 2:Error_Rep="Minor Error";L_Type = Log_Type::TYPE_EROR;break;
		case 3:Error_Rep="Major Error";L_Type = Log_Type::TYPE_EROR;break;
		case 4:Error_Rep="Alert";L_Type = Log_Type::TYPE_WARN;break;
		case 5:Error_Rep="Debug Message";L_Type = Log_Type::TYPE_DBUG;break;
		}
		Error_Rep+= ": " + nError.Message + "\n";
		this->Log( Error_Rep, L_Type );

		if(nError.Level==3)
		{
#ifdef _WIN32
			MessageBox(NULL,nError.Message.c_str(),"CRITICAL ERROR",MB_OK | MB_ICONINFORMATION);
#else
			printf("ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR ERROR\n");
#endif
			if( this->Is_Running )
				this->Shutdown();
		}
	}
}

#ifdef _WIN32
bool S_Engine::Win32_Assert(std::string Info, u_char Level)// 0 = none, 1 = log, 2 = don't continue, 3 = shutdown
{
	DWORD n_Error = GetLastError();
	LPSTR lpMsgBuf=NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,n_Error,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR)&lpMsgBuf,0,NULL);

	std::stringstream Str_Err;
	Str_Err<<n_Error<<'-'<<lpMsgBuf;

	Info = "Win32 " + Info + ": " + Str_Err.str();

	LocalFree( lpMsgBuf );

	if(n_Error == ERROR_SUCCESS)
	{
		return false;
	}
	this->Register_Error(S_Error( Info, Level) );
	return true;
}

bool S_Engine::WSA_Assert( std::string Info, u_char Level )// 0 = none, 1 = log, 2 = don't continue, 3 = shutdown
{
	DWORD n_Error = WSAGetLastError();
	LPSTR lpMsgBuf = NULL;

	FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER |
				  FORMAT_MESSAGE_FROM_SYSTEM |
				  FORMAT_MESSAGE_IGNORE_INSERTS,
				  NULL,n_Error,
				  MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), // Default language
				  (LPTSTR)&lpMsgBuf,0,NULL);

	std::stringstream Str_Err;
	Str_Err<<n_Error<<'-'<<lpMsgBuf;

	Info = "WSA " + Info + ": " + Str_Err.str();

	LocalFree( lpMsgBuf );

	if(n_Error == ERROR_SUCCESS)
	{
		return false;
	}

	this->Register_Error(S_Error( Info, Level) );
	return true;
}
#else

int S_Engine::X11_Error( Display* dpy, XErrorEvent* xev )
{
	return 0;
}

#endif

bool S_Engine::OpenGL_Assert(std::string Info, u_char Level)// 0 = none, 1 = log, 2 = don't continue, 3 = shutdown
{
	GLenum n_Error = glGetError();

	if(n_Error==GL_NO_ERROR && Level != 5)
	{
		return false;
	}
	else
	{
		const char* glu_Str = (const char*)gluErrorString(n_Error);

		Info = "OpenGL " + Info + ": " + To_Str(n_Error, 16);

		if(glu_Str)
			Info += " " + std::string(glu_Str);

		this->Register_Error(S_Error(Info, Level) );

#ifdef SE_USE_ASSERT
		return true;
#else
		return false;
#endif
	}
}

#include <bass.h>
bool S_Engine::BASS_Assert( std::string Info, u_char Level )
{
	int Error = BASS_ErrorGetCode();

	if( Error != BASS_OK )
	{
		Info = "BASS " + Info + ": " + To_Str(Error) + " " + E_Bass::BASS_Error_Str( Error );

		this->Register_Error( S_Error(Info, Level) );

#ifdef SE_USE_ASSERT
		return true;
#else
		return false;
#endif
	}
	else
		return false;
}

#include <queue>
#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>
bool S_Engine::Assert_IL( std::string Info, u_char Level )
{
	bool Ret = false;
	ILenum Error_Now = ilGetError();// first error, latest error
/*
	std::queue< S_Error > Reverse_Stack;

	ILenum Error;
	while((Error = ilGetError()) != IL_NO_ERROR)
	{
		std::string L_Info = "DevIL " + Info + ": " + To_Str(Error) + " " + std::string( iluErrorString(Error) );
		Reverse_Stack.push( S_Error(L_Info, 2) );
		Ret = true;
	}

	while( Reverse_Stack.size() > 0 )
	{
		this->Register_Error( Reverse_Stack.front() );
		Reverse_Stack.pop();
	}

	Info = "DevIL " + Info + ": " + To_Str(Error_Now) + " " + std::string( iluErrorString(Error_Now) );
	this->Register_Error( S_Error(Info, Level) );
*/
#ifdef SE_USE_ASSERT
	return Ret;
#else
	return false;
#endif
}

#ifdef _WIN32
#include <ShlObj.h>

void S_Engine::Wind_Command(unsigned int Com)
{
	switch(Com)
	{
	case 0:
		return;
		break;

	default:
		{
			if(false)
			{
				char Buf[16];
				memset(Buf,0,16);
				_itoa_s(Com,Buf,10);
				std::string Bad_Com(Buf);
				this->Register_Error(S_Error( "Unknown Window Command " + Bad_Com, 0));
			}
		}break;
	}
}
#endif


void S_Engine::Log( std::string Message, uchar Type )
{
	//this->Console.Add_Line( Message );
	this->Log_Color( Type );
	printf( "%s", Message.c_str() );
	this->Application.Log_File.Write( Message, Type );

	this->Log_Buf += Message;

	while( this->Log_Buf.find_first_of( '\n' ) != this->Log_Buf.npos )
	{
		std::string Line_Buf = this->Log_Buf.substr( 0, this->Log_Buf.find_first_of( '\n' ) );
		this->Log_Buf.erase( 0, this->Log_Buf.find_first_of( '\n' ) + 1 );

		Vector3 Con_Color( 1, 1, 1 );

		switch( Type )
		{
		case Log_Type::TYPE_EROR:
			Con_Color.Set( 1, 0, 0 );
			break;
		case Log_Type::TYPE_LOAD:
			Con_Color.Set( 122/255.f, 77/255.f, 50/255.f );
			break;
		case Log_Type::TYPE_DBUG:
			Con_Color.Set( 75/255.f, 125/255.f, 75/255.f );
			break;
		case Log_Type::TYPE_WARN:
			Con_Color.Set( 1, 200/255.f, 50/255.f );
			break;
		case Log_Type::TYPE_NOTIF_CONFIG:
			Con_Color.Set( 59/255.f, 155/255.f, 184/255.f );
			break;
		}

		this->Console.Add_Line( Line_Buf, Con_Color );

		/*if( Line_Buf.length() > 2 )// more than '\n'
			Engine.Console.Add_Line( this->F_Name + Line_Buf, this->Con_Color );
		else*/
			//Engine.Console.Add_Line( Line_Buf, this->Con_Color );
	}
}

void S_Engine::Log_Color( uchar Type )
{
	if( this->Log_No_Color ) // don't print color
		return;

	switch( Type )
	{
	case Log_Type::TYPE_EROR:
		printf("\x1b[5;31m");
		break;
	case Log_Type::TYPE_LOAD:
		printf("\x1b[33m");
		break;
	case Log_Type::TYPE_DBUG:
		printf("\x1b[32m");
		break;
	case Log_Type::TYPE_WARN:
		printf("\x1b[31m");
		break;
	case Log_Type::TYPE_NOTIF_CONFIG:
		printf("\x1b[34m");
		break;
	default:
		printf("\x1b[0m");
		break;
	}
}

void S_Engine::Printf( const char* format, uchar L_Type, ... )
{
	char Str[512];memset( Str, 0, 512 );

    va_list args;
    va_start( args, format );
    vsprintf( Str, format, args );
    va_end( args );

	this->Log( Str, L_Type );
}

void S_Engine::Logic()
{
	this->Event_Manager.Logic_Thread.Update();

	this->Bass_Manager.Update( this->Thread_Logic.m_Timer.Elapsed_Time );
	this->Application.Update( this->Thread_Logic.m_Timer.Elapsed_Time );

	this->Console.Update( this->Thread_Logic.m_Timer.Elapsed_Time );
	this->Menu.Update( this->Thread_Logic.m_Timer.Elapsed_Time );
	this->GUI_Manager.Update( this->Thread_Logic.m_Timer.Elapsed_Time );


	if( this->Game )
	{
		this->Game->Update( this->Thread_Logic.m_Timer.Elapsed_Time );
	}
}



void S_Engine::Main_Loop()
{
	this->E_Timer.Update();
	this->Event_Manager.Engine_Thread.Update();

	this->Application.Update( this->E_Timer.Elapsed_Time );
	this->Stream_Manager.Update();


#ifndef _WIN32
	this->Win_X11_Events();
	this->Draw();
#endif
}

#ifndef _WIN32

void S_Engine::Win_X11_Events()
{
	this->Application.Input.Get_Mouse();

	while( XPending(this->Application.Window.m_dpy) != 0 )
	{
		XNextEvent( this->Application.Window.m_dpy, &this->Application.Window.m_xev);

		switch( this->Application.Window.m_xev.type )
		{
		case MotionNotify:
		{
			this->Application.Input.Mouse_X = this->Application.Window.m_xev.xmotion.x;
			this->Application.Input.Mouse_Y = this->Application.Window.m_xev.xmotion.y;
		}break;

		case KeyPress:
		{
			this->Input.New_Keysym( XLookupKeysym(&this->Application.Window.m_xev.xkey, 0), true );
		}break;

		case KeyRelease:
		{
			this->Input.New_Keysym( XLookupKeysym(&this->Application.Window.m_xev.xkey, 0), false );
		}break;

		case ButtonPress:
		{
			switch( this->Application.Window.m_xev.xbutton.button )
			{
			case Button1:
//				this->Application.Input.Mouse_L_Down = true;
				this->Input.New_Input( SE_Input_Codes::MOUSE_LEFT, true );
			break;
			case Button3:
//				this->Application.Input.Mouse_R_Down = true;
				this->Input.New_Input( SE_Input_Codes::MOUSE_RIGHT, true );
			break;
			case Button4:/* middle mouse wheel moved */
//				this->Application.Input.Mouse_Scroll++;
				this->Input.New_Input( SE_Input_Codes::MOUSE_WHEEL_UP, 0 );
			break;
			case Button5:/* middle mouse wheel moved */
//				this->Application.Input.Mouse_Scroll--;
				this->Input.New_Input( SE_Input_Codes::MOUSE_WHEEL_DOWN, 0 );
			break;
			}
		}break;

		case ButtonRelease:
		{
			switch( this->Application.Window.m_xev.xbutton.button )
			{
			case Button1:
//				this->Application.Input.Mouse_L_Down = false;
				this->Input.New_Input( SE_Input_Codes::MOUSE_LEFT, false );
			break;
			case Button3:
//				this->Application.Input.Mouse_R_Down = false;
				this->Input.New_Input( SE_Input_Codes::MOUSE_RIGHT, false );
			break;
			}
		}break;

		case EnterNotify:
		{
			this->Focus( true );
		}break;

		case LeaveNotify:
		{
			this->Focus( false );
		}break;

		case ConfigureNotify:
		{
			//XWindowAttributes xwa;
			//XGetWindowAttributes(this->Application.Window.m_dpy, this->Application.Window.m_win, &xwa);
			if( this->Application.Window.Border_H == 0 && this->Application.Window.Border_W == 0 )// not set
			{
				int x, y;
				Window child_return;

				XTranslateCoordinates(this->Application.Window.m_dpy, this->Application.Window.m_win, this->Application.Window.m_root,
												   0, 0, & x, & y, & child_return);

				this->Application.Window.Border_W = x - 1;// ionclude pixel border
				this->Application.Window.Border_H = y - 1;
				XMapRaised( this->Application.Window.m_dpy, this->Application.Window.m_win);
			}

			this->Application.Window.Wind_x = this->Application.Window.m_xev.xconfigure.x - this->Application.Window.Border_W;
			this->Application.Window.Wind_y = this->Application.Window.m_xev.xconfigure.y - this->Application.Window.Border_H;

			SE_Events::Event_uint2* nEvent = new SE_Events::Event_uint2;
			nEvent->Type = SE_Events::Event_Types::TYPE_APP;
			nEvent->Code = SE_Events::Events::EVENT_WIN_RESIZE;
			nEvent->Argument_1 = this->Application.Window.m_xev.xconfigure.width;
			nEvent->Argument_2 = this->Application.Window.m_xev.xconfigure.height;

			this->Event_Manager.Add_Event( nEvent, "X11 Resize" );
		}break;

		case ClientMessage:
		{
			if( (Atom)this->Application.Window.m_xev.xclient.data.l[0] == this->Application.Window.wmDeleteMessage )
			{
				this->Log( "Shutdown by window message\n" );
				this->Shutdown();
			}
		}break;
		}
	}
}
#endif

void S_Engine::Focus( bool Gained )
{
	if( Gained )
	{
		XAutoRepeatOff( this->Application.Window.m_dpy );
	}
	else
	{
		XAutoRepeatOn( this->Application.Window.m_dpy );
	}
}

void S_Engine::Make_Game( E_Game::G_Game* n_Game )
{
	if(!this->Game)
	{
		this->Game = n_Game;
		this->Mem_Add( n_Game, "new game" );

		if( this->Game )
		{
			if( this->Game->Initialize() ) // went good
				this->Log( "Intialized Game\n", Log_Type::TYPE_DBUG );
			else
			{
				this->Log( "Failed To Initialize Game\n", Log_Type::TYPE_EROR );
				delete n_Game;
				this->Mem_Remove( n_Game );
			}
		}
		else//something went wrong
		{
			this->Register_Error(S_Error("Failed To Make A New Game",3));
		}
	}
	else
	{
		this->Register_Error(S_Error("Tried To Make New Game When One Was Present",1));
	}
}

void S_Engine::Enter_Game()
{
	if(this->Game)
	{
		this->Log( "Entered Game\n" );
		this->Application.Utility.Show_Cursor(false);
		this->Menu.Cleanup();
		this->In_Game = true;
		this->Is_Paused = false;
	}
}

void S_Engine::Enter_Screen()
{
	this->Application.Utility.Show_Cursor(true);
//	this->Screen.Menu_Trace.clear();
//	this->Screen.Load_Menu("pause");
	this->In_Game = false;
	this->Log( "Entered Menu\n" );
}

void S_Engine::Pause_Game()
{
	this->Log( "Paused Game\n" );

	this->Is_Paused = true;

	this->Menu.Load_Menu( this->Menu_Load_Pause );
	this->Enter_Screen();
}

void S_Engine::Resume_Game()
{
	this->Log( "Resumed Game\n" );
	//this->Enter_Game();
	this->Menu.Load_Menu( "resume" );
}

void S_Engine::Make_Net_Manager( bool Be_Host )// make net manager, if should be host / client
{
	if( this->Net_Manager == NULL )
	{
		if( Be_Host )
		{
			this->Net_Manager = new SE_Network::SE_Net_Manager_HOST;
			this->Log( "Made New HOST Net Manager\n", Log_Type::TYPE_DBUG );
		}
		else
		{
			this->Net_Manager = new SE_Network::SE_Net_Manager_CLIENT;
			this->Log( "Made New CLIENT Net Manager\n", Log_Type::TYPE_DBUG );
		}

		assert( this->Net_Manager != NULL );

		this->Net_Manager->Initialize();
	}
	else
		this->Log( "Tried to make net manager while one exists\n", Log_Type::TYPE_EROR );
}

void S_Engine::Get_Options()
{
//	this->Application.Main_Log.Print_Time();
	//this->Log( "Getting Options...\n" );
	std::string Data = this->Application.Input.Load_File_Str("data/options.txt");

	if(Data.length()==0)
	{
		this->Register_Error(S_Error("Cannot load options, no data",1));
		return;
	}
	//else
		//this->Log("Success.\n", false);

	std::vector<std::string> Options;
	std::vector<std::string> Variables;

	char Buffer[64];memset(Buffer,0,64);
	char Buf_At=0;

	for(uint l=0;l<Data.size();l++)
	{
		if( Data[l] == ':' )
		{
			Options.push_back(std::string(Buffer));
			memset(Buffer,0,64);
			Buf_At = 0;
		}
		else if(Data[l]=='\n' && Options.size() -1 == Variables.size())//done with that data. and added option
		{
			Variables.push_back(std::string(Buffer));
			memset(Buffer,0,64);
			Buf_At = 0;
		}
		else if( (isalnum(Data[l]) || Data[l] == ' ' ) && Data[l] != 13 && Buf_At<64 )
		{
			Buffer[Buf_At]=Data[l];
			Buf_At++;
		}
		else if(Buf_At == 64)
		{
			memset(Buffer, 0, 64);
			Buf_At = 0;
		}
	}

	for(uint i = 0;i < Options.size(); i++)
	{
		if(Options[i] == "texture load")
		{
			if(Variables[i] == "fast")
				this->Resources.Load_Fast = true;
		}
		else if( Options[i] == "record load history" )
		{
			if( Variables[i] == "false" )
				this->Application.Record_Load_History = false;
		}
		else if( Options[i] == "log unfound files" )
		{
			if( Variables[i] == "true" )
				this->Content.Log_Unfound_Files = true;
		}
		else if( Options[i] == "force old shaders" )
		{
			if( Variables[i] == "true" )
				this->Application.OpenGL.Use_New_Shaders = false;
		}
		else if( Options[i] == "force old rtt" )
		{
			if( Variables[i] == "true" )
				this->Application.OpenGL.Use_New_RTT = false;
		}
		else if( Options[i] == "force old vbo" )
		{
			if( Variables[i] == "true" )
				this->Application.OpenGL.Use_New_VBO = false;
		}
		else if( Options[i] == "print extensions" )
		{
			if( Variables[i] == "false" )
			this->Application.OpenGL_Settings.Print_Extensions = false;
		}
		else
			continue;//do not log

		this->Log( "\t" + Options[i] + " = " + Variables[i] + "\n" );
	}

	this->Log( "Got Options\n" );
}

void S_Engine::Mem_Add( void* What, std::string Name )
{
	this->Mem_Mutex.Wait_For_Lock( __FUNCTION__ );
	this->Mem_Mutex.Lock();

	for( uint i = 0;i < this->Memory_Allocs.size(); i++)
		if( this->Memory_Allocs[i] == What )
			return;

	this->Memory_Allocs.push_back( What );
	this->Memory_Names.push_back( Name );

	this->Mem_Mutex.Unlock();
}

void S_Engine::Mem_Remove( void* What )
{
	this->Mem_Mutex.Wait_For_Lock( __FUNCTION__ );
	this->Mem_Mutex.Lock();

	for( uint i = 0;i < this->Memory_Allocs.size(); i++)
	{
		if( this->Memory_Allocs[i] == What )
		{
			this->Memory_Allocs.erase( this->Memory_Allocs.begin() + i );
			this->Memory_Names.erase( this->Memory_Names.begin() + i );
			i--;
		}
	}

	this->Mem_Mutex.Unlock();
}

extern S_Engine Engine;
void SE_Array_Mem::Add_To_Mem( void* Data, std::string Name )
{
	Engine.Mem_Add( Data, Name );
}
void SE_Array_Mem::Rem_To_Mem( void* Data )
{
	Engine.Mem_Remove( Data );
}

void S_Engine::Mem_List()
{
	this->Mem_Mutex.Wait_For_Lock( __FUNCTION__ );
	this->Mem_Mutex.Lock();

	this->Log("Current Memory Allocations:\n");
	for( uint i = 0;i < this->Memory_Allocs.size(); i++)
		this->Log( To_Str(i) + ": " + To_Str((uint)((long)this->Memory_Allocs[i]), 16) + ", " + this->Memory_Names[i] + "\n" );

	this->Mem_Mutex.Unlock();
}

S_Engine_Version_Desc* S_Engine::Get_Version()
{
	return &this->m_Version;
}

std::string S_Engine::Get_CWD()
{
	return get_current_dir_name();
}

std::string S_Engine::Get_EXE_Name()
{
	std::string Ret;

#ifdef _WIN32
#else
	char Buf[256]; memset( Buf, 0, 256 );
	readlink( "/proc/self/exe", Buf, sizeof(Buf) );

	Ret.append( Buf );
#endif

	size_t WD_End = Ret.find_last_of( '/' );
	Ret.erase( 0, WD_End + 1 );

	return Ret;
}



void S_Engine::Event_Draw_Processor( SE_Events::Event_Base* Event )
{
	switch( Event->Code )
	{
	case SE_Events::Events::EVENT_WIN_RESIZE:
		Engine.Application.Window.Reshape( ((SE_Events::Event_uint2*)Event)->Argument_1, ((SE_Events::Event_uint2*)Event)->Argument_2);
		break;
	}
}

void S_Engine::Event_Logic_Processor( SE_Events::Event_Base* Event )
{
	switch( Event->Code )
	{
	case 0:
		break;

	default:
		Engine.Handle_Logic_Event_Ex( Event );
		break;
	}
}

void S_Engine::Event_Main_Processor( SE_Events::Event_Base* Event )
{
	//if( Event->Code != E_Events::Event_Types::TYPE_APP )//not in right place
		//;

	switch( Event->Code )
	{

	case SE_Events::Events::EVENT_PLAY_SAMPLE:
		{
			Engine.Bass_Manager.Play_Sample( ((SE_Events::Event_uint*)Event)->Argument );
		}break;
	case SE_Events::Events::EVENT_PLAY_STREAM:
		{
			Engine.Bass_Manager.Play_Stream( ((SE_Events::Event_uint*)Event)->Argument );
		}break;


	case SE_Events::Events::EVENT_WIN_MOVE:
		break;


	case SE_Events::Events::EVENT_M_LOAD:
		{
			Engine.Menu.Load_Menu( ((SE_Events::Event_Str*)Event)->Argument );
		}break;
	case SE_Events::Events::EVENT_M_CLEANUP:
		{
			Engine.Menu.Cleanup_Cont();
		}break;


	case SE_Events::Events::EVENT_RESUME:
		{
			Engine.Resume_Game();
		}break;
	case SE_Events::Events::EVENT_PAUSE:
		{
			Engine.Pause_Game();
		}break;

	case SE_Events::Events::EVENT_ENTER:
		{
			Engine.Enter_Game();
		}break;
	case SE_Events::Events::EVENT_LEAVE:
		{
			Engine.Enter_Screen();
		}break;
	}
}


	   /*
	if( this->Changing_Menu != false )
	{
		Engine.Application.Error_Log.Write_Log("Change Before");
		while( this->Ready_Draw == false );

		if( this->Changing_Menu == TRUE + 1 )//external
		else if( this->Changing_Menu == TRUE + 2 )
		else

		Engine.Application.Error_Log.Write_Log("Change After");
	}*/
#endif
#include "../module/Module.hpp"

namespace S_Engine
{
/*  struct Reg_Func_Thread
	{
		SE_Module_ID m_Mod;
		void(*m_Func)();

		Reg_Func()
		{
			this->m_Mod = NULL;
			this->m_Func = NULL;
		}
	};
	typedef std::vector< Reg_Func > Reg_Func_List;*/

/**********   GLOBALS   **************/

	// Called in each thread
//	Reg_Func_List Thread_Engine, Thread_Update, Thread_Render;
}

