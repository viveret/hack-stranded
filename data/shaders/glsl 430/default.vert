#version 120
varying vec3 outNormal, outPosition;

uniform int Texture_State;  

uniform bool Light_Enabled,Fog_Enabled;	

void main()
{
	outPosition = vec3(gl_Position = gl_ModelViewProjectionMatrix*vec4(gl_Vertex.xyz,1.0)).xyz;
	outNormal = gl_Normal;//gl_NormalMatrix*

	vec4 Calc_Color = gl_Color;

/*	if(Fog_Enabled)
	{
		float FogDist = gl_Position.x*gl_Position.x + gl_Position.z*gl_Position.z;//sqrt

		float FogAlpha=1.;

		if(FogDist>1000.)
			FogAlpha=1.-(FogDist-1000.)/1000.;

		else if(FogDist>400.)
			FogAlpha=0.;

		Calc_Color.a *= FogAlpha;
	}
*/
	
	gl_TexCoord[0] = gl_MultiTexCoord0;
	gl_FrontColor = Calc_Color;
}
