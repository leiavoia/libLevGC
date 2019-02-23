#include "DropBox.h"
//#include "../../core/DrawingTools.h"
#include "../../core/ResourceMgr.h"
#include "../../utilities/LogManager.h"




namespace LGC {

Font DropBox::default_dropbox_font = Font();
const static float SCROLLBAR_SIZE = 15;


DropBox::DropBox( float x, float y, BasicWidget* parent, unsigned int num_options ) :
	BasicWidget(x,y,25,15,parent),
	num_options(num_options),
	option_height(15),
	option_width(30),
	open(false)
	{
	SetBGColor( ColorQuad(0,0,0,0) );
	SetBorderColor( ColorQuad(1,1,1,0.8) );
	SetBorderWidth(2);
	SetFont( default_dropbox_font );

// 	BasicWidget::signal_left_click_down.connect( this, &DropBox::HandleLeftClickDown );
// 	BasicWidget::signal_left_click_up.connect( this, &DropBox::HandleLeftClickUp );
// 	BasicWidget::signal_mouse_move.connect( this, &DropBox::HandleMouseMove );

	// get height of sample
	Button* b = new Button(0,0,NULL,"|ABQabq@()[]y");
	b->SetBorderWidth(0);
	option_height = b->Height();
	delete b;

	// create internals
	pane = new BasicWidget(0,0,30,option_height,this);
	pane->SetBorderWidth(0);
	pane->SetBGColor( ColorQuad(0,0,0,0) );
	slider = new VSlider( Width() - SCROLLBAR_SIZE, 0, SCROLLBAR_SIZE, option_height * num_options, 0, 100, this );
	drop_button = new Button( Width() - SCROLLBAR_SIZE, 0, SCROLLBAR_SIZE, option_height,this );
	selected = new Button( 0, 0, this, "");
	selected->SetHeight(option_height);

	drop_button->signal_button_down.connect( this, &DropBox::HandleDropButton );
	slider->signal_slider_move.connect( this, &DropBox::HandleScroll );

	pane->Mask();
	slider->Mask();
	pane->Hide();
	slider->Hide();
	SetHeight(option_height);
	open = false;

	}

void DropBox::ReserveResources() {
	default_dropbox_font = RM->GetFont("default dropbox font", "DropBox");
	}

void DropBox::DumpResources() {
	RM->DumpFont(default_dropbox_font, "DropBox");
	}

void DropBox::AddEntry( const std::string& str ) {
	// check for duplicate entry
	for ( std::vector< Button* >::iterator i = items.begin(); i != items.end(); i++ ) {
		if ((*i)->Text() == str) {
			LogMgr::Write(LOGMSG_ERROR, "DropBox::AddEntry() - Ignoring duplicate entry");
			return;
			}
		}
	Button* b = new Button( 0, 0, pane, str);
	b->signal_button_down.connect( this, &DropBox::HandleSelect );
	b->SetHeight(option_height);
	if (!open) { b->Hide(); }
	items.push_back( b );
	std::string s =items[0]->Text();
	selected->SetText( s );
	ResizeAllButtons();
	slider->SetRange( 0, ((float)items.size() * option_height) - (option_height * num_options) );
	}

void DropBox::RemoveEntry( const std::string& str ) {
	// delete item
	if (selected && selected->Text() == str) { selected = NULL; }
	for ( std::vector< Button* >::iterator i = items.begin(); i != items.end(); i++ ) {
		if ((*i)->Text() == str) {
			delete (*i);
			items.erase( i );
			}
		}
	ResizeAllButtons();
	}

void DropBox::SelectEntry( const std::string& str ) {
	selected->SetText( str.c_str() );
	CloseList(0);
	}

bool DropBox::Poll() {
	BasicWidget::Poll();
	return true;
	}

void DropBox::Draw() {
	StartClipping();
	glPushMatrix();
		DrawFoundation();
		DrawBorder();
		// scootch over and draw kids
		glTranslatef( Xpos(), Ypos(), 0 );
		engine.DrawAll();
	glPopMatrix();
	StopClipping();
	}


void DropBox::HandleLeftClickUp( int x, int y ) {
// 	if ( Visible() && !Contains(x,y))  {
// 		CloseList();
// 		}
	BroadcastHandleLeftClickUp( x, y );
	}

void DropBox::HandleLeftClickDown( int x, int y ) {
	if ( Visible() && !Contains(x,y))  {
		CloseList();
		}
	BroadcastHandleLeftClickDown( x, y );
	}

void DropBox::HandleMouseMove( int x, int y ) {
// 	if ( Visible() )  { mouseover = this->Contains(x,y) ? true : false; }
	BroadcastHandleMouseMove( x, y );
	}

void DropBox::HandleDropButton( Button* obj ) {
	OpenList(0.2);
	}

void DropBox::HandleSelect( Button* obj ) {
	std::string s = obj->Text();
	selected->SetText( s );
	CloseList(0.2);
	signal_select(obj);
	}

void DropBox::HandleScroll( float value, Slider* obj ) {
	pane->SetYPos( -value );
	}


void DropBox::OpenList( float time ) {
	if (open) { return; }
	drop_button->Mask();
	selected->Mask();
	selected->FadeOut(time);
	drop_button->FadeOut(time);
	pane->Unmask();
	slider->Unmask();
	pane->FadeIn(time);
	slider->FadeIn(time);
	SetHeight(option_height * num_options);
	open = true;
	PushModal();
	}

void DropBox::CloseList( float time ) {
	if (!open) { return; }
	pane->Mask();
	slider->Mask();
	selected->Unmask();
	drop_button->Unmask();
	selected->FadeIn(time);
	drop_button->FadeIn(time);
	pane->FadeOut(time);
	slider->FadeOut(time);
	SetHeight(option_height);
	open = false;
	PopModal();
	}

void DropBox::ResizeAllButtons() {
	// recalc max width:
	float new_max = 0;
	for ( std::vector< Button* >::iterator i = items.begin(); i != items.end(); i++ ) {
		if ( (*i)->Width() > new_max ) { new_max = (*i)->Width(); }
		}
	option_width = new_max;

	// set all buttons to the new max width and space them out vertically
	for ( unsigned int i = 0; i < items.size(); i++ ) {
		items[i]->SetWidth( option_width );
		items[i]->SetYPos( option_height * float(i) );
		}

	// change the selected button, scrollbar, and drop button
	selected->SetWidth( option_width );
	slider->SetXPos(option_width);
	drop_button->SetXPos(option_width);
	pane->SetWidth(option_width);
	pane->SetHeight( option_height * float(items.size()) );

	// change my own size:
	SetWidth(option_width + SCROLLBAR_SIZE);
	if (open) { SetHeight(option_height * num_options); }
	else { SetHeight(option_height); }
	}


REGISTER_CLASS_AS_RESOURCE_CLIENT(DropBox)


} // end namespace LGC

