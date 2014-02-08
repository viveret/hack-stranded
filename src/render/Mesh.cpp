#ifdef _TEST
#include "Mesh.hpp"
using namespace SE_Render_Mesh;

#include "../S_Engine.hpp"

#include <iostream>

#include "../gl/OpenGL.hpp"
#include "../utility/Functions.hpp"

//const aiScene* scene = NULL;
//GLuint scene_list = 0;

Mesh_Vertex::Mesh_Vertex()
{
	this->U = 0.f;
	this->V = 0.f;
}


Mesh_Texture::Mesh_Texture()
{
	this->ID = 0.f;
	this->Type = 0.f;
}

SE_Mesh_Man_Item::SE_Mesh_Man_Item()
{
	this->m_Mesh = NULL;
}

void SE_Mesh_Man_Item::Cleanup()
{
	if( this->m_Mesh )
	{
		this->m_Mesh->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( this->m_Mesh );
#endif
		delete this->m_Mesh;
		this->m_Mesh = NULL;
	}
}

SE_Mesh_Manager::SE_Mesh_Manager()
{

}

SE_Mesh_Manager::~SE_Mesh_Manager()
{
	this->Cleanup();
}

bool SE_Mesh_Manager::Initialize()
{
	// get a handle to the predefined STDOUT log stream and attach
	// it to the logging system. It remains active for all further
	// calls to aiImportFile(Ex) and aiApplyPostProcessing.
	//this->Assimp_Stream = aiGetPredefinedLogStream(aiDefaultLogStream_STDOUT,NULL);
	//aiAttachLogStream( &this->Assimp_Stream);

	// ... same procedure, but this stream now writes the
	// log messages to assimp_log.txt
	this->Assimp_Stream = aiGetPredefinedLogStream(aiDefaultLogStream_FILE,"data/log_assimp.log");
	aiAttachLogStream(&this->Assimp_Stream);

	return true;
}

void SE_Mesh_Manager::Cleanup()
{
	// cleanup - calling 'aiReleaseImport' is important, as the library
	// keeps internal resources until the scene is freed again. Not
	// doing so can cause severe resource leaking.
	//aiReleaseImport(scene);

	// We added a log stream to the library, it's our job to disable it
	// again. This will definitely release the last resources allocated
	// by Assimp.
	aiDetachAllLogStreams();
}

SE_Manager::SE_Man_Item* SE_Mesh_Manager::Gen_Item()// generate new item
{
	SE_Mesh_Man_Item* nItem = new SE_Mesh_Man_Item;
#ifdef _TEST
	Engine.Mem_Add( nItem, "mesh item" );
#endif
	nItem->m_Mesh = NULL;
	this->Item_Add_New( nItem );

	return nItem;
}

#include "../utility/Functions.hpp"
void SE_Mesh_Manager::Draw( uint Which )
{
	SE_Mesh_Interface* nMesh = this->Get_Mesh( Which );
	if( nMesh )
		nMesh->Draw();
}

SE_Mesh_Base* SE_Mesh_Manager::Get_Mesh( uint Which )
{
	SE_Mesh_Man_Item* gItem = (SE_Mesh_Man_Item*)this->Get_Item( Which );

	if( gItem )
		return gItem->m_Mesh;

	else
		return NULL;
}


void SE_Render_Mesh::recursive_render (const aiScene *sc, const aiNode* nd)
{
	unsigned int i;
	unsigned int n = 0, t;
	aiMatrix4x4 m = nd->mTransformation;

	// update transform
	aiTransposeMatrix4(&m);
	glPushMatrix();
	glMultMatrixf((float*)&m);

	// draw all meshes assigned to this node
	for (; n < nd->mNumMeshes; ++n) {
		const struct aiMesh* mesh = sc->mMeshes[nd->mMeshes[n]];

		//apply_material(sc->mMaterials[mesh->mMaterialIndex]);

		if(mesh->mNormals == NULL) {
			glDisable(GL_LIGHTING);
		} else {
			glEnable(GL_LIGHTING);
		}

		for (t = 0; t < mesh->mNumFaces; ++t) {
			const struct aiFace* face = &mesh->mFaces[t];
			GLenum face_mode;

			switch(face->mNumIndices) {
				case 1: face_mode = GL_POINTS; break;
				case 2: face_mode = GL_LINES; break;
				case 3: face_mode = GL_TRIANGLES; break;
				default: face_mode = GL_POLYGON; break;
			}

			glBegin(face_mode);

			for(i = 0; i < face->mNumIndices; i++) {
				int index = face->mIndices[i];
				if(mesh->mColors[0] != NULL)
					glColor4fv((GLfloat*)&mesh->mColors[0][index]);
				if(mesh->mNormals != NULL)
					glNormal3fv(&mesh->mNormals[index].x);
				glVertex3fv(&mesh->mVertices[index].x);
			}

			glEnd();
		}

	}

	// draw all children
	for (n = 0; n < nd->mNumChildren; ++n) {
		recursive_render(sc, nd->mChildren[n]);
	}

	glPopMatrix();
}

/*
 *

	glBindBuffer(GL_ARRAY_BUFFER, this->Meshes[Which]->VBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->Meshes[Which]->IND );

	//glEnableVertexAttribArray();
	//glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),0);

	//glEnableVertexAttribArray(normal);
	//glVertexAttribPointer(normal,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(3*sizeof(float)));

	/*glEnableVertexAttribArray(tangent);
	glVertexAttribPointer(tangent,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(6*sizeof(float)));

	glEnableVertexAttribArray(color);
	glVertexAttribPointer(color,3,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(9*sizeof(float)));

	glEnableVertexAttribArray(UV);
	glVertexAttribPointer(UV,2,GL_FLOAT,GL_FALSE,sizeof(Mesh_Vertex),(void*)(12*sizeof(float)));*//*

	glDrawElements(GL_TRIANGLES,this->Meshes[Which]->Indices.size(),GL_UNSIGNED_INT,0);

	glDisableVertexAttribArray(1);
	/*glDisableVertexAttribArray(normal);
	glDisableVertexAttribArray(tangent);
	glDisableVertexAttribArray(color);
	glDisableVertexAttribArray(UV);*//*

	glBindBuffer(GL_ARRAY_BUFFER,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);

	*/
#endif
