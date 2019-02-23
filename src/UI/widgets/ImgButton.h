#ifndef IMGBUTTON_H
#define IMGBUTTON_H

#include "Button.h"
#include "../../core/Texture.h"
#include <string>


namespace LGC {
	
class ImgButton: public Button {
public:

	ImgButton( 
		float x, 
		float y, 
		float w, 
		float h, 
		BasicWidget* parent, 
		std::string image, 
		std::string image_over="", 
		std::string image_down="" 
		);
	
	virtual ~ImgButton();
	
	virtual void Draw();
	
protected:

	Texture img;
	Texture img_over;
	Texture img_down;
	
	};


	
} // end namespace LGC


#endif


