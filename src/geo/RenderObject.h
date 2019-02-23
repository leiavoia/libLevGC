#ifndef LGC_RENDERABLE_H
#define LGC_RENDERABLE_H


#include "Vector3.h"
#include <vector>

#include "../view/Material.h"


namespace LGC {

class RenderingLayer; // forward dec


class RenderObject {

public: // -------------\/---------------

	RenderObject() : hide(false), light(false), wireframe(false), shaded(true)  {
	
		}
		
	virtual ~RenderObject();
	
	Vector3 Position() { return pos; }
	float X() { return pos.x; }
	float Y() { return pos.y; }
	float Z() { return pos.z; }
	
	void SetPosition( float x, float y, float z ) {
		pos.x = x;
		pos.y = y;
		pos.z = z;
		}
		
	virtual void Render( RenderingLayer* x ) = 0;
	
	bool Occlude(); // ?
	
	void Hide( bool x ) { hide = x; };
	void Light( bool x ) { light = x; };
	void Wireframe( bool x ) { wireframe = x; };
	void Shaded( bool x ) { shaded = x; };
	
	bool Hidden() { return hide; }
	bool Light() { return light; }
	bool Wireframe() { return wireframe; }
	bool Shaded() { return shaded; }	
	
	void SetMaterial( Material* m ) { material = m; }
	
	
protected: // -------------\/---------------
	
	Vector3 pos;
	// TODO orientation data
	
	bool hide;
	bool light; // lit or unlit?
	bool wireframe; // false = POLYGON_FILL
	bool shaded; // false = FLAT;
	
	Material* material;
	
	// 1 or 2 sided?
	
	friend class RenderingLayer; // rendering layer needs full access to data
	
	};

} // end namespace LGC


#endif


