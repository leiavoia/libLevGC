#include "FaderMessageBox.h"


#include "../../utilities/LogManager.h"
#include "../../core/ResourceMgr.h"
 
#include <iostream>
#include <fstream>
#include <stdlib.h>


namespace LGC {


const static float FADE_TIME = 4.0;

FaderMessageBox::FaderMessageBox( 
	float x, 
	float y, 
	float w, 
	float h, 
	BasicWidget* parent, 
	const std::string messages_file 
	) 
	: BasicWidget(x,y,w,h,parent),
	time_this_display(0),
	message_index(0)
	{
	
	display_curve.AddPoint(0,0);
	display_curve.AddPoint(FADE_TIME*0.15,1);
	display_curve.AddPoint(FADE_TIME*0.65,1);
	display_curve.AddPoint(FADE_TIME*0.8,0);
	display_curve.AddPoint(FADE_TIME,0);
	
	font = RM->GetFont("FaderMessageBox", "FaderMessageBox");
	
	std::ifstream file(messages_file.c_str());

	if (file.bad() || !file.is_open()) { 
		LogMgr::Write( LOGMSG_ERROR, "Could not open FaderMessageBox messages file '%s'", messages_file.c_str() ); 
		return;
		}
		
	std::string buffer;
	while (!file.eof()) {
		std::getline(file,buffer);
		if (!buffer.empty()) { messages.push_back(buffer); }
		}

	text = messages[0];

	}
	
FaderMessageBox::~FaderMessageBox() {
	RM->DumpFont(font, "FaderMessageBox");
	}

bool FaderMessageBox::Poll() {
	time_this_display += TimeFromLast();
	if (time_this_display >= FADE_TIME) { 
		time_this_display = 0; 
		if (++message_index >= (int)messages.size()) { 
			// spacer
			message_index = -1; 
			text = "";
			}
		else {
			text = messages[message_index];
			}
		}
	return true;
	}
	
void FaderMessageBox::Draw() {
	glPushMatrix();
	glLoadIdentity();
	DrawFoundation();
	DrawBorder();
	text_color.A = display_curve.Value( time_this_display );
	DrawTextCentered();	
	glPopMatrix();
	}



} // end namespace LGC

