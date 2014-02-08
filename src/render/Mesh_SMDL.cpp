#ifdef _TEST
#include "Mesh.hpp"
using namespace SE_Render_Mesh::SE_Mesh_SMDL;

#include "../S_Engine.hpp"

#include <iostream>

#include "../gl/OpenGL.hpp"
#include "../utility/Functions.hpp"




SMDL_Texture_Map_Info::SMDL_Texture_Map_Info(float nx,float ny, float nw, float nh)
{
	this->Set(nx,ny,nw,nh);
}

SMDL_Texture_Map_Info::SMDL_Texture_Map_Info()
{
	this->Reset();
}

void SMDL_Texture_Map_Info::Reset()
{
	this->m_x=0;
	this->m_y=0;

	this->m_w=1;
	this->m_h=1;
}

void SMDL_Texture_Map_Info::Set(float nx,float ny, float nw, float nh)
{
	this->m_x=nx;
	this->m_y=ny;

	this->m_w=nw;
	this->m_h=nh;
}

float SMDL_Texture_Map_Info::x()
{
	return this->m_x;
}

float SMDL_Texture_Map_Info::y()
{
	return this->m_y;
}

float SMDL_Texture_Map_Info::w()//x+w
{
	return this->m_w + this->x();
}

float SMDL_Texture_Map_Info::h()//y+h
{
	return this->m_h + this->y();
}


SMDL_Pivot_Attachment::SMDL_Pivot_Attachment()
{
	this->Attach_To = NULL;
	this->Rotation = 0.f;
}

void SMDL_Pivot_Attachment::Attach()
{
	if( this->Attach_To )
		*this->Attach_To = this->Rotation;
}


Mesh_SMDL_Base::Mesh_SMDL_Base()
{
	this->r=1;
	this->g=1;
	this->b=1;
}

void Mesh_SMDL_Base::Draw()
{
	glPushMatrix();

	glTranslatef(this->Position.x,this->Position.y,this->Position.z);

	glScalef(this->Size.x/2.f,this->Size.y/2.f,this->Size.z/2.f);


    glBegin(GL_QUADS);//left
        glNormal3f(-1,0,0);
        glTexCoord2f(Tex_Maps[0].x(),Tex_Maps[0].y()); glVertex3f(-1, 1,-1);
        glTexCoord2f(Tex_Maps[0].x(),Tex_Maps[0].h()); glVertex3f(-1,-1,-1);
        glTexCoord2f(Tex_Maps[0].w(),Tex_Maps[0].h()); glVertex3f(-1,-1, 1);
        glTexCoord2f(Tex_Maps[0].w(),Tex_Maps[0].y()); glVertex3f(-1, 1, 1);
    glEnd();

    glBegin(GL_QUADS);//right
        glNormal3f(1,0,0);
        glTexCoord2f(Tex_Maps[1].x(),Tex_Maps[1].y()); glVertex3f( 1, 1, 1);
        glTexCoord2f(Tex_Maps[1].x(),Tex_Maps[1].h()); glVertex3f( 1,-1, 1);
        glTexCoord2f(Tex_Maps[1].w(),Tex_Maps[1].h()); glVertex3f( 1,-1,-1);
        glTexCoord2f(Tex_Maps[1].w(),Tex_Maps[1].y()); glVertex3f( 1, 1,-1);
    glEnd();

    glBegin(GL_QUADS);//back
        glNormal3f(0,0,-1);
        glTexCoord2f(Tex_Maps[2].x(),Tex_Maps[2].y()); glVertex3f( 1, 1,-1);
        glTexCoord2f(Tex_Maps[2].x(),Tex_Maps[2].h()); glVertex3f( 1,-1,-1);
        glTexCoord2f(Tex_Maps[2].w(),Tex_Maps[2].h()); glVertex3f(-1,-1,-1);
        glTexCoord2f(Tex_Maps[2].w(),Tex_Maps[2].y()); glVertex3f(-1, 1,-1);
    glEnd();

    glBegin(GL_QUADS);//front
        glNormal3f(0,0,1);
        glTexCoord2f(Tex_Maps[3].x(),Tex_Maps[3].y()); glVertex3f(-1, 1, 1);
        glTexCoord2f(Tex_Maps[3].x(),Tex_Maps[3].h()); glVertex3f(-1,-1, 1);
        glTexCoord2f(Tex_Maps[3].w(),Tex_Maps[3].h()); glVertex3f( 1,-1, 1);
        glTexCoord2f(Tex_Maps[3].w(),Tex_Maps[3].y()); glVertex3f( 1, 1, 1);
    glEnd();

    glBegin(GL_QUADS);//bottom
        glNormal3f(0,-1,0);
        glTexCoord2f(Tex_Maps[4].x(),Tex_Maps[4].y()); glVertex3f( 1,-1,-1);
        glTexCoord2f(Tex_Maps[4].x(),Tex_Maps[4].h()); glVertex3f( 1,-1, 1);
        glTexCoord2f(Tex_Maps[4].w(),Tex_Maps[4].h()); glVertex3f(-1,-1, 1);
        glTexCoord2f(Tex_Maps[4].w(),Tex_Maps[4].y()); glVertex3f(-1,-1,-1);
    glEnd();

    glBegin(GL_QUADS);//top
        glNormal3f(0,1,0);
        glTexCoord2f(Tex_Maps[5].x(),Tex_Maps[5].y()); glVertex3f( 1, 1, 1);
        glTexCoord2f(Tex_Maps[5].x(),Tex_Maps[5].h()); glVertex3f( 1, 1,-1);
        glTexCoord2f(Tex_Maps[5].w(),Tex_Maps[5].h()); glVertex3f(-1, 1,-1);
        glTexCoord2f(Tex_Maps[5].w(),Tex_Maps[5].y()); glVertex3f(-1, 1, 1);
    glEnd();


	glPopMatrix();
}



Mesh_SMDL_Bone::Mesh_SMDL_Bone()
{
	this->Meshes = NULL;
	this->Num_Meshes = 0;

	this->Bones = NULL;
	this->Num_Bones = 0;

	this->my_Pivots = NULL;
	this->Num_my_Pivots = 0;
}

uchar Mesh_SMDL_Bone::Get_Type()
{
	return SE_Render_Mesh::Types::TYPE_SMDL;
}

void Mesh_SMDL_Bone::Make(u_char nNum_Meshes, u_char nNum_Pivots, u_char nNum_Bones)
{
	if(!this->Meshes && !this->my_Pivots && !this->Bones)
	{
		this->Meshes = new Mesh_SMDL_Base[nNum_Meshes];
#ifdef _TEST
		Engine.Mem_Add( this->Meshes, "meshes" );
		this->Num_Meshes = nNum_Meshes;

		this->my_Pivots = new Mesh_SMDL_Pivot[nNum_Pivots];
		Engine.Mem_Add( this->my_Pivots, "pivots" );
		this->Num_my_Pivots = nNum_Pivots;

		this->Bones = new Mesh_SMDL_Bone*[nNum_Bones];
		Engine.Mem_Add( this->Bones, "bone array" );
		this->Num_Bones = nNum_Bones;
#endif
	}
}

void Mesh_SMDL_Bone::Cleanup()
{
	if( this->Meshes )
	{
#ifdef _TEST
		Engine.Mem_Remove( this->Meshes );
#endif
		delete[] this->Meshes;
		this->Meshes = NULL;

		this->Num_Meshes = 0;
	}

	if( this->my_Pivots )
	{
#ifdef _TEST
		Engine.Mem_Remove( this->my_Pivots );
#endif
		delete[] this->my_Pivots;
		this->my_Pivots = NULL;

		this->Num_my_Pivots = 0;
	}

	if( this->Bones )
	{
		for(u_char i = 0;i < this->Num_Bones; i++)
			this->Bones[i]->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( this->Bones );
#endif
		delete[] this->Bones;
		this->Bones = NULL;

		this->Num_Bones = 0;
	}
}

void Mesh_SMDL_Bone::Draw()
{
	glPushMatrix();

	for(u_char i = 0;i < this->Num_my_Pivots; i++)// for each pivot
	{
		glTranslatef(this->my_Pivots[i].Pos.x, this->my_Pivots[i].Pos.y, this->my_Pivots[i].Pos.z);

		glRotatef( this->my_Pivots[i].Rotate.y, 0, 1, 0 );
		glRotatef( this->my_Pivots[i].Rotate.x, 1, 0, 0 );
		glRotatef( this->my_Pivots[i].Rotate.z, 0, 0, 1 );

		glTranslatef(-this->my_Pivots[i].Pos.x, -this->my_Pivots[i].Pos.y, -this->my_Pivots[i].Pos.z);
	}

	for(u_char i = 0;i < this->Num_Meshes; i++)
		this->Meshes[i].Draw();

	for(u_char i = 0;i < this->Num_Bones; i++)
		this->Bones[i]->Draw();


	glPopMatrix();
}

Mesh_SMDL_Base* Mesh_SMDL_Bone::Get_Mesh(std::string Name)
{
	for(u_char i = 0;i < this->Num_Meshes; i++)
		if( this->Meshes[i].Name == Name )
			return &this->Meshes[i];

	return NULL;
}

Mesh_SMDL_Pivot* Mesh_SMDL_Bone::Get_Pivot(std::string Name)
{
	for(u_char i = 0;i < this->Num_my_Pivots; i++)
		if( this->my_Pivots[i].Name == Name )
			return &this->my_Pivots[i];

	return NULL;
}

Mesh_SMDL_Bone* Mesh_SMDL_Bone::Get_Bone(std::string Name)
{
	for(u_char i = 0;i < this->Num_Bones; i++)
		if( this->Bones[i]->my_Name == Name )
			return this->Bones[i];

	return NULL;
}

void Attatch_Rotation(Vector3* Rots)// update rotations
{
}





void* Mesh_SMDL_Bone::Make_New()
{
	return NULL;// must load from file
}

void Mesh_SMDL_Bone::Load( void** Obj, std::string File )
{
	*Obj = new Mesh_SMDL_Container;
#ifdef _TEST
	Engine.Mem_Add( *Obj, "mesh container" );
#endif
	//*((Mesh_SMDL_Bone*)*Obj) = Mesh_SMDL_Loaders::Load_Mesh( File );
}

void Mesh_SMDL_Bone::Delete( void** Obj )
{
	if( *Obj != NULL )
	{
		Mesh_SMDL_Container* nObj = (Mesh_SMDL_Container*)*Obj;

		nObj->Cleanup();
#ifdef _TEST
		Engine.Mem_Remove( nObj );
#endif
		delete nObj;

		*Obj = NULL;
	}
}






Mesh_SMDL_Container* Mesh_SMDL_Loaders::Load_Noncompiled_Mesh(std::string File)
{
	Mesh_SMDL_Container* Ret;

	std::string Data;
#ifdef _TEST
	= Engine.Application.Input.Load_File_Str(File);
#endif

	if(Data.length()==0)
	{
#ifdef _TEST
		Engine.Register_Error(S_Error("Cannot load mesh: " + File + ", no data",1));
#endif
		return Ret;
	}

	uint Line_At=0,Mesh_At=0,Tex_At=0;//to tell what info it is reading

	char Buffer[32];memset(Buffer,0,32);
	char Buf_At=0;
	bool Is_Data=false;//if we want to use it

	//char Tex_xScale=0,Tex_yScale=0;//to compile tex map

	Vector3 M_Origin;

	for(uint l=0;l<Data.size();l++)
	{
		if(((Data[l]>=48 && Data[l]<=57) || Data[l]=='.' || Data[l]=='-') && Is_Data && Buf_At<32)
		{
			Buffer[Buf_At]=Data[l];
			Buf_At++;
		}
		else if(Data[l]==':')
			Is_Data=true;//we want to use it

		else if(Data[l]=='\n' || l==Data.length()-1)//done with that data
		{
			if(Buf_At>0)
			{
				if(Line_At==0)//number of meshes
					Ret->Make(atoi(Buffer), 0, 0);


				else if(Line_At==1)//ORIGIN x
					M_Origin.x = (float)atof(Buffer);

				else if(Line_At==2)//ORIGIN y
					M_Origin.y = (float)atof(Buffer);

				else if(Line_At==3)//ORIGIN z
					M_Origin.z = (float)atof(Buffer);



				else if(Line_At==4)//x
					Ret->Meshes[Mesh_At].Position.x = (float)atof(Buffer)+M_Origin.x;

				else if(Line_At==5)//y
					Ret->Meshes[Mesh_At].Position.y = (float)atof(Buffer)+M_Origin.y;

				else if(Line_At==6)//z
					Ret->Meshes[Mesh_At].Position.z = (float)atof(Buffer)+M_Origin.z;



				else if(Line_At==7)//w
					Ret->Meshes[Mesh_At].Size.x = (float)atof(Buffer);

				else if(Line_At==8)//h
					Ret->Meshes[Mesh_At].Size.y = (float)atof(Buffer);

				else if(Line_At==9)//l
					Ret->Meshes[Mesh_At].Size.z = (float)atof(Buffer);

				//TEXTURES

				else if(Line_At==10)//tex x
					Ret->Meshes[Mesh_At].Tex_Maps[Tex_At].m_x = (float)atof(Buffer);

				else if(Line_At==11)//tex y
					Ret->Meshes[Mesh_At].Tex_Maps[Tex_At].m_y = (float)atof(Buffer);

				else if(Line_At==12)//tex w
					Ret->Meshes[Mesh_At].Tex_Maps[Tex_At].m_w = (float)atof(Buffer);

				else if(Line_At==13)//tex h
					Ret->Meshes[Mesh_At].Tex_Maps[Tex_At].m_h = (float)atof(Buffer);





				//done
				memset(Buffer,0,32);
				Buf_At=0;
				Is_Data=false;
				Line_At++;

				if(Line_At==14)//reset tex counter
				{
					//Ret->Meshes[Mesh_At].Tex_Maps[Tex_At].Compile_Map(Tex_xScale,Tex_yScale,true);
					Line_At=10;
					Tex_At++;
					if(Tex_At==6)//done with textures
					{
						Tex_At=0;
						Line_At=4;
						Mesh_At++;
						if(Mesh_At==Ret->Num_Meshes)
							break;
					}
				}
			}
		}
	}

	return Ret;
}

Mesh_SMDL_Container* Mesh_SMDL_Loaders::Load_XML_Mesh(std::string File)
{
	Mesh_SMDL_Container* Ret = NULL;

	SE_Application::XML_File Data;
#ifdef _TEST
	if( Engine.Application.Input.Load_XML_File(&Data, File)==false )
	{
		Engine.Register_Error(S_Error("Could not load XML mesh: " + File, 1 ));
		return Ret;
	}
#endif

	//Vector3 Offset( Data.attribute("off_x").as_float(), Data.attribute("off_y").as_float(), Data.attribute("off_z").as_float() );

	Ret = Gen_XML_Bone( Data.child("model") );

	return Ret;
}

#include "../utility/Functions.hpp"
void Mesh_SMDL_Loaders::Save_XML_Mesh( Mesh_SMDL_Container* Mesh, std::string File)
{
	SE_Application::XML_File nFile;


	SE_Application::XML_Node Model = nFile.append_child("model");
	//Model.append_attribute("off_x");Model.append_attribute("off_y");Model.append_attribute("off_z");
	Create_XML_Bone( &Model, Mesh );


	nFile.save_file( File.c_str() );
}

void Mesh_SMDL_Loaders::Create_XML_Bone( SE_Application::XML_Node* At, Mesh_SMDL_Bone* Bone )// create XML string from bone
{
	At->append_attribute("name");At->attribute("name") = Bone->my_Name.c_str();

	At->append_attribute("size_x");At->attribute("size_x") = Bone->Size.x;
	At->append_attribute("size_y");At->attribute("size_y") = Bone->Size.y;
	At->append_attribute("size_z");At->attribute("size_z") = Bone->Size.z;

	// get parts
	for(uint i = 0;i < Bone->Num_Meshes; i++)// Part; Part = Part.next_sibling("part"))
	{
		pugi::xml_node Part = At->append_child("part");// keep track of part at
		Part.append_attribute("name");Part.attribute("name") = Bone->Meshes[i].Name.c_str();

		Part.append_attribute("size_x");Part.attribute("size_x") = Bone->Meshes[i].Size.x;
		Part.append_attribute("size_y");Part.attribute("size_y") = Bone->Meshes[i].Size.y;
		Part.append_attribute("size_z");Part.attribute("size_z") = Bone->Meshes[i].Size.z;

		Part.append_attribute("x");Part.attribute("x") = Bone->Meshes[i].Position.x;
		Part.append_attribute("y");Part.attribute("y") = Bone->Meshes[i].Position.y;
		Part.append_attribute("z");Part.attribute("z") = Bone->Meshes[i].Position.z;

		Part.append_attribute("w");Part.attribute("w") = Bone->Meshes[i].Size.x;
		Part.append_attribute("h");Part.attribute("h") = Bone->Meshes[i].Size.y;
		Part.append_attribute("l");Part.attribute("l") = Bone->Meshes[i].Size.z;

		pugi::xml_node Tex = Part.append_child("tex_maps");

		Tex.append_child("left");Tex.append_child("right");Tex.append_child("back");
		Tex.append_child("front");Tex.append_child("down");Tex.append_child("up");

		Tex.child("left").append_attribute("x");Tex.child("left").append_attribute("y");Tex.child("left").append_attribute("w");Tex.child("left").append_attribute("h");
		Tex.child("right").append_attribute("x");Tex.child("right").append_attribute("y");Tex.child("right").append_attribute("w");Tex.child("right").append_attribute("h");
		Tex.child("back").append_attribute("x");Tex.child("back").append_attribute("y");Tex.child("back").append_attribute("w");Tex.child("back").append_attribute("h");
		Tex.child("front").append_attribute("x");Tex.child("front").append_attribute("y");Tex.child("front").append_attribute("w");Tex.child("front").append_attribute("h");
		Tex.child("down").append_attribute("x");Tex.child("down").append_attribute("y");Tex.child("down").append_attribute("w");Tex.child("down").append_attribute("h");
		Tex.child("up").append_attribute("x");Tex.child("up").append_attribute("y");Tex.child("up").append_attribute("w");Tex.child("up").append_attribute("h");


		Tex.child("left").attribute("x") = Bone->Meshes[i].Tex_Maps[0].m_x;
		Tex.child("left").attribute("y") = Bone->Meshes[i].Tex_Maps[0].m_y;
		Tex.child("left").attribute("w") = Bone->Meshes[i].Tex_Maps[0].m_w;
		Tex.child("left").attribute("h") = Bone->Meshes[i].Tex_Maps[0].m_h;

		Tex.child("right").attribute("x") = Bone->Meshes[i].Tex_Maps[1].m_x;
		Tex.child("right").attribute("y") = Bone->Meshes[i].Tex_Maps[1].m_y;
		Tex.child("right").attribute("w") = Bone->Meshes[i].Tex_Maps[1].m_w;
		Tex.child("right").attribute("h") = Bone->Meshes[i].Tex_Maps[1].m_h;

		Tex.child("back").attribute("x") = Bone->Meshes[i].Tex_Maps[2].m_x;
		Tex.child("back").attribute("y") = Bone->Meshes[i].Tex_Maps[2].m_y;
		Tex.child("back").attribute("w") = Bone->Meshes[i].Tex_Maps[2].m_w;
		Tex.child("back").attribute("h") = Bone->Meshes[i].Tex_Maps[2].m_h;

		Tex.child("front").attribute("x") = Bone->Meshes[i].Tex_Maps[3].m_x;
		Tex.child("front").attribute("y") = Bone->Meshes[i].Tex_Maps[3].m_y;
		Tex.child("front").attribute("w") = Bone->Meshes[i].Tex_Maps[3].m_w;
		Tex.child("front").attribute("h") = Bone->Meshes[i].Tex_Maps[3].m_h;

		Tex.child("down").attribute("x") = Bone->Meshes[i].Tex_Maps[4].m_x;
		Tex.child("down").attribute("y") = Bone->Meshes[i].Tex_Maps[4].m_y;
		Tex.child("down").attribute("w") = Bone->Meshes[i].Tex_Maps[4].m_w;
		Tex.child("down").attribute("h") = Bone->Meshes[i].Tex_Maps[4].m_h;

		Tex.child("up").attribute("x") = Bone->Meshes[i].Tex_Maps[5].m_x;
		Tex.child("up").attribute("y") = Bone->Meshes[i].Tex_Maps[5].m_y;
		Tex.child("up").attribute("w") = Bone->Meshes[i].Tex_Maps[5].m_w;
		Tex.child("up").attribute("h") = Bone->Meshes[i].Tex_Maps[5].m_h;
	}

	// get bones
	for(uint i = 0;i < Bone->Num_Bones; i++)//pugi::xml_node Bone = Data.child("bone"); Bone; Bone = Bone.next_sibling("bone"))
	{
		SE_Application::XML_Node Node_Bone = At->append_child("bone");
		Create_XML_Bone( &Node_Bone, Bone->Bones[i] );
	}

	// get pivots
	for (uint i = 0;i < Bone->Num_my_Pivots; i++)
	{
		SE_Application::XML_Node Pivot = At->append_child("pivot");
		Pivot.append_attribute("name");Pivot.attribute("name") = Bone->my_Pivots[i].Name.c_str();

		Pivot.append_attribute("x");Pivot.append_attribute("y");Pivot.append_attribute("z");
		Pivot.append_attribute("rot_x");Pivot.append_attribute("rot_y");Pivot.append_attribute("rot_z");

		Pivot.attribute("x") = Bone->my_Pivots[i].Pos.x;
		Pivot.attribute("y") = Bone->my_Pivots[i].Pos.y;
		Pivot.attribute("z") = Bone->my_Pivots[i].Pos.z;

		Pivot.attribute("rot_x") = Bone->my_Pivots[i].Rotate.x;
		Pivot.attribute("rot_y") = Bone->my_Pivots[i].Rotate.y;
		Pivot.attribute("rot_z") = Bone->my_Pivots[i].Rotate.z;
	}
}

Mesh_SMDL_Bone* Mesh_SMDL_Loaders::Gen_XML_Bone( SE_Application::XML_Node Data )// create bone from xml bone
{
	Mesh_SMDL_Bone* Ret = new Mesh_SMDL_Bone;
#ifdef _TEST
	Engine.Mem_Add( Ret, "bone" );
#endif

	Ret->my_Name = Data.attribute("name").as_string();

	Ret->Size.Set( Data.attribute("size_x").as_float(), Data.attribute("size_y").as_float(), Data.attribute("size_z").as_float() );

	// get number of parts
	for(pugi::xml_node Part = Data.child("part"); Part; Part = Part.next_sibling("part"))
		Ret->Num_Meshes++;

	// get number of bones
	for(pugi::xml_node Bone = Data.child("bone"); Bone; Bone = Bone.next_sibling("bone"))
		Ret->Num_Bones++;

	// get number of pivots
	for(pugi::xml_node Pivot = Data.child("pivot"); Pivot; Pivot = Pivot.next_sibling("pivot"))
		Ret->Num_my_Pivots++;


	Ret->Make( Ret->Num_Meshes, Ret->Num_my_Pivots, Ret->Num_Bones );

	// get parts
	uint i = 0; // keep track of what part at
	for (pugi::xml_node Part = Data.child("part"); Part; Part = Part.next_sibling("part"))
	{
		Ret->Meshes[i].Name = Part.attribute("name").as_string();

		Ret->Meshes[i].Position.x = Part.attribute("x").as_float();
		Ret->Meshes[i].Position.y = Part.attribute("y").as_float();
		Ret->Meshes[i].Position.z = Part.attribute("z").as_float();

		Ret->Meshes[i].Size.x = Part.attribute("w").as_float();
		Ret->Meshes[i].Size.y = Part.attribute("h").as_float();
		Ret->Meshes[i].Size.z = Part.attribute("l").as_float();


		pugi::xml_node Tex = Part.child("tex_maps");

		Ret->Meshes[i].Tex_Maps[0].m_x = Tex.child("left").attribute("x").as_float();
		Ret->Meshes[i].Tex_Maps[0].m_y = Tex.child("left").attribute("y").as_float();
		Ret->Meshes[i].Tex_Maps[0].m_w = Tex.child("left").attribute("w").as_float();
		Ret->Meshes[i].Tex_Maps[0].m_h = Tex.child("left").attribute("h").as_float();

		Ret->Meshes[i].Tex_Maps[1].m_x = Tex.child("right").attribute("x").as_float();
		Ret->Meshes[i].Tex_Maps[1].m_y = Tex.child("right").attribute("y").as_float();
		Ret->Meshes[i].Tex_Maps[1].m_w = Tex.child("right").attribute("w").as_float();
		Ret->Meshes[i].Tex_Maps[1].m_h = Tex.child("right").attribute("h").as_float();

		Ret->Meshes[i].Tex_Maps[2].m_x = Tex.child("back").attribute("x").as_float();
		Ret->Meshes[i].Tex_Maps[2].m_y = Tex.child("back").attribute("y").as_float();
		Ret->Meshes[i].Tex_Maps[2].m_w = Tex.child("back").attribute("w").as_float();
		Ret->Meshes[i].Tex_Maps[2].m_h = Tex.child("back").attribute("h").as_float();

		Ret->Meshes[i].Tex_Maps[3].m_x = Tex.child("front").attribute("x").as_float();
		Ret->Meshes[i].Tex_Maps[3].m_y = Tex.child("front").attribute("y").as_float();
		Ret->Meshes[i].Tex_Maps[3].m_w = Tex.child("front").attribute("w").as_float();
		Ret->Meshes[i].Tex_Maps[3].m_h = Tex.child("front").attribute("h").as_float();

		Ret->Meshes[i].Tex_Maps[4].m_x = Tex.child("down").attribute("x").as_float();
		Ret->Meshes[i].Tex_Maps[4].m_y = Tex.child("down").attribute("y").as_float();
		Ret->Meshes[i].Tex_Maps[4].m_w = Tex.child("down").attribute("w").as_float();
		Ret->Meshes[i].Tex_Maps[4].m_h = Tex.child("down").attribute("h").as_float();

		Ret->Meshes[i].Tex_Maps[5].m_x = Tex.child("up").attribute("x").as_float();
		Ret->Meshes[i].Tex_Maps[5].m_y = Tex.child("up").attribute("y").as_float();
		Ret->Meshes[i].Tex_Maps[5].m_w = Tex.child("up").attribute("w").as_float();
		Ret->Meshes[i].Tex_Maps[5].m_h = Tex.child("up").attribute("h").as_float();

		i++;
	}

	// get pivots
	i = 0; // keep track of what pivot at
	for (pugi::xml_node Pivot = Data.child("pivot"); Pivot; Pivot = Pivot.next_sibling("pivot"))
	{
		Ret->my_Pivots[i].Name = Pivot.attribute("name").as_string();

		Ret->my_Pivots[i].Pos.x = Pivot.attribute("x").as_float();
		Ret->my_Pivots[i].Pos.y = Pivot.attribute("y").as_float();
		Ret->my_Pivots[i].Pos.z = Pivot.attribute("z").as_float();

		Ret->my_Pivots[i].Rotate.x = Pivot.attribute("rot_x").as_float();
		Ret->my_Pivots[i].Rotate.y = Pivot.attribute("rot_y").as_float();
		Ret->my_Pivots[i].Rotate.z = Pivot.attribute("rot_z").as_float();

		i++;
	}

	// get bones
	i = 0; // keep track of what bone at
	for (pugi::xml_node Bone = Data.child("bone"); Bone; Bone = Bone.next_sibling("bone"))
	{
		Ret->Bones[i] = Gen_XML_Bone( Bone );
		i++;
	}


	if( Ret->Size.x == 0 || Ret->Size.y == 0 || Ret->Size.z == 0 )// need to generate size
	{
		//float far_x_neg = 0.f, far_x_pos = 0.f;
		//for( uint b = 0;b < Ret->Num_Bones)
	}

	return Ret;
}

#define MESH_PACK 120
Mesh_SMDL_Container* Mesh_SMDL_Loaders::Load_Compiled_Mesh( std::string File )
{
	Mesh_SMDL_Container* Ret = NULL;

	const char* Data;
#ifdef _TEST
	= Engine.Application.Input.Load_File_Str(File).c_str();
#endif

	if(Data[0]==0)
	{
#ifdef _TEST
		Engine.Register_Error(S_Error("Cannot load mesh: " + File + ", no data",1));
#endif
		return Ret;
	}

	Ret = new Mesh_SMDL_Container;
#ifdef _TEST
	Engine.Mem_Add( Ret, "mesh cont" );
#endif

	Ret->Make(Data[0], 0, 0);

	for(uint l=0;l<Ret->Num_Meshes;l++)
	{
		//POSITION
		mematof(&Ret->Meshes[l].Position.x,((char*)Data) + l*MESH_PACK + 1);
		mematof(&Ret->Meshes[l].Position.y,((char*)Data) + l*MESH_PACK + 5);
		mematof(&Ret->Meshes[l].Position.z,((char*)Data) + l*MESH_PACK + 9);
		//SIZE
		mematof(&Ret->Meshes[l].Size.x,((char*)Data) + l*MESH_PACK + 13);
		mematof(&Ret->Meshes[l].Size.y,((char*)Data) + l*MESH_PACK + 17);
		mematof(&Ret->Meshes[l].Size.z,((char*)Data) + l*MESH_PACK + 21);
		//   TEXTURE MAPS
		for(u_char i=0;i<6;i++)
		{
			mematof(&Ret->Meshes[l].Tex_Maps[i].m_x, ((char*)Data) + l*MESH_PACK + i*16 + 25);
			mematof(&Ret->Meshes[l].Tex_Maps[i].m_y, ((char*)Data) + l*MESH_PACK + i*16 + 29);
			mematof(&Ret->Meshes[l].Tex_Maps[i].m_w, ((char*)Data) + l*MESH_PACK + i*16 + 33);
			mematof(&Ret->Meshes[l].Tex_Maps[i].m_h, ((char*)Data) + l*MESH_PACK + i*16 + 37);
		}
	}

	return Ret;
}

void Mesh_SMDL_Loaders::Compile_Mesh(std::string Source,std::string Dest)
{
	Mesh_SMDL_Container* nMesh = Load_Noncompiled_Mesh(Source);

	std::ofstream Writer(Dest.c_str(),std::ios::binary);

	Writer.write((const char*)&nMesh->Num_Meshes,1);

	char Buffer[6];memset(Buffer,0,6);

	for(uint i=0;i<nMesh->Num_Meshes;i++)
	{
		//POSITION
		memftoa(Buffer,&nMesh->Meshes[i].Position.x);
		  Writer.write(Buffer,4);memset(Buffer,0,4);
		memftoa(Buffer,&nMesh->Meshes[i].Position.y);
		  Writer.write(Buffer,4);memset(Buffer,0,4);
		memftoa(Buffer,&nMesh->Meshes[i].Position.z);
		  Writer.write(Buffer,4);memset(Buffer,0,4);
		//SIZE
		memftoa(Buffer,&nMesh->Meshes[i].Size.x);
		  Writer.write(Buffer,4);memset(Buffer,0,4);
		memftoa(Buffer,&nMesh->Meshes[i].Size.y);
		  Writer.write(Buffer,4);memset(Buffer,0,4);
		memftoa(Buffer,&nMesh->Meshes[i].Size.z);
		  Writer.write(Buffer,4);memset(Buffer,0,4);
		//   TEXTURE MAPS
		for(u_char l=0;l<6;l++)
		{
			memftoa(Buffer,&nMesh->Meshes[i].Tex_Maps[l].m_x);
			  Writer.write(Buffer,4);memset(Buffer,0,4);
			memftoa(Buffer,&nMesh->Meshes[i].Tex_Maps[l].m_y);
			  Writer.write(Buffer,4);memset(Buffer,0,4);
			memftoa(Buffer,&nMesh->Meshes[i].Tex_Maps[l].m_w);
			  Writer.write(Buffer,4);memset(Buffer,0,4);
			memftoa(Buffer,&nMesh->Meshes[i].Tex_Maps[l].m_h);
			  Writer.write(Buffer,4);memset(Buffer,0,4);
		}
	}
}
#endif
