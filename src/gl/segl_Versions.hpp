#ifndef SURVIVOR_ENGINE_OPENGL_VERSIONS
#define SURVIVOR_ENGINE_OPENGL_VERSIONS

#include "se_gl.hpp"

namespace segl_Shader
{
	struct Shader_Interface;
}

// Versions return own set functions



namespace SEGL_Ver_110
{
	segl_Shader::Shader_Interface Ver_Intface();
	segl_ID Create_Texture( uint Int_Format, uint Data_Format, uint Width, uint Height, void* Data );
}



namespace SEGL_Ver_300
{
	segl_Shader::Shader_Interface Ver_Intface();
	segl_ID Create_Texture( uint Int_Format, uint Data_Format, uint Width, uint Height, void* Data );
}








#endif
