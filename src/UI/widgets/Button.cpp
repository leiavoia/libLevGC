#include "Button.h"
#include "../../core/DrawingTools.h"
#include "../../core/ResourceMgr.h"

namespace LGC {

Font Button::default_button_font = Font();



Button::Button( float x, float y, float w, float h, BasicWidget* parent, std::string text) :
	BasicWidget(x,y,w,h,parent),
	mouseover(false),
	mousedown(false)
	{
	SetBGColor( ColorQuad(1,1,1,0.2) );
	SetBorderColor( ColorQuad(1,1,1,0.4) );
	SetTextColor( ColorQuad(1,1,1,1) );
	SetBorderWidth(2);
	SetFont( default_button_font );
// 	BasicWidget::signal_left_click_down.connect( this, &Button::HandleLeftClickDown );
// 	BasicWidget::signal_left_click_up.connect( this, &Button::HandleLeftClickUp );
// 	BasicWidget::signal_mouse_move.connect( this, &Button::HandleMouseMove );
	SetText(text);
	}

Button::Button( float x, float y, BasicWidget* parent, std::string text) :
	BasicWidget(x,y,0,0,parent),
	mouseover(false),
	mousedown(false)
	{
	SetBGColor( ColorQuad(1,1,1,0.2) );
	SetBorderColor( ColorQuad(1,1,1,0.4) );
	SetTextColor( ColorQuad(1,1,1,1) );
	SetBorderWidth(2);
	SetFont( default_button_font );
// 	BasicWidget::signal_left_click_down.connect( this, &Button::HandleLeftClickDown );
// 	BasicWidget::signal_left_click_up.connect( this, &Button::HandleLeftClickUp );
// 	BasicWidget::signal_mouse_move.connect( this, &Button::HandleMouseMove );
	SetText(text);
	// auto resize
	float newx,newy;
	font.GetStringSize( newx, newy, text );
	SetWidth( newx + 10 + BorderWidth()*2 );
	SetHeight( newy + 10 + BorderWidth()*2 );
	}

void Button::ReserveResources() {
	default_button_font = RM->GetFont("default button font", "Button");
	}

void Button::DumpResources() {
	RM->DumpFont(default_button_font, "Button");
	}



bool Button::Poll() {
	DoFading();
	return true;
	}

void Button::Draw() {
	// background color
	if (mouseover && !mousedown) {  SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.4) );  }
	else if (mousedown) {  SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.75) );  }
	else {  SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.2) );  }

	StartClipping();
	DrawFoundation();
	DrawBorder();
	DrawTextCentered();
	StopClipping();
	}


void Button::HandleLeftClickUp( int x, int y ) {
	if ( Visible() && mousedown )  {
		mousedown = false;
		signal_button_up(this);
		}
	}

void Button::HandleLeftClickDown( int x, int y ) {
	if ( Visible() && this->Contains(x,y) ) {
		signal_button_down(this);
		mousedown = true;
		}
	}

void Button::HandleMouseMove( int x, int y ) {
	if ( Visible() )  { mouseover = this->Contains(x,y) ? true : false; }
	}



REGISTER_CLASS_AS_RESOURCE_CLIENT(Button)


} // end namespace LGC

