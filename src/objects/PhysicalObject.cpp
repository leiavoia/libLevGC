#include "PhysicalObject.h"

#include <math.h>

#include <iostream>

#include "../core/extra_math.h"
#include "../utilities/LogManager.h"



namespace LGC {





PhysicalObject::PhysicalObject( float x, float y, float gravity)
	: pos( Vector(x,y) ), movement( Vector(0,0) ), gravity(gravity), frozen(false)
	{;;}
	
	
void PhysicalObject::MoveTo( float x, float y ) {
	if ( IsUnmovable() ) { return; }
	pos.x = x;
	pos.y = y;
// 	if ( pos.x < 0 ) { pos.x = 0; }
// 	else if ( pos.x > gamespace->SpaceWidth()-1 ) { pos.x = gamespace->SpaceWidth()-1; }
// 	if ( pos.y < 0 ) { pos.y = 0; }
// 	else if ( pos.y > gamespace->SpaceHeight()-1 ) { pos.y = gamespace->SpaceHeight()-1; }
	}

void PhysicalObject::Snap( float dx, float dy ) {
	if ( IsUnmovable() ) { return; }
	pos.x += dx;
	pos.y += dy;
// 	if ( pos.x < 0 ) { pos.x = 0; }
// 	else if ( pos.x > gamespace->SpaceWidth()-1 ) { pos.x = gamespace->SpaceWidth()-1; }
// 	if ( pos.y < 0 ) { pos.y = 0; }
// 	else if ( pos.y > gamespace->SpaceHeight()-1 ) { pos.y = gamespace->SpaceHeight()-1; }
	}

// moves if the object is moving
void PhysicalObject::UpdatePosition( float deltatime ) {
	if ( IsUnmovable() ) { return; }
	movement += Vector( 0, gravity * deltatime ); // internally apply gravity every frame
	pos += movement * deltatime;
	//LogMgr::Write(LOGMSG_DEBUG, "offset %f, %f", movement.x, movement.y);
	//LogMgr::Write(LOGMSG_DEBUG, "Now at: %f, %f --- offset %f, %f, --- delta: %f", pos.x, pos.y, movement.x, movement.y, deltatime);
	}

	
// adjust the movement vector
void PhysicalObject::AddImpulse(const Vector& F) {
	if ( IsUnmovable() ) { return; }
	movement += F; // mass is not included anymore
	}




	
} // end namespace LGC	






