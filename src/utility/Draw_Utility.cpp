#include "Draw_Utility.hpp"
using namespace Draw_Utility;
#include "../gl/se_gl.hpp"

#include "../engine/Engine.hpp"
#include "../math/Math.hpp"

#include "../gui/GUI.hpp"

void Draw_Utility::String_Draw_Core(std::string txt,int x,int y,float r,float g,float b,float a, uchar Pos, float Scale)
{
	if( txt.length() == 0 ) return;

	glPushMatrix();
	
    glColor4f( r,g,b,a );
#ifdef _TEST
	segl_Use_Texture( Engine.Resources.Resource_Req.Text );
#endif

	float Tx=1.f/16.f,Ty=1.f/8.f;
	float Length = txt.length()*8.f;
	
	switch( Pos )
	{
	case SE_Gravity::Mid_Bottom:
		glTranslatef( -Length, 0, 0 );
		break;
	case SE_Gravity::Right_Bottom:
		glTranslatef( -Length * 2.f, 0, 0 );
		break;
	}

    for(unsigned int l=0;l<txt.length();l++)
    {
		Vector2f Pos;
		Pos.y() = uint(txt[l])/16;
		Pos.x() = uint(txt[l])-16*Pos.y();

		if( txt[l] == '\t' )
		{
			glTranslatef( 32, 0, 0 );
			continue;
		}

		float L = l-txt.length()/2.f;

        glBegin(GL_QUADS);
			glTexCoord2f(Pos.x()*Tx,Pos.y()*Ty);glVertex2f(x+8.f-8.f*Scale+L*16.0f*Scale+Length,y+8.f-8.f*Scale);
            glTexCoord2f(Pos.x()*Tx+Tx,Pos.y()*Ty);glVertex2f(x+8.f+8.0f*Scale+L*16.0f*Scale+Length,y+8.f-8.f*Scale);
            glTexCoord2f(Pos.x()*Tx+Tx,Pos.y()*Ty+Ty);glVertex2f(x+8.f+8.0f*Scale+L*16.0f*Scale+Length,y+8.f+8.0f*Scale);
            glTexCoord2f(Pos.x()*Tx,Pos.y()*Ty+Ty);glVertex2f(x+8.f-8.f*Scale+L*16.0f*Scale+Length,y+8.f+8.0f*Scale);
        glEnd();
    }
    glColor4f(1,1,1,1);

	glPopMatrix();
}	 

#include "../render/Font.hpp"

void Draw_Utility::String_Draw(std::string txt,int x,int y,float r,float g,float b,float a, uchar Pos, float Scale)
{ 
	if( txt.length() == 0 ) return;

#ifdef _TEST
	if( Engine.Application.OpenGL.GL_Shader_Is_Supported == false )
	{
		String_Draw_Core( txt, x, y, r, g, b, a, Pos, Scale );
		return;
	}
#endif

	txt += "\n";

	glPushMatrix();	   						
	glTranslatef( (float)x, y + 14.f, 0 );			  
	glScalef( Scale, Scale, 1 );

	DTX::dtx_use_font( NULL, 32 );

//	Engine.Resources.Font_Manager.Use_Font( Engine.SE_Font, 32 );
    glColor4f(r,g,b,a);				 
	//Engine.Resources.Font_Manager.Draw_String( txt, Pos );
	DTX::dtx_string( txt.c_str(), Pos );

    glColor4f(1,1,1,1);
	glPopMatrix();
} 

void Draw_Utility::Splash(std::string Info)
{
    glClear(GL_COLOR_BUFFER_BIT);
	glPushMatrix();		 

    segl_Clear_Screen();


#ifdef _TEST
	Engine.Resources.Use_Texture( Engine.Resources.Resource_Req.GUI_Splash );
#endif

    glBegin(GL_QUADS);
		glTexCoord2f(0, 0);glVertex2d(ScreenW/2-320,ScreenH/2-240);
		glTexCoord2f(1, 0);glVertex2d(ScreenW/2+320,ScreenH/2-240);
		glTexCoord2f(1, 1);glVertex2d(ScreenW/2+320,ScreenH/2+240);
        glTexCoord2f(0, 1);glVertex2d(ScreenW/2-320,ScreenH/2+240);
    glEnd();

    String_Draw(Info,ScreenW/2,ScreenH-22,1,1,1,1, SE_Gravity::Mid_Bottom);


	segl_Disable( GL_TEXTURE_2D );
	// progress bar																		 
	glColor3f( 0.7, 0.7, 0.7 );
	glBegin(GL_QUADS);								   
		glVertex2f( ScreenW/2.f - 300, ScreenH - 60 );
		glVertex2f( ScreenW/2.f + 300, ScreenH - 60 );
		glVertex2f( ScreenW/2.f + 300, ScreenH - 30 );
		glVertex2f( ScreenW/2.f - 300, ScreenH - 30 );
	glEnd();
#ifdef _TEST
	glColor3f( 1, 1, 1 );
	glBegin(GL_QUADS);								   
		glVertex2f( ScreenW/2.f - 300, ScreenH - 60 );
		glVertex2f( ScreenW/2.f - 300 + 600.f * Engine.Content.Percent / Engine.Content.Perc_Max, ScreenH - 60 );
		glVertex2f( ScreenW/2.f - 300 + 600.f * Engine.Content.Percent / Engine.Content.Perc_Max, ScreenH - 30 );
		glVertex2f( ScreenW/2.f - 300, ScreenH - 30 );
	glEnd();




	std::stringstream Draw_Perc;
	Draw_Perc << "Loaded: " << uint( 100.f * Engine.Content.Percent / Engine.Content.Perc_Max ) << "%, ";
	Draw_Perc << Engine.Content.Percent << " out of " << Engine.Content.Perc_Max;
	
	String_Draw(Draw_Perc.str(),ScreenW/2,ScreenH-50,0,0,0,1, SE_GUI::GUI_Gravity::Mid_Bottom );
#endif
    glPopMatrix();
    glFlush();

/*#ifdef _WIN32
	SwapBuffers(Engine.Application.Window.hDC);
#else
#ifdef _TEST
    glXSwapBuffers(Engine.Application.Window.m_dpy, Engine.Application.Window.m_win);
#endif
#endif*/
}																						



/*



  7      int i, j;
  8      for(i = 0; i <= lats; i++) {
  9          double lat0 = M_PI * (-0.5 + (double) (i - 1) / lats);
 10          double z0  = sin(lat0);
 11          double zr0 =  cos(lat0);
    
 12          double lat1 = M_PI * (-0.5 + (double) i / lats);
 13          double z1 = sin(lat1);
 14          double zr1 = cos(lat1);
    
 15          glBegin(GL_QUAD_STRIP);
 16          for(j = 0; j <= longs; j++) {
 17              double lng = 2 * M_PI * (double) (j - 1) / longs;
 18              double x = cos(lng);
 19              double y = sin(lng);
    
 20              glNormal3f(x * zr0, y * zr0, z0);
 21              glVertex3f(x * zr0, y * zr0, z0);
 22              glNormal3f(x * zr1, y * zr1, z1);
              glVertex3f(x * zr1, y * zr1, z1);
           }
           glEnd();
       }


 */
