#ifndef DECAL_H
#define DECAL_H

#include "BasicWidget.h"
#include "../../core/Texture.h"
#include <string>


namespace LGC {
	
class Decal: public BasicWidget {
public:

	// use this to "stretch" the image over the widget surface
	Decal( float x, float y, float w, float h, BasicWidget* parent, std::string image );
	// use this to auto-size the widget to the image
	Decal( float x, float y, BasicWidget* parent, std::string image );
	
	virtual ~Decal();
	
	virtual void Draw();
	
	virtual bool Poll();
	
protected:
	Texture img;
	bool stretch;
	};


	
} // end namespace LGC


#endif
