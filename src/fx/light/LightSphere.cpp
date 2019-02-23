#include "LightSphere.h"

#include "../../utilities/LogManager.h"
#include "../../core/ResourceMgr.h"





namespace LGC {



Texture LightSphere::img = Texture();
bool LightSphere::img_loaded = false;


// KILLME 
inline float DegToRad(float angle) {
	return angle * (3.1415926f / 180.0f);
	}


void LightSphere::ReserveResources() {
	img = ResourceMgr::Inst()->GetTexture( "light_sphere", "LightSphere" );
	if (img.IsValid()) { 
		img_loaded = true; 
		}
	}

void LightSphere::DumpResources() {
	ResourceMgr::Inst()->DumpTexture( img, "LightSphere" );
	img_loaded = false;
	}



LightSphere::LightSphere(
	float x,
	float y,
	float r,
	float angle,
	float speed,
	float red,
	float green,
	float blue,
	float alpha
	) :PhysicalObject(x,y)  {

	rad = r;
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;
	xoff = 0;
	yoff = 0;
	tracking = false;
	track_obj = NULL;

	// FIXME - GO THROUGH MANAGER
	if (!img_loaded) {
		img = ResourceMgr::Inst()->GetTexture("light_sphere", "LightSphere" );
		img_loaded = true;
		}
		
	AddImpulse(  Vector(  cos( DegToRad(angle) ) * speed, sin( DegToRad(angle) ) * speed  )  );

	}


LightSphere::LightSphere(
	PhysicalObject* obj,
	float rad,
	float xoff,
	float yoff,
	float red,
	float green,
	float blue,
	float alpha
	) :PhysicalObject(0,0)  {

	tracking = true;
	track_obj = obj;
	this->xoff = xoff;
	this->yoff = yoff;
	this->rad = rad;
	this->red = red;
	this->green = green;
	this->blue = blue;
	this->alpha = alpha;


	// check for tracking object:
	if (!track_obj) {
		LogMgr::Write(LOGMSG_ERROR, "LightSphere::LightSphere() No tracking Object");
		StopPolling();
		return;
		}
	else {
		MoveTo( track_obj->Xpos() + xoff, track_obj->Ypos() + yoff );
		}

// 	if (!img_loaded) {
// 		img = ResourceMgr::Inst()->GetTexture("light_sphere", "LightSphere" );
// 		img_loaded = true;
// 		}

	}





bool LightSphere::Poll() {
	if (!tracking) {
		UpdatePosition( TimeFromLast() );
		}

	else {
		if (!track_obj) {LogMgr::Write(LOGMSG_ERROR, "LightSphere::Poll() no tracking object!");}
		MoveTo( track_obj->Xpos() + xoff, track_obj->Ypos() + yoff );
		}

	return true;
	}

	
	
void LightSphere::Draw() {
	glColor4f(red, green, blue, alpha);
	img.BlitRotoZoomFromCenter( Xpos(), Ypos(), 0, (rad*2 / img.Width()), BLEND_ADD );
	}
	
	
	
REGISTER_CLASS_AS_RESOURCE_CLIENT(LightSphere)



} // end namespace LGC	








