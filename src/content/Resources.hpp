#ifndef RESOURCES_H
#define RESOURCES_H

#include "../Core.hpp"
#include "../gl/se_gl.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include <vector>
//#include "../os/Bass.h"

#include <IL/il.h>
#include <IL/ilu.h>
#include <IL/ilut.h>


#include "../render/Font.hpp"

#define ICON_SIZE 16 //16 x 16 icons, 256 in total

namespace E_Resources
{
	typedef unsigned int id;

	struct GL_ID_Pair
	{
		GL_ID_Pair();

		uint m_ID;
		GLuint GL_ID;
	};

	struct GL_Prerender_Hint
	{
		GL_Prerender_Hint();

		id* m_ID;// point to allow changes
		uint w, h;
		void* Data;
		void (*Prerender_Func)(void*);// void* data
	};

	/*    DOES NOT EVER CHANGE    */

	struct R_Res_Req
	{
		R_Res_Req();
		void Get_Resources();// get from virtual folder

		id Text, Error;

		// GUI
		id GUI_Background;
		//screen tops
		id GUI_Splash;
		//sounds
		id GUI_Click;
		//the rest
		id GUI_Buttons,GUI_Scroll_Buttons;
	};

	struct R_Other_Resources
	{
		virtual void Get_Resources() = 0;
	};

	struct R_Resources
	{
		R_Resources();
		~R_Resources();

		bool Initialize();
		void Cleanup();

		void Update();

		R_Res_Req Resource_Req;
		R_Other_Resources* Stuff;

		id Load_Texture( std::string Filename );//load file by extension
		id Load_Texture_BMP(std::string Filename);//load BMP tex, return id
		id Load_Texture_PNG(std::string Filename);//load PNG tex, return id

		uint Texture_Type( std::string F_Name );// get type from extension name
		GLuint IL_Format_GL( uint IL_Format );


		void Use_Texture(id ID);//load texture into opengl, return success

		GLuint Get_GL( id ID );
		id New_Tex();
		bool New_Tex( id Hint );// try to get this texture
		id Add_Tex(GLuint ID);

		void Hint_Prerender( GL_Prerender_Hint& nHint );

		bool Loaded_Necesities;//for splash screen
		bool Load_Fast;//compressing

#ifdef _WIN32
		HGLRC hRC;//loading context
#endif

//		SE_Font::F_Font_Manager Font_Manager;

	private:
		std::vector< GL_ID_Pair > Images;
		id Get_New_ID();

		std::vector< GL_Prerender_Hint > Prerender_Hints;// want to prerender something

		id Current_Texture;//texture currently binded
	};
}


#endif
