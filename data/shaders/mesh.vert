varying vec3 outNormal;
varying vec3 outTangent;
varying vec3 outColor;
varying vec2 outUV;
varying vec3 position;

varying vec3 outLightPos;

attribute vec3 vertex;
attribute vec3 normal;
attribute vec3 color;
attribute vec3 tangent;
attribute vec2 UV;

void main()
{
	gl_Position = gl_ModelViewProjectionMatrix*vec4(vertex,1.0);
	position = vec3(gl_ModelViewMatrix*vec4(vertex,1.0));
	outNormal = gl_NormalMatrix*normal;
	outTangent = gl_NormalMatrix*outTangent;

	outColor = color;
	gl_FrontColor = gl_Color;

	outUV = UV + tangent.xy;// to stop optimization
}
