#include "../gl/OpenGL.hpp"
using namespace SE_GL_Interfaces::GL_Default;

#include "../utility/Functions.hpp"

#include "../engine/Engine.hpp"


RTT_Default::RTT_Default()
{
	//
}

bool RTT_Default::Initialize()
{
	return true;
}

void RTT_Default::Destroy()
{
	//
}

void RTT_Default::Start_Render( uint nWidth, uint nHeight, uint To )
{
	//
}

uint RTT_Default::End_Render()
{
	return 0;
}


Shader_Default::Shader_Default()
{
	//
}

bool Shader_Default::Initialize()
{
	return true;
}

void Shader_Default::Destroy()
{
	//
}

void Shader_Default::Change(uint What,uint Value)
{
	switch(What)
	{  
	case GL_VERTEX_ARRAY:
		{
			if( Value == TRUE )
				glEnableClientState( GL_VERTEX_ARRAY );
			else
				glDisableClientState( GL_VERTEX_ARRAY );
		}break;
	case GL_TEXTURE_COORD_ARRAY:
		{										 
			if( Value == TRUE )
				glEnableClientState( GL_TEXTURE_COORD_ARRAY );
			else
				glDisableClientState( GL_TEXTURE_COORD_ARRAY );
		}break;
	case GL_LIGHTING:
		{												
			if(Value)
			{
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
			}
			else
			{
				glDisable(GL_LIGHTING);
				glDisable(GL_LIGHT0);
			}
		}break;	
	case SEGL_GREYSCALE:
		{
			//
		}break;
	default:// assume enableing or disabling
		{												 
			if( Value == TRUE )
				glEnable( What );
			else
				glDisable( What );
		}break;
	}																			  
	
#ifdef _TEST
	Engine.OpenGL_Assert( "Change error: " + To_Str(What) + ", " + To_Str(Value), 1 );
#endif
}

void Shader_Default::Change(uint What,float Value)
{
	switch(What)
	{  
	case SEGL_GREYSCALE:
		{
			//
		}break;
	}																			  
#ifdef _TEST
	Engine.OpenGL_Assert( "Change error: " + To_Str(What) + ", " + To_Str(Value), 1 );
#endif
}

void Shader_Default::Change( uint What,Vector3f Value )
{
	switch(What)
	{
	case GL_LIGHTING:
		{								
			//GLfloat lightpos[] = {Value.x, Value.y, Value.z};
			glLightfv(GL_LIGHT0, GL_POSITION, Value );//lightpos);
		}break;
	}
}

void Shader_Default::Shade_Light( SE_Render_Material::Material_Base* Mat )
{
	//
}

void Shader_Default::Cull(bool Mode)
{  
	if(Mode)
	{
		segl_Enable(GL_CULL_FACE);
		glPolygonMode(GL_BACK,GL_POINT);	
	}
	else
	{
		segl_Disable(GL_CULL_FACE);
		glPolygonMode(GL_BACK,GL_FILL);	
	}
}