#include "BasicWidget.h"
#include <GL/gl.h>
#include "../../utilities/LogManager.h"
#include "../../core/ResourceMgr.h"
#include "../../core/GameApplication.h"
#include "../../core/InputHandler.h"





namespace LGC {


// signal2<int,int> BasicWidget::signal_left_click_up;
// signal2<int,int> BasicWidget::signal_left_click_down;
// signal2<int,int> BasicWidget::signal_right_click_up;
// signal2<int,int> BasicWidget::signal_right_click_down;
// signal2<int,int> BasicWidget::signal_mouse_move;
// signal1<char> BasicWidget::signal_type_char;
// signal1<std::string&> BasicWidget::signal_key_command;


//BasicWidget* BasicWidget::widget_in_focus = NULL;
ScissorStack* BasicWidget::scissorstack = new ScissorStack();
std::stack<BasicWidget*> BasicWidget::modal_stack;



static const float DROPSHADOW_OFFSET = 2.0;
static const float BORDER_OFFSET_H = 0.13;
static const float BORDER_OFFSET_V = 0.2;

BasicWidget::BasicWidget( float x, float y, float w, float h, BasicWidget* parent ) :
	children( std::vector<BasicWidget*>() ),
	parent(parent),
	engine(PollingEngine()),
	xpos(x),
	ypos(y),
	width(w),
	height(h),
	text(""),
	text_align( TEXT_CENTER ),
	border_padding(0),
	border_width(2),
	bg_image( Texture() ),
	bg_mode( BG_TILE ),
	bg_color( ColorQuad() ),
	border_color( ColorQuad() ),
	text_color( ColorQuad() ),
	font(Font()),
	fader( Interpolator(Interpolator::NO_REPEAT) ),
	fading_in(false),
	fading_out(false),
	opacity(1.0),
	masked(false)
	{

	if (parent) { parent->AddChild(this); }
	bg_color.MakeWhite();
	border_color.MakeWhite();
	text_color.MakeWhite();
	}

BasicWidget::~BasicWidget() {
	DeleteChildren();
// 	LogMgr::Write(LOGMSG_DEBUG, "[%s]'s parent is -> [%d]", text.c_str(), parent );
// 	if (parent) {
// 		LogMgr::Write(LOGMSG_DEBUG, "Asking parent to remove me [me = %d]", this );
// 		parent->RemoveChild(this); // but don't delete!
// 		}
// 	else {
// 		LogMgr::Write(LOGMSG_DEBUG, "I have no parent [me = %d]", this );
// 		}
	}

bool BasicWidget::Poll() {
	DoFading();
	engine.PollAll( TimeFromLast() );
	return true;
	}

void BasicWidget::Draw() {
	if (opacity == 0) { return; }
	StartClipping();
	glPushMatrix();
	//	glLoadIdentity();
		DrawFoundation();
		DrawBorder();
		DrawTextCentered();
		// scootch over and draw kids
		glTranslatef( Xpos(), Ypos(), 0 );
		engine.DrawAll();
	glPopMatrix();
	StopClipping();
	}


void BasicWidget::DrawFoundation() {
	if (opacity == 0) { return; }
	// use image
	if ( bg_image.IsValid() ) {
		glColor4f( bg_color.R, bg_color.G, bg_color.B, bg_color.A * opacity );
		bg_image.TileAcross( Xpos(), Ypos(), Width(), Height() );
		}

	// or use bg color instead
	else {
		glBindTexture( GL_TEXTURE_2D, 0 );
		glBegin(GL_QUADS);
			glColor4f( bg_color.R, bg_color.G, bg_color.B, bg_color.A * opacity );
			glVertex2f( Xpos(), Ypos() + Height() );
			glVertex2f( Xpos() + Width(), Ypos() + Height() );
			glVertex2f( Xpos() + Width(), Ypos() );
			glVertex2f( Xpos(), Ypos() );
		glEnd();
		}
	}

void BasicWidget::DrawBorder() {
	if (opacity == 0) { return; }
	// draws a default beveled border
	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin(GL_QUADS);
		// top
		glColor4f( border_color.R+BORDER_OFFSET_V, border_color.G+BORDER_OFFSET_V, border_color.B+BORDER_OFFSET_V, (border_color.A+BORDER_OFFSET_V) * opacity );
		glVertex2f( Xpos() + border_width , 		Ypos() + border_width ); // BL
		glVertex2f( Xpos() + Width() - border_width, Ypos() + border_width ); // BR
		glVertex2f( Xpos() + Width(), 			Ypos() ); 			// TR
		glVertex2f( Xpos(), 					Ypos() );				// TL
		// bottom
		glColor4f( border_color.R-BORDER_OFFSET_V, border_color.G-BORDER_OFFSET_V, border_color.B-BORDER_OFFSET_V, (border_color.A-BORDER_OFFSET_V) * opacity );
		glVertex2f( Xpos(), 					Ypos() + Height() ); 				// BL
		glVertex2f( Xpos() + Width(), 			Ypos() + Height() ); 				// BR
		glVertex2f( Xpos() + Width() - border_width,	Ypos() + Height() - border_width ); 	// TR
		glVertex2f( Xpos() + border_width, 		Ypos() + Height() - border_width ); 	// TL
		// left
		glColor4f( border_color.R+BORDER_OFFSET_H, border_color.G+BORDER_OFFSET_H, border_color.B+BORDER_OFFSET_H, (border_color.A+BORDER_OFFSET_H) * opacity );
		glVertex2f( Xpos(), 			Ypos() + Height() ); 			// BL
		glVertex2f( Xpos() + border_width, Ypos() + Height() - border_width );// BR
		glVertex2f( Xpos() + border_width,	Ypos() + border_width ); 		// TR
		glVertex2f( Xpos(), 			Ypos() );						// TL
		// right
		glColor4f( border_color.R-BORDER_OFFSET_H, border_color.G-BORDER_OFFSET_H, border_color.B-BORDER_OFFSET_H, (border_color.A-BORDER_OFFSET_H) * opacity );
		glVertex2f( Xpos() + Width() - border_width, Ypos() + Height() - border_width );// BL
		glVertex2f( Xpos() + Width(), 			Ypos() + Height() );			// BR
		glVertex2f( Xpos() + Width(),				Ypos() ); 					// TR
		glVertex2f( Xpos() + Width() - border_width, Ypos() + border_width );			// TL
	glEnd();
	}





float BasicWidget::AbsoluteXpos() {
	return xpos + ((parent) ? parent->AbsoluteXpos() : 0);
	}
float BasicWidget::AbsoluteYpos() {
	return ypos + ((parent) ? parent->AbsoluteYpos() : 0);
	}


BasicWidget* BasicWidget::GetChild( unsigned int x ) {
	if ( x >= children.size() ) { return NULL; }
	return children[x];
	}

void BasicWidget::AddChild( BasicWidget* obj ) {
	if (obj) {
		if (obj->parent != this && obj->parent) { obj->parent->RemoveChild(obj); }
		obj->parent = this;
		children.push_back(obj);
		engine.Register(obj);
		}
	}

void BasicWidget::RemoveChild( BasicWidget* obj ) {
	if (obj) {
// 		LogMgr::Write(LOGMSG_DEBUG, "[%d] Unlinking my child [%d] ...", this, obj );
		obj->Unlink();
// 		LogMgr::Write(LOGMSG_DEBUG, "[%d] Erasing my child [%d] ...", this, obj );
		children.erase(   std::find( children.begin(), children.end(), obj )   );
// 		LogMgr::Write(LOGMSG_DEBUG, "[%d] Nullifying my child [%d] ...", this, obj );
		obj->parent = NULL;
		}
	}

void BasicWidget::RemoveChildren() {
	for ( std::vector<BasicWidget*>::iterator i = children.begin(); i < children.end(); i++ ) {
// 		LogMgr::Write(LOGMSG_DEBUG, "[%d] Unlinking my child [%d] ...", this, (*i) );
		(*i)->Unlink();
// 		LogMgr::Write(LOGMSG_DEBUG, "[%d] Nullifying my child [%d] ...", this, (*i) );
		(*i)->parent = NULL;
		}
// 	LogMgr::Write(LOGMSG_DEBUG, "[%d] Erasing my children ...", this );
	children.clear();
	// no delete
	}

void BasicWidget::DeleteChild( BasicWidget* obj ) {
	if (obj) {
		obj->Unlink();
		children.erase(   std::find( children.begin(), children.end(), obj )   );
		delete obj;
		}
	}

void BasicWidget::DeleteChildren() {
	engine.Flush(); // this will try to delete children anyway :-)
// 	for ( unsigned int i = 0; i < children.size(); i++ ) {
// // 		LogMgr::Write(LOGMSG_DEBUG, "[%d] Deleting my child [%d] ...", this, children[i] );
// // 		children[i]->DeleteChildren();
// // 		children[i]->Unlink();
// // 		delete children[i];
// // 		children.erase( children.begin() + 1 );
// 		}
	}


void BasicWidget::DrawText( float x , float y, bool shadow ) {
	if (opacity == 0) { return; }
	float target_x = Xpos();
	float target_y = Ypos();
	if ( this->text_align == TEXT_RIGHT ) {
		float w, h;
		this->font.GetStringSize(w,h,text);
		target_x = Xpos() + (Width() - w);
		target_y = Ypos() + (Height() - h);
		}
	else if ( this->text_align == TEXT_CENTER ) {
		target_x = Xpos() + (Width() * 0.5);
		target_y = Ypos() + (Height() * 0.5);
		}
	glPushMatrix();
		if (shadow) {
			glTranslatef( target_x + x + DROPSHADOW_OFFSET, target_y + y + DROPSHADOW_OFFSET, 0 );
			glColor4f( 0,0,0,text_color.A * opacity );
			font.RenderFont( text );
			glTranslatef( -DROPSHADOW_OFFSET, -DROPSHADOW_OFFSET, 0 );
			glColor4f( text_color.R, text_color.G, text_color.B, text_color.A * opacity );
			font.RenderFont( text );
			}
		else {
			glTranslatef( target_x + x, target_y + y, 0 );
			glColor4f( text_color.R, text_color.G, text_color.B, text_color.A * opacity );
			font.RenderFont( text );
			}
	glPopMatrix();
	}

void BasicWidget::DrawTextCentered( bool shadow ) {
	if (opacity == 0) { return; }
	if (shadow) {
		glColor4f( 0,0,0,text_color.A * opacity );
		font.DrawTextCentered( text, Xpos() + (Width() * 0.5) + DROPSHADOW_OFFSET, Ypos() + (Height() * 0.5) + DROPSHADOW_OFFSET );
		}
	glColor4f( text_color.R, text_color.G, text_color.B, text_color.A * opacity );
	font.DrawTextCentered( text, Xpos() + (Width() * 0.5), Ypos() + (Height() * 0.5) );
	}


bool BasicWidget::Contains(float x, float y) {
	return (
		x < AbsoluteXpos() + Width() &&
		x > AbsoluteXpos() &&
		y < AbsoluteYpos() + Height() &&
		y > AbsoluteYpos()
		);
	}

bool BasicWidget::Contains(float x, float y, float w, float h) {
	return (
		x+w < AbsoluteXpos() + Width() &&
		x > AbsoluteXpos() &&
		y+h < AbsoluteYpos() + Height() &&
		y > AbsoluteYpos()
		);
	}

void BasicWidget::FadeIn( float time ) {
	fading_in = true;
	fading_out = false;
	fader.Clear();
	// create ramp
	fader.AddPoint( Time(), 0 );
	fader.AddPoint( Time()+time, 1 );
	for (unsigned int i=0; i < children.size(); i++) {
		if ( !children[i]->Masked() ) { children[i]->FadeIn(time); }
		}
	}

void BasicWidget::FadeOut( float time ) {
	fading_in = false;
	fading_out = true;
	fader.Clear();
	// create ramp
	fader.AddPoint( Time(), 1 );
	fader.AddPoint( Time()+time, 0 );
	for (unsigned int i=0; i < children.size(); i++) {
		if ( !children[i]->Masked() ) children[i]->FadeOut(time);
		}
	}

void BasicWidget::Hide() {
	for (unsigned int i=0; i < children.size(); i++) {
		if ( !children[i]->Masked() ) children[i]->Hide();
		}
	opacity = 0.0;
	}

void BasicWidget::Show( float opacity ) {
	if ( opacity < 0 ) { opacity = 0; }
	if ( opacity > 1 ) { opacity = 1; }
	this->opacity = opacity;
	fading_in = false;
	fading_out = false;
	for (unsigned int i=0; i < children.size(); i++) {
		if ( !children[i]->Masked() ) children[i]->Show(opacity);
		}
	}



void BasicWidget::DoFading() {
	if (fading_in || fading_out) {
		// stop fading if needed
		if (fading_in && fader.Value( Time() ) == 1.0) {
			fading_in = false;
			opacity = 1.0;
			return;
			}
		else if (fading_out && fader.Value( Time() ) == 0.0) {
			fading_out = false;
			opacity = 0.0;
			return;
			}
		else {
			opacity = fader.Value( Time() );
			return;
			}
		}
	}


void BasicWidget::StartClipping() {
	scissorstack->Push(
		(int)AbsoluteXpos(),
		App->ScreenHeight() - (int)AbsoluteYpos() - (int)height,
		(int)width,
		(int)height
		);
	}

void BasicWidget::StopClipping() {
	scissorstack->Pop();
	}

float BasicWidget::MouseToWidgetXCoord( int x ) {
	return (float)x - AbsoluteXpos();
	}

float BasicWidget::MouseToWidgetYCoord( int y ) {
	return (float)y - AbsoluteYpos();
	}


void BasicWidget::PushModal() {
	modal_stack.push(this);
	}

void BasicWidget::PopModal() {
	modal_stack.pop();
	}


void BasicWidget::HandleRightClickUp( int x, int y ) {
	BroadcastHandleRightClickUp(x,y);
	}
void BasicWidget::HandleRightClickDown( int x, int y ) {
	BroadcastHandleRightClickDown(x,y);
	}
void BasicWidget::HandleLeftClickUp( int x, int y ) {
	BroadcastHandleLeftClickUp(x,y);
	}
void BasicWidget::HandleLeftClickDown( int x, int y ) {
	BroadcastHandleLeftClickDown(x,y);
	}
void BasicWidget::HandleMouseMove( int x, int y ) {
	BroadcastHandleMouseMove(x,y);
	}
void BasicWidget::HandleTypeChar( char c ) {
	BroadcastHandleTypeChar(c);
	}
void BasicWidget::HandleCommand( std::string& s ) {
	BroadcastHandleCommand(s);
	}



void BasicWidget::BroadcastHandleRightClickUp( int x, int y ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleRightClickUp(x,y);
		}
	}
void BasicWidget::BroadcastHandleRightClickDown( int x, int y ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleRightClickDown(x,y);
		}
	}
void BasicWidget::BroadcastHandleLeftClickUp( int x, int y ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleLeftClickUp(x,y);
		}
	}
void BasicWidget::BroadcastHandleLeftClickDown( int x, int y ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleLeftClickDown(x,y);
		}
	}
void BasicWidget::BroadcastHandleMouseMove( int x, int y ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleMouseMove(x,y);
		}
	}
void BasicWidget::BroadcastHandleTypeChar( char c ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleTypeChar(c);
		}
	}
void BasicWidget::BroadcastHandleCommand( std::string& s ) {
	for(unsigned int i=0; i < children.size(); i++) {
		children[i]->HandleCommand(s);
		}
	}


void BasicWidget::Event_RightClickUp( int x, int y ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleRightClickUp(x,y);
		}
	}

void BasicWidget::Event_RightClickDown( int x, int y ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleRightClickDown(x,y);
		}
	}

void BasicWidget::Event_LeftClickUp( int x, int y ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleLeftClickUp(x,y);
		}
	}

void BasicWidget::Event_LeftClickDown( int x, int y ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleLeftClickDown(x,y);
		}
	}

void BasicWidget::Event_MouseMove( int x, int y ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleMouseMove(x,y);
		}
	}

void BasicWidget::Event_TypeChar( char c ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleTypeChar(c);
		}
	}

void BasicWidget::Event_Command( std::string& s ) {
	if ( modal_stack.size() ) {
		modal_stack.top()->HandleCommand(s);
		}
	}

void BasicWidget::ProcessEvent( SDL_Event& e ) {
	// KEYPRESS
 	if (e.type == SDL_KEYDOWN) {
 		if ( IsPrintable(e.key.keysym.sym) ) {
 			LogMgr::Write(LOGMSG_DEBUG, "[%c]", GetKeyChar(e.key.keysym.sym) );
 			BasicWidget::Event_TypeChar(  GetKeyChar(e.key.keysym.sym)  );
 			}
		else if ( IsKeyCommand(e.key.keysym.sym) ) {
			LogMgr::Write(LOGMSG_DEBUG, "[%s]", TranslateKey(e.key.keysym.sym).c_str() );
			std::string s = TranslateKey(e.key.keysym.sym);
			BasicWidget::Event_Command(  s  );
			}
		}
	// MOUSE MOVEMENT
	// WARNING: MOUSE COORDS NEED TO BE IN WORLD SPACE NOT SCREEN SPACE
	if (e.type == SDL_MOUSEMOTION) {
		BasicWidget::Event_MouseMove( e.motion.x, e.motion.y );
		}
	else if (e.type == SDL_MOUSEBUTTONDOWN) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			BasicWidget::Event_LeftClickDown( e.button.x, e.button.y );
			}
		else if (e.button.button == SDL_BUTTON_RIGHT) {
			BasicWidget::Event_RightClickDown( e.button.x, e.button.y );
			}

		}
	else if (e.type == SDL_MOUSEBUTTONUP) {
		if (e.button.button == SDL_BUTTON_LEFT) {
			BasicWidget::Event_LeftClickUp( e.button.x, e.button.y );
			}
		else if (e.button.button == SDL_BUTTON_RIGHT) {
			BasicWidget::Event_RightClickUp( e.button.x, e.button.y );
			}
		}
	}


//--------------------------------------------------------------------------



} // end namespace LGC

