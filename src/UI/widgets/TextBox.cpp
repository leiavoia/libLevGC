#include "TextBox.h"
#include "../../core/DrawingTools.h"
#include "../../core/ResourceMgr.h"
#include "../../utilities/LogManager.h"

const static float CURSOR_WIDTH = 2;

namespace LGC {

Font TextBox::default_textbox_font = Font();



TextBox::TextBox( float x, float y, float w, float h, BasicWidget* parent, std::string text) :
	BasicWidget(x,y,w,h,parent),
	focused(false),
	cursor_index(0),
	cursor_pos(0),
	cursor_blink( Interpolator() ),
	y_start(0)
	{
	SetBGColor( ColorQuad(1,1,1,0.2) );
	SetBorderColor( ColorQuad(1,1,1,0.4) );
	SetTextColor( ColorQuad(1,1,1,1) );
	SetBorderWidth(2);
	SetBorderPadding(4);
	SetFont( default_textbox_font );
// 	BasicWidget::signal_left_click_down.connect( this, &TextBox::HandleLeftClickDown );
// 	BasicWidget::signal_left_click_up.connect( this, &TextBox::HandleLeftClickUp );
// 	BasicWidget::signal_mouse_move.connect( this, &TextBox::HandleMouseMove );
// 	BasicWidget::signal_type_char.connect( this, &TextBox::HandleTypeChar );
// 	BasicWidget::signal_key_command.connect( this, &TextBox::HandleCommand );
	SetText(text);
	cursor_blink.Preset_Alternate(0.6, 1, 0);

	// get height of sample
	std::string s = "|ABQabq@()[]y";
	float height,junk;
	font.GetStringSize(junk,height,s);
	y_start = (Height() - height) * 0.5;

	SetTextAlign( TEXT_LEFT );
	}

void TextBox::ReserveResources() {
	default_textbox_font = RM->GetFont("default textbox font", "TextBox");
	}

void TextBox::DumpResources() {
	RM->DumpFont(default_textbox_font, "TextBox");
	}



bool TextBox::Poll() {
	DoFading();
	return true;
	}

void TextBox::Draw() {
	StartClipping();
	glPushMatrix();
		DrawFoundation();
		DrawBorder();
		DrawText( border_padding, y_start, true );
		// draw cursor on top
		if (focused) { SetColor(   1, 1, 1, cursor_blink.Value( Time() )   ); }
		else { SetColor( 1, 1, 1, 0 ); }
		glTranslatef( Xpos(), Ypos(), 0 );
		DrawQuad( cursor_pos, 0, CURSOR_WIDTH, Height() );
	glPopMatrix();
	StopClipping();
	}


void TextBox::Focus() {
	focused = Visible() ?  true  : false;
	if ( focused )  {
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.4) );
		cursor_index = 0;
		cursor_pos = 0;
		}
	else {
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.2) );
		}
	}

void TextBox::UnFocus() {
	focused = false;
	this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.2) );
	signal_enter(this);
	}

void TextBox::HandleLeftClickUp( int x, int y ) {
	}

void TextBox::HandleLeftClickDown( int x, int y ) {
	if ( this->Contains(x,y) ) { Focus(); }
	if ( focused )  {
		// set cursor from mouse coords
		float x_click = MouseToWidgetXCoord(x);
		cursor_index = 0;
		cursor_pos = 0;
		bool hint = false;
		while ( cursor_pos < (x_click + border_padding) && cursor_index < text.size() ) {
			++cursor_index;
			MoveCursorPos();
			if (cursor_index == text.size() && cursor_pos < (x_click + border_padding)) {hint = true;}
			}
		// move back one unless off the end
		if (cursor_index > 0 && !hint) {
			--cursor_index;
			MoveCursorPos();
			}
		}
	}

void TextBox::HandleMouseMove( int x, int y ) {
	// BGCOLOR: hilite if mouseover or focuses
	if ( this->Visible() && (this->Contains(x,y) || focused) )  {
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.4) );
		}
	else {
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.2) );
		}
	}

void TextBox::HandleTypeChar( char c ) {
	if (!focused) {return;}
	if (cursor_index >= text.size()) {
		text.append(1,c);
		}
	else {
		text.insert(cursor_index, 1, c);
		}
	++cursor_index;
	MoveCursorPos();
	signal_change(this);
	}

void TextBox::HandleCommand( std::string& s ) {
	if (!focused) {return;}
	if (s == "enter") {
		focused = false;
		this->SetBGColor( ColorQuad(bg_color.R,bg_color.G,bg_color.B,0.2) );
		signal_enter(this);
		}
	if (s == "backspace") {
		if (cursor_index != 0) {
			text.erase(cursor_index-1, 1);
			--cursor_index;
			MoveCursorPos();
			signal_change(this);
			}
		}
	if (s == "delete") {
		if (cursor_index < text.size()) {
			text.erase(cursor_index, 1);
			signal_change(this);
			}
		}
	if (s == "left") {
		if ( cursor_index > 0 ) {
			--cursor_index;
			MoveCursorPos();
			}
		}
	if (s == "right") {
		++cursor_index;
		if ( cursor_index > text.size() ) { cursor_index = text.size(); }
		MoveCursorPos();
		}
	}


void TextBox::MoveCursorPos() {
	if (cursor_index == 0) {
		cursor_pos = border_padding;
		return;
		}
	std::string temp = text.substr(0,cursor_index); // minus one to start at beginning of char, not after
	float mx,my;
	font.GetStringSize(mx,my,temp);
	cursor_pos = mx + border_padding;
	//LogMgr::Write( LOGMSG_DEBUG, "temp '%s', size = %f, index %d", temp.c_str(), mx, cursor_index);
	}



void TextBox::SetBGColor( const ColorQuad& color ) {
	bg_color = color;
	for ( unsigned int i = 0; i < children.size(); i++ ) {
		children[i]->SetBGColor( color );
		}
	}



REGISTER_CLASS_AS_RESOURCE_CLIENT(TextBox)


} // end namespace LGC

