#ifndef SLIDER_H
#define SLIDER_H

#include <string>
#include "../sigslot.h"
#include "BasicWidget.h"
#include "Button.h"



namespace LGC {

using namespace sigslot;


static const float INCREMENTS_IF_TOO_SMALL = 20;



// you can use either INTS or FLOATS or whatever
class Slider: public BasicWidget {
public:
	virtual ~Slider();
	virtual bool Poll();
	virtual void Draw();

	float Value() { return value; }
	float Start() { return start; }
	float End() { return end; }

	void SetValue( float x );
	void SetRange( float start, float end );

	void SetBGColor( const ColorQuad& color );
	void SetBorderColor( const ColorQuad& color );

	// SIGNALS
 	signal2< float, Slider* > signal_slider_move; // <current value, object>

	// SLOTS (for core interface)
	virtual void HandleMouseMove( int x, int y ) = 0;

	virtual void HandleFirstButton( Button* obj );
	virtual void HandleSecondButton( Button* obj );
	virtual void HandleSlideButtonUp( Button* obj );
	virtual void HandleSlideButtonDown( Button* obj );

protected:

	// can't instantiate - only V and H sliders can be created
	Slider( float x, float y, float w, float h, float range_start, float range_end, BasicWidget* parent );

	float start;
	float end;
	float value;
	float incr;
	float space_left;
	float last_pos;

	Button* first_button;
	Button* second_button;
	Button* mid_button;

	bool track_slider;
	float mousedown_offset;

	// helper function
	virtual void UpdateSliderPos()=0;
	};



// HORIZONTAL SLIDER
class HSlider: public Slider {
public:

	HSlider( float x, float y, float w, float h, float range_start, float range_end, BasicWidget* parent );

	virtual ~HSlider() {}

	virtual void UpdateSliderPos();

	// SLOTS (for core interface)
	virtual void HandleMouseMove( int x, int y );
	};


// VERTICAL SLIDER
class VSlider: public Slider {
public:

	VSlider( float x, float y, float w, float h, float range_start, float range_end, BasicWidget* parent );

	virtual ~VSlider() {}

	virtual void UpdateSliderPos();

	// SLOTS (for core interface)
	virtual void HandleMouseMove( int x, int y );

	};




////////////////////////////////////////////////////////////////////////////////////////////////


} // end namespace LGC


#endif



