#ifndef SURVIVOR_ENGINE_GL_SHADER
#define SURVIVOR_ENGINE_GL_SHADER

#include "se_gl.hpp"

namespace segl_Shader
{
	struct Shader_Data
	{
		Shader_Data();

		std::string File_Name;

		GLuint Prog_ID, Vert_ID, Frag_ID;

		// S_Engine variables
		GLuint Lighting_Enabled, Fog_Enabled;//STATES
		GLuint Grey_Scale_Perc;
		GLuint Texture_State;
	};
	typedef Shader_Data* Shader_ID;


	Shader_ID Load( std::string File, bool Ver = false );
	uint Free( Shader_ID Which );

	Shader_ID Get_Cur_Shader();
	void Set_Cur_Shader( Shader_ID What );


	struct Shader_Interface
	{
		Shader_Interface();

		void (*Set1f)( uint Which, float Value );
		void (*Set2f)( uint Which, float Value_1, float Value_2 );
		void (*Set3f)( uint Which, float Value_1, float Value_2, float Value_3 );

		void (*Set1i)( uint Which, uint Value );
		void (*Set2i)( uint Which, uint Value_1, uint Value_2 );
		void (*Set3i)( uint Which, uint Value_1, uint Value_2, uint Value_3 );

		void (*Set1p)( uint Which, void* Value );
	};

	Shader_Interface* Get_Cur_Intface(); // Just return current
	void Set_Cur_Intface( const Shader_Interface& Intf ); // can only be used once
}



#endif
