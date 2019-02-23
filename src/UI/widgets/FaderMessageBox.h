#ifndef FADER_MESSAGE_BOX_H
#define FADER_MESSAGE_BOX_H

//#include <GL/gl.h>
#include <vector>
#include <string>

#include "BasicWidget.h"
#include "../../core/Fonts.h"
#include "../../etc/Interpolator.h"

namespace LGC {


// Fades the previous screen out to black and the new screen in 
class FaderMessageBox: public BasicWidget {
public:
     FaderMessageBox( float x, float y, float w, float h, BasicWidget* parent, const std::string messages_file );
	~FaderMessageBox();
	bool Poll();	
	void Draw();	
private:
	std::vector<std::string> messages;
	Interpolator display_curve;
	//Font font;
	float time_this_display;
	int message_index;
	};
	
	
} // end namespace LGC

	
#endif





