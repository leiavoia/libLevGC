#include "Material.h"
#include <GL/gl.h>

#include <iostream>

namespace LGC {
	
	
std::string Material::active_material = "_";
	
	
	
void Material::SetAmbient( float r, float g, float b, float a ) { 
	ambient[0]=r; 
	ambient[1]=g; 
	ambient[2]=b; 
	ambient[3]=a; 
	}
	
void Material::SetSpecular( float r, float g, float b, float a ) { 
	specular[0]=r; 
	specular[1]=g; 
	specular[2]=b; 
	specular[3]=a; 
	}
	
void Material::SetDiffuse( float r, float g, float b, float a ) { 
	diffuse[0]=r; 
	diffuse[1]=g; 
	diffuse[2]=b; 
	diffuse[3]=a; 
	}
	
void Material::SetEmission( float r, float g, float b, float a ) { 
	emission[0]=r; 
	emission[1]=g; 
	emission[2]=b; 
	emission[3]=a; 
	}	
	
void Material::Switch( bool push ) {
	if ( active_material == name ) { return; }
	if ( push ) {
		pushed = push;
		glPushAttrib( GL_LIGHTING_BIT );	
		}
	active_material = name;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &shine);
	std::cout << "switched material to \"" << name << "\"\n";
	}
	
void Material::Pop() {
	if ( pushed ) {
		glPopAttrib();	
		pushed = false;
		}
	}
}



