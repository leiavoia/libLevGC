#include "LightFlare.h"
#include "../../etc/Interpolator.h"
#include "../../core/ResourceMgr.h"




namespace LGC {



ColorCurve LightFlare::burn_color = ColorCurve(Interpolator::NO_REPEAT);
ColorCurve LightFlare::default_color = ColorCurve(Interpolator::NO_REPEAT);
ColorCurve LightFlare::blinder_color = ColorCurve(Interpolator::NO_REPEAT);

Texture LightFlare::light_sphere = Texture();
Texture LightFlare::light_sphere_pointed = Texture();
bool LightFlare::img_loaded = false;


// FIXME THIS IS ELSEWHERE
inline float DegToRad(float angle) {
	return angle * (3.1415926f / 180.0f);
	}




LightFlare::LightFlare(
	float x,
	float y,
	float start_rad,
	float mid_rad,
	float end_rad,
	float mid_time,
	float end_time,
	ColorCurve& color,
	Texture custom_texture
	)  :PhysicalObject(x,y)  {

	this->start_rad = start_rad;
	this->mid_rad = mid_rad;
	this->end_rad = end_rad;
	this->mid_time = mid_time;
	this->end_time = end_time;
	my_color = color;

	// divide by zero:
	if (mid_time == 0) { mid_time = 1; }

	// check endtime;
	if (end_time < mid_time) { end_time = mid_time; }

	if ( custom_texture.IsValid() ) { texture = custom_texture; } //custom
	else { texture = light_sphere; } // default
	}


LightFlare::LightFlare( float x, float y, flash_type type ) :PhysicalObject(x,y) {

	switch (type) {
		case FLASH_BURN: // laser burn mark
			start_rad = 1;
			mid_rad = 40;
			end_rad = 10;
			mid_time = 20.0 / 1000.0;
			end_time = 850.0 / 1000.0;
			my_color = burn_color;
			texture = light_sphere_pointed;
			break;
		case FLASH_MUZZLE: // mussle flash
			start_rad = 5;
			mid_rad = 55;
			end_rad = 5;
			mid_time = 10.0 / 1000.0;
			end_time = 100.0 / 1000.0;
			my_color = default_color; // was 0.8
			texture = light_sphere_pointed;
			break;
		case FLASH_BLINDER: // blinder
			start_rad = 10;
			mid_rad = 1000;
			end_rad = 10;
			mid_time = 100.0 / 1000.0;
			end_time = 800.0 / 1000.0;
			my_color = blinder_color; // was 0.7
			texture = light_sphere;
			break;
		case FLASH_ULTRABLINDER: // blinder
			start_rad = 10;
			mid_rad = 2000;
			end_rad = 10;
			mid_time = 100.0 / 1000.0;
			end_time = 2000.0 / 1000.0;
			my_color = blinder_color; // was 0.9
			texture = light_sphere;
			break;
		case FLASH_SMALLBURST: // smaller version of the default burst
			start_rad = 15;
			mid_rad = 150;
			end_rad = 15;
			mid_time = 10.0 / 1000.0;
			end_time = 95.0 / 1000.0;
			my_color = default_color; // was 0.25
			texture = light_sphere_pointed;
		default: // semi-fullscreen fast burst
			start_rad = 50;
			mid_rad = 600;
			end_rad = 50;
			mid_time = 20.0 / 1000.0;
			end_time = 200.0 / 1000.0;
			my_color = default_color;
			texture = light_sphere_pointed;
		}
	}




bool LightFlare::Poll( /*int time*/ ) {
	// are we dead?
	if (TimeFromBirth() >= end_time) { return false; }

	return true;
	}



void LightFlare::Draw() {

	if ( !texture.IsValid() ) { return; }

	// create the radius based on the time
	float rad = 0;

	// random rotation
	// NOTE: DOESN'T SEEM TO WORK ???
	float rot = float( int(BirthTick()) % 360 );

	// if we're growing (PHASE 1)
	if ( TimeFromBirth() <= mid_time ) {
		rad = mid_rad * ( (float)TimeFromBirth() / (float)mid_time );
		}
	// if we're shrinking (PHASE 2)
	else {
		rad = end_rad + (mid_rad - end_rad) * ( 1.0 - ( float(TimeFromBirth() - mid_time) / float(end_time - mid_time)  )  )   ;
		}

	float point = (float)TimeFromBirth() / (float)end_time;
	glColor4f( my_color.R(point), my_color.G(point), my_color.B(point), my_color.A(point));

	float scale = rad*2 / texture.Width() ;
	texture.BlitRotoZoomFromCenter( Xpos(), Ypos(), rot, scale, BLEND_ADD );

	}





void LightFlare::ReserveResources() {
	light_sphere = ResourceMgr::Inst()->GetTexture( "light_sphere", "LightFlare" );
	light_sphere_pointed = ResourceMgr::Inst()->GetTexture( "light_sphere_pointed", "LightFlare" );
	if (light_sphere.IsValid() && light_sphere_pointed.IsValid()) {
		img_loaded = true;
		}

	// while we're reserving resources, set some color curves too.
	burn_color.AddPoint( 0.0, 1.0, 1.0, 1.0, 0.9); // white
	burn_color.AddPoint( 0.2, 0.88, 0.9, 0.6, 0.9); // white/yellow
	burn_color.AddPoint( 0.8, 0.75, 0.2, 0.1, 0.9); // dark red
	burn_color.AddPoint( 1.0, 0.75, 0.2, 0.1, 0.0); // dark red fadeout

	default_color.AddPoint( 0.0, 1.0, 1.0, 1.0, 0.7);
	default_color.AddPoint( 1.0, 1.0, 1.0, 1.0, 0.3);

	blinder_color.AddPoint( 0.0, 1.0, 1.0, 1.0, 0.9);
	blinder_color.AddPoint( 0.7, 1.0, 1.0, 1.0, 0.7);
	blinder_color.AddPoint( 1.0, 1.0, 1.0, 0.8, 0.1);
	}

void LightFlare::DumpResources() {
	ResourceMgr::Inst()->DumpTexture( light_sphere, "LightFlare" );
	ResourceMgr::Inst()->DumpTexture( light_sphere_pointed, "LightFlare" );
	img_loaded = false;
	}


REGISTER_CLASS_AS_RESOURCE_CLIENT( LightFlare )



} // end namespace LGC



