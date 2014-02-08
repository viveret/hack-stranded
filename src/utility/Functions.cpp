#include "Functions.hpp"
#include "../math/Math.hpp"  

#include <cstdlib>
#include <cstring>
#include <cstdio>
/*#include <Windows.h>
#include "App.h"
extern myApp APP;

using namespace DrawFunctions;

GLuint UtilityIO::GetTexture(std::string Filename,bool ShowSplash)
{
    if(ShowSplash){ShowSplashScreen("Loading Image: "+Filename);}
	GLuint tex_ID =0;
	tex_ID= SOIL_load_OGL_texture
    (
      Filename.c_str(),
      SOIL_LOAD_AUTO,
      SOIL_CREATE_NEW_ID,
	  SOIL_FLAG_MIPMAPS | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT// | SOIL_FLAG_INVERT_Y
    );
    if(glGetError()!=GL_NO_ERROR)
    {
        std::string Mess((const char*)gluErrorString(glGetError()));
        APP.LoadLog.Write((Mess+":"+Filename).c_str());
    }
    return tex_ID;
}

void reshape(int w, int h)
{
    glViewport(0,0,w,h);
    APP.Screenh=h;
	APP.Screenw=w;
	if(APP.Playing)
	{
        SetCursorPos(APP.Screenw/2, APP.Screenh/2);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f,APP.Screenw* (1.0f/APP.Screenh),VIEW_MIN_DIST,VIEW_MAX_DIST);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	}
	else if(!APP.Playing)
	{
	    glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(0.0, APP.Screenw, APP.Screenh, 0.0, -1.0, 10.0);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
	}
}

#include "ImageHolder.h"
//extern ImageHolder Images;
#include "Resources.h"
extern Struct_Resources Resources;

void DrawFunctions::ShowSplashScreen(std::string Loading)
{
    glPushMatrix();
        glClear(GL_COLOR_BUFFER_BIT);
        glLoadIdentity();
        glClearColor(0,0,0,0);
        glPushAttrib(GL_ENABLE_BIT);
		APP.Texture2D(true);
        sglEnable(GL_BLEND);


        const int ScreenW=APP.Screenw,ScreenH=APP.Screenh;

		Resources.Stuff.Use_Texture(Resources.GUI.Splash);
        glBegin(GL_QUADS);
            glTexCoord2f(0, 0);glVertex2f(ScreenW/2-320.0f,ScreenH/2-240.0f);
            glTexCoord2f(1, 0);glVertex2f(ScreenW/2+320.0f,ScreenH/2-240.0f);
            glTexCoord2f(1, 1);glVertex2f(ScreenW/2+320.0f,ScreenH/2+240.0f);
            glTexCoord2f(0, 1);glVertex2f(ScreenW/2-320.0f,ScreenH/2+240.0f);
        glEnd();

        DrawFunctions::DrawString(Loading,(int)(ScreenW/2-(Loading.length()/2.0f)*16),ScreenH-22,1,1,1,0);
    glPopMatrix();

    glFlush();
    SwapBuffers(APP.hDC);
}


std::string CommandPrep(std::string What)
{
	std::string Ret;
	for(unsigned int l=0;l<What.length();l++)
	{
		if(What[l]!=' ')
		{
			Ret+=tolower(What[l]);
		}
	}
	return Ret;
}

#include "ServerStats.h"
extern ServerStats Server;
#include "ScreenClass.h"
extern Screen MainScreen;

void CheckServerReadys()
{
	if(Server.ReadyToStartGame==1){MainScreen.LoadScreen("play/story");Server.ReadyToStartGame=2;}
	//else if()
}*/


   /*
std::string ReadTextFile(std::string Dest)
{
	std::ifstream Reader(Dest,std::ios::binary);
	std::stringstream Report;
				 
	Reader.seekg(0,std::ios::end);
	unsigned int Length=(unsigned int)Reader.tellg();
	char *Buf=new char[Length+1];
	Buf[Length]='\0';
	Reader.seekg(0,std::ios::beg);

	Report<<"Loading: "<<Dest<<", Size: "<<Length;

	Reader.read(Buf,Length);

	//Report
	std::string Ret(Buf);
	delete[] Buf;

	APP.LoadLog.Write(Report.str().c_str());

	return Ret;
}						*/


void memftoa(char* Dest,float* Source)
{
	memcpy(Dest,(const char*)Source,4);
}

void mematof(float* Dest,char* Source)
{
	memcpy(Dest,Source,4);
}


std::string To_Str( uint Value, int Radix)
{
	char Buf[16];memset(Buf,0,16);

#ifdef _WIN32
	itoa(Value,Buf,Radix);
#else
	switch( Radix )
	{
	case 16:sprintf(Buf,"0x%x",Value);break;
	default:sprintf(Buf,"%d",Value);break;
	}
#endif

	return std::string(Buf);
}	   

std::string To_Str(int Value, int Radix)
{							  
	char Buf[16];memset(Buf,0,16);

#ifdef _WIN32
	itoa(Value,Buf,Radix);
#else
	switch( Radix )
	{
	case 16:sprintf(Buf,"0x%x",Value);break;
	default:sprintf(Buf,"%d",Value);break;
	}
#endif

	return std::string(Buf);
}

std::string To_Str(float Value)
{					
	char Buf[24];memset(Buf,0,24);
	sprintf(Buf,"%f",Value);

	return std::string(Buf);
}

#include <sstream>
std::string ftostr( float Value )
{
	std::stringstream Buf;
	Buf << Value;

	return Buf.str();
}

void replaceAll(std::string& str, const std::string& from, const std::string& to)
{
    if(from.empty())
        return;
    size_t start_pos = 0;
    while((start_pos = str.find(from, start_pos)) != std::string::npos) {
        str.replace(start_pos, from.length(), to);
        start_pos += to.length(); // In case 'to' contains 'from', like replacing 'x' with 'yx'
    }
}
