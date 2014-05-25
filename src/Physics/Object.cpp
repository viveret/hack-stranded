#include "Object.hpp"	 

#include "../S_Engine.hpp"

#include "../utility/Functions.hpp"


/*Vector3 Animation_Step::Get_Rot()
{
	return Vector3();//this->Current;
}

void Animation_Step::New(Vector3 Pos,Vector3 Rot,float speed)
{
	this->Position=Pos;
	this->Rotation=Rot;

	this->Speed=speed;

	this->At=0.f;
}

void rotate3D::New(float nx,float ny,float nz)
{
	this->Position=Vector3(nx,ny,nz);

	this->Rot.Clear();

    rotate3D::local=true;
}

void Part::New(Vector3 nPos,Vector3 nSize,std::string Nname,float Mass)
{
    Part::name=Nname;
    Part::draw=true;
	this->ShowDots=false;

	this->Color.Clear();
	//this->Position=nPos;
	this->Size=nSize;
	//BULLET
	/*startTransform.setOrigin(btVector3(
										btScalar(2.0*i + start_x),
										btScalar(20+2.0*k + start_y),
										btScalar(2.0*j + start_z)));

			
					//using motionstate is recommended, it provides interpolation capabilities, and only synchronizes 'active' objects
					btDefaultMotionState* myMotionState = new btDefaultMotionState(startTransform);
					btRigidBody::btRigidBodyConstructionInfo rbInfo(mass,myMotionState,colShape,localInertia);
					btRigidBody* body = new btRigidBody(rbInfo);
					

					m_dynamicsWorld->addRigidBody(body);   *//*
	//
	btBoxShape* nShape=new btBoxShape(btVector3(nSize.x/2.,nSize.y/2.,nSize.z/2.));

	this->Body=mGame->Make_Body(Mass,nPos,nShape);
}

Vector3 Part::Get_Position()
{
	btVector3 nPos=Body->getCenterOfMassTransform().getOrigin();

	return Vector3(nPos.x(),nPos.y(),nPos.z());
}

Vector3 Part::Get_hSize()
{
	//btVector3 nSize=Body->getCollisionShape()->getAabb(

	return Vector3(this->Size.x/2.f,this->Size.y/2.f,this->Size.z/2.f);
}

Part::Part()
{
    this->draw=false;this->ShowDots=false;this->usetex=false;this->rep=false;//rep = repeat texture, objt = use object (parent) texture

	this->flags=0;
    this->Size=Vector3();
	this->Body=NULL;

    float r,g,b,a;
	void Color(float r,float g,float b,float a);

	void Texture(unsigned int nt0,unsigned int nt1,unsigned int nt2,unsigned int nt3,unsigned int nt4,unsigned int nt5,bool nRep);
	unsigned int t0,t1,t2,t3,t4,t5;//textures to use
    float tx0,ty0,tw0,th0;//Texturing 0
    float tx1,ty1,tw1,th1;//Texturing 1
    float tx2,ty2,tw2,th2;//Texturing 2
    float tx3,ty3,tw3,th3;//Texturing 3
    float tx4,ty4,tw4,th4;//Texturing 4
    float tx5,ty5,tw5,th5;//Texturing 5
}

Part::~Part()
{

}

void Part::DefTx()
{
	this->tx0=0;this->ty0=0;this->tw0=1;this->th0=1;//Texturing 0
    this->tx1=0;this->ty1=0;this->tw1=1;this->th1=1;//Texturing 1
    this->tx2=0;this->ty2=0;this->tw2=1;this->th2=1;//Texturing 2
    this->tx3=0;this->ty3=0;this->tw3=1;this->th3=1;//Texturing 3
    this->tx4=0;this->ty4=0;this->tw4=1;this->th4=1;//Texturing 4
    this->tx5=0;this->ty5=0;this->tw5=1;this->th5=1;//Texturing 5
}
		 
void Part::reColor(Vector4 nColor)
{			   
	this->Color=nColor;
}

void Part::Texture(unsigned int nt0,unsigned int nt1,unsigned int nt2,unsigned int nt3,unsigned int nt4,unsigned int nt5,bool nRep)
{	
	this->usetex=true;

	this->t0=nt0;
	this->t1=nt1;
	this->t2=nt2;
	this->t3=nt3;
	this->t4=nt4;
	this->t5=nt5;

	this->rep=nRep;

	this->reColor(1,1,1,1);
}

void Part::Draw(Vector3 nPos,int flags)
{
	if(this->draw)
    {				  
		glPolygonMode(GL_BACK,GL_POINT);
        glColor4f(1,1,1,1);

        glPushMatrix();

		/*glTranslatef(nPos.x,nPos.y,nPos.z);

        for(unsigned int r=0;r<this->rots.size();r++)
        {
            if(this->rots[r].local){glTranslatef(this->rots[r].Position.x+this->Position.x,this->rots[r].Position.y+this->Position.y,this->rots[r].Position.z+this->Position.z);}
            glRotatef(this->rots[r].Rot.y,0,1,0);
            glRotatef(this->rots[r].Rot.z,0,0,1);
            glRotatef(this->rots[r].Rot.x,1,0,0);
            if(this->ShowDots){
			sglDisable(GL_DEPTH_TEST);Engine.Application.Window.Use_Texture(false);
            glPointSize(3);
            glBegin(GL_POINTS);
                glVertex3f(0,0,0);
            glEnd();
			sglEnable(GL_DEPTH_TEST);Engine.Application.Window.Use_Texture(true);}
            if(this->rots[r].local){glTranslatef(-this->rots[r].Position.x-this->Position.x,-this->rots[r].Position.y-this->Position.y,-this->rots[r].Position.z-this->Position.z);}
        }	  
		
		glTranslatef(-nPos.x,-nPos.y,-nPos.z);*//*

        if(this->usetex)
        {
				Engine.Application.Window.Use_Texture(true);
				sglEnable(GL_DEPTH_TEST);
				glColor4f(this->Color.x,this->Color.y,this->Color.z,this->Color.a);

			//float Ad=1.0f/16.0f;
            float tx=this->tx0,ty=this->ty0,tw=this->tw0,th=this->th0;
			if(this->rep){tw=this->Get_hSize().z*2;th=this->Get_hSize().y*2;}
			float x=this->Get_Position().x+nPos.x,y=this->Get_Position().y+nPos.y,z=this->Get_Position().z+nPos.z;
			float l=this->Get_hSize().x*2,h=this->Get_hSize().y*2,w=this->Get_hSize().z*2;

			Engine.Resources.Stuff.Use_Texture(this->t0);this->RepProp();
            glBegin(GL_QUADS);//back
                glNormal3f(1,0,0);
                glTexCoord2f(tx,ty); glVertex3f(x,y+h,z);
                glTexCoord2f(tx,ty+th); glVertex3f(x,y,z);
                glTexCoord2f(tx+tw,ty+th); glVertex3f(x,y,z+w);
                glTexCoord2f(tx+tw,ty); glVertex3f(x,y+h,z+w);
            glEnd();
            tx=this->tx1,ty=this->ty1,tw=this->tw1,th=this->th1;  
			if(this->rep){tw=this->Get_hSize().z*2;th=this->Get_hSize().y*2;}
			Engine.Resources.Stuff.Use_Texture(this->t1);this->RepProp();
            glBegin(GL_QUADS);//front
                glNormal3f(-1,0,0);
                glTexCoord2f(tx,ty); glVertex3f(x+l,y+h,z+w);
                glTexCoord2f(tx,ty+th); glVertex3f(x+l,y,z+w);
                glTexCoord2f(tx+tw,ty+th); glVertex3f(x+l,y,z);
                glTexCoord2f(tx+tw,ty); glVertex3f(x+l,y+h,z);
            glEnd();
            tx=this->tx2,ty=this->ty2,tw=this->tw2,th=this->th2; 								 
			if(this->rep){tw=this->Get_hSize().x*2;th=this->Get_hSize().y*2;} 
			Engine.Resources.Stuff.Use_Texture(this->t2);this->RepProp();
            glBegin(GL_QUADS);//left
                glNormal3f(0,0,1);
                glTexCoord2f(tx,ty); glVertex3f(x+l,y+h,z);
                glTexCoord2f(tx,ty+th); glVertex3f(x+l,y,z);
                glTexCoord2f(tx+tw,ty+th); glVertex3f(x,y,z);
                glTexCoord2f(tx+tw,ty); glVertex3f(x,y+h,z);
            glEnd();
            tx=this->tx3,ty=this->ty3,tw=this->tw3,th=this->th3;  
			if(this->rep){tw=this->Get_hSize().x*2;th=this->Get_hSize().y*2;} 
			Engine.Resources.Stuff.Use_Texture(this->t3);this->RepProp();
            glBegin(GL_QUADS);//right
                glNormal3f(0,0,-1);
                glTexCoord2f(tx,ty); glVertex3f(x,y+h,z+w);
                glTexCoord2f(tx,ty+th); glVertex3f(x,y,z+w);
                glTexCoord2f(tx+tw,ty+th); glVertex3f(x+l,y,z+w);
                glTexCoord2f(tx+tw,ty); glVertex3f(x+l,y+h,z+w);
            glEnd();
            tx=this->tx4,ty=this->ty4,tw=this->tw4,th=this->th4;   
			if(this->rep){tw=this->Get_hSize().x*2;th=this->Get_hSize().z*2;}
			Engine.Resources.Stuff.Use_Texture(this->t4);this->RepProp();
            glBegin(GL_QUADS);//bottom
                glNormal3f(0,1,0);
                glTexCoord2f(tx,ty); glVertex3f(x+l,y,z+w);
                glTexCoord2f(tx,ty+th); glVertex3f(x,y,z+w);
                glTexCoord2f(tx+tw,ty+th); glVertex3f(x,y,z);
                glTexCoord2f(tx+tw,ty); glVertex3f(x+l,y,z);
            glEnd();
            tx=this->tx5,ty=this->ty5,tw=this->tw5,th=this->th5; 
			if(this->rep){tw=this->Get_hSize().x*2;th=this->Get_hSize().z*2;} 
			Engine.Resources.Stuff.Use_Texture(this->t5);this->RepProp();
            glBegin(GL_QUADS);//top
                glNormal3f(0,-1,0);
                glTexCoord2f(tx,ty); glVertex3f(x+l,y+h,z);
                glTexCoord2f(tx,ty+th); glVertex3f(x,y+h,z);
                glTexCoord2f(tx+tw,ty+th); glVertex3f(x,y+h,z+w);
                glTexCoord2f(tx+tw,ty); glVertex3f(x+l,y+h,z+w);
            glEnd();


			/*if(CheckFlag(flags,DRAW_OUTLINE))
			{
				Engine.Application.Window.Use_Texture(false);
				glColor3f(0,0,0);
				glBegin(GL_LINES);//back
					glVertex3f(x,y+h,z);
					glVertex3f(x,y,z);
					glVertex3f(x,y,z+w);
					glVertex3f(x,y+h,z+w);
				glEnd();
				glBegin(GL_LINES);//front
					glVertex3f(x+l,y+h,z+w);
					glVertex3f(x+l,y,z+w);
					glVertex3f(x+l,y,z);
					glVertex3f(x+l,y+h,z);
				glEnd();
				glBegin(GL_LINES);//left
					glVertex3f(x+l,y+h,z);
					glVertex3f(x+l,y,z);
					glVertex3f(x,y,z);
					glVertex3f(x,y+h,z);
				glEnd();
				glBegin(GL_LINES);//right
					glVertex3f(x,y+h,z+w);
					glVertex3f(x,y,z+w);
					glVertex3f(x+l,y,z+w);
					glVertex3f(x+l,y+h,z+w);
				glEnd();
				glBegin(GL_LINES);//bottom
					glVertex3f(x+l,y,z+w);
					glVertex3f(x,y,z+w);
					glVertex3f(x,y,z);
					glVertex3f(x+l,y,z);
				glEnd();
				glBegin(GL_LINES);//top
					glVertex3f(x+l,y+h,z);
					glVertex3f(x,y+h,z);
					glVertex3f(x,y+h,z+w);
					glVertex3f(x+l,y+h,z+w);
				glEnd();
				Engine.Application.Window.Use_Texture(true); 
			} *//*
					  
			
			glColor4f(1,1,1,1);
            glPopMatrix();
        }		
	glPolygonMode(GL_BACK,GL_FILL);
    }
}

void Part::RepProp()
{
	if(this->rep)
	{
	    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}
	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

void Object::New(std::string nName,Vector3 nPos,unsigned int ntex,bool nusetex,bool texrep)
{
	this->Name=nName;
	this->Position=nPos;

    Object::usetex=nusetex;
    Object::tex=ntex;
    Object::ShowDots=false;
	for(unsigned int p=0;p<this->Parts.size();p++)
	{
		this->Parts[p].Texture(ntex,ntex,ntex,ntex,ntex,ntex,texrep);
	}
}

void Object::Draw(Vector3 Pos,int nflags)
{
    glPushMatrix();

    //glLoadIdentity();
	glTranslatef(this->Position.x+Pos.x,this->Position.y+Pos.y,this->Position.z+Pos.z);
    for(unsigned int r=0;r<Object::rots.size();r++)
    {
        glTranslatef(Object::rots[r].Position.x,Object::rots[r].Position.y,Object::rots[r].Position.z);
        glRotatef(Object::rots[r].Rot.y,0,1,0);
        glRotatef(Object::rots[r].Rot.z,0,0,1);
        glRotatef(Object::rots[r].Rot.x,1,0,0);
        if(Object::ShowDots){
		sglDisable(GL_DEPTH_TEST);Engine.Application.Window.Use_Texture(false);
        glPointSize(3);
        glBegin(GL_POINTS);
            glVertex3f(0,0,0);
        glEnd();
		sglEnable(GL_DEPTH_TEST);Engine.Application.Window.Use_Texture(true);}
        glTranslatef(-Object::rots[r].Position.x,-Object::rots[r].Position.y,-Object::rots[r].Position.z);
    }
	glTranslatef(-this->Position.x-Pos.x,-this->Position.y-Pos.y,-this->Position.z-Pos.z);


    if(Object::usetex)
    {
		Engine.Resources.Stuff.Use_Texture(this->tex);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    for(unsigned int p=0;p<Object::Parts.size();p++)
    {
		this->Parts[p].Draw(Vector3(this->Position.x+Pos.x,this->Position.y+Pos.y,this->Position.z+Pos.z),0);
		/*if(CheckFlag(flags,DRAW_SHADOWS))
		{
			glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);
			glDepthMask(GL_FALSE);
			sglEnable(GL_STENCIL_TEST);

			this->Parts[p].Draw(Vector3(this->Position.x+Pos.x,this->Position.y+Pos.y,this->Position.z+Pos.z),DRAW_SHADOWS);

			glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
			glDepthMask(GL_TRUE);
			sglDisable(GL_STENCIL_TEST);
		}*/	/*
    }

    glPopMatrix();
}

void Object::Make(Object What)
{
    Object::Parts.clear();
    Object::rots.clear();

    for(unsigned int o=0;o<What.Parts.size();o++)
    {
        Object::Parts.push_back(What.Parts[o]);
    }
    for(unsigned int r=0;r<What.rots.size();r++)
    {
        Object::rots.push_back(What.rots[r]);
    }
}
/////////////////////////////////////////////////
using namespace Object_Namespace;

void Draw_Component::Do_Nulls()
{
	this->Texture=NULL;
	this->Tex_Map_Pos=NULL;
	this->Tex_Map_Size=NULL;
}

Draw_Component::Draw_Component(Base_Object* Parent)
{
	this->Do_Nulls();
	this->Defaults(Parent);
}

Draw_Component::Draw_Component()
{
	this->Do_Nulls();
	this->Defaults(NULL);
}

Draw_Component::~Draw_Component()
{
	this->Remove_Texture();
	this->Remove_Color();
	//
}		

void Draw_Component::Defaults(Base_Object* Parent)
{	   
	this->Remove_Texture();
	this->Remove_Color();
	//COLORS

	this->Parent=Parent;//additional info
}

void Draw_Component::Draw()
{
	//
}

void Draw_Component::Apply_Texture(unsigned int Tex)//only 1
{
	this->Remove_Texture();//back to normal
						   	
	this->Texture=new unsigned int(Tex);	   
	this->Multi_Texture=false;//only 1
	this->Texture_Defaults();//just maps
}

void Draw_Component::Apply_Texture(unsigned int* Texs)//only 6
{
	//
}

void Draw_Component::Apply_Texture_Map(Vector2 Pos,Vector2 Size)//only 1 for all faces
{
	//
}

void Draw_Component::Apply_Texture_Map(Vector2* Pos,Vector2* Size)//only 6
{
	//
}

void Draw_Component::Remove_Texture()
{
	if(this->Texture)
	{
		delete[] this->Texture;
		this->Texture=NULL;
	}
	if(this->Tex_Map_Pos)
	{
		delete[] this->Tex_Map_Pos;
		this->Tex_Map_Pos=NULL;
	}
	if(this->Tex_Map_Size)
	{
		delete[] this->Tex_Map_Size;
		this->Tex_Map_Size=NULL;
	}
}

void Draw_Component::Texture_Defaults()//just maps
{
	if(this->Texture && this->Tex_Map_Pos && this->Tex_Map_Size)
	{
		unsigned int Num_Texs=1;
		if(this->Multi_Texture){Num_Texs=6;}

		for(unsigned int t=0;t<Num_Texs;t++)
		{
			this->Tex_Map_Pos->Clear();
			this->Tex_Map_Size->Clear();
		}
	}
}

void Draw_Component::Apply_Color(Vector4 nColor)//only 1 color
{
	//
}

void Draw_Component::Apply_Color(Vector4* nColor)//only 6
{
	//
}

void Draw_Component::Remove_Color()
{
	if(this->Color)
	{
		delete[] this->Color;
		this->Color=NULL;
	}
}	

/*
//cleanup in the reverse order of creation/initialization

	//remove the rigidbodies from the dynamics world and delete them
	int i;
	for (i=m_dynamicsWorld->getNumCollisionObjects()-1; i>=0 ;i--)
	{
		btCollisionObject* obj = m_dynamicsWorld->getCollisionObjectArray()[i];
		btRigidBody* body = btRigidBody::upcast(obj);
		if (body && body->getMotionState())
		{
			delete body->getMotionState();
		}
		m_dynamicsWorld->removeCollisionObject( obj );
		delete obj;
	}

	//delete collision shapes
	for (int j=0;j<m_collisionShapes.size();j++)
	{
		btCollisionShape* shape = m_collisionShapes[j];
		delete shape;
	}
	*/


/*
using namespace E_Objects;

#include "../S_Engine.h"
extern S_Engine Engine;


#include "Bullet_Stuff.h"


#define DRAW_BULLET_TRAIL true
#define TRAIL_SIZE 10
#define TRAIL_TIME 500

#include "Game.h"

			   /*
Bullet_Data::Bullet_Data()
{
	this->id = Bullet_Data::ID_UNKNOWN;
	this->Hit = false;
	this->Body = NULL;
	this->Own_Obj = (void*)NULL;
}			 */
/*
Bullet_Data::Bullet_Data( uint nID, btRigidBody* nBody, void* Own,std::string nName )
{						  
	this->id = nID;
	this->Hit = false;
	this->Body = nBody;
	this->Own_Obj = Own;

	this->Name = nName;
}

void Bullet_Data::Update()
{
	//
}



Object_Projectile::Object_Projectile()
{
	memset( this->GLMatrix, 0, sizeof(btScalar) * 16 );

	this->Draw_Desc = NULL;
	this->Body = NULL;

	this->Trail_Time = 0.;
	this->Life_Time = 0.;
}

void Object_Projectile::Delete()
{
	this->Draw_Desc = NULL;	  
	if( this->Body )
	{
		mGame->Delete_Body( this->Body );
		this->Body = NULL;
	}
}


void Object_Projectile::Make( Vector3 Position, Vector3 Velocity, Projectile_Descriptor* Proj_Desc )
{
	if( !this->Body )
	{
		this->Draw_Desc = Proj_Desc;

		float Largest_Size = Proj_Desc->Size.x;
		if( Proj_Desc->Size.y > Largest_Size )
			Largest_Size = Proj_Desc->Size.y;

		//if( Proj_Desc->Size.z > Largest_Size )
			//Largest_Size = Proj_Desc->Size.z;
		btSphereShape* nShape = new btSphereShape( Largest_Size );

		this->Body = mGame->Make_Body(1.f,Vector3(Position.x + Proj_Desc->Size.x, Position.y + Proj_Desc->Size.y, Position.z + Proj_Desc->Size.z ),nShape, OBJ_NOROT, FILE_LINE );
		this->Body->setCollisionFlags( this->Body->getCollisionFlags() | btRigidBody::CF_CUSTOM_MATERIAL_CALLBACK );

		this->Body->setUserPointer( new Bullet_Data(Bullet_Data::ID_PROJECTILE, Body, (void*)this, "Projectile") );


		Body->setGravity( btVector3(0, -0.2 ,0) );
		this->Body->applyCentralImpulse( btVector3( Velocity.x*10, Velocity.y*10, Velocity.z*10 ) );
	}
}

void Object_Projectile::Update( double dt )
{
	this->Trail_Time += dt;
	this->Life_Time += dt;

	if( this->Trail_Time >= TRAIL_TIME )//after 1 second
	{
		this->Trail_Time = 0.;
		this->Update_Trail();
	}
}

void Object_Projectile::Draw()
{			
	glPushMatrix();
						
	this->Get_Matrix();
	glMultMatrixf((GLfloat*)this->GLMatrix);

	glScalef(this->Draw_Desc->Size.x,this->Draw_Desc->Size.y,this->Draw_Desc->Size.z); 

	glColor3f( this->Draw_Desc->Color.x, this->Draw_Desc->Color.y, this->Draw_Desc->Color.z );

	glBegin(GL_QUADS);//back	 
		glNormal3f(-1,0,0);
		glVertex3f(-1, 1,-1);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1,-1, 1);
		glVertex3f(-1, 1, 1);
	glEnd();

	glBegin(GL_QUADS);//front	
		glNormal3f(1,0,0);													
		glVertex3f( 1, 1, 1);
		glVertex3f( 1,-1, 1);
		glVertex3f( 1,-1,-1);
		glVertex3f( 1, 1,-1);
	glEnd();

	glBegin(GL_QUADS);//left   
		glNormal3f(0,0,-1);
		glVertex3f( 1, 1,-1);
		glVertex3f( 1,-1,-1);
		glVertex3f(-1,-1,-1);
		glVertex3f(-1, 1,-1);
	glEnd();

	glBegin(GL_QUADS);//right  
		glNormal3f(0,0,1);
		glVertex3f(-1, 1, 1);
		glVertex3f(-1,-1, 1);
		glVertex3f( 1,-1, 1);
		glVertex3f( 1, 1, 1);
	glEnd();

	glBegin(GL_QUADS);//bottom	  
		glNormal3f(0,-1,0);
		glVertex3f( 1,-1, 1);
		glVertex3f(-1,-1, 1);
		glVertex3f(-1,-1,-1);
		glVertex3f( 1,-1,-1);
	glEnd();

	glBegin(GL_QUADS);//top	 
		glNormal3f(0,1,0);
		glVertex3f( 1, 1,-1);
		glVertex3f(-1, 1,-1);
		glVertex3f(-1, 1, 1);
		glVertex3f( 1, 1, 1);
	glEnd();

	glPopMatrix();

	if( DRAW_BULLET_TRAIL && this->Trail.size() > 1 )
	{
		glColor3f( 1, 0, 1 );
		glBegin(GL_LINES);
		for(u_char i = 0;i < this->Trail.size() - 1; i++)
		{																		
			glVertex3f( this->Trail[i].x, this->Trail[i].y, this->Trail[i].z );
			glVertex3f( this->Trail[i+1].x, this->Trail[i+1].y, this->Trail[i+1].z );
		}
		glEnd();
	}
}

void Object_Projectile::Update_Trail()
{
	this->Trail.push_back( Bullet_Stuff::Get_Position(this->Body) );
	while( this->Trail.size() > TRAIL_SIZE )
		this->Trail.erase( this->Trail.begin() + 0 );// delete front/oldest
}


void Object_Projectile::Get_Matrix()
{
	if( this->Body )
	{
		btTransform trans3;
		this->Body->getMotionState()->getWorldTransform(trans3);
		trans3.getOpenGLMatrix(this->GLMatrix);
	}
}




Object_Base_Lean::Object_Base_Lean()
{		  
	this->Body=NULL;
}

Object_Base_Lean::~Object_Base_Lean()
{
	//
}

void Object_Base_Lean::Reset()
{		   		 
	this->Delete_Body();
	this->Tex_ID=0;
}

void Object_Base_Lean::Delete_Body()
{
	if(this->Body)
	{
		mGame->Delete_Body(this->Body);
		this->Body=NULL;
	}
}

void Object_Base::Make_From(Vector3 nPos,Vector3 nSize,float Mass,uint Flags)//center of mass and half size
{
	if(!this->Body)
	{
		this->Size.Set(nSize.x,nSize.y,nSize.z);
		this->Position=nPos;
							 
		Position.x+=nSize.x/2.f;
		Position.y+=nSize.y/2.f;
		Position.z+=nSize.z/2.f;

		btBoxShape* nShape=new btBoxShape(btVector3(nSize.x/2.f,Size.y/2.f,nSize.z/2.f));

		this->Body=mGame->Make_Body(Mass,Position,nShape,Flags, FILE_LINE );
	}
}



Object_Base::Object_Base()
{
	this->Mesh=NULL;

	this->Reset();
}

Object_Base::~Object_Base()
{
	//this->Delete_Body();
}

void Object_Base::Reset()
{
	this->Delete_Body();

	this->Flags=NULL;	   

	memset(this->GLMatrix,0,sizeof(btScalar)*16);
		
	this->Draw_Bounding_Box=false;

	this->Tex_ID=0;
	this->Mesh=NULL;
}

void Object_Base_Lean::Make_From(Vector3 nPos,Vector3 nSize,float Mass,uint Flags)//center of mass and half size
{
	if(!this->Body)
	{
		this->Size.Set(nSize.x,nSize.y,nSize.z);
		this->Position=nPos;
							 
		Position.x+=nSize.x/2.f;
		Position.y+=nSize.y/2.f;
		Position.z+=nSize.z/2.f;

		btBoxShape* nShape=new btBoxShape(btVector3(nSize.x/2.f,Size.y/2.f,nSize.z/2.f));

		this->Body=mGame->Make_Body(Mass,Position,nShape,Flags, FILE_LINE );
	}
}

void Object_Base_Lean::Draw()
{
	glPushMatrix();
						  
	Engine.Resources.Use_Texture(this->Tex_ID);

	glTranslatef(this->Position.x,this->Position.y,this->Position.z);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glScalef(this->Size.x/2.f,this->Size.y/2.f,this->Size.z/2.f);

	glBegin(GL_QUADS);//back
		glNormal3f(-1,0,0);
		glTexCoord2f(           0,           0); glVertex3f(-1, 1,-1);
		glTexCoord2f(           0,this->Size.y); glVertex3f(-1,-1,-1);
		glTexCoord2f(this->Size.x,this->Size.y); glVertex3f(-1,-1, 1);
		glTexCoord2f(this->Size.x,           0); glVertex3f(-1, 1, 1);
	glEnd();

	glBegin(GL_QUADS);//front
		glNormal3f(1,0,0);
		glTexCoord2f(           0,           0); glVertex3f( 1, 1, 1);
		glTexCoord2f(           0,this->Size.y); glVertex3f( 1,-1, 1);
		glTexCoord2f(this->Size.x,this->Size.y); glVertex3f( 1,-1,-1);
		glTexCoord2f(this->Size.x,           0); glVertex3f( 1, 1,-1);
	glEnd();

	glBegin(GL_QUADS);//left
		glNormal3f(0,0,-1);
		glTexCoord2f(           0,           0); glVertex3f( 1, 1,-1);
		glTexCoord2f(           0,this->Size.y); glVertex3f( 1,-1,-1);
		glTexCoord2f(this->Size.z,this->Size.y); glVertex3f(-1,-1,-1);
		glTexCoord2f(this->Size.z,           0); glVertex3f(-1, 1,-1);
	glEnd();

	glBegin(GL_QUADS);//right
		glNormal3f(0,0,1);
		glTexCoord2f(           0,           0); glVertex3f(-1, 1, 1);
		glTexCoord2f(           0,this->Size.y); glVertex3f(-1,-1, 1);
		glTexCoord2f(this->Size.z,this->Size.y); glVertex3f( 1,-1, 1);
		glTexCoord2f(this->Size.z,           0); glVertex3f( 1, 1, 1);
	glEnd();

	glBegin(GL_QUADS);//bottom
		glNormal3f(0,-1,0);
		glTexCoord2f(           0,           0); glVertex3f( 1,-1, 1);
		glTexCoord2f(           0,this->Size.z); glVertex3f(-1,-1, 1);
		glTexCoord2f(this->Size.x,this->Size.z); glVertex3f(-1,-1,-1);
		glTexCoord2f(this->Size.x,           0); glVertex3f( 1,-1,-1);
	glEnd();

	glBegin(GL_QUADS);//top
		glNormal3f(0,1,0);
		glTexCoord2f(           0,           0); glVertex3f( 1, 1,-1);
		glTexCoord2f(           0,this->Size.z); glVertex3f(-1, 1,-1);
		glTexCoord2f(this->Size.x,this->Size.z); glVertex3f(-1, 1, 1);
		glTexCoord2f(this->Size.x,           0); glVertex3f( 1, 1, 1);
	glEnd();

	glPopMatrix();
}



void Object_Base::Make_From(Vector3 nPos,Vector3 nSize,uint Flags)//center of mass and half size
{
	this->Make_From(nPos,nSize,(nSize.x * nSize.y * nSize.z),Flags);
}

Vector3 Object_Base::Get_Position()
{
	btVector3 nPos=this->Body->getCenterOfMassTransform().getOrigin();

	return Vector3(nPos.x(),nPos.y(),nPos.z());
}

Vector3 Object_Base::Get_hSize()
{
	//btVector3 nSize=Body->getCollisionShape()->getAabb(

	return Vector3(this->Size.x/2.f,this->Size.y/2.f,this->Size.z/2.f);
}


void Object_Base::Update(double dt)
{
	if(this->Body)
	{
		if(this->Acceleration.x!=0)
		{	
			this->Body->applyCentralImpulse(btVector3(this->Acceleration.x*(btScalar)dt*P_ADD,0,0)); 
			this->Body->activate();

			
			signed char Sign_Change = Sign(this->Acceleration.x);
			this->Acceleration.x-=float(P_DRAG * dt * Sign(this->Acceleration.x));
			if(Sign_Change!=Sign(this->Acceleration.x)){this->Acceleration.x=0;}//changed sign
											 
			//signed char Sign_Change=Sign(this->Acceleration.x);
			//this->Acceleration.x-=((P_DRAG * Sign(this->Acceleration.x)) + P_DRAG_1)*dt;
			//if(Sign_Change!=Sign(this->Acceleration.x)){this->Acceleration.x=0;}//changed sign
		}		  
		if(this->Acceleration.y!=0)
		{																					  
			this->Body->applyCentralImpulse(btVector3(0,this->Acceleration.y*(btScalar)dt*P_ADD,0));
			this->Body->activate();
																				   
			signed char Sign_Change=Sign(this->Acceleration.y);
			this->Acceleration.y-=float(P_DRAG * dt * Sign(this->Acceleration.y));
			if(Sign_Change!=Sign(this->Acceleration.y)){this->Acceleration.y=0;}//changed sign


			//signed char Sign_Change=Sign(this->Acceleration.y);
			//this->Acceleration.y-=((P_DRAG * Sign(this->Acceleration.y)) + P_DRAG_1)*dt;
			//if(Sign_Change!=Sign(this->Acceleration.y)){this->Acceleration.y=0;}//changed sign
		}
		if(this->Acceleration.z!=0)
		{																					  
			this->Body->applyCentralImpulse(btVector3(0,0,this->Acceleration.z*(btScalar)dt*P_ADD));
			this->Body->activate();
														  
			signed char Sign_Change=Sign(this->Acceleration.z);
			this->Acceleration.z-=float(P_DRAG * dt * Sign(this->Acceleration.z));
			if(Sign_Change!=Sign(this->Acceleration.z)){this->Acceleration.z=0;}//changed sign

			//signed char Sign_Change=Sign(this->Acceleration.z);
			//this->Acceleration.z-=((P_DRAG * Sign(this->Acceleration.z)) + P_DRAG_1)*dt;
			//if(Sign_Change!=Sign(this->Acceleration.z)){this->Acceleration.z=0;}//changed sign
		}
							 
		btTransform trans3;
		this->Body->getMotionState()->getWorldTransform(trans3);

		this->Position = Vector3(trans3.getOrigin().getX(),trans3.getOrigin().getY(),trans3.getOrigin().getZ());		
	}
}

void Object_Base::Get_Matrix()
{			 
	btTransform trans3;
	this->Body->getMotionState()->getWorldTransform(trans3);
	trans3.getOpenGLMatrix(this->GLMatrix);
}

void Object_Base::Draw()
{
	glPushMatrix();

	this->Get_Matrix();
	// pass that matrix to OpenGL and render the cube	
	glMultMatrixf((GLfloat*)this->GLMatrix);

	Engine.Resources.Use_Texture(this->Tex_ID);


	if(this->Mesh)
		this->Mesh->Draw();
	else
	{
		glPushMatrix();

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,  GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glScalef(this->Size.x/2.f,this->Size.y/2.f,this->Size.z/2.f);

		glBegin(GL_QUADS);//back
			glNormal3f(-1,0,0);
			glTexCoord2f(           0,           0); glVertex3f(-1, 1,-1);
			glTexCoord2f(           0,this->Size.y); glVertex3f(-1,-1,-1);
			glTexCoord2f(this->Size.x,this->Size.y); glVertex3f(-1,-1, 1);
			glTexCoord2f(this->Size.x,           0); glVertex3f(-1, 1, 1);
		glEnd();

		glBegin(GL_QUADS);//front
			glNormal3f(1,0,0);
			glTexCoord2f(           0,           0); glVertex3f( 1, 1, 1);
			glTexCoord2f(           0,this->Size.y); glVertex3f( 1,-1, 1);
			glTexCoord2f(this->Size.x,this->Size.y); glVertex3f( 1,-1,-1);
			glTexCoord2f(this->Size.x,           0); glVertex3f( 1, 1,-1);
		glEnd();

		glBegin(GL_QUADS);//left
			glNormal3f(0,0,-1);
			glTexCoord2f(           0,           0); glVertex3f( 1, 1,-1);
			glTexCoord2f(           0,this->Size.y); glVertex3f( 1,-1,-1);
			glTexCoord2f(this->Size.z,this->Size.y); glVertex3f(-1,-1,-1);
			glTexCoord2f(this->Size.z,           0); glVertex3f(-1, 1,-1);
		glEnd();

		glBegin(GL_QUADS);//right
			glNormal3f(0,0,1);
			glTexCoord2f(           0,           0); glVertex3f(-1, 1, 1);
			glTexCoord2f(           0,this->Size.y); glVertex3f(-1,-1, 1);
			glTexCoord2f(this->Size.z,this->Size.y); glVertex3f( 1,-1, 1);
			glTexCoord2f(this->Size.z,           0); glVertex3f( 1, 1, 1);
		glEnd();

		glBegin(GL_QUADS);//bottom
			glNormal3f(0,-1,0);
			glTexCoord2f(           0,           0); glVertex3f( 1,-1, 1);
			glTexCoord2f(           0,this->Size.z); glVertex3f(-1,-1, 1);
			glTexCoord2f(this->Size.x,this->Size.z); glVertex3f(-1,-1,-1);
			glTexCoord2f(this->Size.x,           0); glVertex3f( 1,-1,-1);
		glEnd();

		glBegin(GL_QUADS);//top
			glNormal3f(0,1,0);
			glTexCoord2f(           0,           0); glVertex3f( 1, 1,-1);
			glTexCoord2f(           0,this->Size.z); glVertex3f(-1, 1,-1);
			glTexCoord2f(this->Size.x,this->Size.z); glVertex3f(-1, 1, 1);
			glTexCoord2f(this->Size.x,           0); glVertex3f( 1, 1, 1);
		glEnd();

		glPopMatrix();
	}

	if(this->Draw_Bounding_Box)
	{
		glPushMatrix();

		glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
		glColor3f(1,0,0);		 
		Engine.Application.OpenGL.GL_Shader->Disable(GL_TEXTURE_2D);
		Engine.Application.OpenGL.GL_Shader->Disable(GL_LIGHTING);

		glScalef(this->Size.x/2.f,this->Size.y/2.f,this->Size.z/2.f);


		glBegin(GL_QUADS);//back
			glVertex3f(-1, 1,-1);
			glVertex3f(-1,-1,-1);
			glVertex3f(-1,-1, 1);
			glVertex3f(-1, 1, 1);
		glEnd();

		glBegin(GL_QUADS);//front
			glVertex3f( 1, 1, 1);
			glVertex3f( 1,-1, 1);
			glVertex3f( 1,-1,-1);
			glVertex3f( 1, 1,-1);
		glEnd();

		glBegin(GL_QUADS);//left
			glVertex3f( 1, 1,-1);
			glVertex3f( 1,-1,-1);
			glVertex3f(-1,-1,-1);
			glVertex3f(-1, 1,-1);
		glEnd();

		glBegin(GL_QUADS);//right
			glVertex3f(-1, 1, 1);
			glVertex3f(-1,-1, 1);
			glVertex3f( 1,-1, 1);
			glVertex3f( 1, 1, 1);
		glEnd();

		glBegin(GL_QUADS);//bottom
			glVertex3f( 1,-1, 1);
			glVertex3f(-1,-1, 1);
			glVertex3f(-1,-1,-1);
			glVertex3f( 1,-1,-1);
		glEnd();

		glBegin(GL_QUADS);//top
			glVertex3f( 1, 1,-1);
			glVertex3f(-1, 1,-1);
			glVertex3f(-1, 1, 1);
			glVertex3f( 1, 1, 1);
		glEnd();


		glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
		glColor3f(1,1,1);		
		Engine.Application.OpenGL.GL_Shader->Enable(GL_TEXTURE_2D);
		Engine.Application.OpenGL.GL_Shader->Enable(GL_LIGHTING);

		glPopMatrix();
	}


	glPopMatrix();or
}		

void Object_Base::Set_Rotation(Vector3 Angles)
{
	//this->Body->setAngularFactor(btVector3(Angles.x,Angles.y,Angles.z));
}

void Object_Base::Set_RotationX(float Angle)
{
	//this->Body->set
}

void Object_Base::Set_RotationY(float Angle)
{
}

void Object_Base::Set_RotationZ(float Angle)
{
}
								   //internalApplyImpulse


void Object_Container::Delete()
{
	for(uint i=0;i<this->Objects.size();i++)
		delete this->Objects[i];

	this->Objects.clear();
}

void Object_Container::Add_To(Object_Container* Other)
{			 
	for(uint i=0;i<Other->Objects.size();i++)
		this->Objects.push_back(Other->Objects[i]);
}

void E_Objects::Object_Container::Draw()
{
	glPushMatrix();

	for(uint i=0;i<this->Objects.size();i++)
		this->Objects[i]->Draw();

	glPopMatrix();
}	
*/
