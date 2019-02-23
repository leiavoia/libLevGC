#ifndef LIGHT_FLARE_H
#define LIGHT_FLARE_H

#include "../../objects/PollingObject.h"
#include "../../objects/PhysicalObject.h"
#include "../../core/Texture.h"
#include "../../etc/ColorCurve.h"

#include <GL/gl.h>
#include <SDL/SDL.h>






namespace LGC {



class LightFlare: public PollingObject, public PhysicalObject {
public:

	enum flash_type {
		FLASH_BURST,
		FLASH_SMALLBURST,
		FLASH_MUZZLE,
		FLASH_BURN,
		FLASH_BLINDER,
		FLASH_ULTRABLINDER
		};

	// quicky constructor: see the "flash_type" enum for possible values
	LightFlare( float x, float y, flash_type type );

	// insane custom constructor (for all the crazy people out there)
	LightFlare(
		float x,
		float y,
		float start_rad,
		float mid_rad,
		float end_rad,
		float mid_time,
		float end_time,
		ColorCurve& color,
		Texture custom_texture
		);


	~LightFlare() {}

	virtual bool Poll();

	virtual void Draw();

	static void ReserveResources();

	static void DumpResources();



private:

	Texture texture;

	static Texture light_sphere;
	static Texture light_sphere_pointed;
	static bool img_loaded;

	float red;
	float green;
	float blue;
	float alpha;

	float start_rad;
	float mid_rad;
	float end_rad;

	// REPLACED WITH BirthTick() // int start_time; // exact starting time - created in constructor
	float mid_time; // ms from start time for midpoint (Start shrinking)
	float end_time; // ms from start time to die.

	ColorCurve my_color;

	static ColorCurve burn_color;
	static ColorCurve default_color;
	static ColorCurve blinder_color;

	};




} // end namespace LGC


#endif





