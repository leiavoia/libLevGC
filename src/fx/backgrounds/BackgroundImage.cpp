#include "BackgroundImage.h"
#include "../../core/GameSpace.h"
#include "../../core/ResourceMgr.h"

#include <SDL/SDL_image.h>

namespace LGC {




BackgroundImage::BackgroundImage(
	const char* file,
	GameSpace* parent_space,
	int screen_width,
	int screen_height,
	bg_scrollmode mode
	)

	:

	scrollmode(mode),
	parallax(0),
	screen_w(screen_width),
	screen_h(screen_height),
	space(parent_space)

	{

	img = RM->GetTexture(file, "BackgroundImage" );

	}

BackgroundImage::~BackgroundImage() {
	RM->DumpTexture(img, "BackgroundImage" );
	}

void BackgroundImage::Draw( ) {
	if ( !img.IsValid() ) { return; }

	switch ( scrollmode ) {

		case BG_SCROLLMODE_TILE: {
			if (!space) { return; }
			glColor4f(1,1,1,1);
			// find the starting point on the texture to start "tiling" from the topleft of the screen
			float tex_start_x = float(  ( int((float)space->Xoff() * parallax) % img.Width() )   )   /  (float)img.Width();
			float tex_start_y = float(  ( int((float)space->Yoff() * parallax) % img.Height() )   )  /  (float)img.Height();
			img.TileAcrossWithOffset(
				space->Xoff(),
				space->Yoff(),
				screen_w,
				screen_h,
				tex_start_x,
				tex_start_y//,
				//BLEND_NONE
				);
			break;
			}
		case BG_SCROLLMODE_SINGLE: {
			// just paste that sucker down:
			glColor4f(1,1,1,1);
			img.Blit(0,0);
			break;
			}
		case BG_SCROLLMODE_CENTER: {
			// just paste that sucker down:
			glColor4f(1,1,1,1);
			img.BlitFromCenter(screen_w * 0.5f, screen_h * 0.5f);
			break;
			}
		case BG_SCROLLMODE_STRETCH: {
			glColor4f(1,1,1,1);
// 			float ratio_h = (float)screen_w / (float)img.Width();
// 			float ratio_v = (float)screen_h / (float)img.Height();
			img.BlitCustomStretch(0,0,screen_w, screen_h);
			break;
			}
		default: ;;

		}
	}


} // end namespace LGC



