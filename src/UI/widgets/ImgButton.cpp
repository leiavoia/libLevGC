#include "ImgButton.h"
#include "../../core/ResourceMgr.h"

namespace LGC {

ImgButton::ImgButton( 
	float x, 
	float y, 
	float w, 
	float h, 
	BasicWidget* parent, 
	std::string image, 
	std::string image_over, 
	std::string image_down 
	) :  Button(x,y,w,h,parent) 
	{ 
	img = RM->GetTexture(image, "ImgButton");
	if (!image_over.empty()) { img_over = RM->GetTexture(image_over, "ImgButton"); }
	if (!image_down.empty()) { img_down = RM->GetTexture(image_down, "ImgButton"); }
	}

ImgButton::~ImgButton() {
	RM->DumpTexture(img, "ImgButton");
	if (img_over.IsValid()) { RM->DumpTexture(img_over, "ImgButton"); }
	if (img_down.IsValid()) { RM->DumpTexture(img_down, "ImgButton"); }
	}

void ImgButton::Draw() {
	// background color
	if (mouseover && !mousedown) {  SetBGColor( ColorQuad(1,1,1,0.4) );  }
	else if (mousedown) {  SetBGColor( ColorQuad(1,1,1,0.75) );  }
	else {  SetBGColor( ColorQuad(1,1,1,0.2) );  }
	
	StartClipping();
	DrawFoundation();
	DrawBorder();
	
	// draw image
	if (mouseover && !mousedown) {  
		if (img_over.IsValid()) { img_over.BlitFromCenter( Xpos() + Width()*0.5, Ypos() + Height()*0.5 ); }  
		else { img.BlitFromCenter( Xpos() + Width()*0.5, Ypos() + Height()*0.5 ); }
		}
	else if (mousedown) {  
		if (img_down.IsValid()) { img_down.BlitFromCenter( Xpos() + Width()*0.5, Ypos() + Height()*0.5 ); }  
		else { img.BlitFromCenter( Xpos() + Width()*0.5, Ypos() + Height()*0.5 ); } 
		}
	else { 
		img.BlitFromCenter( Xpos() + Width()*0.5, Ypos() + Height()*0.5 );  
		}
	
	StopClipping();
	}

} // end namespace LGC	
	
