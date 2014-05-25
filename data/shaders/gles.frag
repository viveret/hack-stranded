varying vec4 v_texCoord;
uniform sampler2D s_texture;
varying vec4 v_color;
uniform bool enable_s_texture;

void main()
{
    if( enable_s_texture )
        gl_FragColor = v_color * texture2D(s_texture, v_texCoord.st);
    else
        gl_FragColor = v_color;
}
