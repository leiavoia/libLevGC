#ifndef LIGHT_SPHERE_H
#define LIGHT_SPHERE_H

#include "../../objects/PollingObject.h"
#include "../../objects/PhysicalObject.h"
#include "../../core/Texture.h"

#include <GL/gl.h>
#include <SDL/SDL.h>




namespace LGC {




class LightSphere: public PollingObject, public PhysicalObject {
public:

	LightSphere(
		float x,
		float y,
		float r,
		float angle,
		float speed,
		float red=1.0,
		float green=1.0,
		float blue=1.0,
		float alpha=0.3
		);

	LightSphere(
		PhysicalObject* obj,
		float rad,
		float xoff=0,
		float yoff=0,
		float red=1.0,
		float green=1.0,
		float blue=1.0,
		float alpha=0.3
		);

	~LightSphere() {}

	virtual bool Poll();
	
	virtual void Draw();

	static void ReserveResources();

	static void DumpResources();
	
private:

	static Texture img;
	static bool img_loaded;

	float red;
	float green;
	float blue;
	float alpha;

	float rad;

	// tracking
	bool tracking;
	PhysicalObject* track_obj;
	float xoff;
	float yoff;

	};




} // end namespace LGC	


#endif





