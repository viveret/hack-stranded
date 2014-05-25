#version 110	
uniform int Texture_State;  

varying float Light_intensity;
varying vec4 Light_Pos;
varying float Light_Dist;
varying vec3 lightDir,normal;
varying vec4 Light_Col;

uniform bool Light_Enabled,Fog_Enabled;	
varying vec4 Vert_Pos; 
//uniform int Grey_Scale;

void main()
{	

	vec4 nCol=gl_Color;

	if(Light_Enabled)
	{
		Light_Pos=gl_LightSource[0].position;

		lightDir = normalize(vec3(gl_LightSource[0].position));
		Light_intensity =dot(lightDir,normalize(gl_Vertex.xyz));
		normal = normalize(gl_NormalMatrix * gl_Normal);
		
		Light_Col=gl_LightSource[0].ambient;

		vec3 LightPos=vec3(gl_LightSource[0].position.x-gl_Vertex.x,gl_LightSource[0].position.y-gl_Vertex.y,gl_LightSource[0].position.z-gl_Vertex.z);
		Light_Dist=LightPos.x*LightPos.x+LightPos.y*LightPos.y+LightPos.z*LightPos.z;//sqrt
	}

	if(Fog_Enabled)
	{
		float FogDist=Vert_Pos.x*Vert_Pos.x+Vert_Pos.z*Vert_Pos.z;//sqrt

		float FogAlpha=1.;

		if(FogDist>1000.)
			FogAlpha=1.-(FogDist-1000.)/1000.;

		else if(FogDist>400.)
			FogAlpha=0.;

		nCol.a*=FogAlpha;
	}
						
	vec4 Pos=vec4(gl_Vertex);
	gl_Position = gl_ModelViewProjectionMatrix * Pos;//ftransform(); 
	
	Vert_Pos=gl_Position;

	gl_FrontColor = nCol;
	gl_TexCoord[0] = gl_MultiTexCoord0;
}