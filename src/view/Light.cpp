#include "Light.h"

#include <iostream>

namespace LGC {
	
std::map< GLenum, bool > Light::lights_available;
	
	
void Light::LookAt( float x, float y, float z ) { 
	direction[0] = x - pos[0]; 
	direction[1] = y - pos[1]; 
	direction[2] = z - pos[2]; 
	glLightfv( light_number, GL_SPOT_DIRECTION, direction );
	}
	
void Light::SetAmbient( float r, float g, float b, float a ) { 
	ambient[0]=r; 
	ambient[1]=g; 
	ambient[2]=b; 
	ambient[3]=a; 
	glLightfv( light_number, GL_AMBIENT, ambient );
	}
	
void Light::SetSpecular( float r, float g, float b, float a ) { 
	specular[0]=r; 
	specular[1]=g; 
	specular[2]=b; 
	specular[3]=a; 
	glLightfv( light_number, GL_SPECULAR, specular );
	}
	
void Light::SetDiffuse( float r, float g, float b, float a ) { 
	diffuse[0]=r; 
	diffuse[1]=g; 
	diffuse[2]=b; 
	diffuse[3]=a; 
	glLightfv( light_number, GL_DIFFUSE, diffuse );
	}
	
void Light::SetPosition( float x, float y, float z ) { 
	pos[0]=x; 
	pos[1]=y; 
	pos[2]=z; 
// 	glLightfv( light_number, GL_POSITION, pos );
	}
	
void Light::SetDirection( float x, float y, float z ) { 
	direction[0]=x; 
	direction[1]=y; 
	direction[2]=z; 
	glLightfv( light_number, GL_SPOT_DIRECTION, direction );
	}
	 
void Light::SetDirectional( bool x ) {
	pos[3] = (x ? 1.0 : 0.0 ); 
// 	glLightfv( light_number, GL_POSITION, pos );
	}
	
void Light::SetSpotRadius( float x ) {
	spot_radius = x;
	glLightfv( light_number, GL_SPOT_CUTOFF, &spot_radius );
	}
	
void Light::SetSpotExponant( float x ) {
	spot_exponant = x;
	glLightfv( light_number, GL_SPOT_EXPONENT, &spot_exponant );
	}
	
void Light::SetConstantAttenuation( float x ) {
	constant_attenuation = x;
	glLightfv( light_number, GL_CONSTANT_ATTENUATION, &constant_attenuation );
	}
	
void Light::SetLinearAttenuation( float x ) {
	linear_attenuation = x;
	glLightfv( light_number, GL_LINEAR_ATTENUATION, &linear_attenuation );
	}
	
void Light::SetQuadraticAttenuation( float x ) {
	quadratic_attenuation = x;
	glLightfv( light_number, GL_QUADRATIC_ATTENUATION, &quadratic_attenuation );
	}	


GLenum Light::NewLightNumber() {
	// first of all, init the lights available
	if ( lights_available.empty() ) {
		lights_available.insert(   std::make_pair( GL_LIGHT0, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT1, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT2, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT3, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT4, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT5, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT6, false )   );
		lights_available.insert(   std::make_pair( GL_LIGHT7, false )   );
		}
	// find the first light that's inactive
	for ( std::map< GLenum, bool >::iterator i = lights_available.begin(); i != lights_available.end(); i++ ) {
		if ( !i->second ) {
			i->second = true; 
			return i->first;
			} 
		}
	// no can haz numb3rz?
	return 0;
	} 
	
void Light::RetireLightNumber( GLenum n ) {
	lights_available[n] = false;
	}
	
void Light::RetireMyLightNumber() {
	RetireLightNumber( light_number );
	}
	
void Light::SnagANewLightNumber() {
	light_number = Light::NewLightNumber();
	// update based on new number
	glLightfv( light_number, GL_AMBIENT, ambient );
	glLightfv( light_number, GL_SPECULAR, specular );
	glLightfv( light_number, GL_DIFFUSE, diffuse );
	glLightfv( light_number, GL_SPOT_DIRECTION, direction );
	glLightfv( light_number, GL_SPOT_CUTOFF, &spot_radius );
	glLightfv( light_number, GL_SPOT_EXPONENT, &spot_exponant );
	glLightfv( light_number, GL_CONSTANT_ATTENUATION, &constant_attenuation );
	glLightfv( light_number, GL_LINEAR_ATTENUATION, &linear_attenuation );
	glLightfv( light_number, GL_QUADRATIC_ATTENUATION, &quadratic_attenuation );
	}	
		
void Light::AddToScene() {
	glLightfv( light_number, GL_POSITION, pos );
	}
 
void Light::Draw() {
	static const float size = 0.5f;
	
	if ( !glIsEnabled(light_number) ) { return; }
	
	glPushAttrib( GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT );
				
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glDisable(GL_LIGHTING);
	glDisable(GL_TEXTURE_2D);
	
	if ( pos[3] != 0.0f ) {
		glColor4f( 1.0f, 0.3f, 0.3f, 1.0f );
		}
	else {
		glColor4f( 0.3f, 0.3f, 1.0f, 1.0f );
		}
	
	glBegin( GL_QUADS ); 
		// top         
		glVertex3f( pos[0]-size, pos[1]+size, pos[2]+size );
		glVertex3f( pos[0]-size, pos[1]-size, pos[2]+size );
		glVertex3f( pos[0]+size, pos[1]-size, pos[2]+size );
		glVertex3f( pos[0]+size, pos[1]+size, pos[2]+size );
		// bottom         
		glVertex3f( pos[0]-size, pos[1]+size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]+size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]-size, pos[2]-size );
		glVertex3f( pos[0]-size, pos[1]-size, pos[2]-size );
		// front         
		glVertex3f( pos[0]-size, pos[1]-size, pos[2]+size );
		glVertex3f( pos[0]-size, pos[1]-size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]-size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]-size, pos[2]+size );
		// back         
		glVertex3f( pos[0]-size, pos[1]+size, pos[2]+size );
		glVertex3f( pos[0]-size, pos[1]+size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]+size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]+size, pos[2]+size );
		// left         
		glVertex3f( pos[0]-size, pos[1]+size, pos[2]+size );
		glVertex3f( pos[0]-size, pos[1]+size, pos[2]-size );
		glVertex3f( pos[0]-size, pos[1]-size, pos[2]-size );
		glVertex3f( pos[0]-size, pos[1]-size, pos[2]+size );
		// right         
		glVertex3f( pos[0]+size, pos[1]+size, pos[2]+size );
		glVertex3f( pos[0]+size, pos[1]+size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]-size, pos[2]-size );
		glVertex3f( pos[0]+size, pos[1]-size, pos[2]+size );
	glEnd(); 	
	
	// Draw Direction
	glBegin( GL_LINES ); 
	if ( pos[3] == 0.0f ) {
		glVertex3f( pos[0] - direction[0], pos[1] - direction[1], pos[2] - direction[2] );
		glVertex3f( pos[0] + direction[0] * 2, pos[1] + direction[1] * 2, pos[2] + direction[2] * 2 );
		}
	else {
		glVertex3f( pos[0], pos[1], pos[2] );
		glVertex3f( pos[0] + direction[0], pos[1] + direction[1], pos[2] + direction[2] );
		}
	glEnd();	
		
	glPopAttrib();
	}
	
void Light::TurnOn() 	{ glEnable(light_number); std::cout << "turning on light [" << light_number << "]\n";}
void Light::TurnOff() 	{ glDisable(light_number); }
	
}



