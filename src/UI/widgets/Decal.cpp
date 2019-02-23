#include "Decal.h"
#include "../../core/ResourceMgr.h"

namespace LGC {

Decal::Decal( float x, float y, float w, float h, BasicWidget* parent, std::string image ) : BasicWidget(x,y,w,h,parent), stretch(true)
	{ 
	img = RM->GetTexture(image, "Decal");
	}

Decal::Decal( float x, float y, BasicWidget* parent, std::string image ) : BasicWidget(x,y,1,1,parent), stretch(false)
	{ 
	img = RM->GetTexture(image, "Decal");
	SetHeight( img.Height() );
	SetWidth( img.Width() );
	}
	
Decal::~Decal() {
	RM->DumpTexture(img, "Decal");
	}

void Decal::Draw() {
	StartClipping();
	if (stretch) {
		img.BlitCustomStretch( Xpos(), Ypos(), Width(), Height() );
		}
	else {
		img.Blit( Xpos(), Ypos() );
		}
	StopClipping();
	}

bool Decal::Poll() { 
	DoFading();
	return true; 
	}
	
} // end namespace LGC	
	
