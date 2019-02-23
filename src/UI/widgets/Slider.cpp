#include "Slider.h"
#include "../../utilities/LogManager.h"

namespace LGC {



Slider::~Slider() {
//	NOTE: do not delete child widgets. polling engine does this for us!
// 	delete first_button;
// 	delete second_button;
// 	delete mid_button;
	}



Slider::Slider( float x, float y, float w, float h, float range_start, float range_end, BasicWidget* parent ) :
	// default setup is for Vertical - override for Horz.
	BasicWidget(x,y,w,h,parent),
	start(range_start),
	end(range_end),
	value(range_start),
	incr(0),
	space_left(h - (w*3)),
	last_pos(0),
	first_button( new Button(0,0,w,w,this) ),
	second_button( new Button(0,h-w,w,w,this) ),
	mid_button( new Button(0,w,w,w,this) ),
	track_slider(false),
	mousedown_offset(0)
	{

	// set out stuph
	SetBGColor( ColorQuad(1,1,1,0.2) );
	SetBorderWidth(0);
	SetText(text);
	incr = ( (end - start) / space_left ); // range divided by pixel space left
	if (incr == 0) { incr = 1; }

	// wire up buttons
	first_button->signal_button_down.connect( this, &Slider::HandleFirstButton );
	second_button->signal_button_down.connect( this, &Slider::HandleSecondButton );
	mid_button->signal_button_down.connect( this, &Slider::HandleSlideButtonDown );
	mid_button->signal_button_up.connect( this, &Slider::HandleSlideButtonUp );

	}

void Slider::SetBGColor( const ColorQuad& color ) {
	bg_color = color;
	for ( unsigned int i = 0; i < children.size(); i++ ) {
		children[i]->SetBGColor( color );
		}
	}

void Slider::SetBorderColor( const ColorQuad& color ) {
	border_color = color;
	for ( unsigned int i = 0; i < children.size(); i++ ) {
		children[i]->SetBorderColor( color );
		}
	}

HSlider::HSlider( float x, float y, float w, float h, float range_start, float range_end, BasicWidget* parent ) :
	Slider(x,y,w,h,range_start,range_end,parent)
	{
// 	BasicWidget::signal_mouse_move.connect( this, &HSlider::HandleMouseMove );
	// reset button dims from Vertical default
	this->first_button->SetDims(0,0,h,h);
	this->second_button->SetDims(w-h,0,h,h);
	this->mid_button->SetDims(h,0,h,h);

	// recalc space left and increment
	space_left = w - (h*3);
	incr = ( (end - start) / space_left ); // range divided by pixel space left
	if (incr == 0) { incr = 1; }

	// enough space for scrollbar button?
	if ( w < h*3 ) {
		this->DeleteChild( mid_button );
		mid_button = NULL;
		incr = ( (end - start) / INCREMENTS_IF_TOO_SMALL );
		LogMgr::Write(LOGMSG_ERROR, "Slider::Slider : disabling slider");
		}

	// enough space for end buttons?
	if (w < h*2) {
		LogMgr::Write(LOGMSG_ERROR, "Slider::Slider : Slider size is too small to function");
		}

	}



VSlider::VSlider( float x, float y, float w, float h, float range_start, float range_end, BasicWidget* parent ) :
	Slider(x,y,w,h,range_start,range_end,parent)
	{

// 	BasicWidget::signal_mouse_move.connect( this, &VSlider::HandleMouseMove );

	// enough space for scrollbar button?
	if ( h < w*3 ) {
		this->DeleteChild( mid_button );
		mid_button = NULL;
		incr = ( (end - start) / INCREMENTS_IF_TOO_SMALL );
		LogMgr::Write(LOGMSG_ERROR, "Slider::Slider : disabling slider");
		}

	// enough space for end buttons?
	if (h < w*2) {
		LogMgr::Write(LOGMSG_ERROR, "Slider::Slider : Slider size is too small to function");
		}
	}



void Slider::SetValue( float x ) {
	value = x;
	if (value > end) { value = end; }
	if (value < start) { value = start; }
	//signal_slider_move(value, this);
	UpdateSliderPos();
	}



void Slider::SetRange( float range_start, float range_end ) {
	start = range_start;
	end = range_end;
	incr = ( (end - start) / space_left ); // range divided by pixel space left
	if (incr == 0) { incr = 1; }
	UpdateSliderPos();
	}





bool Slider::Poll() {
	engine.PollAll( TimeFromLast() );
	return true;
	}


void Slider::Draw() {
	glPushMatrix();
		DrawFoundation();
		// scootch over and draw kids
		glTranslatef( Xpos(), Ypos(), 0 );
		engine.DrawAll();
	glPopMatrix();
	}



void HSlider::HandleMouseMove( int x, int y ) {
	// BGCOLOR: hilite if sliding or mouseover
	if ( this->Visible() && (this->Contains(x,y) || track_slider) )  {
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.4) );
		}
	else {
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.2) );
		}

	// slide if tracking
	if (track_slider) {
		// move slider button
		float pixel_delta = float(x) - last_pos;
		float new_x = mid_button->Xpos() + pixel_delta;
		if (new_x < Height()) { new_x = Height(); }
		if ( new_x > Width() - (Height()*2) ) { new_x = Width() - (Height()*2); }
		// update everything
		value = incr * (new_x-Height());
		UpdateSliderPos();
		signal_slider_move(value, this);
		}

	last_pos = (float)x;
	if ( last_pos > this->AbsoluteXpos() + Height()*0.5 + (Width() - (Height()*2)) ) { last_pos = this->AbsoluteXpos() + Height()*0.5 + (Width() - (Height()*2)); }
	else if ( last_pos < this->AbsoluteXpos() + Height()*0.5 + Height() ) { last_pos = this->AbsoluteXpos() + Height()*0.5 + Height(); }

	BroadcastHandleMouseMove( x,y );
	}


void VSlider::HandleMouseMove( int x, int y ) {
	// BGCOLOR: hilite if sliding or mouseover
	if ( this->Visible() && (this->Contains(x,y) || track_slider) )  {
		this->SetBGColor( ColorQuad(1,1,1,0.4) );
		}
	else {
		this->SetBGColor( ColorQuad(1,1,1,0.2) );
		}

	// slide if tracking
	if (track_slider) {
		// move slider button
		float pixel_delta = float(y) - last_pos;
		float new_y = mid_button->Ypos() + pixel_delta;
		if (new_y < Width()) { new_y = Width(); }
		if ( new_y > Height() - (Width()*2) ) { new_y = Height() - (Width()*2); }
		// update everything
		value = incr * (new_y-Width());
		UpdateSliderPos();
		signal_slider_move(value, this);
// 		LogMgr::Write(LOGMSG_DEBUG, "y_in[%f] - last_pos[%f] = [%f] ***** Ypos[%f] + delta[%f] = new_y[%f]", float(y), last_pos, float(y) - last_pos, mid_button->Ypos(), pixel_delta, new_y);
//
		}

	last_pos = (float)y;
	if ( last_pos > this->AbsoluteYpos() + (Height() - (Width()*1.5)) ) {
		last_pos = this->AbsoluteYpos() + (Height() - (Width()*1.5));
		}
	else if ( last_pos < this->AbsoluteYpos() + Width()*1.5 ) {
		last_pos = this->AbsoluteYpos() + Width()*1.5;
		}

	BroadcastHandleMouseMove( x,y );
	}



// void VSlider::HandleMouseMove( int x, int y ) {
// 	static float last_pos = 0;
//
// 	// BGCOLOR: hilite if sliding or mouseover
// 	if ( this->Visible() && (this->Contains(x,y) || track_slider) )  {
// 		this->SetBGColor( ColorQuad(1,1,1,0.4) );
// 		}
// 	else {
// 		this->SetBGColor( ColorQuad(1,1,1,0.2) );
// 		}
//
// 	// slide if tracking
// 	if (track_slider) {
// 		// get movement data
// 		float pixel_delta = float(y) - last_pos;
// 		float new_y = mid_button->Ypos() + pixel_delta;
// 		if (new_y < Width()) { new_y = Width(); }
// 		if ( new_y > Height() - (Width()*2) ) { new_y = Height() - (Width()*2); }
// 		// update everything
// 		value = incr * ( new_y - Width() );
// 		UpdateSliderPos();
// 		signal_slider_move(value, this);
// 		//LogMgr::Write(LOGMSG_DEBUG, "y_in[%f] - last_pos[%f] = [%f] ***** Ypos[%f] + delta[%f] = new_y[%f]", float(y), last_pos, float(y) - last_pos, mid_button->Ypos(), pixel_delta, new_y);
// 		}
//
// 	last_pos = (float)y;
// 	// out of bounds check/reset
// 	if ( last_pos > this->AbsoluteYpos() + (Width()*0.5) + (Height() - (Width()*2)) ) { last_pos = this->AbsoluteYpos() + (Width()*0.5) + (Height() - (Width()*2));  }
// 	if ( last_pos < this->AbsoluteYpos() + (Width()*0.5) + Width() ) {  last_pos = this->AbsoluteYpos() + (Width()*0.5) + Width(); }
// 	}


void Slider::HandleFirstButton( Button* obj ) {
	value -= incr != 0 ? incr : 1;
	if (value < start) { value = start; }
	signal_slider_move(value, this);
	UpdateSliderPos();
	}


void Slider::HandleSecondButton( Button* obj ) {
	value += incr != 0 ? incr : 1;
	if (value > end) { value = end; }
	signal_slider_move(value, this);
	UpdateSliderPos();
	}


void Slider::HandleSlideButtonDown( Button* obj ) {
	track_slider = true;
	}


void Slider::HandleSlideButtonUp( Button* obj ) {
	track_slider = false;
	}


void HSlider::UpdateSliderPos() {
	if (!mid_button) { return; }
	mid_button->SetXPos(   Height() + space_left * ((value - start) / (end - start))   );
	}


void VSlider::UpdateSliderPos() {
	if (!mid_button) { return; }
	mid_button->SetYPos(   Width() + space_left * ((value - start) / (end - start))   );
	}



} // END NAMESPACE LGC

