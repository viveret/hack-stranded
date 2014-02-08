#include "Stranded_Core.hpp"
#include "../module/Module.hpp"
#include "../module/Thread.hpp"
#include "../os/Input.hpp"
#include "../os/Input_Codes.hpp"

#include "../physics/Physics.hpp"
#include "../physics/Object.hpp"

#include "../gl/OpenGL.hpp"
#include "../gl/se_gl.hpp"
#include "../app/Window.hpp"
#include "../utility/Draw_Utility.hpp"

#include "../filesys/Filesystem.hpp"

#include "../game/Dialogue.hpp"

#define STR_CAM_TYPE_CHASE	0x00
#define STR_CAM_TYPE_GRAV	0x01

namespace Stranded
{
	struct Grav_Node;
	struct m_Dialogue_Man: public SE_Game_Dialogue::Dialogue_Manager
	{
		void Draw();
		void Play_Tick_Noise();
	};

	struct Dialog_Spot
	{
		Vector2f Pos;
		std::string F_Name;
	};

	uint Initialize( const std::vector<std::string>& mArgs );
	uint Cleanup();
	SE_Module_ID m_Mod = NULL;

	void Thread_Update();
	void Thread_Render();

	void Render_Grav_Node( Grav_Node* mNode );
	void Update_Grav_Node( Grav_Node* mNode );

	void Thread_Render_Zoom_Push();
	void Thread_Render_Zoom_Pop();

	void Thread_Render_Dialogue();

	void Add_Grav_Node( const Grav_Node& What, Grav_Node* To );


	void Create_From_Script( std::string Text );

	// Structures
	struct Grav_Node
	{
		Grav_Node();
		~Grav_Node();
		void Cleanup();

		std::vector< Grav_Node > Children;

		SE_Physics::Obj_ID m_Obj;
		float Radius, Orbit_Speed, Orbit_Location, Mass;
		Vector2f Orbit_Origin;

		bool Render; // within player, so render
		std::string Name;
	};

	std::vector< SE_Physics::Obj_ID > Fuels;

	// Globals
	uchar Camera_Type = 0;
	float Camera_Zoom = 1.f, Camera_Rot = 0.f;

	Grav_Node Sun;

	SE_Physics::Obj_ID Player_Obj = NULL, Plr_Ship = NULL;

	m_Dialogue_Man Dialogue;

	namespace Resources
	{
		uint Load( const std::vector<std::string>& mArgs );
		uint Ship_Default = 0, Ship_Fly = 0, Ship_Damage = 0;

		uint Planet_Base_Tex = 0;
		uint Star_Tex = 0;

		uint Dialog_Border = 0, Dialog_Arrow = 0;

		uint Avatars[5];
	}


#define PLR_FUEL_MAX 1000.f
#define PLR_FUEL_USE 0.0f
	float Player_Fuel = 80.f, Fuel_Required = PLR_FUEL_MAX;

#define NUM_STARS 1024
	std::vector< Vector2f > Stars;
}
using namespace Stranded;

#define PLANET_DETAIL 64
namespace Stranded_Planet
{
	Vector2f Verts[ PLANET_DETAIL ];
	void Calculate_Planet();
	void Render_Planet( Grav_Node* mNode );

#define NUM_ELEMENTS 10
	Vector3f Elements[NUM_ELEMENTS];
}




uint Stranded::Initialize( const std::vector<std::string>& mArgs )
{
	S_Engine::Module_Set_Color( "\x1b[35m" );
	sePrintf( SEPRINT_MODULE, "In Stranded\n" );


	for( uint i = 0; i < NUM_STARS; i++ )
	{
		Stars.push_back( Vector2f( rand()%1000 - 500, rand()%1000 - 500 ) );
	}


	// Initialize elements
	Stranded_Planet::Elements[0].Set( 1, 0, 0 );
	Stranded_Planet::Elements[1].Set( 0, 1, 0 );


	Stranded_Planet::Calculate_Planet();

	SE_Physics::Set_World_Scale( Vector2f( 32, 32 ) );

	Player_Obj = SE_Physics::Gen_Object( Vector2f(0,0), Vector2f( 0.5f,1 ), -1, SE_PHYS_SHAPE_RECT, &Player_Obj );
	SE_Physics::Renderable( Player_Obj ) = false;
//	Player_Obj->mbt_Body->setAngularFactor(btVector3( 0, 0, 0 ));
//	Player_Obj->mbt_Body->setLinearFactor(btVector3( 0, 0, 0 ));

	SE_Raw_Buffer File_Buf = SE_Filesys::File_Load_Instant( "solar_systems/rexon_500.txt" );
	Create_From_Script( File_Buf.Data() );
	File_Buf.Delete();

	return SE_SUCCESS;
}

uint Resources::Load( const std::vector<std::string>& mArgs )
{
	segl_Set3f( SEGL_CLEARCOLOR, 0.05f, 0.05f, 0.05f );

	Resources::Ship_Default = segl::Texture_Load( "world/spaceship_default.png" );
	Resources::Ship_Fly = segl::Texture_Load( "world/spaceship_fly.png" );
	Resources::Ship_Damage = segl::Texture_Load( "world/spaceship_crash.png" );

	Resources::Star_Tex = Texture_Load( "world/star.png" );

	Resources::Dialog_Border = Texture_Load( "screen/Dialog_Border.png" );
	Resources::Dialog_Arrow = Texture_Load( "screen/Dialog_Arrow.png" );

	Resources::Avatars[0] = Texture_Load( "screen/Captain_Shapton_Portrait.png" );
	Resources::Avatars[1] = Texture_Load( "screen/General_Gord_Portrait.png" );
	Resources::Avatars[2] = Texture_Load( "screen/Jacklyn_Foster_Portrait.png" );

	Resources::Planet_Base_Tex = Texture_Load( "world/Base_Texture.png" );

	SE_Physics::Obj_Tex(Player_Obj) = Resources::Ship_Fly;
	SE_Physics::Color(Player_Obj).Set( 1, 1, 1, 1 );

	return SE_SUCCESS;
}

uint Stranded::Cleanup()
{
	return SE_SUCCESS;
}

void Stranded::Thread_Update()
{
	Vector2<schar> Dir;
	if( SE_Input::Is_Down( SE_Input_Codes::KEY_D ) )
		Dir.x()++;
	if( SE_Input::Is_Down( SE_Input_Codes::KEY_A ) )
		Dir.x()--;

	if( SE_Input::Is_Down( SE_Input_Codes::KEY_S ) )
		Dir.y()++;
	if( SE_Input::Is_Down( SE_Input_Codes::KEY_W ) )
		Dir.y()--;

	if( SE_Input::Is_Down( SE_Input_Codes::KEY_Q ) )
	{
		Camera_Rot += 1;
		if( Camera_Rot > 360 )
			Camera_Rot -= 360;
	}

	if( SE_Input::Is_Down( SE_Input_Codes::KEY_E ) )
	{
		Camera_Rot -= 1;
		if( Camera_Rot < 0 )
			Camera_Rot += 360;
	}
	
	if( (Dir.x() != 0 || Dir.y() != 0) && Player_Fuel > 0 )
	{
		float Angle_X = Camera_Rot * -Dir.x();
		float Angle_Y = Camera_Rot * Dir.y() - 90;

		if( Dir.x() != 0 )
			SE_Physics::Apply_Force( Vector2f( cos( Angle_X * 3.14f / 180 ), sin( Angle_X * 3.14f / 180 )), Player_Obj );
		if( Dir.y() != 0 )
			SE_Physics::Apply_Force( Vector2f( cos( Angle_Y * 3.14f / 180 ), sin( Angle_Y * 3.14f / 180 )), Player_Obj );

		Player_Fuel -= PLR_FUEL_USE;

		SE_Physics::Obj_Tex( Player_Obj ) = Resources::Ship_Fly;
	}
	else
		SE_Physics::Obj_Tex( Player_Obj ) = Resources::Ship_Default;



	if( SE_Input::Is_Down( SE_Input_Codes::KEY_SHIFT_L ) && Player_Fuel > 0 )
	{
		Vector2f Inv_Vel = SE_Physics::Get_Velocity2D( Player_Obj );
		Inv_Vel.Normalize();
		Inv_Vel = Inv_Vel * -0.1f;

		Player_Fuel -= PLR_FUEL_USE;

		SE_Physics::Apply_Force( Inv_Vel, Player_Obj );
	}

	if( ((sshort)SE_Input::Get( SE_Input_Codes::MOUSE_WHEEL_POS )) < 0 )//&& Camera_Zoom > 0.01f )
		Camera_Zoom *= 1.5f;
	else if( ((sshort)SE_Input::Get( SE_Input_Codes::MOUSE_WHEEL_POS )) > 0 )// && Camera_Zoom < 10.f )
		Camera_Zoom /= 1.5f;

	SE_Input::Set( SE_Input_Codes::MOUSE_WHEEL_POS, 0 );
	
	Dialogue.Update( SE_Thread::Get_DT() );
}

void Stranded::Thread_Render()
{
	glPushMatrix();

	segl_Use_Texture( Resources::Star_Tex );

	glTranslatef( SE_Window::Get_W() / 2.f, SE_Window::Get_H() / 2.f, 0 );
	glScalef( SE_Window::Get_W() / 500.f, SE_Window::Get_H() / 500.f, 1 );
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

	glTranslatef( SE_Window::Get_W() / 2.f, SE_Window::Get_H() / 2.f, 0 );

	glScalef( Camera_Zoom, Camera_Zoom, 1 );

	segl_Use_Texture( SE_Physics::Obj_Tex(Player_Obj) );
	glBegin( GL_QUADS );							   
		glTexCoord2f(0, 0);glVertex2f(-8,-16);
		glTexCoord2f(1, 0);glVertex2f(+8,-16);
		glTexCoord2f(1, 1);glVertex2f(+8,+16);
		glTexCoord2f(0, 1);glVertex2f(-8,+16);
	glEnd();

	glRotatef( Camera_Rot, 0, 0, 1 );
//	glTranslatef( -100, 0, 0 );
	SE_Physics::Mult_World_Scale();

//	SE_Physics::MultGL( Player_Obj );
//	glScalef( -1, -1, 1 );
//	SE_Physics::MultGL( Player_Obj );
//	glScalef( -1, -1, 1 );

	Vector2f Plr_Pos = SE_Physics::Pos2D(Player_Obj);
	glTranslatef( -Plr_Pos.x(), -Plr_Pos.y(), 0 );

	segl_Disable( GL_TEXTURE_2D );
	if( Sun.m_Obj )
		Render_Grav_Node( &Sun );

	glPopMatrix();
}

void Stranded::Render_Grav_Node( Grav_Node* mNode )
{
	for( uint i = 0; i < mNode->Children.size(); i++ )
		Render_Grav_Node( &mNode->Children[i] );

	Stranded_Planet::Render_Planet( mNode );
}

void Stranded::Thread_Render_Zoom_Push()
{
	glPushMatrix();

	glTranslatef( SE_Window::Get_W() / 2.f, SE_Window::Get_H() / 2.f, 0 );

	glRotatef( Camera_Rot, 0, 0, 1 );
	glScalef( Camera_Zoom, Camera_Zoom, 1 );

//	SE_Physics::Mult_World_Scale();
//	glScalef( -1, -1, 1 );
//	SE_Physics::MultGL( Player_Obj );
//	glScalef( -1, -1, 1 );

//	glTranslatef( 0, 95, 0 );

//	glScalef(-1,-1,1);
//	SE_Physics::MultGL( Player_Obj );
//	glScalef(-1,-1,1);
//
//	glScalef( -1, -1, 1 );
//	SE_Physics::Mult_World_Scale();
	glTranslatef( -SE_Physics::Pos2D(Player_Obj).x(), -SE_Physics::Pos2D(Player_Obj).y(), 0 );
//	SE_Physics::Mult_World_Scale_Inv();
//	glScalef( -1, -1, 1 );
//	sePrintf( SEPRINT_DEBUG, "%f,%f | ", -SE_Physics::Pos2D(Player_Obj).x(), -SE_Physics::Pos2D(Player_Obj).y() );
}

void Stranded::Thread_Render_Zoom_Pop()
{
	glPopMatrix();
}

void Stranded::Thread_Render_Dialogue()
{
	Dialogue.Draw();
}
// UPDATE

void Stranded::Update_Grav_Node( Grav_Node* mNode )
{
	for( uint i = 0; i < mNode->Children.size(); i++ )
		Update_Grav_Node( &mNode->Children[i] );

	// do stuff
}


// GRAVITY NODE
Grav_Node::Grav_Node()
{
	this->Radius = 1000;
	this->Orbit_Speed = 10;
	this->m_Obj = NULL;

	this->Render = true;
}

Grav_Node::~Grav_Node()
{
//	this->Cleanup();
}

void Grav_Node::Cleanup()
{
	for( uint i = 0; i < this->Children.size(); i++ )
		this->Children[i].Cleanup();

	this->Children.clear();

	if( this->m_Obj )
	{
		SE_Physics::Del_Object( this->m_Obj );
		this->m_Obj = NULL;
	}
}

void Stranded::Add_Grav_Node( const Grav_Node& What, Grav_Node* To )
{
	assert( What.m_Obj != NULL );
	To->Children.push_back( What );

	SE_Physics::Color(What.m_Obj).Set( rand()%128 / 255.f + 0.5f, rand()%128 / 255.f + 0.5f, rand()%128 / 255.f + 0.5f, 1 );
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

	glColor4fv( SE_Physics::Color(mNode->m_Obj) );
	SE_Physics::MultGL( mNode->m_Obj );
	glScalef( SE_Physics::Size2D(mNode->m_Obj).x()/4.f, SE_Physics::Size2D(mNode->m_Obj).x()/4.f, 1 );

	glBegin( GL_POLYGON );
		for( uint i = 0; i < PLANET_DETAIL - 1; i++ )
		{
			glVertex2d( Stranded_Planet::Verts[i].x(), Stranded_Planet::Verts[i].y() );
			glVertex2d( Stranded_Planet::Verts[i + 1].x(), Stranded_Planet::Verts[i + 1].y() );
		}
		
		glVertex2d( Stranded_Planet::Verts[PLANET_DETAIL - 1].x(), Stranded_Planet::Verts[PLANET_DETAIL - 1].y() );
		glVertex2d( Stranded_Planet::Verts[0].x(), Stranded_Planet::Verts[0].y() );
	glEnd();

	glPopMatrix();
}


// LEVEL IO

void Stranded::Create_From_Script( std::string Text )
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
				sePrintf( SEPRINT_MODULE, "Amount Fuel: %s\n", Amount_Fuel.c_str() );
				Fuel_Required = atoi( Amount_Fuel.c_str() );

				// Player position x
				ssize_t Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				std::string Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				SE_Physics::Pos2D(Player_Obj).x() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Player position y
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				SE_Physics::Pos2D(Player_Obj).y() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				SE_Physics::Set_Pos( SE_Physics::Pos2D(Player_Obj), Player_Obj );

				sePrintf( SEPRINT_DEBUG, "PLr x/y: %f, %f\n", SE_Physics::Pos2D(Player_Obj).x(), SE_Physics::Pos2D(Player_Obj).y() );

				State = 1;
			}break;
			case 1: // Default -> new node
			{
				Grav_Node nNode;
				// Start by getting tree name
				ssize_t Txt_Buf_Pos = Text.find_first_of( '\n' );
				nNode.Name = Text.substr( 0, Txt_Buf_Pos );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				if( Txt_Buf_Pos == 0 )
					continue;

				std::string Txt_Buf;

				// Get origin X
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode.Orbit_Origin.x() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Get origin Y
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode.Orbit_Origin.y() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				// Get radius
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode.Radius = atof( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Get orbit speed
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode.Orbit_Speed = atof( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				// Get elem id
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
//				nNode.Orbit_Origin.x() = atoi( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				// Get mass
				Txt_Buf_Pos = Text.find_first_of( ':' );
				Text.erase( 0, Txt_Buf_Pos + 1 );
				Txt_Buf_Pos = Text.find_first_of( '\n' );
				Txt_Buf = Text.substr( 0, Txt_Buf_Pos );
				nNode.Mass = atof( Txt_Buf.c_str() );
				Text.erase( 0, Txt_Buf_Pos + 1 );

				nNode.m_Obj = SE_Physics::Gen_Object( nNode.Orbit_Origin, Vector2f( nNode.Radius,nNode.Radius ), 0, SE_PHYS_SHAPE_ELIPSE );
				SE_Physics::Renderable(nNode.m_Obj) = false;
//				SE_Physics::Color( nNode.m_Obj ).Set( 0.3f, 0.3f, 0.3f, 1 );
				SE_Physics::Attract( nNode.m_Obj ) = nNode.Mass;

				if( nNode.Name == "sun" )
				{
					Sun = nNode;
					continue;
				}

				Grav_Node* Cur_Node = &Sun;
				nNode.Name.erase( 0, nNode.Name.find_first_of('/') + 1 );
				while( nNode.Name.find('/') != std::string::npos )
				{
					Grav_Node* Check_Found = Cur_Node;
					std::string nName = nNode.Name.substr( 0, nNode.Name.find_first_of('/') );
					nNode.Name.erase( 0, nNode.Name.find_first_of('/') + 1 );

					for( uint n = 0; n < Cur_Node->Children.size(); n++ )
					{
						if( Cur_Node->Children[n].Name == nName )
							Cur_Node = &Cur_Node->Children[n];
					}

					if( Check_Found == Cur_Node )
						sePrintf( SEPRINT_WARN, "Could not find '%s' in '%s'\n", nName.c_str(), Cur_Node->Name.c_str() );
				}

				sePrintf( SEPRINT_MODULE, "%s, %f, %f, %f, %f, %f, %f\n", nNode.Name.c_str(),
						nNode.Orbit_Origin.x(), nNode.Orbit_Origin.y(), nNode.Radius,
						nNode.Orbit_Speed, nNode.Mass );
				Add_Grav_Node( nNode, Cur_Node );
			}break;
		}
	}
}

// DIALOGUE
#include <sstream>
void m_Dialogue_Man::Draw()
{
	glPushMatrix();

	std::stringstream Draw_Ship_Pos;
	Draw_Ship_Pos << SE_Physics::Pos2D(Player_Obj).x() << ", " << SE_Physics::Pos2D(Player_Obj).y() << "\n";
	Draw_Utility::String_Draw( Draw_Ship_Pos.str(), 300, SE_Window::Get_H() - 300, 1, 1, 1, 1, SE_Gravity::Left_Mid, 2.f );

//	segl_Disable( GL_TEXTURE_2D );

//	glColor4f( 0.4f, 0.4f, 0.4f, 0.7f );
	
	segl_Use_Texture( Resources::Dialog_Border );
					  
	glBegin( GL_QUADS );							   
		glTexCoord2f(0, 0);glVertex2f(0, SE_Window::Get_H() - 200);// CHAT BOX
		glTexCoord2f(1, 0);glVertex2f(500, SE_Window::Get_H() - 200);
		glTexCoord2f(1, 1);glVertex2f(500,SE_Window::Get_H());
		glTexCoord2f(0, 1);glVertex2f(0,SE_Window::Get_H());
					  // 6 padding
		glTexCoord2f(0, 0);glVertex2f(0, SE_Window::Get_H() - 140 - 200);// CHAT AVATAR
		glTexCoord2f(1, 0);glVertex2f(140, SE_Window::Get_H() -140 - 200);
		glTexCoord2f(1, 1);glVertex2f(140, SE_Window::Get_H() - 200);
		glTexCoord2f(0, 1);glVertex2f(0,SE_Window::Get_H() - 200);
	glEnd();
							   
	glColor4f( 1, 1, 1, 1 );
						
	this->Dialogue_Mutex.Wait_For_Lock();
	this->Dialogue_Mutex.Lock();

	if( this->Cur_Dialogue.size() > 0 )
	{
		//if( this->Cur_Dialogue[0]->Who < AVATAR_NUM )
		{
			segl_Use_Texture( Resources::Avatars[ this->Cur_Dialogue[0]->Who ] );
			glBegin( GL_QUADS );			// 6 padding
				glTexCoord2f(0, 0);glVertex2f(6, SE_Window::Get_H() - 134 - 200);// CHAT AVATAR
				glTexCoord2f(1, 0);glVertex2f(134, SE_Window::Get_H() -134 - 200);
				glTexCoord2f(1, 1);glVertex2f(134, SE_Window::Get_H() - 6 - 200);
				glTexCoord2f(0, 1);glVertex2f(6,SE_Window::Get_H() - 6 - 200);
			glEnd();
		}

		std::string Text_Amount_Draw;
		uint Text_Amount = this->Letter_At;

		Text_Amount_Draw = this->Cur_Dialogue[0]->Text.substr( 0, Text_Amount );

		Draw_Utility::String_Draw( Text_Amount_Draw, 4, SE_Window::Get_H() - 140, 1, 1, 1, 1, SE_Gravity::Left_Mid, 2.f );

		if( this->Done_Scrolling )
		{
			segl_Use_Texture( Resources::Dialog_Arrow );
			glBegin( GL_QUADS );			// 6 padding
				glTexCoord2f(0, 0);glVertex2f(500 - 64, SE_Window::Get_H() - 200);// CHAT AVATAR
				glTexCoord2f(1, 0);glVertex2f(500, SE_Window::Get_H() - 200);
				glTexCoord2f(1, 1);glVertex2f(500, SE_Window::Get_H() - 200 + 32);
				glTexCoord2f(0, 1);glVertex2f(500 - 64,SE_Window::Get_H() - 200 + 32);
			glEnd();
		}
	}

	this->Dialogue_Mutex.Unlock();


	glPopMatrix();
}

void m_Dialogue_Man::Play_Tick_Noise()
{

}

// MODULE
void Stranded::Register_Module()
{
	m_Mod = S_Engine::Generate_Module( "stranded" );

	SE_Window::Set_Name( "Stranded - Survivor Engine" );

	S_Engine::Register_Module_Engine( m_Mod, S_Engine::Reg_Mod_Time::MED_LEVEL + 50, Initialize, Cleanup );
	S_Engine::Register_Module_Render( m_Mod, S_Engine::Reg_Mod_Time::MED_LEVEL + 50, Resources::Load, NULL );

	S_Engine::Register_Module_Thread_Render( Thread_Render, S_Engine::Module_Positions::POS_MED + 20, 128 );
	S_Engine::Register_Module_Thread_Logic( Thread_Update, S_Engine::Module_Positions::POS_MED + 20, 128 );

	// Set zoom render before / after physics render
	S_Engine::Register_Module_Thread_Render( Thread_Render_Zoom_Push, S_Engine::Module_Positions::POS_MED - 1, 64 );
	S_Engine::Register_Module_Thread_Render( Thread_Render_Zoom_Pop, S_Engine::Module_Positions::POS_MED + 1, 64 );

//	S_Engine::Register_Module_Thread_Render( Thread_Render_Dialogue, S_Engine::Module_Positions::POS_MED + 10, 64 );
}
