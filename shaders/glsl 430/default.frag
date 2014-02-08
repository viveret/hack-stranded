#version 120
// CONSTANTS			  
const int TEXTURE_NONE  = 0x00;
const int TEXTURE_ALL   = 0x01;
const int TEXTURE_RGB   = 0x02;
const int TEXTURE_ALPHA = 0x03;

uniform int Texture_State;

uniform float Grey_Scale;
uniform bool Light_Enabled, Fog,Enabled;

varying vec3 outNormal, outPosition;
varying vec3 outLightPos;

uniform sampler2D texture0;
uniform vec3 cameraPosition;

const vec3 ambientColor=vec3(0.05,0.05,0.05);
const vec3 diffuseColor=vec3(0.7,0.7,0.7);
const vec3 specularColor=vec3(1.0,1.0,1.0);

void main()
{
	vec4 nTex;

	vec3 normal=normalize(outNormal);
	vec3 surf2camera=normalize(-outPosition);
	vec3 reflection=-reflect(surf2camera,normal);

	if( Texture_State == TEXTURE_ALL )
	{		  
		nTex = texture2D(texture0,gl_TexCoord[0].st);
	
		nTex.r *= gl_Color.r;
		nTex.g *= gl_Color.g;
		nTex.b *= gl_Color.b;
		nTex.a *= gl_Color.a;

	}
	else if( Texture_State == TEXTURE_RGB )
	{
		nTex = texture2D(texture0,gl_TexCoord[0].st);
	
		nTex.r*=gl_Color.r;
		nTex.g*=gl_Color.g;
		nTex.b*=gl_Color.b;
		nTex.a =gl_Color.a;

	}
	else if( Texture_State == TEXTURE_ALPHA )
	{
		nTex = texture2D(texture0,gl_TexCoord[0].st);
	
		nTex.r =gl_Color.r;
		nTex.g =gl_Color.g;
		nTex.b =gl_Color.b;
		nTex.a*=gl_Color.a;
	}
	else
		nTex = gl_Color;


	if(Light_Enabled)
	{		
		vec3 surf2light=normalize(gl_LightSource[0].position.xyz-outPosition);	

		vec4 ambient_color = gl_FrontMaterial.ambient * gl_LightSource[0].ambient + gl_LightModel.ambient * gl_FrontMaterial.ambient;
		vec4 diffuse_color = gl_FrontMaterial.diffuse * gl_LightSource[0].diffuse;
		vec4 specular_color = gl_FrontMaterial.specular * gl_LightSource[0].specular ;

		float diffuse_value = max(0.0,dot(normal,surf2light));//max(dot(outNormal, surf2light), 0.0);
		float specular_value = pow(max(0.0,dot(reflection,surf2light)),4.0);//*pow(max(dot(outNormal,surf2light),0.0),gl_FrontMaterial.shininess)
	
		//max(intensity * diffuse + spec, ambient);
//		nTex = vec4( ambientColor * nTex.xyz + diffuseContribution * diffuseColor * nTex.xyz + specularContribution * specularColor, nTex.a);
		nTex *= (ambient_color + diffuse_color * diffuse_value + specular_color * specular_value);
	}

	// grey scale
	float Grey_Scale_f = float(Grey_Scale) / 100.0, Average = (nTex.r + nTex.g + nTex.b) / 3.0 * (1.0 - Grey_Scale_f);
					 
	nTex.r = Average + nTex.r * Grey_Scale_f;
	nTex.g = Average + nTex.g * Grey_Scale_f;
	nTex.b = Average + nTex.b * Grey_Scale_f;

	gl_FragColor = nTex;
}
