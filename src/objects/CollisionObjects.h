

#ifndef COLLISION_OBJECTS_H
#define COLLISION_OBJECTS_H

//#include <stdlib.h>
//#include <iostream>
#include <list>
#include <vector>
#include "PhysicalObject.h"
#include "PollingObject.h"
#include "LifeObject.h"
#include <math.h>





namespace LGC {






/** Basic collisionable game object types */
enum collision_obj_type {
	COLLISION_OBJ_NULL=0,
	COLLISION_OBJ_PLAYER_WEAPON, // weapons go first for processing
	COLLISION_OBJ_ENEMY_WEAPON,
	COLLISION_OBJ_UNOWNED_WEAPON,
	COLLISION_OBJ_PLAYER,	
	COLLISION_OBJ_ENEMY,	
	COLLISION_OBJ_UNOWNED,	
	COLLISION_OBJ_WORLD,
	COLLISION_OBJ_NUM_TYPES
	};


	
	
//------------------------------------------------------------------------------------------------
// SquareLookupTable:
//    this is a table template that holds anything.
//    It's rows and columns are created by the number of different game objects you have,
//    creating a square table.
//------------------------------------------------------------------------------------------------ 
template < typename TYPE, int NUMTYPES> struct SquareLookupTable {
public:
	SquareLookupTable() {
		memset(table, 0, sizeof(table));
    		}

	void SetElement(int A, int B, TYPE x) {
		table[A][B] = x;
		table[B][A] = x; // this means only combos are good, no perms
		}

	TYPE operator()(int A, int B) { return table[A][B]; }

private:
	TYPE table[NUMTYPES][NUMTYPES];
	};






// predecs:
class CollisionObjectPlayer;
class CollisionObjectPWeapon;
class CollisionObjectEnemy;
class CollisionObjectEWeapon;
class CollisionObjectWorld;
class CollisionObjectUnowned;
class Quadtree;
	
	
// CollisionObject -------------------------------------\/---------------------

/**
CollisionObject defines all the necessary things to make an object collidable.
*/
class CollisionObject: public PhysicalObject {
public:

	// TYPEDEFS -------------------------------------------
	
	typedef std::vector<  std::vector<Vector>  > polygon_list;
	typedef std::vector<Vector> vertex_list;
	
	
	// *STRUCTORS -------------------------------------------
	
	// default with NO geometry
	CollisionObject(
		float x, 
		float y, 		
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=1.0, 
		float friction=0.0
		);
	
	// custom polygon vertices
	CollisionObject( 
		const vertex_list& incoming_vertices, 
		const Vector& xPosition, 
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=1.0, 
		float friction=0.0
		);
	
	// rectangle - used to create walls that are unmovable by default
	CollisionObject(
		const Vector& xPosition, 
		float width, 
		float height, 
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=1.0, 
		float friction=0.0
		);

	// destructor
	virtual ~CollisionObject();

	
	
	
	
	// ACCESSOR FUNCTIONS ------------------------------
	
	inline bool	IsUnmovable() { return (Mass < 0.0001f) || PhysicalObject::IsUnmovable(); }
	inline collision_obj_type GetObjType() { return obj_type; }
	
	inline float&	GetMass() { return Mass; }
	inline float&	GetInvMass() { return InvMass; }
	inline float	GetFriction() { return Friction; }
	inline float	GetElasticity() { return Elasticity; }
	
	inline void	SetFriction (float x) { Friction = x; }
	inline void	SetElasticity (float x) { Elasticity = x; }
	inline void	SetMass (float x) { Mass = x; }
	
	
	
	// GEOMETRY FUNCTIONS ----------------------------
	
	void AddPolygon( const vertex_list v ); 
	
	inline void ClearGeometry() { vertices.clear(); }
	
	void DefineCollisionRect( float x, float y, float w, float h );
	
	void DefineCollisionCircle( float x, float y, int radius, int points );



	// INTERSECT FUNCTIONS -------------------------
	
	bool Collide (CollisionObject& Body, float dt, Vector& N); // N = provided collision normal
	
	void ProcessCollision (CollisionObject& xBody, const Vector& N);
	
	void ProcessOverlap (CollisionObject& xBody, const Vector& MTD);
	
	/** Performs a basic box test to simplify more expensive computations.
	    NOTE: Returns true if the rough spatial areas overlap, not if they truly intersect. */
	bool TestBounds( CollisionObject* obj )  {
		if (
			obj->XMin() > XMax() ||
			obj->XMax() < XMin() ||
			obj->YMin() > YMax() ||
			obj->YMax() < YMin()
			) { return false; }
		return true;
		}
		
	// INTERSECTION RESPONSE FUNCTIONS --------------------------------
	
	virtual bool WantsAdjustment() { return true; }
	
	// the dispatch function. Defers to the specialized versions below.
	virtual void Intersect(CollisionObject* obj, Vector& collision_normal) = 0;
	
	virtual void Intersect(CollisionObjectPlayer* obj, Vector& collision_normal) { return; }

	virtual void Intersect(CollisionObjectPWeapon* obj, Vector& collision_normal) { return; }

	virtual void Intersect(CollisionObjectEnemy* obj, Vector& collision_normal) { return; }

	virtual void Intersect(CollisionObjectEWeapon* obj, Vector& collision_normal) { return; }
	
	virtual void Intersect(CollisionObjectWorld* obj, Vector& collision_normal) { return; }

	virtual void Intersect(CollisionObjectUnowned* obj, Vector& collision_normal) { return; }

	
	
	
	// QUADTREE FUNCTIONS -------------------------

	inline int SpaceNode() { return space_node; }

	inline void SetSpaceNode( int x ) { space_node = x; }
	
	// this is accurate as long as FPS > 10
	inline float XMin() { return xmin + pos.x - (fabs(movement.x) * 0.1); };
	inline float XMax() { return xmax + pos.x + (fabs(movement.x) * 0.1); };
	inline float YMin() { return ymin + pos.y - (fabs(movement.y) * 0.1); };
	inline float YMax() { return ymax + pos.y + (fabs(movement.y) * 0.1); };
	
	inline float XMinRaw() { return xmin + pos.x; };
	inline float XMaxRaw() { return xmax + pos.x; };
	inline float YMinRaw() { return ymin + pos.y; };
	inline float YMaxRaw() { return ymax + pos.y; };



	// ACTIVATION FUNCTIONS -------------------------

	void SetCollisionActive( bool x = true );

	inline bool IsCollisionActive() { return collision_active; }



	// MOVEMENT FUNCTIONS -------------------------

	// inherited from PhysicalObject - we add quadtree sorting here
	void UpdatePosition( float deltatime );
	
	// inherited from PhysicalObject - we add quadtree sorting here
	void MoveTo( float x, float y );



	// STATIC FUNCTIONS -------------------------

	static void Cycle( float deltatime );

	inline static void ToggleDrawGeometry() { draw_geo = !draw_geo; }
	
	inline static bool GeometryBeingDrawn() { return draw_geo; }

	static void ResetQuadtree( int x, int y, int div=250 );
	
	
protected:

	polygon_list vertices;
	
	collision_obj_type obj_type;	
	bool collision_active;
	float	Mass;
	float	InvMass;
	float 	Friction;
	float 	Elasticity;
	float 	xmax, xmin, ymax, ymin;
	
	static bool draw_geo;

	/** DEBUG: Draws the Box */
	void DrawGeometry();
	
	void FindExtents();
	
private:

	typedef std::vector<  std::vector<Vector>  >::iterator cluster_iter;
	typedef std::vector<Vector>::iterator vertex_list_iter;
	
	static SquareLookupTable<bool, COLLISION_OBJ_NUM_TYPES> collisionLUT;
	
	static std::list<CollisionObject*>  passive_registry;

	static std::list<CollisionObject*>  active_registry;

	static bool Init();
	static bool class_was_inited;
	
	int space_node;

	static Quadtree* qt; // space partitioning


	};

// CollisionObject -------------------------------------/\---------------------









// CollisionObjectPlayer -------------------------------------\/---------------------

/** Basic Collisionable Player Class */
class CollisionObjectPlayer: public CollisionObject, public LifeObject, public PollingObject {
public:
	CollisionObjectPlayer( 		
		float x, 
		float y, 		
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=1.0, 
		float friction=0.0 
		)
		:
		CollisionObject(x,y,fMass,Gravity,elasticity,friction),
		LifeObject(100)
		{ 
		obj_type = COLLISION_OBJ_PLAYER;
		SetCollisionActive(true);
		}

	virtual ~CollisionObjectPlayer() {};

	virtual void Intersect(CollisionObject* obj, Vector& collision_normal) { obj->Intersect(this,collision_normal); }

protected:
	};

// CollisionObjectPlayer -------------------------------------/\---------------------





// CollisionObject: Player Weapon -------------------------------------\/---------------------

/** Basic Collisionable Player's Weapon Class */
class CollisionObjectPWeapon: public CollisionObject {
public:
	CollisionObjectPWeapon(
		float x, 
		float y, 		
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=1.0, 
		float friction=0.2 
		)
		:CollisionObject(x,y,fMass,Gravity,elasticity,friction) {
		obj_type = COLLISION_OBJ_PLAYER_WEAPON;
		SetCollisionActive(true);
		}

	virtual ~CollisionObjectPWeapon() {};
	
	virtual void Intersect(CollisionObject* obj, Vector& collision_normal) { obj->Intersect(this,collision_normal); }

protected:
	};

// CollisionObjectPlayer -------------------------------------/\---------------------






// CollisionObjectEnemy -------------------------------------\/---------------------

/** Basic Collisionable Enemy Class */
class CollisionObjectEnemy: public CollisionObject, public LifeObject, public PollingObject {
public:

	CollisionObjectEnemy( 
		float x, 
		float y, 		
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=0.2, 
		float friction=0.2 
		)
		:
		CollisionObject(x,y,fMass,Gravity,elasticity,friction),
		LifeObject(100)
		{
		obj_type = COLLISION_OBJ_ENEMY;
		SetCollisionActive(true);
		}

	virtual ~CollisionObjectEnemy() {};
	
	virtual void Intersect(CollisionObject* obj, Vector& collision_normal) { obj->Intersect(this,collision_normal); }

	};

// CollisionObjectEnemy -------------------------------------/\---------------------






// CollisionObjectWorld -------------------------------------\/---------------------

/** Basic Collisionable Enemy Class */
class CollisionObjectWorld: public CollisionObject {
public:

	CollisionObjectWorld( 
		float x, 
		float y, 		
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=0.0, 
		float friction=0.2 
		)
		: CollisionObject(x,y,fMass,Gravity,elasticity,friction)  {
			obj_type = COLLISION_OBJ_WORLD;
		}

	virtual ~CollisionObjectWorld() {};
	
	virtual void Intersect(CollisionObject* obj, Vector& collision_normal) { obj->Intersect(this,collision_normal); }

	enum SnapDirection {
		SNAP_DOWN,
		SNAP_UP,
		SNAP_LEFT,
		SNAP_RIGHT
		};
		
	void SnapTo( const Vector& point, const SnapDirection dir, Vector& intersect );
	
	};

// CollisionObjectWorldBlock -------------------------------------/\---------------------






// CollisionObjectUnowned -------------------------------------\/---------------------

/** Basic Collisionable Enemy Class */
class CollisionObjectUnowned: public CollisionObject {
public:

	CollisionObjectUnowned( 
		float x, 
		float y, 		
		float fMass=0.0, 
		float Gravity = 300.0,
		float elasticity=0.2, 
		float friction=0.2 
		)
		:CollisionObject(x,y,fMass,Gravity,elasticity,friction) {
		obj_type = COLLISION_OBJ_UNOWNED;
		SetCollisionActive(true);
		}

	virtual ~CollisionObjectUnowned() {};
	
	virtual void Intersect(CollisionObject* obj, Vector& collision_normal) { obj->Intersect(this,collision_normal); }

	};

// CollisionObjectUnowned -------------------------------------/\---------------------



	
} // end namespace LGC	





#endif
