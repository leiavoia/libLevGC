#ifndef TEXTBOX_H
#define TEXTBOX_H

#include <string>
#include "../sigslot.h"
#include "../../core/Fonts.h"
#include "BasicWidget.h"
#include "../../etc/Interpolator.h"


namespace LGC {

using namespace sigslot;

class TextBox: public BasicWidget {
public:

	TextBox( float x, float y, float w, float h, BasicWidget* parent, std::string text = "");

	virtual ~TextBox() {}

	virtual bool Poll();

	virtual void Draw();

	void SetBGColor( const ColorQuad& color );

 	void Focus();
 	void UnFocus();
	inline bool Focused() { return focused; }


	// SIGNALS
 	signal1<TextBox*> signal_enter;
 	signal1<TextBox*> signal_change;

	// SLOTS (for core interface)
	virtual void HandleLeftClickUp( int x, int y );
	virtual void HandleLeftClickDown( int x, int y );
	virtual void HandleMouseMove( int x, int y );
	virtual void HandleTypeChar( char c );
	virtual void HandleCommand( std::string& s );

  	static void ReserveResources();
	static void DumpResources();

protected:

	void MoveCursorPos();

	bool focused;
	unsigned int cursor_index; // position into string: index = just before char.
	float cursor_pos; // actual pixel position
	Interpolator cursor_blink;
	float y_start;

	static Font default_textbox_font;
	};








} // end namespace LGC


#endif


