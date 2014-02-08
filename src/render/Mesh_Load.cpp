#ifdef _TEST
#include "Mesh.hpp"
using namespace SE_Render_Mesh;

#include "../S_Engine.hpp"

#include <iostream>

#include "../gl/OpenGL.hpp"
#include "../utility/Functions.hpp"
#include "../utility/Utility.hpp"



uint SE_Mesh_Manager::Load_Mesh( std::string File )
{
	if( File.find( ".smdl" ) != File.npos )// is a survivior mesh
		return Load_Mesh_SurvivorMDL( File );

	else // try with assimp
		return Load_Mesh_Assimp( File );
}



uint SE_Mesh_Manager::Load_Mesh_Assimp( std::string File )
{
	SE_Raw_Buffer Data;
#ifdef _TEST
	= Engine.Application.Input.Load_File( File );
#endif
	if( Data.Size() == 0 )
	{
		Data.Delete();
		return 0;
	}

	Assimp::Importer importer;
	//const aiScene* scene = importer.ReadFile( File.c_str(), aiProcess_GenSmoothNormals | aiProcess_Triangulate |
		//										  aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_FlipUVs );

	const aiScene* scene = importer.ReadFileFromMemory( Data.Data(), Data.Size(),
	aiProcess_GenSmoothNormals | aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_FlipUVs, File_Ext(File).c_str() );



	bool Err = false;
	if( scene == NULL )
		Err = true;

	else if(scene->mFlags==AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
		Err = true;

	if( Err )
	{
#ifdef _TEST
		Engine.Printf( "Assimp error logged for: \'%s\'\n", Log_Type::TYPE_EROR, File.c_str() );
#endif
		Data.Delete();
		return 0;
	}

	Data.Delete();

	SE_Mesh_VBO_List Mesh_Buf;
	SE_Mesh_VBO::Recursive_Process(scene->mRootNode,scene, &Mesh_Buf);

	if( Mesh_Buf.size() > 0 )
	{
		SE_Mesh_Man_Item* nItem  = (SE_Mesh_Man_Item*)this->Gen_Item();
		nItem->m_Mesh = Mesh_Buf[0];
		return nItem->Handle_ID;
	}
	else
		return 0;
}




uint SE_Mesh_Manager::Load_Mesh_SurvivorMDL( std::string File )
{
	SE_Mesh_Base* Loaded_Mesh = NULL;


	if( File.find(".xml") != std::string::npos )// is xml format
		Loaded_Mesh = SE_Mesh_SMDL::Mesh_SMDL_Loaders::Load_XML_Mesh( File );

	else if( File.find(".bin") != std::string::npos )//is compiled
		Loaded_Mesh = SE_Mesh_SMDL::Mesh_SMDL_Loaders::Load_Compiled_Mesh(File);

	else
		Loaded_Mesh = SE_Mesh_SMDL::Mesh_SMDL_Loaders::Load_Noncompiled_Mesh(File);

	if( Loaded_Mesh != NULL )
	{
		SE_Mesh_Man_Item* nItem  = (SE_Mesh_Man_Item*)this->Gen_Item();
		nItem->m_Mesh = Loaded_Mesh;
		return nItem->Handle_ID;
	}
	else
		return 0;
}

#endif
