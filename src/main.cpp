#ifdef _WIN32
#include <process.h>
#endif

//#define _TEST

//#ifdef _TEST
#ifdef _TEST
#include "S_Engine.hpp"
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   GLOBAL VARIABLES
//S_Engine Engine;//main engine
/* ServerStats Server; */
//@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@

//#pragma comment (lib, "freetype2412.lib")
//#pragma comment (lib, "ftgl_static.lib")
void SE_Application::Set_OpenGL_Settings()
{
	segl_Enable(GL_DEPTH_TEST);
	segl_Enable(GL_COLOR_MATERIAL);
	segl_Enable(GL_BLEND);
	segl_Enable(GL_ALPHA_TEST);


	glShadeModel(GL_SMOOTH);
	glClearDepth(1.f);
	glClearColor(0.f,0.f,0.f,0.f);
	glDepthFunc(GL_LEQUAL);
	glDepthMask(GL_TRUE);
	glAlphaFunc(GL_GREATER, 0.1f);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	//glBlendFuncSeparate(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ZERO, GL_ONE);
	glHint(GL_PERSPECTIVE_CORRECTION_HINT,GL_NICEST);
	glClear(GL_DEPTH_BUFFER_BIT);

	segl_Enable(GL_TEXTURE_2D);
	segl_Disable(GL_LIGHTING);
#ifdef _TEST
	Engine.Application.OpenGL.GL_Shader->Change(GL_LIGHTING,Vector3(0,128,0));
#endif
	segl_Disable(GL_FOG);
#ifdef _TEST
	segl_Set1f(SEGL_GREYSCALE, 100.f);
#endif


	/*glLightf( GL_LIGHT0, GL_CONSTANT_ATTENUATION, 0.0f );
	glLightf( GL_LIGHT0, GL_LINEAR_ATTENUATION , 0.0f );
	glLightf( GL_LIGHT0, GL_QUADRATIC_ATTENUATION , 0.0002 );		  */


	SE_Application::Switch_To_2D();
}

void SE_Application::Switch_To_3D()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
#ifdef _TEST
	gluPerspective(45.0f + Engine.Application.Utility.Zoom,(float)Engine.Application.Window.Wind_w/(float)Engine.Application.Window.Wind_h, 0.001f, 256.f );
#endif
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	segl_Enable(GL_DEPTH_TEST);
    //glDepthMask(GL_TRUE);
	//sglEnable(GL_POLYGON_SMOOTH);
    //glBlendFunc(GL_SRC_ALPHA_SATURATE, GL_ONE);
	//APP.UseLighting(true);

	segl_Enable(GL_LIGHTING);
	segl_Enable(GL_FOG);
}

void SE_Application::Switch_To_2D()
{
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
#ifdef _TEST
	glOrtho( 0.f ,Engine.Application.Window.Wind_w, Engine.Application.Window.Wind_h, 0, 0, 1.0);
#endif
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glClear(GL_DEPTH_BUFFER_BIT);
	segl_Disable(GL_DEPTH_TEST);
    //glDepthMask(GL_FALSE);
	//Engine.Application.OpenGL.GL_Shader->Cull(false);

	segl_Disable(GL_LIGHTING);
	segl_Disable(GL_FOG);
    //glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
#ifdef _TEST
void Input_Event_Handler( SE_Input::Input_Event* nEv )
{

}
#endif
#ifdef _TEST
#include "game/Map_Height.hpp"
struct nnRes:public E_Resources::R_Other_Resources
{
	//std::string Test_Stream;

	//SE_Map_Height_Namespace::SE_Map_Height Test_Map;

	void Get_Resources()
	{
		//SE_File_Stream::Def_Files::File_TXT* nStrema = new SE_File_Stream::Def_Files::File_TXT;
		//nStrema->Bytes_Per_Load = 1;// able to see process

		//nStrema->Load( "stuff/test_stream.txt", &this->Test_Stream );
	}
};
#endif
#include "menu/Menu.hpp"
/* 

std::string SE_Menu::Menu_Manager::Menuint_TO_String( uint Which )
{
	return "lol";
}

uint SE_Menu::Menu_Manager::Menu_String_TO_Int( std::string Which )
{
	return 0;
}*/

#ifdef _WIN32
bool E_Game::Obj_Callback( btManifoldPoint& cp, const btCollisionObject* Obj1, int id1, int index1, const btCollisionObject* Obj2, int id2, int index2 )
{
	return true;
}
#endif
#ifdef _TEST
struct nnGame:public E_Game::G_Game
{
	bool Initialize()
	{
		return this->Initialize_Base( false );
	}

	void Destroy()
	{
		this->Destroy_Base();
	}

	void Draw()
	{

	}

	void Draw_HUD()
	{
		//
	}

	void Update( double dt )
	{
		if( Engine.In_Game )
			this->Update_Input( Engine.Thread_Logic.m_Timer.Elapsed_Time );// only get input when in game

		if( Engine.Is_Paused == false )
			this->Update_Base( dt );
	}

	void Update_Input( double dt )
	{
		//
	}

	void Clean()
	{
		//
	}
#ifdef _WIN32
	void Add_Body_Props( btRigidBody* Body )
	{
		//
	}
#endif
};
#endif
void SE_Application::A_Application::Update( double dt )
{
	this->Update_Base( dt );
}
#ifdef _TEST
void S_Engine::Handle_Logic_Event_Ex( SE_Events::Event_Base* Event )
{
	//
}
#endif
int Netcounter = 0;
void SE_Application::String_Submit_Default( std::string Str )
{
	if( Str[0] == '/' )
	{
		if( Str.find("/set name ") != Str.npos )
		{
			Str.erase( Str.begin(), Str.begin() + strlen( "/set name " ) );
#ifdef _TEST
			Engine.Net_Manager->Main_Socket.Display_Name = Str;
#endif
		}
	}
	else
	{
		if( Netcounter == 0 )
		{
#ifdef _TEST
			if( Str == "h" )// host
				Engine.Make_Net_Manager( true );
			else
				Engine.Make_Net_Manager( false );
#endif
		}
#ifdef _TEST
		else if( Engine.Net_Manager->Is_Host )
		{
			switch( Netcounter )
			{
			case 1: Engine.Net_Manager->Con_Port = 7777;/*atoi(Str.c_str());*/
				break;
			case 2: ((SE_Network::SE_Net_Manager_HOST*)Engine.Net_Manager)->Start_Listening_For_Clients();
				break;
			case 3:
			{
				SE_Message_Sys::SE_Message nMsg;
				nMsg.Source = Engine.Net_Manager->Main_Socket.Display_Name;
				nMsg.What = Str;
				nMsg.What_Color.Set( 1, 1, 1 );

				Engine.GUI_Manager.Message_Log.Print_Message( nMsg );

				Netcounter--;
			}break;
			}
		}
		else
		{
			switch( Netcounter )
			{
			case 1: ((SE_Network::SE_Net_Manager_CLIENT*)Engine.Net_Manager)->Con_IP = "127.0.0.1";//Str;
				break;
			case 2: Engine.Net_Manager->Con_Port = 7777;/*atoi(Str.c_str());*/
				break;
			case 3: ((SE_Network::SE_Net_Manager_CLIENT*)Engine.Net_Manager)->Connect();
				break;
			case 4:
			{
				SE_Message_Sys::SE_Message nMsg;
				nMsg.Source = Engine.Net_Manager->Main_Socket.Display_Name;
				nMsg.What = Str;
				nMsg.What_Color.Set( 1, 1, 1 );

				Engine.GUI_Manager.Message_Log.Print_Message( nMsg );

				Netcounter--;
			}break;
			}
		}

		Netcounter++;
#endif
	}
}


/*

#ifdef _WIN32
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd)
#else
int main( int argc, char** argv )
#endif
{
#ifndef _WIN32
	/*if( chdir( "../../" ) != 0 )
	{
		printf("COULD NOT CHANGE WORKING DIRECTORY\n");
		getchar();
		return 1;
	}*//*
#endif

	Engine.Menu.Splash_R = 1.f;
	Engine.Menu.Splash_G = 0.5f;
	Engine.Menu.Splash_B = 0.1f;

	Engine_Descriptor E_Desc = {
		"Survivor Engine Test",
#ifdef _WIN32Tags.size() - 1 ].Size = 0;
	assert( Size != 0 );
		hInstance,
#endif
		false,
		new nnRes,
		Input_Event_Handler,
		"adventure.ttf", 32,
		"pause"
	};

#ifdef _WIN32
	Engine.Start( E_Desc, 1, &lpCmdLine );
#else
	Engine.Start( E_Desc, argc, argv );
#endif

	Engine.Input.Bind_Key( SE_Input_Codes::KEY_0, SE_Network::Input_Start_Host, "start-host", true );
	Engine.Input.Bind_Key( SE_Input_Codes::KEY_9, SE_Network::Input_Start_Client, "start-client", true );

	Engine.Menu.Load_Menu( "title" );// start there



	uint B_Ashes = Engine.Bass_Manager.Load_Stream( "world/AshesPiano.ogg" );
	//Engine.Bass_Manager.Play_Stream( B_Ashes );

	Engine.Content.Mesh_Manager.Load_Mesh( "test/test_mesh.ply" );


#ifdef _WIN32
    MSG msg;
	while(Engine.Is_Running)
    {
		while (PeekMessage(&msg,Engine.Application.Window.hWnd,0,0,PM_REMOVE)>0)
        {
            TranslateMessage (&msg);
            DispatchMessage (&msg);
        }
		Engine.Main_Loop();
    }
#else
	while(Engine.Is_Running)
	{
		Engine.Main_Loop();
	}
#endif

	Engine.Exit();
	return 0;
}

#endif
*/

#endif
