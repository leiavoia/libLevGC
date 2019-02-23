#ifndef BUTTON_H
#define BUTTON_H

#include <string>
#include "../sigslot.h"
#include "../../core/Fonts.h"
#include "BasicWidget.h"



namespace LGC {
	
using namespace sigslot;
	
class Button: public BasicWidget {
public:

	Button( float x, float y, float w, float h, BasicWidget* parent, std::string text = "");
	Button( float x, float y, BasicWidget* parent, std::string text); // auto resize
	
	virtual ~Button() {}
	
	virtual bool Poll();
	
	virtual void Draw();
	
	// SIGNALS
 	signal1<Button*> signal_button_down;
 	signal1<Button*> signal_button_up;
	
	// SLOTS (for core interface)
	virtual void HandleLeftClickUp( int x, int y );
	virtual void HandleLeftClickDown( int x, int y );
	virtual void HandleMouseMove( int x, int y );
	
  	static void ReserveResources();
	static void DumpResources();
	
	inline bool MouseIsDown() { return mousedown; }
	inline bool MouseIsOver() { return mouseover; }
	
	
protected:

	bool mouseover;
	bool mousedown;
	
	static Font default_button_font;
	};







	
} // end namespace LGC


#endif








