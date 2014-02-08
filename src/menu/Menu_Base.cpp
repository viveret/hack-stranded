#include "Menu.hpp"
using namespace SE_Menu;
#include "../utility/Draw_Utility.hpp"
using namespace Draw_Utility;	
#include "../gl/se_gl.hpp"


Menu_Base::Menu_Base()
{

}

Menu_Base::~Menu_Base()
{
	this->Clean_Up();
}	  

void Menu_Base::Clean_Up_Base()
{		  
	if( this->Buttons.Data() )
	{
		for(uint x=0;x<this->Buttons.Size();x++)
		{
			SE_Debug::Unwatch( this->Buttons[x] );

			delete this->Buttons[x];
			this->Buttons[x] = NULL;
		}
	} 

	this->Buttons.Delete();
}

void Menu_Base::Clean_Up()
{
	this->Clean_Up_Base();
}

void Menu_Base::Update_Base()
{			
	for(uint b=0;b < this->Buttons.Size();b++)
		if(this->Buttons[b])
			this->Buttons[b]->Update();
}

void Menu_Base::Update()
{	
	this->Update_Base();
}

void Menu_Base::Draw_Base()
{	
	String_Draw(this->Title,ScreenW/2.f, 80, 1, 1,1,1, SE_Gravity::Mid_Bottom, 3.f);

	glPushMatrix();
	glTranslatef( SE_Window::Get_W()/2.f, SE_Window::Get_H()/2.f, 0 );

	for(uint b=0;b<this->Buttons.Size();b++)
		if(this->Buttons[b])
			this->Buttons[b]->Render();

	glPopMatrix();
}

void Menu_Base::Draw()
{
	this->Draw_Base();
}

void Menu_Base::Make_Buttons( u_char Num )
{
	this->Buttons.Make( Num, "button array" );
}

void Menu_Base::Add_Buttons_Mem()// add to engine memory manager
{	
	if( this->Buttons.Data() )
	{
		for( uint x = 0;x < this->Buttons.Size(); x++ )
		{	
			SE_Debug::Watch( this->Buttons[x], "menu button", sizeof(*Buttons[x]) );
		}
	}
}
