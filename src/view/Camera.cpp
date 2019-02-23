#include "Camera.h"
#include <math.h>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../core/extra_math.h"

// degrees per second
#define servo_speed 180.0f

namespace LGC {
	
	
	
void Camera::LookAt( float time_delta ) {

	// camera starts looking down Z, up=Y
	glLoadIdentity();
	
	
	float zdiff = ztarget - zpos;
	float ydiff = ytarget - ypos;
	float xdiff = xtarget - xpos;
	float dist = sqrt( ydiff*ydiff + xdiff*xdiff );
	if ( zdiff == 0 ) { zdiff = 0.0000001; }
	if ( ydiff == 0 ) { ydiff = 0.0000001; }
	if ( dist == 0 ) { dist = 0.0000001; }
	
	float servo_movement = servo_speed *time_delta;
	
	
	
	// ROTATE X
	float angle_to_target = RadToDeg( atan( dist / zdiff ) );
	if ( zdiff >= 0 ) { angle_to_target += 180.0; }
	ClampAngle( angle_to_target );
	// if we're pretty close, just snap
	if ( snap || AngleInRange( angle_to_target, servo2_angle, servo_movement ) ) {
		servo2_angle = angle_to_target;
		}
	else { // move clockwise or counterclockwise?
		float d = angle_to_target - servo2_angle;
		if ( (d > 0 && d <= 90) || (d < -90) ) { servo2_angle += servo_movement; }
		else { servo2_angle -= servo_movement; }
		ClampAngle( servo2_angle );
		}
	
	glRotatef( servo2_angle, 1, 0, 0 );
				
				
				
	// ROTATE Z
	angle_to_target = RadToDeg( atan( xdiff / ydiff ) );
	// figure out which quadrant we're in - normalize the angle to 360 space (arctangent problem)
	if ( ydiff <= 0 ) 		{ angle_to_target = 180.0 + angle_to_target; }
	else if ( xdiff <= 0 ) 	{ angle_to_target = -fabs(angle_to_target); }
	else 				{ angle_to_target = fabs(angle_to_target); }
	ClampAngle( angle_to_target );
	// if we're pretty close, just snap
	if ( snap || AngleInRange( angle_to_target, servo1_angle, servo_movement ) ) {
// 		std::cout << "!ZSNAP: " << servo1_angle << " " << angle_to_target << "\n";
		servo1_angle = angle_to_target;
		}
	else { // move clockwise or counterclockwise?
// 		std::cout << "z rot from " << servo1_angle;
		float d = angle_to_target - servo1_angle;
		if ( (d > 0 && d <= 180) || (d < -180) ) { servo1_angle += servo_movement; }
		else { servo1_angle -= servo_movement; }
		ClampAngle( servo1_angle );
// 		std::cout << " to " << servo1_angle << " (target => " << angle_to_target << ")" << "\n";
		}
		
	glRotatef( servo1_angle, 0, 0, 1 );	
	
	
	// move to location
	glTranslatef( -xpos, -ypos, -zpos );
	
	snap = false;
	}
		
void Camera::SetLocation( float x, float y, float z ) {
	xpos = x;
	ypos = y;
	zpos = z;
	}
	
void Camera::SetTarget( float x, float y, float z, bool snap ) {
	xtarget = x;
	ytarget = y;
	ztarget = z;
	this->snap = snap;
	}
		
void Camera::ChangeLens( float size, int screen_width, int screen_height ) {
	lens_size = size;
   	if ( screen_height == 0 ) { screen_height = 1; }
	GLfloat ratio = ( GLfloat )screen_width / ( GLfloat )screen_height;
   	glMatrixMode( GL_PROJECTION );
    	glLoadIdentity();
	gluPerspective( lens_size, ratio, 0.1, 1000.0f );
	glMatrixMode( GL_MODELVIEW );
	}		
	
void Camera::ClampAngle( float &a ) {
	if ( a < 0 ) { a += 360; }
	else if ( a > 360 ) { a -= 360; }		
	}
	
// assume angles are clamped 0-360;
bool Camera::AngleInRange( float a, float b, float range ) {
	if ( a > b ) {
		float temp = b;
		b = a;
		a = temp;
		}
	a += 360;
	b += 360;
	if ( b - a > 180 ) { a += 360; }
	return ( b > a - range && b < a + range );
	}	
	
	
	
	
}



