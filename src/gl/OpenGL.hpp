#ifndef SURVIVOR_ENGINE_OPENGL_INTERFACE
#define SURVIVOR_ENGINE_OPENGL_INTERFACE


// for interfacing with non-supported parts of openGL

#include <vector>
#include <string>

#include "../Core.hpp"
#include "../math/Math.hpp"
#include "se_gl.hpp"
#include "../render/Material.hpp"


struct VBO_Vert
{
	Vector3f Pos;
	Vector3f Norm;
	Vector2f Tex;
};

// Return true(1) if error, false(0) if OK
bool OpenGL_Assert( std::string Comment, uint A = 0 );

namespace SE_GL_Interfaces
{
/*	struct S_GL_RTT// render to texture
	{
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;

		virtual void Start_Render( uint nWidth, uint nHeight, uint To = 0 ) = 0;//to render a image to a texture
		virtual uint End_Render() = 0;//save render to texture and return ID
	};

	struct S_GL_VBO
	{
		S_GL_VBO();
		void Draw( uchar Flags = 0);// render the vbo

		void Make( uint nVert_Count, bool Tex = true );
		void Delete();

		//void Get_From( E_Objects::O_Mesh::Mesh_Container* Mesh );
		void Build_VBO();

		uint Vert_Count;
		Vector3f* Vertices;
		Vector2f* Tex_Coords;

		uint VBO_Verts, VBO_TexCoords;

		uchar Triangle_Draw;
	};

	struct S_GL_Shader// includes GL Settings
	{
		virtual bool Initialize() = 0;
		virtual void Destroy() = 0;
		uint nWidth, nHeight, To;

		void Enable( uint What );
		void Disable( uint What );
		virtual void Change( uint What, uint Value) = 0;
		virtual void Change( uint What, float Value) = 0;
		virtual void Change( uint What, Vector3f Value) = 0;


		virtual void Shade_Light( SE_Render_Material::Material_Base* Mat ) = 0;
		virtual void Cull(bool Mode) = 0;
	};

	struct S_GL_Interface
	{
	//  VERSION SPECIFIC FUNCTIONS
//		uint (*glCreate_Image)( uint, uint, uint, uint, void* );// uint int_format, uint data_format, uint width, uint height, void* data

		std::vector< S_GL_VBO > VBOs;
		S_GL_VBO* Get_VBO( uint Index );
		uint Get_VBO( S_GL_VBO* Which );


	};

	namespace GL_Default
	{
		struct RTT_Default:public S_GL_RTT
		{
			RTT_Default();
			~RTT_Default();

			bool Initialize();
			void Destroy();

			void Start_Render( uint nWidth, uint nHeight, uint To = 0 );//to render a image to a texture
			uint End_Render();//save render to texture and return ID

		private:
			uint Rendering_To;

			GLuint Render_Tex,Render_FBO,Render_DepthBuf;

		};

		struct Shader_Default: public S_GL_Shader
		{
			Shader_Default();
			~Shader_Default();

			bool Initialize();
			void Destroy();

			void Enable( uint What );
			void Disable( uint What );
			void Change( uint What, uint Value );
			void Change( uint What, float Value );
			void Change( uint What, Vector3f Value );


			void Shade_Light( SE_Render_Material::Material_Base* Mat );
			void Cull( bool Mode );
		};
	}

	namespace GL_New
	{
		struct RTT_New:public S_GL_RTT
		{
			RTT_New();
			~RTT_New();

			bool Initialize();
			void Destroy();

			void Start_Render(uint nWidth, uint nHeight, uint To = 0);//to render a image to a texture
			uint End_Render();//save render to texture and return ID

		private:
			uint Rendering_To;

			GLuint Render_Tex,Render_FBO,Render_DepthBuf;
		};

		struct Shader_Prog// single shader program
		{
			Shader_Prog();
			void Initialize();

			void Cleanup();

			std::string File_Name;
			uint m_ID;

			GLuint Prog_ID, Vert_ID, Frag_ID;

			// S_Engine variables
			GLuint Lighting_Enabled, Fog_Enabled;//STATES
			GLuint Grey_Scale_Perc;

			GLuint Texture_State;
		};
		struct Shader_New:public S_GL_Shader
		{
			Shader_New();
			~Shader_New();

			bool Initialize();
			void Destroy();


			void Enable(uint What);
			void Disable(uint What);
			void Change(uint What,uint Value);
			void Change(uint What,float Value);
			void Change(uint What,Vector3f Value);


			void Shade_Light( SE_Render_Material::Material_Base* Mat );
			void Cull(bool Mode);

			uint Load_Shader( std::string File );// file.vert, file.frag
			uint Load_Shader_V( std::string File );// version sensitive

		private:

			void Get_Log(GLuint Shader);
			Shader_Prog* Cur_Shader;

			std::vector< Shader_Prog > Shaders;
			uint Shader_New_ID();// get new id
		};
	}
*/
	void Register_Module();
}

// Functions that are changed based on what is supported
namespace SE_OpenGL
{
	ushort Version_OpenGL();
	ushort Version_GLSL();

	std::string Version_OpenGL_Str();
	std::string Version_GLSL_Str();


//	extern SE_GL_Interfaces::S_GL_Shader* GL_Shader;
//	extern SE_GL_Interfaces::S_GL_RTT* GL_RTT;

	// uint int_format, uint data_format, uint width, uint height, void* data 
	extern segl_ID (*Create_Texture)( uint, uint, uint, uint, void* );

//	extern SE_GL_Interfaces::segl_Shader Shader;
}
/*#define segl_Set1i(W,X)		SE_OpenGL::Shader.Set1i(W,X)
#define segl_Set2i(W,X,Y)	SE_OpenGL::Shader.Set1i(W,X,Y)
#define segl_Set3i(W,X,Y,Z)	SE_OpenGL::Shader.Set1i(W,X,Y,Z)

#define segl_Set1f(W,X)		SE_OpenGL::Shader.Set1i(W,X)
#define segl_Set2f(W,X,Y)	SE_OpenGL::Shader.Set1i(W,X,Y)
#define segl_Set3f(W,X,Y,Z)	SE_OpenGL::Shader.Set1i(W,X,Y,Z)

#define segl_Setip(W,X) SE_OpenGL::Shader.Set1p(W,X)
*/


#define OPENGL_VERSION		Engine.Application.OpenGL.Version_GL()
#define sgl Engine.Application.OpenGL

#endif
