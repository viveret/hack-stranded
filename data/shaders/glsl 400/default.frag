#version 120

// CONSTANTS			  
const int TEXTURE_NONE  = 0x00;
const int TEXTURE_ALL   = 0x01;
const int TEXTURE_RGB   = 0x02;
const int TEXTURE_ALPHA = 0x03;

uniform int Texture_State;


uniform sampler2D tex;
						
varying float Light_intensity;	  
varying vec4 Light_Pos;	 
varying float Light_Dist;
varying vec4 Light_Col;
varying vec3 lightDir,normal;
varying vec4 Vert_Pos;	  
uniform vec4 myAmbient;
uniform float Grey_Scale;

uniform bool Light_Enabled, Fog,Enabled;

void main()
{	   
	vec4 nTex;

	if( Texture_State == TEXTURE_ALL)
	{
		nTex = texture2D(tex,gl_TexCoord[0].st);
	
		nTex.r*=gl_Color.r;
		nTex.g*=gl_Color.g;
		nTex.b*=gl_Color.b;
		nTex.a*=gl_Color.a;
	}
	else if( Texture_State == TEXTURE_NONE )
		nTex = gl_Color;

	else if( Texture_State == TEXTURE_RGB )
	{
		nTex = texture2D(tex,gl_TexCoord[0].st);
	
		nTex.r*=gl_Color.r;
		nTex.g*=gl_Color.g;
		nTex.b*=gl_Color.b;
		nTex.a =gl_Color.a;
	}
	else if( Texture_State == TEXTURE_ALPHA )
	{
		nTex = texture2D(tex,gl_TexCoord[0].st);
	
		nTex.r =gl_Color.r;
		nTex.g =gl_Color.g;
		nTex.b =gl_Color.b;
		nTex.a*=gl_Color.a;
	}

	if(Light_Enabled)
	{				
		float intensity = max(dot(lightDir,normalize(normal)),0.0) * 1.5;
		intensity=0.5+intensity/2.;
		nTex.rgb*=intensity;
	}



	// grey scale
	float Grey_Scale_f = float(Grey_Scale) / 100.0, Average = (nTex.r + nTex.g + nTex.b) / 3.0 * (1.0 - Grey_Scale_f);
					 
	nTex.r = Average + nTex.r * Grey_Scale_f;
	nTex.g = Average + nTex.g * Grey_Scale_f;
	nTex.b = Average + nTex.b * Grey_Scale_f;

	gl_FragColor = nTex;
}
