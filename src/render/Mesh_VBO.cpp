#ifdef _TEST
#include "Mesh.hpp"
using namespace SE_Render_Mesh;

#include "../S_Engine.hpp"

#include <iostream>

#include "../gl/OpenGL.hpp"
#include "../utility/Functions.hpp"



SE_Mesh_VBO::SE_Mesh_VBO()
{
	this->VBO = 0;
	this->IND = 0;
}

uchar SE_Mesh_VBO::Get_Type()
{
	return Types::TYPE_VBO;
}

void SE_Mesh_VBO::Initialize(std::vector< Mesh_Vertex >* n_Data, std::vector<uint>* n_id, std::vector<Mesh_Texture>* n_td )
{
	this->Data = *n_Data;
	this->Indices = *n_id;
	if( n_td )
		this->Textures = *n_td;

//	if( glGenBuffers == 0 ) return;

	glGenBuffers( 1, &this->VBO );
	glBindBuffer( GL_ARRAY_BUFFER, this->VBO );
	glBufferData( GL_ARRAY_BUFFER, this->Data.size()*sizeof( Mesh_Vertex ), &this->Data[0], GL_STATIC_DRAW );


	glGenBuffers( 1, &IND );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, IND );
	glBufferData( GL_ELEMENT_ARRAY_BUFFER, this->Indices.size()*sizeof(unsigned int), &this->Indices[0], GL_STATIC_DRAW );

	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 );
}

void SE_Mesh_VBO::Cleanup()
{
	if( this->VBO )
		glDeleteBuffers(1,&VBO);

	if( this->IND )
		glDeleteBuffers(1,&IND);
}

void SE_Mesh_VBO::Draw()
{
#ifdef _TEST
	sglChange( SGL_SHADER_PROG, 1u );// mesh
#endif
	glPushMatrix();


	// get program ID
	GLint programId;
	glGetIntegerv(GL_CURRENT_PROGRAM,&programId);
#ifdef _TEST
	Engine.OpenGL_Assert( "Could not get cur prog ID", 1 );
#endif


	int vertex = glGetAttribLocation(programId,"vertex"); //0
#ifdef _TEST
	Engine.OpenGL_Assert( "Could not get prog vert ID", 1 );

	int normal = glGetAttribLocation(programId,"normal"); //1
	Engine.OpenGL_Assert( "Could not get prog norm ID", 1 );

	int tangent = glGetAttribLocation(programId,"tangent"); //2
	Engine.OpenGL_Assert( "Could not get prog tang ID", 1 );

	int color = glGetAttribLocation(programId,"color"); //3
	Engine.OpenGL_Assert( "Could not get prog color ID", 1 );

	int UV = glGetAttribLocation(programId,"UV"); //4
	Engine.OpenGL_Assert( "Could not get prog UV ID", 1 );

	//texture0
	//texture1...
	std::string str="texture";
	for(int i=0;i<this->Textures.size();i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D,this->Textures[i].ID);
		glUniform1i(glGetUniformLocation(programId,(str+(char)(i+'0')).c_str()),i);
	}

	glBindBuffer(GL_ARRAY_BUFFER,this->VBO);
	Engine.OpenGL_Assert( "Could not bind mesh vbo", 1 );

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,this->IND);
	Engine.OpenGL_Assert( "Could not bind mesh indecies", 1 );


	glEnableVertexAttribArray(vertex);
	Engine.OpenGL_Assert( "Could not enable mesh vert array", 1 );
	glVertexAttribPointer(vertex,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),0);
	Engine.OpenGL_Assert( "Could not set mesh vert ptr", 1 );

	glEnableVertexAttribArray(normal);
	Engine.OpenGL_Assert( "Could not enable mesh norm array", 1 );
	glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(3*sizeof(float)));
	Engine.OpenGL_Assert( "Could not set mesh norm ptr", 1 );

	glEnableVertexAttribArray(tangent);
	Engine.OpenGL_Assert( "Could not enable mesh tang array-> " + To_Str(GL_MAX_VERTEX_ATTRIBS), 1 );
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(6*sizeof(float)));
	Engine.OpenGL_Assert( "Could not set mesh tang ptr", 1 );   // not used in shaders

	glEnableVertexAttribArray(color);
	Engine.OpenGL_Assert( "Could not enable mesh color array", 1 );
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(9*sizeof(float)));
	Engine.OpenGL_Assert( "Could not set mesh color ptr", 1 );

	glEnableVertexAttribArray(UV);
	Engine.OpenGL_Assert( "Could not enable mesh uv array", 1 );
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(12*sizeof(float)));
	Engine.OpenGL_Assert( "Could not set mesh uv ptr", 1 );

	glDrawElements(GL_TRIANGLES,this->Indices.size(),GL_UNSIGNED_INT,0);
	Engine.OpenGL_Assert( "Could not draw mesh elems", 1 );

	glDisableVertexAttribArray(UV);
	Engine.OpenGL_Assert( "Could not disable mesh uv", 1 );

	glDisableVertexAttribArray(color);
	Engine.OpenGL_Assert( "Could not disable mesh color", 1 );

	glDisableVertexAttribArray(tangent);
	Engine.OpenGL_Assert( "Could not disable mesh tang", 1 );//    not used in shaders

	glDisableVertexAttribArray(normal);
	Engine.OpenGL_Assert( "Could not disable mesh norm", 1 );

	glDisableVertexAttribArray(vertex);
	Engine.OpenGL_Assert( "Could not disable mesh vert", 1 );

	glBindBuffer(GL_ARRAY_BUFFER,0);
	Engine.OpenGL_Assert( "Could not unbind mesh buf", 1 );
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	Engine.OpenGL_Assert( "Could not unbind mesh elem buf", 1 );

	glPopMatrix();
	sglChange( SGL_SHADER_PROG, 0u );// mesh
#endif
}





void SE_Mesh_VBO::Recursive_Process( aiNode* node, const aiScene* scene, SE_Mesh_VBO_List* M_List )
{
	//process
	for(int i=0;i<node->mNumMeshes;i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		SE_Mesh_VBO::Process_Mesh( mesh, scene, M_List );
	}



	//recursion
	for(int i=0;i<node->mNumChildren;i++)
	{
		return ;//SE_Mesh_VBO::Recursive_Process( node->mChildren[i], scene, M_List);
	}
}

void SE_Mesh_VBO::Process_Mesh( aiMesh* mesh, const aiScene* scene, SE_Mesh_VBO_List* M_List )
{
	std::vector< Mesh_Vertex> data;
	std::vector< uint > indices;
	std::vector< Mesh_Texture > textures;

	aiColor4D col;
	aiMaterial* mat=scene->mMaterials[mesh->mMaterialIndex];
	aiGetMaterialColor(mat,AI_MATKEY_COLOR_DIFFUSE,&col);
	Vector3D defaultColor = { col.r,col.g,col.b };


	for(int i=0;i<mesh->mNumVertices;i++)
	{
			Mesh_Vertex tmp;
			Vector3D tmpVec;

			//position
			tmpVec.x=mesh->mVertices[i].x;
			tmpVec.y=mesh->mVertices[i].y;
			tmpVec.z=mesh->mVertices[i].z;
			tmp.Position=tmpVec;

			//normals
			tmpVec.x=mesh->mNormals[i].x;
			tmpVec.y=mesh->mNormals[i].y;
			tmpVec.z=mesh->mNormals[i].z;
			tmp.Normal=tmpVec;


			//tangent
			if(mesh->mTangents)
			{
				tmpVec.x=mesh->mTangents[i].x;
				tmpVec.y=mesh->mTangents[i].y;
				tmpVec.z=mesh->mTangents[i].z;
			}else{
				tmpVec.x=1.0;
				tmpVec.y=tmpVec.z=0;
			}
			tmp.Tangent=tmpVec;


			//colors
			if(mesh->mColors[0])
			{
				//!= material color
				tmpVec.x=mesh->mColors[0][i].r;
				tmpVec.y=mesh->mColors[0][i].g;
				tmpVec.z=mesh->mColors[0][i].b;
			}else{
				tmpVec=defaultColor;
			}
			tmp.Color=tmpVec;

			//color
			if(mesh->mTextureCoords[0])
			{
				tmpVec.x=mesh->mTextureCoords[0][i].x;
				tmpVec.y=mesh->mTextureCoords[0][i].y;
			}else{
				tmpVec.x=tmpVec.y=tmpVec.z=0.0;
			}
			tmp.U=tmpVec.x;
			tmp.V=tmpVec.y;
			data.push_back(tmp);
	}

	for(int i=0;i<mesh->mNumFaces;i++)
	{
		aiFace face=mesh->mFaces[i];
		for(int j=0;j<face.mNumIndices;j++) //0..2
		{
			indices.push_back(face.mIndices[j]);
		}
	}


	for(int i=0;i<mat->GetTextureCount(aiTextureType_DIFFUSE);i++)
	{
		aiString str;
		mat->GetTexture(aiTextureType_DIFFUSE,i,&str);
		Mesh_Texture tmp;
		tmp.ID = 1;//loadTexture(str.C_Str());
		tmp.Type=0;
		textures.push_back(tmp);
	}

	SE_Mesh_VBO* nMesh = new SE_Mesh_VBO;
	nMesh->Initialize( &data, &indices, &textures );

	M_List->push_back( nMesh );
}
#endif
