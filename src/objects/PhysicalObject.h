#ifndef PHYSICAL_OBJECT_H
#define PHYSICAL_OBJECT_H
#include <SDL/SDL.h>


#include "../algo/collision/Vector.h"

// predec:
//class CollisionObject;


namespace LGC {





class PhysicalObject {
public:

	friend class CollisionObject;
	
	PhysicalObject( float x, float y, float gravity=0.0 );
	
	virtual ~PhysicalObject() {;;}
	
	// moves if the object is moving
	virtual void UpdatePosition( float deltatime );
	
	// adjust the movement vector
	void AddImpulse(const Vector& impulse);

	// move to a coord on the map without travelling in between
	virtual void MoveTo( float x, float y );
	
	// A one-time movement
	virtual void Snap( float dx, float dy );
	
	// halt all movement (nulls the movement vector)
	void StopMovement() { movement = Vector(0,0); }
	void StopXMovement() { movement.x = 0; }
	void StopYMovement() { movement.y = 0; }
	
	// prevents movement in any way. 
	void FreezeMovement() { frozen = true; StopMovement(); }
	void UnfreezeMovement() { frozen = false; }
	
	virtual bool IsUnmovable() { return frozen; }
	virtual bool IsMovable() { return !frozen; }
	
	inline void		SetGravity(float x) 	{ gravity = x; };
	inline float 		Gravity() 			{ return gravity; }
	inline float 		Xpos() 				{ return pos.x; }
	inline float 		Ypos() 				{ return pos.y; }
	inline float		XMovement()			{ return movement.x; }
	inline float		YMovement()			{ return movement.y; }
	inline const Vector	Pos()				{ return pos; }
	

private:

	// new version
	Vector pos;
	Vector movement;
	float gravity;
	bool frozen;
	
	};




	
} // end namespace LGC	



#endif



