#include "Material.hpp"
using namespace SE_Render_Material;

#include "../gl/OpenGL.hpp"


Material_Base::Material_Base()
{
	this->M_Defaults();
}

void Material_Base::G_Defaults()// set global defaults
{
	Material_Base Temp;
	Temp.Apply_Material();
}

void Material_Base::M_Defaults()// set own to defaults
{
	this->Color_Ambient.Set( 0.2f, 0.2f, 0.2f, 1.f );
	this->Color_Diffuse.Set( 0.8f, 0.8f, 0.8f, 1.f );
	this->Color_Specular.Set( 0, 0, 0, 1 );
	this->Color_Emission.Set( 0, 0, 0, 1 );

	this->Shininess = 0.f;
}

void Material_Base::Apply_Material()
{
	glColor4fv( this->Color );
	glMaterialfv( GL_FRONT, GL_AMBIENT, this->Color_Ambient );
	glMaterialfv( GL_FRONT, GL_DIFFUSE, this->Color_Diffuse );
	glMaterialfv( GL_FRONT, GL_SPECULAR, this->Color_Specular );
	glMaterialfv( GL_FRONT, GL_EMISSION, this->Color_Emission );

	glMaterialf( GL_FRONT, GL_SHININESS, this->Shininess );
}
