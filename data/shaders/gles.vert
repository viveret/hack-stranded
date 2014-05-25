attribute vec4 a_position;
attribute vec4 a_texCoord;
varying vec4 v_texCoord;
attribute vec4 a_color;
uniform vec4 u_color;
varying vec4 v_color;
uniform bool enable_v_color;
uniform float u_pointsize;
uniform mat4 mvp_matrix;
uniform mat4 tx_matrix;

void main()
{
	gl_Position = mvp_matrix * a_position;
	v_texCoord = tx_matrix * a_texCoord;
    if( enable_v_color )
        v_color = a_color;
    else
        v_color = u_color;
    gl_PointSize = u_pointsize;
}
