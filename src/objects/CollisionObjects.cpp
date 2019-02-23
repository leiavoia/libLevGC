#include "CollisionObjects.h"


#include <algorithm>
#include <iostream>
#include <math.h>
#include <GL/gl.h>

// killme 
#include "../fx/Particle.h"

#include "../core/extra_math.h"
#include "../utilities/LogManager.h"
#include "../algo/collision/PolygonTools.h"
#include "../algo/Quadtree.h"
#include <float.h>




namespace LGC {





std::list<CollisionObject*> CollisionObject::passive_registry;
std::list<CollisionObject*> CollisionObject::active_registry;
bool CollisionObject::draw_geo = false;
SquareLookupTable<bool, COLLISION_OBJ_NUM_TYPES> CollisionObject::collisionLUT;
Quadtree* CollisionObject::qt = new Quadtree(1000,1000); // default space 

static const float Glue = 0.5;




CollisionObject::CollisionObject(float x, float y, float fMass, float Gravity, float elasticity, float friction) 
	: 
	PhysicalObject(x,y,Gravity), 
	obj_type(COLLISION_OBJ_NULL),
	collision_active(false),	
	Mass(fMass), 
	Friction(friction), 
	Elasticity(elasticity),
	space_node(0)
	{
	
	InvMass = (fMass > 0.0000001f)? 1.0f / fMass : 0.0f;
	
	vertices.clear();
	FindExtents();
	passive_registry.push_back( this );
	}
	
	
	
	
	
CollisionObject::CollisionObject (
	const vertex_list& incoming_vertices, 
	const Vector& xpos, 
	float fMass, 
	float Gravity,
	float elasticity, 
	float friction
	) 
	: 
	PhysicalObject( xpos.x, xpos.y, Gravity ), 
	obj_type(COLLISION_OBJ_NULL),
	collision_active(false),	
	Mass(fMass), 
	Friction(friction), 
	Elasticity(elasticity),
	space_node(0)
	{
	
	vertices.push_back( incoming_vertices );
	InvMass = (fMass > 0.0000001f)? 1.0f / fMass : 0.0f;
	
	FindExtents();
	passive_registry.push_back( this );
	}
	

CollisionObject::CollisionObject(
	const Vector& xpos, 
	float width, 
	float height, 
	float fMass, 
	float Gravity,
	float elasticity,
	float friction
	) 
	: 
	PhysicalObject( xpos.x, xpos.y, Gravity ), 
	obj_type(COLLISION_OBJ_NULL),
	collision_active(false),	
	Mass(fMass), 
	Friction(friction), 
	Elasticity(elasticity),
	space_node(0)
	{
	
	vertices.push_back( vertex_list() );
	PolygonTools::BuildBox(vertices[0], width, height);
	InvMass = (fMass > 0.0000001f)? 1.0f / fMass : 0.0f;
	
	FindExtents();
	passive_registry.push_back( this );
	}

	


CollisionObject::~CollisionObject() {
	if ( collision_active ) {
		active_registry.erase(   find( active_registry.begin(), active_registry.end(), this )   );
		}
	else {
		passive_registry.erase(   find( passive_registry.begin(), passive_registry.end(), this )   );
		}

	// remove from quadtree space:
	qt->RemoveObject(this);
	}


	
void CollisionObject::DefineCollisionRect( float x, float y, float w, float h ) {
	vertices.clear();
	vertices.push_back( vertex_list() );
	PolygonTools::BuildBox(vertices[0], w, h);
	for (unsigned int i = 0; i < vertices[0].size(); i++) {
		vertices[0][i] += Vector(x,y);
		}
	FindExtents();
	qt->UpdateObject(this);
	}
	
	
	
void CollisionObject::DefineCollisionCircle( float x, float y, int radius, int points ) {
	vertices.clear();
	vertices.push_back( vertex_list() );
	PolygonTools::BuildBlob(vertices[0], points, radius);
	for (unsigned int i = 0; i < vertices[0].size(); i++) {
		vertices[0][i] += Vector(x,y);
		}
	FindExtents();
 	qt->UpdateObject(this);
	}
	
	
	
	
	
	
	
	
void CollisionObject::SetCollisionActive( bool x ) {

	bool was_active = collision_active;

	collision_active = x;

	if (  collision_active && !was_active  ) { 
		active_registry.push_back( this );
		passive_registry.erase(   find( passive_registry.begin(), passive_registry.end(), this )   );
 		qt->AddObject(this);
		}

	else if ( !collision_active ) {
		passive_registry.push_back( this );
		active_registry.erase(   find( active_registry.begin(), active_registry.end(), this )   );
 		qt->RemoveObject(this);
		}

	}

 


void CollisionObject::AddPolygon( const vertex_list v ) {
	// TODO: add convex polygon checking code here
	vertices.push_back( v );
	FindExtents();
 	qt->UpdateObject(this);
	}
	 
	
	
void CollisionObject::DrawGeometry() {	
	for (cluster_iter my_poly = vertices.begin(); my_poly != vertices.end(); my_poly++) {
		PolygonTools::Render( pos, *my_poly, false );
		}
		
	glBindTexture( GL_TEXTURE_2D, 0 );
	glColor4f( 0.2, 0.4, 1.0, 1 ); // ORANGE

	// draw box
	glBegin(GL_LINE_STRIP);
		/* Bottom Left Of The Texture and Quad */
		glVertex2f( XMin(), YMax() );
		/* Bottom Right Of The Texture and Quad */
		glVertex2f( XMax(), YMax() );
		/* Top Right Of The Texture and Quad */
		glVertex2f( XMax(), YMin() );
		/* Top Left Of The Texture and Quad */
		glVertex2f( XMin(), YMin() );
		/* Bottom Left Of The Texture and Quad - AGAIN - close box */
		glVertex2f( XMin(), YMax() );
	glEnd();	
	}

	
// moves if the object is moving
void CollisionObject::UpdatePosition( float deltatime ) {
	PhysicalObject::UpdatePosition(deltatime);
 	qt->UpdateObject( this );
	}
	
	
bool CollisionObject::Collide(CollisionObject& xBody, float dt, Vector& N ) {
	if (  IsUnmovable() && xBody.IsUnmovable()  )  { return false; }
	if (  vertices.empty()  ||  xBody.vertices.empty()  ) { return false; }
	
	//Vector xMTD;
	float  t = 1.0f;
	//Vector N;
	
	Vector xRelPos = pos - xBody.pos;
	Vector xRelDis = (movement - xBody.movement) * dt;
	
	bool found_collision = false;
	
	bool adjust = WantsAdjustment() && xBody.WantsAdjustment();
	
	for (cluster_iter my_poly = vertices.begin(); my_poly != vertices.end(); my_poly++) {
		for (cluster_iter his_poly = xBody.vertices.begin(); his_poly != xBody.vertices.end(); his_poly++) {
			
			if ( PolygonTools::Collide(*my_poly, *his_poly, xRelPos, xRelDis, N, t, adjust) ) {
				// slide prevention:
// 				Vector move_norm = movement;
// 				move_norm.Normalise();
// 				if ( fabs(N * move_norm) < 0.5 ) { 
// 					LogMgr::Write(LOGMSG_DEBUG, "contact: [%f]", N * move_norm);
// 					N = -movement + -xBody.movement; 
// 					N.Normalise();
// 					t = dt;					
// 					}
				if (adjust) {
					if (t < 0.0f) {
						ProcessOverlap(xBody, N * -t);
						}
					else {
						ProcessCollision(xBody, N);
						}
					}
				//return true;
				found_collision = true;
				}
		
			}
		}
	
	//return false;
	return found_collision;
	}

	
	
// two objects collided at time t. stop them at that time
void CollisionObject::ProcessCollision(CollisionObject& xBody, const Vector& N) {

     // difference in displacement
	Vector D = movement - xBody.movement;

	// dot product of collision normal and delta displacement
	float n  = D * N;
	
	Vector Dn = N * n;
	Vector Dt = D - Dn;
	
	if (n > 0.0f) { Dn = Vector(0, 0); }

	//float dt  = Dt * Dt;
	//float CoF = (Friction + xBody.Friction) / 2.0 ;
	
	//if (dt < Glue*Glue) CoF = 1.n1f;
 
	// calculate total bounciness
	// - assume that unmovable objects have a bounce of ZERO
	float mod = ( IsUnmovable() || xBody.IsUnmovable() ) ? 1.0 : 2.0;
	D = -(  1.0f + ((Elasticity + xBody.Elasticity) / mod)  ) * Dn /* - (CoF) * Dt*/;
	
	// ratio of my mass vs his mass
	float m0 = GetInvMass();
	float m1 = xBody.GetInvMass();
	float m  = m0 + m1;
	float r0 = m0 / m;
	float r1 = m1 / m;
	
	// this reverses the objects' trajectory due to impact/bounce/friction/glue
	movement += D * r0;
	xBody.movement -= D * r1;



	}

	
	
	
// two objects overlapped. push them away from each other
void CollisionObject::ProcessOverlap(CollisionObject& xBody, const Vector& xMTD) {
	if (IsUnmovable()) {
		xBody.pos -= xMTD;
		}
	else if (xBody.IsUnmovable()) {
		pos += xMTD;
		}
	else {
		pos += xMTD * 0.5f;
		xBody.pos -= xMTD * 0.5f;
		}

	Vector N = xMTD;
	N.Normalise();
	ProcessCollision(xBody, N);
	}
	
	
	
void CollisionObject::FindExtents() {

	xmax = 0;
	xmin = FLT_MAX;
	ymax = 0;
	ymin = FLT_MAX;
	
	if ( vertices.empty() ) {  return;  }
	
	for (cluster_iter ci = vertices.begin(); ci != vertices.end(); ci++) {
		for (vertex_list_iter vi = ci->begin(); vi != ci->end(); vi++) {
			if ( vi->x < xmin ) { xmin = vi->x; }
			if ( vi->x > xmax ) { xmax = vi->x; }
			if ( vi->y < ymin ) { ymin = vi->y; }
			if ( vi->y > ymax ) { ymax = vi->y; }
			}
		}
		
	//LogMgr::Write(LOGMSG_DEBUG, "EXTENTS: %f, %f, %f, %f", xmin, xmax, ymin, ymax);

	}
	

void CollisionObject::Cycle( float deltatime ) {

	// use this for all collisions
	static Vector collision_normal;
	static std::vector<CollisionObject*> v;
	
	// foreach item in the active list:
	for (std::list<CollisionObject*>::iterator A = active_registry.begin(); A != active_registry.end(); A++) {

		// check against objects in the local area:
		v.clear();
 		qt->GetObjectsIn( v, *A );
		
		for (unsigned int i=0; i < v.size(); i++) {
		//for (std::vector<CollisionObject*>::iterator B = v.begin(); B != v.end(); B++) {
			
			CollisionObject* objA = *A;
			CollisionObject* objB = v[i];
			
			// non-permutation check and "i'm not me" check
			if ( objB->IsCollisionActive() && objB <= objA ) { continue; } 
				 
			// Order the objects for table lookup
			// NOTE: Keep this here because weapons always need to hit first
			if (objA->GetObjType() > objB->GetObjType()) {
				CollisionObject* temp = objB;
				objB = objA;
				objA = temp;
				}
			
			// check to see if this pairing is "legal"
			if ( collisionLUT( objA->GetObjType(), objB->GetObjType() )  ) {
				// Check to see if there is an actual collision
				if (  objA->TestBounds(objB)  &&  objA->Collide( *objB, deltatime, collision_normal )  ) {
					// perform collision response
					objB->Intersect(objA, collision_normal);
					objA->Intersect(objB, collision_normal);
					}							 
				}				
			} // end of foreach(object in node)
		} // end of loop
	} // end of function
	
	 
	 
	
void CollisionObject::MoveTo( float x, float y ) {
 	// move coords
 	PhysicalObject::MoveTo(x,y);
	// recalc position in the quadtree space
	if ( !vertices.empty() ) {
		qt->UpdateObject( this );
		}
	}





bool CollisionObject::Init() {
	// flags for managing reactions of basic object types
	collisionLUT.SetElement(COLLISION_OBJ_ENEMY, COLLISION_OBJ_ENEMY, true);
	collisionLUT.SetElement(COLLISION_OBJ_PLAYER, COLLISION_OBJ_ENEMY, true);
	//collisionLUT.SetElement(COLLISION_OBJ_PLAYER, COLLISION_OBJ_PLAYER, true);
	collisionLUT.SetElement(COLLISION_OBJ_ENEMY, COLLISION_OBJ_WORLD, true);
	collisionLUT.SetElement(COLLISION_OBJ_PLAYER, COLLISION_OBJ_WORLD, true);
	collisionLUT.SetElement(COLLISION_OBJ_UNOWNED, COLLISION_OBJ_WORLD, true);
	collisionLUT.SetElement(COLLISION_OBJ_UNOWNED, COLLISION_OBJ_PLAYER, true);
	collisionLUT.SetElement(COLLISION_OBJ_PLAYER_WEAPON, COLLISION_OBJ_ENEMY, true);
	//collisionLUT.SetElement(COLLISION_OBJ_PLAYER, COLLISION_OBJ_PLAYER_WEAPON, true);
	collisionLUT.SetElement(COLLISION_OBJ_PLAYER_WEAPON, COLLISION_OBJ_WORLD, true);
	return true;
	}



	
void CollisionObject::ResetQuadtree( int x, int y, int div ) {
	// new tree
	Quadtree* n = new Quadtree(x,y,div);
	// transfer all objects from old tree
	qt->TransferObjectsTo( n );
	// detroy old tree and replace
	delete qt;
	qt = n;
	}




// This is a cheap C++ trick to get the class basic data initialized at startup
bool CollisionObject::class_was_inited = Init();




/////////////////////////////////////////////////////////////////////


void CollisionObjectWorld::SnapTo( 
	const Vector& point, 
	const SnapDirection dir, 
	Vector& intersect ) 
	{

	// NOTE: we assume that there is only one polygon in the polygon list
	
	// error: no geometry
	if ( vertices.size() < 1 ) { return; }
	
// 	// first check to see if we can snap to it at all ( gone too far )
// 	switch (dir) {
// 		case SNAP_DOWN:  if (point.y < YMinRaw() ) { return; } break;
// 		case SNAP_UP:    if (point.y > YMaxRaw() ) { return; } break;
// 		case SNAP_LEFT:  if (point.x < XMinRaw() ) { return; } break;
// 		case SNAP_RIGHT: if (point.x > XMaxRaw() ) { return; } break;
// 		};	
		
	// plumb a line down 25 pixels
	Vector plumbline;
	switch (dir) {
		case SNAP_DOWN:  plumbline = Vector( point.x, point.y + 10 ); break;
		case SNAP_UP:    plumbline = Vector( point.x, point.y - 10 ); break;
		case SNAP_LEFT:  plumbline = Vector( point.x - 10, point.y ); break;
		case SNAP_RIGHT: plumbline = Vector( point.x + 10, point.y ); break;
		};
	
		
	// check each segment for an intersection:
	intersect = Vector(0,0);
	for (unsigned int i = 0; i < vertices[0].size(); i++) {	
		//             (Ay-Cy)(Dx-Cx)-(Ax-Cx)(Dy-Cy)
		//         r = -----------------------------  (eqn 1)
		//             (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)
		//
		//             (Ay-Cy)(Bx-Ax)-(Ax-Cx)(By-Ay)
		//         s = -----------------------------  (eqn 2)
		//             (Bx-Ax)(Dy-Cy)-(By-Ay)(Dx-Cx)

		unsigned int to = ( i+1 >= vertices[0].size() ) ? 0 : i+1 ;

		Vector A = point;
		Vector B = point + plumbline;
		Vector C = vertices[0][i] + Pos();
		Vector D = vertices[0][to] + Pos();
		
		float denom = ((D.y-C.y)*(B.x-A.x)) - ((D.x-C.x)*(B.y-A.y));
		// check for coincidence and parallel - For this, we do not care about either,
		// or any distinction between the two. But for reference:
		// 	- If the denominator in eqn 1 is zero, AB & CD are parallel
		// 	- If the numerator in eqn 1 is also zero, AB & CD are coincident		
		if ( denom == 0 ) { 
//			LogMgr::Write(LOGMSG_DEBUG, "     denom failed on %d->%d", i, to);
			continue;  
			} 		

		float s = (  (A.y-C.y)*(B.x-A.x) - (A.x-C.x)*(B.y-A.y)  ) / denom;
		if (s < 0.0f || s > 1.0f) { 
//			LogMgr::Write(LOGMSG_DEBUG, "     S failed on %d->%d [%f]", i, to, s);
			continue; 
			} 
		
		float r = (  (A.y-C.y)*(D.x-C.x) - (A.x-C.x)*(D.y-C.y)  ) / denom;
		if (r < 0.0f || r > 1.0f) { 
//			LogMgr::Write(LOGMSG_DEBUG, "     R failed on %d->%d [%f]", i, to, r);
			continue; 
			} 

		
		// intersection point = A+r(B-A)
		if ( intersect.IsBlank() ) { 
			intersect = A + ( r * (B-A) ); 
//			LogMgr::Write(LOGMSG_DEBUG, "Line Snapped. R=%f, S=%f", r, s); 
			}
		else {
			Vector P = A + ( r * (B-A) );
			Vector temp2 = P - A;
			Vector temp1 = intersect - A;
			if ( temp2.Length() < temp1.Length()  ) { 
				intersect = P;
//				LogMgr::Write(LOGMSG_DEBUG, "Line Snapped (again)");
				return;
				}
			}
			
		
		
		}	
		
	}


	
} // end namespace LGC	







