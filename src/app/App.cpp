/*#include "App.h"
#include <math.h>
#include <cstdlib>
#include <Windows.h>
#include <time.h>
#include "GameStats.h"
extern GameStats Game;
#include "ScreenClass.h"
extern Screen MainScreen;
#include "Utility.h"
#pragma warning(disable:4996)//disables "unsafe" (deprecated) function warnings.

void myApp::Init(HINSTANCE nHinst,int w,int h)
{
	std::string OptionReport=this->Options.Load_Ops();
	//if(this->Options.Master_Volume==0){this->Mute(true);}


    myApp::QUITED=false;
    myApp::hInstance=GetModuleHandle(NULL);
    myApp::isRunning=true;
    /*for(unsigned int Con=0;Con<4;Con++)
    {
        myApp::XBOX[Con].Init();
    }*//*myApp::XBOX.Init();
    myApp::UsingXbox=false;
    myApp::DEBUG=true;
    myApp::INITIALIZED=false;
    myApp::State=false;
    myApp::StartTime=timeGetTime();
    myApp::Frames=0;
    myApp::LastSecond=0;
    myApp::fps=0;
    time_t newTime;time(&newTime);
    myApp::Time=localtime(&newTime);
    myApp::Timer=0;
    myApp::Playing=false;
    myApp::GlobalTime=0;
    myApp::Screenw=w;
    myApp::Screenh=h;
    myApp::Focus=true;
    myApp::ConsoleOpen=false;
    myApp::whatTyping="";
    myApp::TypingFor="";
    myApp::Typing=false;
	myApp::MouseX=this->Screenw/2;
	myApp::MouseY=this->Screenh/2;
    myApp::LMouseDown=false;
    myApp::RMouseDown=false;
    myApp::Scroll=0;
    myApp::Shift=false;


    myApp::Log.Open("stuff/data/log.txt");
    myApp::KeyLog.Open("stuff/data/keylog.txt");
    myApp::ServerLog.Open("stuff/data/serverlog.txt");
    myApp::ChatLog.Open("stuff/data/chatlog.txt");
    myApp::WorldLog.Open("stuff/data/worldlog.txt");
    myApp::LoadLog.Open("stuff/data//loadlog.txt");

    std::stringstream CompRep;
    CompRep<<"Wnd W: "<<w<<", Wnd H: "<<h;
    myApp::Log.Write((CompRep.str()).c_str());

	if(OptionReport!="OK")
	{
		this->LoadLog.Write(OptionReport.c_str());
	}

    /*if(std::ifstream("stuff/data/ops.txt"))
    {
        std::ifstream GetOps;GetOps.open("stuff/data/ops.txt");
        std::stringstream NewOps;NewOps<<GetOps.rdbuf();
        std::string Ops=NewOps.str();
        unsigned int At=-1;

        for(int WhatIsIt=0;WhatIsIt<8;WhatIsIt++)
        {
            std::stringstream Buffer;
            bool Searching=true;
            while(Searching)
            {
                At++;
                if(At>=Ops.length()){Searching=false;WhatIsIt=4;OpsReport<<"No More User Option Chars Aavailable To Load\n";OpsFailed=true;}
                else if(Ops[At]=='|')
                {
                    std::string FoundInfo=Buffer.str();
                    switch(WhatIsIt)
                    {
                        case 0:
                        OpsReport<<"Edit= ";
                        if(FoundInfo=="passwordforallowingediting"){myApp::ALLOWEDIT=true;OpsReport<<"True, Dev Commands= ";}
                        else{myApp::ALLOWEDIT=false;OpsReport<<"False, Texture Pack= ";}
                        break;
                        case 1:
                        if(FoundInfo=="passwordforallowingcommands"){myApp::ALLOWCOMMANDS=true;OpsReport<<"True, Texture Pack= ";}
                        else{myApp::ALLOWCOMMANDS=false;OpsReport<<"False, Texture Pack= ";}
                        break;
                        case 2:
                        if(std::ifstream(("stuff/tiles/"+FoundInfo+".bmp").c_str())){myApp::Texturepack=FoundInfo;OpsReport<<FoundInfo<<", FPS= ";}
                        else{myApp::Texturepack="normalTP";OpsReport<<"Default, FPS= ";}
                        break;
                        case 3:
                        myApp::FPS=atoi(FoundInfo.c_str());OpsReport<<FoundInfo<<", Music On= ";
                        break;//should be 60
                        case 4:
                        if(atoi(FoundInfo.c_str())==0){myApp::MusicOn=false;OpsReport<<"False";}
                        else{myApp::MusicOn=true;OpsReport<<"True";}
                        OpsReport<<", Sensitivity= ";
                        break;
                        case 5:
                        myApp::Sensitivity=atoi(FoundInfo.c_str())/10.0f;OpsReport<<FoundInfo<<", Safe Mode: ";
                        break;//
                        case 6:
                        if(FoundInfo=="f"){myApp::SAFE=false;}
                        else{myApp::SAFE=true;}
                        OpsReport<<FoundInfo<<", Host: ";
                        break;
                        case 7:
                        if(FoundInfo=="t"){myApp::HOST=true;}
                        else{myApp::HOST=false;}
                        OpsReport<<FoundInfo<<"\n";
                        break;
                        default:break;
                    }
                    Searching=false;Engine
                }
                else{Buffer<<Ops[At];}
            }
        }
    }*//*

    std::stringstream NewVersion;NewVersion<<"Version: "<<SURVIVOR_VERSION<<'\n';
	this->Log.Write(NewVersion.str().c_str());
    myApp::Version=NewVersion.str();

    std::ifstream ReadInfo;
    ReadInfo.open("stuff/data/info.txt");
    std::stringstream GetingInfo;GetingInfo<<ReadInfo.rdbuf();
    std::string NewInfo=GetingInfo.str();
    int At=-1;
    int NumInfos=0;
    myApp::Info.clear();
    for(int FindInfo=0;FindInfo<100;FindInfo++)
    {
        bool Searching=true;
        std::stringstream Buffer;
        while(Searching)
        {
            At++;
            if(NewInfo[At]=='|'){Searching=false;FindInfo=100;}
            else if(NewInfo[At]=='\n')
            {
                std::string FoundInfo=Buffer.str();
                myApp::Info.push_back(FoundInfo);
                Searching=false;
                NumInfos++;
                std::string Report=FoundInfo;
                Report="New Info Loaded: "+Report;
                myApp::LoadLog.Write(Report.c_str());
            }
            else{Buffer<<NewInfo[At];}
        }
    }
    myApp::InfoTimer=0;
    myApp::Infos=NumInfos-1;
    myApp::InfoAt=rand()%10;
    while(myApp::InfoAt>myApp::Infos)
    {
        myApp::InfoAt=rand()%10;
    }

	this->FPS=60;

    myApp::MAPNAME="TestMap";
    myApp::NUMBEROFTILES=255;
    myApp::MAXSIGHT=30;
    myApp::BPP=32;
    myApp::FRAME_TIME=1000/myApp::FPS;
    myApp::PLAYERSPEED=0.1f;
    myApp::HOLD=1200;
    myApp::NUMAI=20;
    myApp::BULLETS=10;



}

void myApp::UseLighting(bool b)
{
	//this->LightingEn=b;
	glUniform1i(this->LightingEn,b);
}

void myApp::Ambient(float r,float g,float b,float a)
{
	glUniform4f(this->ShadeAmbient,r,g,b,a);
}

void myApp::Texture2D(bool t)
{
	glUniform1i(this->TexEn,t);
	if(t==true){sglEnable(GL_TEXTURE_2D);}
	else{sglDisable(GL_TEXTURE_2D);}
}

/*void myApp::NewSound(HCHANNEL NSound,unsigned int length,float x,float y,float z)
{
    HCHANNEL nSound;
    ALuint source;

    alGenSources(1, &source);

    ALfloat Pos[3]={x,y,z};
    ALfloat Vel[3]={0,0,0};

    alSourcei (source, AL_BUFFER,   NSound );
    alSourcef (source, AL_PITCH,    1.0    );
    alSourcef (source, AL_GAIN,     1.0    );
    alSourcefv(source, AL_POSITION, Pos    );
    alSourcefv(source, AL_VELOCITY, Vel    );
    alSourcei (source, AL_LOOPING,  AL_FALSE);

    alSourcePlay(source);
    nSound.New(length,source);

    myApp::HHUDSources.push_back(nSound);
} *//*

void myApp::FinnishedTyping()
{
    myApp::Typing=false;
    myApp::whatTyping="";
    myApp::TypingFor="";
}

void myApp::Quit(std::string reason)
{
    if(!this->QUITED)
    {
        if(myApp::isRunning){myApp::Pause();}
        int Result;
        if(reason.find("safe/")==reason.npos)
        {
            MessageBeep(750);
            Result=MessageBox(NULL, "Are you sure you want to leave?\n\nAll un-saved data will be lost!","Leave game?",MB_OK | MB_OKCANCEL);
        }
        else
        {
            Result=IDOK;
            reason.replace(0,5,"");
        }
        if(Result==IDOK)
        {
            myApp::isRunning=false;
            this->QUITED=true;
            std::stringstream Report;
            Report<<"Quiting, Reason: "<<reason<<"\n--Program End--";
            myApp::Log.Write((Report.str()).c_str());
            if(reason.find("error")==reason.npos)
            {
                Kill_App(EXIT_SUCCESS);
            }
            else
            {
                Kill_App(EXIT_FAILURE);
            }
        }
    }
}

void myApp::Pause()
{
    MainScreen.LoadScreen("pause");
    myApp::Playing=false;
//    GlobalMusic.Pause();
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, myApp::Screenw, myApp::Screenh, 0.0, 0, 100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClearColor(0,0,0,0);
    glPushAttrib(GL_ENABLE_BIT);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	this->Texture2D(true);
    sglDisable(GL_DEPTH_TEST);
	this->UseLighting(false);
    sglDisable(GL_FOG);
	sglDisable(GL_STENCIL_TEST);
    glDepthMask(GL_FALSE);
    SetCursorPos(myApp::MouseX, myApp::MouseY);
    ShowCursor(TRUE);
    myApp::Log.Write("Pausing\n");
}

void myApp::Resume()
{
    MainScreen.Butts.clear();
    MainScreen.SImages.clear();

    myApp::Playing=true;

    glClearColor (0.0, 0.0, 0.0, 0.0);
    glShadeModel(GL_SMOOTH);
    sglEnable(GL_DEPTH_TEST | GL_BLEND | GL_ALPHA);
	this->Texture2D(true);
	this->UseLighting(true);
	//sglEnable(GL_FOG);
	//sglEnable(GL_STENCIL_TEST);
    glDepthMask(GL_TRUE);
    glClearDepth(1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0f,myApp::Screenw * (1.0f/myApp::Screenh),VIEW_MIN_DIST,VIEW_MAX_DIST);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
    SetCursorPos(myApp::Screenw/2, myApp::Screenh/2);
    ShowCursor(FALSE);
//    GlobalMusic.Resume();
    myApp::Log.Write("Resuming\n");
}

void myApp::NewInfo()
{
    myApp::InfoTimer=0;
    myApp::InfoAt=rand()%myApp::Info.size();
    while(myApp::InfoAt>myApp::Info.size())
    {
        myApp::InfoAt=rand()%myApp::Info.size();
    }
    myApp::InfoToDraw=myApp::Info[myApp::InfoAt];
}

void myApp::Update()
{
	system("SurvivorUpdater.exe");
}

void myApp::Check_Flags()
{
	int nSound=	BASS_GetConfig(BASS_CONFIG_GVOL_STREAM);
	//if(BASS_GetConfig(BASS_CONFIG_GVOL_STREAM)>0 && this->ISMUTED){this->Mute(true);}
	//else if(BASS_GetConfig(BASS_CONFIG_GVOL_STREAM)==0 && !this->ISMUTED){this->Mute(false);}
}


*/

#include "../app/App.hpp"
using namespace SE_Application;


//#include "GL/glew.h"
//#include "GL/gl.h"

#include "../engine/Engine.hpp"
#include "../utility/Functions.hpp"

#ifdef _WIN32
#include "../os/win32/Icons.hpp"
#include <CommCtrl.h>
#endif


/*
void A_Input_Keys::Update_Base( uint Key_Code, bool Set )
{
	if( SE_Input_Codes::KEY_RETURN == Key_Code && Engine.Application.Input.Typing && Set )// submit
	{
		Engine.Application.Input.Finished_Typing();
	}
	else
	{
		this->Update( Key_Code, Set );
	}


	// mod keys
	if( Engine.Application.Input.Typing )
	{
		if( Key_Code == SE_Input_Codes::KEY_SHIFT_L || Key_Code == SE_Input_Codes::KEY_SHIFT_R )
		{
			Engine.Application.Input.Uppercase = Set;
		}
		else if( Key_Code == SE_Input_Codes::KEY_BACKSPACE && Set )
		{
			if( Engine.Application.Input.Typing_What.length() > 0 )
				Engine.Application.Input.Typing_What.pop_back();
		}
		else if( Set && Engine.Application.Input.String_Keypress )// typed something
		{
			if( Key_Code >= 32 && Key_Code < 256 )
			{
				if( Engine.Application.Input.Uppercase )
					Key_Code = toupper( Key_Code );
				else
					Key_Code = tolower( Key_Code );

				Engine.Application.Input.String_Keypress( Key_Code );// let custom handler choose accepted values
			}
		}
	}
}*/


/*void A_Input::Get()
{
	this->Quit=GetAsyncKeyState(this->Keys.Quit);//quit the application
	this->Commands=GetAsyncKeyState(this->Keys.Commands);
	//MENU
	this->Pause=GetAsyncKeyState(this->Keys.Pause);
	this->To_Backpack=GetAsyncKeyState(this->Keys.To_Backpack);
	this->To_Map=GetAsyncKeyState(this->Keys.To_Map);

	//GAMEPLAY
	this->Forward=GetAsyncKeyState(this->Keys.Forward);
	this->Backward=GetAsyncKeyState(this->Keys.Backward);
	this->Left=GetAsyncKeyState(this->Keys.Left);
	this->Right=GetAsyncKeyState(this->Keys.Right);

	this->Jump=GetAsyncKeyState(this->Keys.Jump);
	this->Crouch=GetAsyncKeyState(this->Keys.Crouch);
	this->Run=GetAsyncKeyState(this->Keys.Run);

	this->Zoom=GetAsyncKeyState(this->Keys.Zoom);

	this->Primary_Fire=GetAsyncKeyState(this->Keys.Primary_Fire);
	this->Secondary_Fire=GetAsyncKeyState(this->Keys.Secondary_Fire);

	this->Drop_Item=GetAsyncKeyState(this->Keys.Drop_Item);
	this->Reload=GetAsyncKeyState(this->Keys.Reload);
	this->Chat=GetAsyncKeyState(this->Keys.Chat);
} */

/*SE_File::SE_File()
{
	this->m_File = NULL;
}

SE_File::~SE_File()
{
	//
}

bool SE_File::Open( std::string Dest )
{
	if( this->is_Open() )
		return false;

	this->m_File = fopen( Dest.c_str() , "rb");

	if( this->is_Good() )
	{
		this->Dest = Dest;
		return true;
	}
	else
		return false;
}

void SE_File::Close()
{
	if( this->is_Open() )
		fclose( this->m_File );
}

void SE_File::Write( const char* What, uint Len )
{
	assert( What == NULL );

	if( this->is_Open() )
	{
		this->m_Mutex.Wait_For_Lock( __FUNCTION__ );
		this->m_Mutex.Lock();

		if( Len != 0 )
			fwrite( What, sizeof(char), Len, this->m_File );
		else
			fputs( What, this->m_File );

		this->m_Mutex.Unlock();
	}
}

void SE_File::Write( std::string What )
{
	this->Write( What.c_str(), 0 );
}

void SE_File::Read( char* Buf, uint Len )
{
	if( Len != 0 && this->is_EOF() == false && this->is_Open() )
		fread( Buf, sizeof(char), Len, this->m_File );
}

bool SE_File::is_EOF()
{
	return feof( this->m_File ) == 1;
}

bool SE_File::is_Open()
{
	return this->m_File != NULL;
}

bool SE_File::is_Good()
{
	return ferror( this->m_File ) == false;
}	*/

#ifdef _TEST
AP_IOAPI::AP_IOAPI()
{
	this->Typing = false;//is typing
	this->Uppercase = false;

	this->Submit_String = SE_Application::String_Submit_Default;
	this->String_Keypress = SE_Application::String_Keypress_All;

	//XBOXController XBOX;
	//A_Input_Keys Keys;

	this->Mouse_X=0;this->Mouse_Y=0;
	this->Mouse_Scroll=0;
	this->Mouse_L_Down=false;this->Mouse_R_Down=false;//menu
}
//Survivor



std::string AP_IOAPI::Load_File_Str( std::string File, bool Silent)//load raw file, can be silent = no log
{
	SE_Raw_Buffer Temp_Buf = this->Load_File( File, Silent );

	if( Temp_Buf.Data() != NULL )
	{
		std::string Ret( (const char*)Temp_Buf.Data() );
		Temp_Buf.Delete();
		return Ret;
	}
	else
		return std::string();
}

#include "../content/File_Types.hpp"
SE_Raw_Buffer AP_IOAPI::Load_File( std::string File, bool Silent )//load raw file, can be silent = no log
{
	Silent = false;
	// if it is in the virtual file system,
	// we need to use that version ( overloaded / modded )

	if( File.find(".zip/") != File.npos )// part of zip
		return this->Load_Zip_File( File, Silent );
#ifdef _TEST
	if( Engine.Content.VFS.Get_Child( File, true ) != NULL && File.find(".zip/") == File.npos )//(void*)(-1) )// search for file
	{
		SE_Content_File_Types::Type_File* Nest_File = (SE_Content_File_Types::Type_File*)Engine.Content.VFS.Get_Child( File );
//		return this->Load_File( Nest_File->File_Name, Silent );
	}


	if( Silent == false )
		Engine.Log( "Loading File: " + File + "... ", Log_Type::TYPE_LOAD );
#endif
	std::ifstream Reader(File.c_str(),std::ios::binary);

	SE_Raw_Buffer Ret;

	if(Reader.good())
	{
		Reader.seekg(0,std::ios::end);

		Ret.Make( (uint)Reader.tellg() + 1, "load file buffer" );


		Reader.seekg(0,std::ios::beg);

		Reader.read( Ret.Data(), Ret.Size() );
#ifdef _TEST
		Engine.Log("Success, Size: " + To_Str(Ret.Get_Size()) + "\n", Log_Type::TYPE_LOAD);
#endif
	}
	else
	{
		std::string Report = "Failed to load file: " + File;
#ifdef _TEST
		Engine.Register_Error(S_Error(Report,1));
#endif

		//if( Silent == false )
		//Engine.Log("Failed\n", Log_Type::TYPE_EROR);
	}

	return Ret;
}

void AP_IOAPI::Finished_Typing()
{
	if( this->Submit_String )
	{
		this->Submit_String( this->Typing_What );
		this->Submit_String = SE_Application::String_Submit_Default;
	}

	this->String_Keypress = SE_Application::String_Keypress_All;
	this->Typing_What.clear();

	this->Typing = false;
}

void SE_Application::String_Keypress_All( uchar l )
{
	l+= 2;
#ifdef _TEST
	Engine.Application.Input.Typing_What.push_back( l );
#endif
}

//typedef uint64_t ZPOS64_T;
/*  
#ifdef _WIN32
#pragma comment (lib, "zdll.lib")
#include <zlib/unzip.h>
//#else
//#include "../zlib/unzip.h"
#endif
//#include "../zlib/zip.h"
#include "../zlib/unzip.h"

*/
uint Read_Zip_File( char* Buf, std::string File, uint Max_Len )// returns amount read
{
	assert( Buf == NULL );
	//
	return 0;
}

std::string AP_IOAPI::Load_Zip_File_Str( std::string File, bool Silent )//load raw file, can be silent = no log
{
	SE_Raw_Buffer Temp_Buf = this->Load_Zip_File( File, Silent );

	if( Temp_Buf.Data() != NULL )
	{
		std::string Ret( Temp_Buf.Data() );
		Temp_Buf.Delete();
		return Ret;
	}
	else
		return std::string();
}



SE_Raw_Buffer AP_IOAPI::Load_Zip_File( std::string File, bool Silent)//load raw file, can be silent = no log
{
	SE_Raw_Buffer Ret;

	Silent = false;
#ifdef _TEST
	if( Silent == false )
		Engine.Log( "Loading ZIP File: " + File + "... ", Log_Type::TYPE_LOAD );
#endif
/* 
	unzFile Zip = NULL;
	unz_file_info zInfo;

	ssize_t Zip_End = File.find(".zip/") + 4;
	if( Zip_End == File.npos )
		return Ret;

	std::string Zip_File = File.substr( 0, Zip_End );
	File.erase( 0, Zip_End + 1 );

	Zip = unzOpen( Zip_File.c_str() );

	if( Zip == NULL )
	{
#ifdef _TEST
		if( Silent == false )
			Engine.Log("Could not open Zip\n", Log_Type::TYPE_EROR);
#endif
		return Ret;
	}


	HRESULT Rez = unzLocateFile( Zip, File.c_str(), 1 );
	if( Rez != UNZ_OK )
	{
#ifdef _TEST
		if( Silent == false )
			Engine.Log("Could not locate file\n", Log_Type::TYPE_EROR);
#endif
		return Ret;
	}

	Rez = unzOpenCurrentFile( Zip );
	if( Rez != UNZ_OK )
	{
#ifdef _TEST
		if( Silent == false )
			Engine.Log("Could not open file\n", Log_Type::TYPE_EROR);
#endif
		return Ret;
	}

	Rez = unzGetCurrentFileInfo( Zip, &zInfo, NULL, 0, NULL, 0, NULL, 0 );
	if( Rez != UNZ_OK )
	{
#ifdef _TEST
		if( Silent == false )
			Engine.Log("Could not get info\n", Log_Type::TYPE_EROR);
#endif
		return Ret;
	}


	Ret.Make( zInfo.uncompressed_size + 1, "zip file buffer: " + File );


	Rez = unzReadCurrentFile( Zip, Ret.Data(), zInfo.uncompressed_size );

	if( Rez < 0 )// another error
	{
		Ret.Delete();
#ifdef _TEST
		if( Silent == false )
			Engine.Log("Failed to read\n", Log_Type::TYPE_EROR);
#endif
		return Ret;
	}

	unzCloseCurrentFile( Zip );

	unzClose( Zip );
#ifdef _TEST
	if( Silent == false )
		Engine.Log("Success, Size: " + To_Str(Ret.Get_Size()) + "\n", Log_Type::TYPE_LOAD);
#endif
*/

	return Ret;
}


//#else





bool AP_IOAPI::Load_XML_File( XML_File* File, std::string Dest )
{
	std::string Data = this->Load_File_Str( Dest );
#ifdef _TEST
	Engine.Log("Loading XML File: " + Dest + "... ");
#endif

	std::stringstream Report;
	pugi::xml_parse_result Load_Result = File->load_buffer(Data.c_str(), Data.length() );

	if( !Load_Result )// there was an error
	{
		Report << Load_Result.description() << ", At: " << Load_Result.offset;
#ifdef _TEST
		Engine.Log("Error: " + Dest + ": " + Report.str() + "\n", Log_Type::TYPE_EROR);
#endif
	}
#ifdef _TEST
	Engine.Log( "Success\n", Log_Type::TYPE_LOAD );
#endif

	if( Report.str().length() > 0 )//was filled with error report
	{
#ifdef _TEST
		Engine.Log( Report.str() );
#endif
		return false;
	}
	return true;
}

void AP_IOAPI::Get_Mouse()
{
#ifdef _WIN32

	POINT mousePos;
	GetCursorPos(&mousePos);

	this->Mouse_X = mousePos.x;
	this->Mouse_Y = mousePos.y;
#else

#endif
}

void AP_IOAPI::Set_Mouse( int x, int y )
{
	//if( Engine.Application.Window.Fullscreen == true )
	{
#ifdef _WIN32
    SetCursorPos( x, y );
#else
#ifdef _TEST
	XLockDisplay( Engine.Application.Window.m_dpy );
	XWarpPointer( Engine.Application.Window.m_dpy, None, Engine.Application.Window.m_root, 0, 0, 0, 0,
			x + Engine.Application.Window.Wind_x + Engine.Application.Window.Border_W,
			y + Engine.Application.Window.Wind_y + Engine.Application.Window.Border_H);
	XFlush( Engine.Application.Window.m_dpy ); // Flushes the output buffer, therefore updates the cursor's position. Thanks to Achernar.
	XUnlockDisplay( Engine.Application.Window.m_dpy );
#endif
#endif
	}

	this->Mouse_X = x;
	this->Mouse_Y = y;
}


A_OpenGL_Settings::A_OpenGL_Settings()
{

	this->Print_Extensions = true;
}

A_Utility::A_Utility()
{
	this->Zoom = 0.f;
}



void A_Utility::Show_Cursor(bool b)
{
#ifdef _WIN32
	if( b == true )//show
		ShowCursor( TRUE );
	else	   //hide
		ShowCursor( FALSE );
#else
	if( b == true )
		;
	else
		;
#endif
}

A_Application::A_Application()
{
	this->Update_Time();
}

A_Application::~A_Application()
{
	this->Shutdown();
	this->Log_File.Close();
}

#ifndef _WIN32

int XError_To_Engine( Display* dpy, XErrorEvent* xev )
{
#ifdef _TEST
	return Engine.X11_Error( dpy, xev );
#endif

	return 0;
}

#endif

#ifdef _WIN32
void A_Application::Initialize(HINSTANCE hInstance)
#else
void A_Application::Initialize()
#endif
{
	this->Log_File.Open( "data/log_main.txt" );
//	if(!this->Key_Log.Open("stuff/data/key_log.txt"))
//		this->Error_Log.Write_Log("Could Not Make Key Log");

	//if(!this->Lua_Log.Open("stuff/data/lua_log.txt"))
//		this->Error_Log.Write_Log("Could Not Make Lua Log");

#ifdef _WIN32
	if(CoInitialize(NULL)==S_FALSE)
	{
		Engine.Register_Error(S_Error("Com Initialize",3));
		return;
	}
#else
	if( XInitThreads() == false )
	{
#ifdef _TEST
		Engine.Register_Error(S_Error( "XInit Threads\n", 3 ));
#endif
		return;
	}

	XSetErrorHandler( XError_To_Engine );
#endif

#ifdef _TEST
	Engine.Log( "Initialized Application\n" );
#endif
}


void A_Application::Shutdown()
{
#ifdef _TEST
	if( Engine.Is_Running == false && this->Has_Shutdown == false )
	{
		this->Has_Shutdown = true;

		this->OpenGL.Destroy();

		this->Window.Destroy_Window();
	}
#endif
}

void A_Application::Update_Base( double dt )
{
	this->Update_Time();
}

void A_Application::Update_Time()
{
#ifdef _WIN32
	GetLocalTime( &this->Time );
#else
	usleep(0);
#endif
}

#endif
