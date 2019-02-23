#ifndef BACKGROUNDIMAGE_H
#define BACKGROUNDIMAGE_H

#include <SDL/SDL.h>

#include "../../objects/PollingObject.h"

#include <GL/gl.h>
#include "../../core/Texture.h"


namespace LGC {



enum bg_scrollmode {
	BG_SCROLLMODE_CENTER,
	BG_SCROLLMODE_WRAP_X,
	BG_SCROLLMODE_WRAP_Y,
	BG_SCROLLMODE_TILE,
	BG_SCROLLMODE_SINGLE,
	BG_SCROLLMODE_STRETCH
	};


//predec
class GameSpace;



class BackgroundImage: public PollingObject {
public:

	BackgroundImage( 
		const char* file,  
		GameSpace* parent_space,
		int screen_width,
		int screen_height,
		bg_scrollmode mode=BG_SCROLLMODE_TILE
		);

	virtual ~BackgroundImage();

	virtual bool Poll() {return true; }
	
	virtual void Draw();

	inline void SetScrollMode( bg_scrollmode mode ) { scrollmode = mode; }

	inline void SetParallax( float x=1.0 ) { parallax = x; }

protected:

	bg_scrollmode scrollmode;

	float parallax;
	int screen_w;
	int screen_h;

	Texture img;
	
	GameSpace* space;
	};




} // end namespace LGC	


#endif







