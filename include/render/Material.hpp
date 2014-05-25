#ifndef SURVIVOR_ENGINE_RENDER_MATERIAL
#define SURVIVOR_ENGINE_RENDER_MATERIAL

#include "../math/Math.hpp"

namespace SE_Render_Material // 3D Objects
{
	struct Material_Base
	{
		Material_Base();// sets to openGL's defaults

		static void G_Defaults();// set global defaults
		void M_Defaults();// set own to defaults

		void Apply_Material();

		Vector4f Color, Color_Diffuse, Color_Ambient, Color_Specular, Color_Emission;
		float Shininess;
	};
}


#endif
