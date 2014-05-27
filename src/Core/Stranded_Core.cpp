#include "Stranded_Core.hpp"
#include <SE/module/Module.hpp>
#include <SE/module/Thread.hpp>
#include <SE/debug/Logger.hpp>
#include <SE/os/Input.hpp>
#include <SE/os/Input_Codes.hpp>
#include <SE/module/Event_Defs.hpp>

#include <SE/physics/Physics.hpp>

#include <SE/gl/OpenGL.hpp>
#include <SE/gl/segl.hpp>
#include <SE/Window.hpp>
#include <SE/utility/Draw_Utility.hpp>

#include <SE/filesys/Filesystem.hpp>


#define STR_CAM_TYPE_CHASE	0x00
#define STR_CAM_TYPE_GRAV	0x01

namespace Stranded_Lib
{
	struct Grav_Node;

	struct Dialog_Spot
	{
		Vector2f Pos;
		std::string F_Name;
	};

	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
	void Create_Interface();
	void* Module_Interface( const char* );
	uint Register_Module( const char* );

	SE_Module_ID m_Mod = NULL;

	void Thread_Update();
	void Thread_Render();
//	void thread_Render

	void Render_Grav_Node( Grav_Node* mNode );
	void Update_Grav_Node( Grav_Node* mNode );

	void Thread_Render_Zoom_Push();
	void Thread_Render_Zoom_Pop();

	void Thread_Render_Dialogue();

	void Add_Grav_Node( Grav_Node* What, Grav_Node* To );

	void Create_From_Script( std::string Text );

	std::vector< SE_Physics_Lib::Body_ID > Fuels;

	// Globals
	uchar Camera_Type = 0;
	float Camera_Zoom = 1.f, Camera_Rot = 0.f;

	Grav_Node* Sun = NULL;
	float g_Rotate = 0.f;
	std::string Solar_System_Next, Solar_System_Current;

	bool is_Playing = false, Game_Started = false;
	SE_Physics_Lib::Body_ID Player_Obj = NULL, Plr_Ship = NULL;
	uint Player_Tex = 0;

	m_Dialogue_Man Dialogue;
	void Key_Trigger_Dialogue( sshort* Data );
	void Key_Reset( sshort* Data );

	std::vector< Trigger_Spot > Triggers;

	namespace Resources
	{
		uint Load( const std::vector<std::string>& mArgs );
		uint Ship_Default = 0, Ship_Fly = 0, Ship_Damage = 0;

//		uint Planet_Base_Tex = 0;
		uint Star_Tex = 0;

		uint Dialog_Border = 0, Dialog_Arrow = 0;

		uint Avatars[5];
		uint Planets[8];
	}

#define PLR_SPEED_MAX		.25f
#define PLR_FUEL_USE_MAX	50
#define PLR_FUEL_DRAINAGE	0//.25f
	float Player_Fuel = 80.f, Fuel_Required = 100.f;
	float Fuel_Consumption = 1; // % per second

	float Solar_System_Radius_Sqrd = 1024.f * 1024 * 2;
#define NUM_STARS 1024
	std::vector< Vector2f > Stars;


	// Interface
	SE_Log_Interface* seLog = NULL;
	SE_Physics_Interface* SE_Physics = NULL;
	SE_Filesys_Interface* SE_Filesys = NULL;
	segl_Interface* segl = NULL;
	SE_Window_Interface* SE_Window = NULL;
	SE_Lua_Interface* SE_Lua = NULL;
	SE_GUI_Interface* SE_GUI = NULL;
}
using namespace Stranded_Lib;

#define PLANET_DETAIL 64
namespace Stranded_Planet
{
	Vector2f Verts[ PLANET_DETAIL ];
	void Calculate_Planet();
	void Render_Planet( Grav_Node* mNode );

#define NUM_ELEMENTS 10
	Vector3f Elements[NUM_ELEMENTS];
}


void Stranded_Lib::Start_Game()
{
	is_Playing = Game_Started = true;
	Escaped_Solar_System();
}

uint Stranded_Lib::Initialize( const std::vector<std::string>& mArgs )
{
	srand( time(NULL) );
	S_Engine::Module_Set_Color( "\x1b[35m" );
	SE_Window->Set_Name( "Stranded v2 - Survivor Engine" );

	SE_GUI->Set_Load_Menu( User_Load_Menu );
	SE_GUI->Set_GUI_Background( User_GUI_Background );
	S_Engine::Event_Add_Engine( new SE_Events::Event_Var<std::string>( S_Engine::Get_Module_Generic("gui"), Event_GUI::EVENT_MENU_LOAD, "title" ) );

	SE_Input::Bind_Key( SE_Input_Codes::KEY_T, Key_Trigger_Dialogue, "dialogue-t", true );
	SE_Input::Bind_Key( SE_Input_Codes::KEY_R, Key_Reset, "reset-r", true );

	SE_Lua->Register_Module( Stranded_Lualib, "stranded" );

	for( uint i = 0; i < NUM_STARS; i++ )
	{
		Stars.push_back( Vector2f( rand()%1000 - 500, rand()%1000 - 500 ) );
	}


	// Initialize elements
	Stranded_Planet::Elements[0].Set( 1, 0, 0 );
	Stranded_Planet::Elements[1].Set( 0, 1, 0 );


	Stranded_Planet::Calculate_Planet();

	SE_Physics->Set_Gravity( Vector2f(0, 0), .125f );
	SE_Physics->Set_World_Scale( Vector2f( 32, 32 ) );

	Player_Obj = SE_Physics->Body->Gen_Body( Vector2f(0,0), Vector2f( 0.5f,1 ), SE_Gravity::Mid_Mid, 50, SE_Physics_Lib::SHAPE_RECT );
//	seLog->Printf( "Stranded", "Ship mass: %f\n", *SE_Physics->Body->Inv_Mass( Player_Obj ) );

	Dialogue.Dialogue_Speed = 0.1f;

	SE_Lua->Do_File( "assets/solar_systems/title.lua" );

//	SE_Raw_Buffer File_Buf = SE_Filesys->File_Load_Instant( "solar_systems/rexon_500.txt" );
//	Create_From_Script( File_Buf.Data() );
//	File_Buf.Delete();

	return SE_SUCCESS;
}

uint Resources::Load( const std::vector<std::string>& mArgs )
{
	segl->Set3f( segl_Params::SEGL_CLEARCOLOR, 0.05f, 0.05f, 0.05f );

	Player_Tex = Resources::Ship_Default = segl->Texture_Load( "world/spaceship_default.png" );
	Resources::Ship_Fly = segl->Texture_Load( "world/spaceship_fly.png" );
	Resources::Ship_Damage = segl->Texture_Load( "world/spaceship_crash.png" );

	Resources::Star_Tex = segl->Texture_Load( "world/star.png" );

	Resources::Dialog_Border = segl->Texture_Load( "screen/Dialog_Border.png" );
	Resources::Dialog_Arrow = segl->Texture_Load( "screen/Dialog_Arrow.png" );

	Resources::Avatars[0] = segl->Texture_Load( "screen/Captain_Shapton_Portrait.png" );
	Resources::Avatars[1] = segl->Texture_Load( "screen/General_Gordan_Portrait.png" );
	Resources::Avatars[2] = segl->Texture_Load( "screen/General_Ranson_Portrait.png" );
	Resources::Avatars[3] = segl->Texture_Load( "screen/Jacklyn_Foster_Portrait.png" );
	Resources::Avatars[4] = segl->Texture_Load( "screen/static.png" );

	Resources::Planets[0] = segl->Texture_Load( "planets/earth.png" );
	Resources::Planets[1] = segl->Texture_Load( "planets/moon.png" );
	Resources::Planets[2] = segl->Texture_Load( "planets/mars.png" );
	Resources::Planets[3] = segl->Texture_Load( "planets/mercury.png" );
	Resources::Planets[4] = segl->Texture_Load( "planets/sun.png" );
	Resources::Planets[5] = segl->Texture_Load( "planets/ice.png" );
	Resources::Planets[6] = segl->Texture_Load( "planets/candy_mnm.png" );
	Resources::Planets[7] = segl->Texture_Load( "planets/candy.png" );

//	SE_Physics->Obj_Tex(Player_Obj) = Resources::Ship_Fly;
//	SE_Physics->Color(Player_Obj).Set( 1, 1, 1, 1 );

	return SE_SUCCESS;
}

uint Stranded_Lib::Cleanup()
{
	return SE_SUCCESS;
}

void Stranded_Lib::Thread_Update()
{
	if( Sun )
		Update_Grav_Node( Sun );

	if( is_Playing )
	{
		Vector2<schar> Dir;
/*		if( SE_Input::Is_Down( SE_Input_Codes::KEY_D ) )
			Dir.x() ++;
		if( SE_Input::Is_Down( SE_Input_Codes::KEY_A ) )
			Dir.x() --;*/

		if( SE_Input::Is_Down( SE_Input_Codes::KEY_W ) )
			Dir.y() --;
		if( SE_Input::Is_Down( SE_Input_Codes::KEY_S ) )
			Dir.y() ++;

		if( SE_Input::Is_Down( SE_Input_Codes::KEY_Q ) )
		{
			Camera_Rot += 1;
			if( Camera_Rot > 360 )
				Camera_Rot -= 360;

			Player_Fuel -= PLR_FUEL_DRAINAGE * SE_Thread::DT() * 5.f;
		}

		if( SE_Input::Is_Down( SE_Input_Codes::KEY_E ) )
		{
			Camera_Rot -= 1;
			if( Camera_Rot < 0 )
			Camera_Rot += 360;

			Player_Fuel -= PLR_FUEL_DRAINAGE * SE_Thread::DT() * 5.f;
		}


//		seLog->Printf( "Stranded", "%f: %f, %f\n", Camera_Rot, Cos, Sin );
		if( (Dir.x() != 0 || Dir.y() != 0) && Player_Fuel > 0 )
		{
			Vector2f nForce;
			nForce.x() = Fuel_Consumption * Dir.x() * PLR_SPEED_MAX;
			nForce.y() = Fuel_Consumption * Dir.y() * PLR_SPEED_MAX;

			double Cos = cos( Camera_Rot * 3.1415927f / 180 ), Sin = sin( Camera_Rot * 3.1415927f / 180 );	
			nForce.Set( nForce.x() * Cos + nForce.y() * Sin, nForce.x() * Sin + nForce.y() * Cos );

			SE_Physics->Body->Apply_Force( nForce, Player_Obj );

			Player_Fuel -= Fuel_Consumption * SE_Thread::DT() * PLR_FUEL_DRAINAGE;

			Player_Tex = Resources::Ship_Fly;
		}
		else
			Player_Tex = Resources::Ship_Default;



		if( SE_Input::Is_Down( SE_Input_Codes::KEY_SHIFT_L ) && Fuel_Consumption < PLR_FUEL_USE_MAX )
			Fuel_Consumption += PLR_FUEL_USE_MAX * SE_Thread::DT() / 3.f;

		if( SE_Input::Is_Down( SE_Input_Codes::KEY_CONTROL_L ) && Fuel_Consumption > 0 )
			Fuel_Consumption -= PLR_FUEL_USE_MAX * SE_Thread::DT() / 3.f;

		// Adjust time scale
		if( SE_Input::Is_Down( SE_Input_Codes::KEY_SHIFT_R ) && *SE_Physics->Phys_Speed() < 2 )
			*SE_Physics->Phys_Speed() += SE_Thread::DT() / 2.f;

		if( SE_Input::Is_Down( SE_Input_Codes::KEY_CONTROL_R ) && *SE_Physics->Phys_Speed() > 0.1f )
			*SE_Physics->Phys_Speed() -= SE_Thread::DT() / 2.f;


		if( SE_Input::Is_Down( SE_Input_Codes::KEY_SPACE ) && Player_Fuel > 0 )
		{
			Vector2f* Inv_Vel = SE_Physics->Body->Velocity( Player_Obj );
			Inv_Vel->Normalize();
			*Inv_Vel = *Inv_Vel * -0.5f;

			Player_Fuel -= Fuel_Consumption * SE_Thread::DT() * PLR_FUEL_DRAINAGE;

			SE_Physics->Body->Apply_Force( *Inv_Vel, Player_Obj );
		}

		if( ((sshort)SE_Input::Get( SE_Input_Codes::MOUSE_WHEEL_POS )) < 0 )//&& Camera_Zoom > 0.01f )
		{
			Camera_Zoom *= 1.5f;
			SE_Input::Set( SE_Input_Codes::MOUSE_WHEEL_POS, 0 );
		}
		else if( ((sshort)SE_Input::Get( SE_Input_Codes::MOUSE_WHEEL_POS )) > 0 )// && Camera_Zoom < 10.f )
		{
			Camera_Zoom /= 1.5f;
			SE_Input::Set( SE_Input_Codes::MOUSE_WHEEL_POS, 0 );
		}
	}
	
	for( uint i = 0; i < Triggers.size(); i++ )
	{
		Vector2f Dist = Triggers[i].Pos - *SE_Physics->Body->Pos_Local(Player_Obj);
		if( Dist.x() * Dist.x() + Dist.y() * Dist.y() < Triggers[i].Radius_Sqrd )
		{
			Dialogue.Load( Triggers[i].Conv );
			Triggers.erase( Triggers.begin() + i );
			i--;
		}
	}

	Dialogue.Update( SE_Thread::Get_DT() );
	*SE_Physics->Phys_Enabled() = !Dialogue.In_Dialogue;
	is_Playing = !Dialogue.In_Dialogue && Game_Started;

	if( SE_Physics->Body->Pos_Local(Player_Obj)->x() * SE_Physics->Body->Pos_Local(Player_Obj)->x() +
		SE_Physics->Body->Pos_Local(Player_Obj)->y() * SE_Physics->Body->Pos_Local(Player_Obj)->y() >
		Solar_System_Radius_Sqrd )
	{
		Escaped_Solar_System();
	}
}

void Stranded_Lib::Thread_Render()
{
	glPushMatrix();

	segl->Clear_Screen();

	segl->Use_Texture( Resources::Star_Tex );

	glTranslatef( SE_Window->Get_W() / 2.f, SE_Window->Get_H() / 2.f, 0 );
	glScalef( SE_Window->Get_W() / 500.f, SE_Window->Get_H() / 500.f, 1 );
	glRotatef( Camera_Rot, 0, 0, 1 );

	glColor3f( 1, 1, 1 );

	glBegin( GL_QUADS );
	for( uint i = 0; i < NUM_STARS; i++ )
	{
		glTexCoord2f(0, 0);glVertex2f(-1 + Stars[i].x(),-1 + Stars[i].y());
		glTexCoord2f(1, 0);glVertex2f(+1 + Stars[i].x(),-1 + Stars[i].y());
		glTexCoord2f(1, 1);glVertex2f(+1 + Stars[i].x(),+1 + Stars[i].y());
		glTexCoord2f(0, 1);glVertex2f(-1 + Stars[i].x(),+1 + Stars[i].y());
	}
	glEnd();

	glPopMatrix();
	glPushMatrix();
	
//Draw rest of map
	glTranslatef( SE_Window->Get_W() / 2.f, SE_Window->Get_H() / 2.f, 0 );

	glScalef( Camera_Zoom, Camera_Zoom, 1 );

	segl->Use_Texture( Player_Tex );
	glBegin( GL_QUADS );							   
		glTexCoord2f(0, 0);glVertex2f(-8,-16);
		glTexCoord2f(1, 0);glVertex2f(+8,-16);
		glTexCoord2f(1, 1);glVertex2f(+8,+16);
		glTexCoord2f(0, 1);glVertex2f(-8,+16);
	glEnd();

	glRotatef( Camera_Rot, 0, 0, 1 );
//	glTranslatef( -100, 0, 0 );
//	SE_Physics->Mult_World_Scale();
	glScalef( 32, 32, 1 );

//	SE_Physics->MultGL( Player_Obj );
//	glScalef( -1, -1, 1 );
//	SE_Physics->MultGL( Player_Obj );
//	glScalef( -1, -1, 1 );

	Vector2f* Plr_Pos = SE_Physics->Body->Pos_Local(Player_Obj);
	glTranslatef( -Plr_Pos->x(), -Plr_Pos->y(), 0 );

	segl_Disable( GL_TEXTURE_2D );
	if( Sun && Sun->m_Obj )
		Render_Grav_Node( Sun );

	// Draw solar system boundary
	float Sqrd_Radius = sqrt(Solar_System_Radius_Sqrd);
	glScalef( Sqrd_Radius, Sqrd_Radius, 1 );
	segl_Disable( GL_TEXTURE_2D );
	glColor3f( 1, 0, 0 );
	glBegin( GL_LINES );
		for( uint i = 0; i < PLANET_DETAIL - 1; i++ )
		{
			glVertex2d( Stranded_Planet::Verts[i].x(), Stranded_Planet::Verts[i].y() );
			glVertex2d( Stranded_Planet::Verts[i + 1].x(), Stranded_Planet::Verts[i + 1].y() );
		}
		
		glVertex2d( Stranded_Planet::Verts[PLANET_DETAIL - 1].x(), Stranded_Planet::Verts[PLANET_DETAIL - 1].y() );
		glVertex2d( Stranded_Planet::Verts[0].x(), Stranded_Planet::Verts[0].y() );
	glEnd();
	glColor3f( 1, 1, 1 );

	glPopMatrix();

	if( is_Playing )
	{
	// Draw HUD
		glPushMatrix();
		glTranslatef( SE_Window->Get_W() - SE_Window->Get_W() / 24.f - 8, 0, 0 );
		// Draw Fuel consumption
		segl_Disable( GL_TEXTURE_2D );
		glColor3f( 0.6f, 1.f, 0.3f ); // Background
		glBegin( GL_QUADS );							   
			glTexCoord2f(0, 0);glVertex2f( 8						, SE_Window->Get_H() / 2.f - 210 );
			glTexCoord2f(1, 0);glVertex2f( SE_Window->Get_W() / 24.f, SE_Window->Get_H() / 2.f - 210 );
			glTexCoord2f(1, 1);glVertex2f( SE_Window->Get_W() / 24.f, SE_Window->Get_H() / 2.f + 210 );
			glTexCoord2f(0, 1);glVertex2f( 8						, SE_Window->Get_H() / 2.f + 210 );
		glEnd();

		glColor3f( 0.6f, 0.f, 0.f ); // Amount left
		glBegin( GL_QUADS );		   
			glTexCoord2f(0, 0);glVertex2f( 8						, SE_Window->Get_H() / 2.f + 210 - 420 * (Player_Fuel / Fuel_Required) );
			glTexCoord2f(1, 0);glVertex2f( SE_Window->Get_W() / 24.f, SE_Window->Get_H() / 2.f + 210 - 420 * (Player_Fuel / Fuel_Required) );
			glTexCoord2f(1, 1);glVertex2f( SE_Window->Get_W() / 24.f, SE_Window->Get_H() / 2.f + 210 );
			glTexCoord2f(0, 1);glVertex2f( 8						, SE_Window->Get_H() / 2.f + 210 );
		glEnd();
	
		glColor3f( 0.6f, 0.6f, 0.6f ); // Consumption rate
		glPushMatrix();
		glTranslatef( 0, 210 - 412 * (Fuel_Consumption / PLR_FUEL_USE_MAX), 0 );
		glBegin( GL_QUADS );							   
			glTexCoord2f(0, 0);glVertex2f( 8						, SE_Window->Get_H() / 2.f - 8 );
			glTexCoord2f(1, 0);glVertex2f( SE_Window->Get_W() / 24.f, SE_Window->Get_H() / 2.f - 8 );
			glTexCoord2f(1, 1);glVertex2f( SE_Window->Get_W() / 24.f, SE_Window->Get_H() / 2.f );
			glTexCoord2f(0, 1);glVertex2f( 8						, SE_Window->Get_H() / 2.f );
		glEnd();
		glPopMatrix();

		glColor3f( 1, 1, 1 );

		glPopMatrix();
	}

	if( Dialogue.In_Dialogue )
		Dialogue.Draw();
}

void Stranded_Lib::Render_Grav_Node( Grav_Node* mNode )
{
	assert( mNode != NULL );

	for( uint i = 0; i < mNode->Children.size(); i++ )
		if( mNode == mNode->Children[i] )
			assert( false );
		else
			Render_Grav_Node( mNode->Children[i] );

	Stranded_Planet::Render_Planet( mNode );
}

void Stranded_Lib::Thread_Render_Zoom_Push()
{
	glPushMatrix();

	glTranslatef( SE_Window->Get_W() / 2.f, SE_Window->Get_H() / 2.f, 0 );

	glRotatef( Camera_Rot, 0, 0, 1 );
	glScalef( Camera_Zoom, Camera_Zoom, 1 );

//	SE_Physics->Mult_World_Scale();
//	glScalef( -1, -1, 1 );
//	SE_Physics->MultGL( Player_Obj );
//	glScalef( -1, -1, 1 );

//	glTranslatef( 0, 95, 0 );

//	glScalef(-1,-1,);
//	SE_Physics->MultGL( Player_Obj );
//	glScalef(-1,-1,1);
//
//	glScalef( -1, -1, 1 );
//	SE_Physics->Mult_World_Scale();
	glTranslatef( -SE_Physics->Body->Pos_Local(Player_Obj)->x(), -SE_Physics->Body->Pos_Local(Player_Obj)->y(), 0 );
//	SE_Physics->Mult_World_Scale_Inv();
//	glScalef( -1, -1, 1 );
//	sePrintf( SEPRINT_DEBUG, "%f,%f | ", -SE_Physics->Pos2D(Player_Obj).x(), -SE_Physics->Pos2D(Player_Obj).y() );
}

void Stranded_Lib::Thread_Render_Zoom_Pop()
{
	glPopMatrix();
}

void Stranded_Lib::Thread_Render_Dialogue()
{
//	Dialogue.Draw();
}
// UPDATE

void Stranded_Lib::Update_Grav_Node( Grav_Node* mNode )
{
	for( uint i = 0; i < mNode->Children.size(); i++ )
		Update_Grav_Node( mNode->Children[i] );

	mNode->Rotate += SE_Thread::DT() * 90.f * ( 64.f / mNode->Radius);
	if( mNode->Rotate > 360 )
		mNode -= 360;
}



void Stranded_Lib::Add_Grav_Node( Grav_Node* What, Grav_Node* To )
{
	assert( What != NULL );
	assert( What->m_Obj != NULL );
	To->Children.push_back( What );

//	SE_Physics->Color(What.m_Obj).Set( rand()%128 / 255.f + 0.5f, rand()%128 / 255.f + 0.5f, rand()%128 / 255.f + 0.5f, 1 );
}



// PLANET STUFF
#define PLAN_ANGLE(X) (((float)X / PLANET_DETAIL) * 360.f) // Get angle by point #
void Stranded_Planet::Calculate_Planet()
{
	for( uint i = 0; i < PLANET_DETAIL; i++ )
	{
		Stranded_Planet::Verts[i].Set( cos( PLAN_ANGLE(i) * 3.14 / 180 ),
				sin( PLAN_ANGLE(i) * 3.14 / 180 ) );
	}
}

void Stranded_Planet::Render_Planet( Grav_Node* mNode )
{
	glPushMatrix();

	SE_Physics->Body->MultGL( mNode->m_Obj );
	glScalef( SE_Physics->Body->hSize(mNode->m_Obj)->x(), SE_Physics->Body->hSize(mNode->m_Obj)->x(), 1 );
	glRotatef( (mNode->Rotate + g_Rotate) * 3.14 / 180.f, 0, 0, 1 );

#ifdef STRANDED_PLANET_FLAT
	glColor4fv( Vector4f( 1, 0, 1, 1 ) );//SE_Physics->Color(mNode->m_Obj) );
	glBegin( GL_POLYGON );
		for( uint i = 0; i < PLANET_DETAIL - 1; i++ )
		{
			glVertex2d( Stranded_Planet::Verts[i].x(), Stranded_Planet::Verts[i].y() );
			glVertex2d( Stranded_Planet::Verts[i + 1].x(), Stranded_Planet::Verts[i + 1].y() );
		}
		
		glVertex2d( Stranded_Planet::Verts[PLANET_DETAIL - 1].x(), Stranded_Planet::Verts[PLANET_DETAIL - 1].y() );
		glVertex2d( Stranded_Planet::Verts[0].x(), Stranded_Planet::Verts[0].y() );
	glEnd();
#else
	segl->Use_Texture( Resources::Planets[mNode->Tex] );
	glBegin( GL_QUADS );							   
		glTexCoord2f(0, 0);glVertex2f( -1, -1 );
		glTexCoord2f(1, 0);glVertex2f( +1, -1 );
		glTexCoord2f(1, 1);glVertex2f( +1, +1 );
		glTexCoord2f(0, 1);glVertex2f( -1, +1 );
	glEnd();
#endif


	glPopMatrix();
}


// LEVEL IO

void Stranded_Lib::Create_From_Script( std::string Text )
{
	uchar State = 0;

	for( uint i = 0; Text.length() > i; i++ )
	{
		switch( State )
		{
			case 0: // Script header
			{
				ssize_t Fuel_Start = Text.find(":") + 1;
				std::string Amount_Fuel = Text.substr( Fuel_Start, Text.find("\n") - Fuel_Start );
				Text.erase( 0, Amount_Fuel.length() + Fuel_Start + 1 );
				seLog->Printf( "Stranded", "Amount Fuel: %s\n", Amount_Fuel.c_str() );
				Fuel_Required = atoi( Amount_Fuel.c_str() );

				// Get Player fuel to start with
				ssize_t Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				std::string Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				Player_Fuel = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				// Player position x
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				SE_Physics->Body->Pos_Local(Player_Obj)->x() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Player position y
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				SE_Physics->Body->Pos_Local(Player_Obj)->y() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

//				*SE_Physics->Body->Pos_Local( Player_Obj ) = *SE_Physics->Body->Pos_Local(Player_Obj);

				seLog->Printf( "Stranded", "$(d)PLr x/y: %f, %f\n", SE_Physics->Body->Pos_Local(Player_Obj)->x(), SE_Physics->Body->Pos_Local(Player_Obj)->y() );

				State = 1;
			}break;
			case 1: // Default -> new node
			{
				Grav_Node* nNode = new Grav_Node;
				// Start by getting tree name
				ssize_t Txt_Buf_Pos = Text.find_first_of( '\n' );
				nNode->Name = Text.substr( 0, Txt_Buf_Pos );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				if( Txt_Buf_Pos == 0 )
					continue;

				std::string Txt_Buf;

				// Get origin X
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode->Orbit_Origin.x() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Get origin Y
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode->Orbit_Origin.y() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				// Get radius
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode->Radius = atof( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Get orbit speed
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode->Orbit_Speed = atof( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				// Get elem id
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				// Get mass
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode->Mass = atof( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				Register_Gravnode( nNode );
			}break;
		}
	}
}

void Stranded_Lib::Clear_Solar_System() // Just start fresh
{
	seLog->Printf( "Stranded", "$(d)Clearing solar system...\n" );

	Dialogue.End_Dialogue();
	Triggers.clear();

	// fuel used, player data, dialog 
	while( Fuels.size() > 0 )
	{
		SE_Physics->Body->Delete_Body( Fuels[0] );
		Fuels.erase( Fuels.begin() );
	}

	Player_Fuel = 80.f;
	Fuel_Required = 100.f;
	Fuel_Consumption = 25; // per second

	SE_Physics->Body->Pos_Local( Player_Obj )->Set( 0, 0 );
//	SE_Physics->Body->Velocity( Player_Obj )->Set( 0, 0 );
//	Vector2f Stop;
//	SE_Physics->Body->Set_Velocity( &Stop, Player_Obj );

	Grav_Node* Sun_Temp = Sun;
	Sun = NULL;
	SE_Thread::Cool_Off();
	delete Sun_Temp;

	Solar_System_Next.clear();
}

void Stranded_Lib::Escaped_Solar_System() // Next time!
{
	Solar_System_Radius_Sqrd = -1;
	Vector2f Stop;
	SE_Physics->Body->Set_Velocity( &Stop, Player_Obj );
//	SE_Physics->Body->Pos_Local( Player_Obj )->Set( 0, 0 );

	if( Solar_System_Next.length() > 0 )
	{
		Solar_System_Current = Solar_System_Next;
		seLog->Printf( "Stranded", "Escaped solarsystem!\n" );
		if( SE_Lua->Do_File( "assets/solar_systems/" + Solar_System_Next + ".lua" )
			== SE_SUCCESS )
		{
//			SE_Physics->Body->Set_Velocity( &Stop, Player_Obj );
//			SE_Physics->Body->Pos_Local( Player_Obj )->Set( 0, 0 );
			return;
		}
	}

	// Everything failed, go to title
	seLog->Printf( "Stranded", "Going to title...\n" );
	S_Engine::Event_Add_Engine( new SE_Events::Event_Var<std::string>( S_Engine::Get_Module_Generic("gui"), Event_GUI::EVENT_MENU_LOAD, "title" ) );
	SE_Lua->Do_File( "assets/solar_systems/title.lua" );
}


void Stranded_Lib::Register_Gravnode( Grav_Node* nNode )
{
	nNode->m_Obj = SE_Physics->Body->Gen_Body( nNode->Orbit_Origin, Vector2f( nNode->Radius,nNode->Radius ), SE_Gravity::Mid_Mid, 0, SE_Physics_Lib::SHAPE_ELIPSE );
	nNode->m_Force = SE_Physics->Force_Point_Add( nNode->Orbit_Origin, 1024, nNode->Mass * nNode->Radius * nNode->Radius * .25f );

	if( nNode->Name == "sun" )
	{
		Sun = nNode;
		return;
	}

	Grav_Node* Cur_Node = Sun;
	nNode->Name.erase( 0, nNode->Name.find_first_of('/') + 1 );
	while( nNode->Name.find('/') != std::string::npos )
	{
		Grav_Node* Check_Found = Cur_Node;
		std::string nName = nNode->Name.substr( 0, nNode->Name.find_first_of('/') );
		nNode->Name.erase( 0, nNode->Name.find_first_of('/') + 1 );

		for( uint n = 0; n < Cur_Node->Children.size(); n++ )
		{
			if( Cur_Node->Children[n]->Name == nName )
				Cur_Node = Cur_Node->Children[n];
		}

		if( Check_Found == Cur_Node )
			seLog->Printf( "Stranded", "$(w)Could not find '%s' in '%s'\n", nName.c_str(), Cur_Node->Name.c_str() );
	}

	seLog->Printf( "Stranded", "%s, %f, %f, %f, %f, %f, %f\n", nNode->Name.c_str(),
	nNode->Orbit_Origin.x(), nNode->Orbit_Origin.y(), nNode->Radius,
	nNode->Orbit_Speed, nNode->Mass );
	Add_Grav_Node( nNode, Cur_Node );
}

// DIALOGUE
#include <sstream>
void m_Dialogue_Man::Draw()
{
	glPushMatrix();

/*	std::stringstream Draw_Ship_Pos;
	Draw_Ship_Pos << SE_Physics->Body->Pos_Local(Player_Obj)->x() << ", " << SE_Physics->Body->Pos_Local(Player_Obj)->y() << "\n";
	segl->Render_String( Draw_Ship_Pos.str(), 300, SE_Window->Get_H() - 300, 1, 1, 1, 1, SE_Gravity::Left_Mid, 2.f );
*/
//	segl->Disable( GL_TEXTURE_2D );

//	glColor4f( 0.4f, 0.4f, 0.4f, 0.7f );
	
	segl->Use_Texture( Resources::Dialog_Border );
					  
	glBegin( GL_QUADS );							   
		glTexCoord2f(0, 0);glVertex2f(0, SE_Window->Get_H() - 200);// CHAT BOX
		glTexCoord2f(1, 0);glVertex2f(500, SE_Window->Get_H() - 200);
		glTexCoord2f(1, 1);glVertex2f(500,SE_Window->Get_H());
		glTexCoord2f(0, 1);glVertex2f(0,SE_Window->Get_H());
					  // 6 padding
		glTexCoord2f(0, 0);glVertex2f(0, SE_Window->Get_H() - 140 - 200);// CHAT AVATAR
		glTexCoord2f(1, 0);glVertex2f(140, SE_Window->Get_H() -140 - 200);
		glTexCoord2f(1, 1);glVertex2f(140, SE_Window->Get_H() - 200);
		glTexCoord2f(0, 1);glVertex2f(0,SE_Window->Get_H() - 200);
	glEnd();
							   
	glColor4f( 1, 1, 1, 1 );
						
	this->Dialogue_Mutex.Wait_For_Lock();
	this->Dialogue_Mutex.Lock();

	if( this->Cur_Dialogue.size() > 0 )
	{
		//if( this->Cur_Dialogue[0]->Who < AVATAR_NUM )
		{
			segl->Use_Texture( Resources::Avatars[ this->Cur_Dialogue[0].Who ] );
			glBegin( GL_QUADS );			// 6 padding
				glTexCoord2f(0, 0);glVertex2f(6, SE_Window->Get_H() - 134 - 200);// CHAT AVATAR
				glTexCoord2f(1, 0);glVertex2f(134, SE_Window->Get_H() -134 - 200);
				glTexCoord2f(1, 1);glVertex2f(134, SE_Window->Get_H() - 6 - 200);
				glTexCoord2f(0, 1);glVertex2f(6,SE_Window->Get_H() - 6 - 200);
			glEnd();
		}

		std::string Text_Amount_Draw;
		uint Text_Amount = this->Letter_At;

		Text_Amount_Draw = this->Cur_Dialogue[0].Text.substr( 0, Text_Amount );

		segl->Render_String( Text_Amount_Draw, 4, SE_Window->Get_H() - 140, 1, 1, 1, 1, SE_Gravity::Left_Mid, 1.f );

		if( this->Done_Scrolling )
		{
			segl->Use_Texture( Resources::Dialog_Arrow );
			glBegin( GL_QUADS );			// 6 padding
				glTexCoord2f(0, 0);glVertex2f(500 - 64, SE_Window->Get_H() - 200);// CHAT AVATAR
				glTexCoord2f(1, 0);glVertex2f(500, SE_Window->Get_H() - 200);
				glTexCoord2f(1, 1);glVertex2f(500, SE_Window->Get_H() - 200 + 32);
				glTexCoord2f(0, 1);glVertex2f(500 - 64,SE_Window->Get_H() - 200 + 32);
			glEnd();
		}
	}

	this->Dialogue_Mutex.Unlock();


	glPopMatrix();
}

void m_Dialogue_Man::Play_Tick_Noise()
{

}

void Stranded_Lib::Key_Trigger_Dialogue( sshort* Data )
{
	if( *Data == false )
		return;

	Dialogue.Next_Dialogue();
}

void Stranded_Lib::Key_Reset( sshort* Data )
{
	if( *Data == false )
		return;

	SE_Lua->Do_File( "assets/solar_systems/" + Solar_System_Current + ".lua" );
}

// MODULE
uint Stranded_Lib::Register_Module( const char* )
{
	m_Mod = S_Engine::Generate_Module( "stranded" );


	S_Engine::Register_Module_Engine( m_Mod, S_Engine::Reg_Mod_Time::MED_LEVEL + 50, Initialize, Cleanup );
	S_Engine::Register_Module_Render( m_Mod, S_Engine::Reg_Mod_Time::MED_LEVEL + 50, Resources::Load, NULL );

	S_Engine::Register_Module_Thread_Render( Thread_Render, S_Engine::Module_Positions::POS_MED + 20, 128 );
	S_Engine::Register_Module_Thread_Logic( Thread_Update, S_Engine::Module_Positions::POS_MED + 20, 128 );

	// Set zoom render before / after physics render
	S_Engine::Register_Module_Thread_Render( Thread_Render_Zoom_Push, S_Engine::Module_Positions::POS_MED - 1, 64 );
	S_Engine::Register_Module_Thread_Render( Thread_Render_Zoom_Pop, S_Engine::Module_Positions::POS_MED + 1, 64 );

//	S_Engine::Register_Module_Thread_Render( Thread_Render_Dialogue, S_Engine::Module_Positions::POS_MED + 10, 64 );

	seLog = (SE_Log_Interface*) SE_Mod_Plugin::Get_Plugin( "log", SE_LOG_HEADER_VER );
	segl = (segl_Interface*) SE_Mod_Plugin::Get_Plugin( "segl", SEGL_HEADER_VER );
	SE_GUI = (SE_GUI_Interface*) SE_Mod_Plugin::Get_Plugin( "gui", "awd" );//SE_GUI_HEADER_VER );
	SE_Filesys = (SE_Filesys_Interface*) SE_Mod_Plugin::Get_Plugin( "filesys", SE_FILESYS_HEADER_VER );
	SE_Window = (SE_Window_Interface*) SE_Mod_Plugin::Get_Plugin( "window", SE_WINDOW_HEADER_VER );
	SE_Physics = (SE_Physics_Interface*) SE_Mod_Plugin::Get_Plugin( "physics_2D", SE_PHYSICS_HEADER_VER );
	SE_Lua = (SE_Lua_Interface*) SE_Mod_Plugin::Get_Plugin( "lua", SE_LUA_HEADER_VER );

	return SE_SUCCESS;
}

void Stranded_Lib::Create_Interface()
{

}

void* Stranded_Lib::Module_Interface( const char* )
{
	return segl;
}


extern "C" {

DLL_EXPORT SE_Mod_Plugin_Interface SE_Plugin_Data = {
		__DATE__ ", " __TIME__,
		"stranded-core",
		"Stranded Core Utility",
		Stranded_Lib::Create_Interface,
		Stranded_Lib::Register_Module,
		Stranded_Lib::Module_Interface
};

DLL_EXPORT void* SE_Plugin()
{
	return &SE_Plugin_Data;
}

} // extern "C"
