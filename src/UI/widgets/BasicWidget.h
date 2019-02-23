// HOW TO USE WIDGETS:
//
// Usually best to create a "window pane" widget to contain other widgets.
// Make sure at least this window widget has focus:
//
// 	window->PushModal(); // REQUIRED TO RECEIVE INPUT
//
// Make sure at least one widget has model. Otherwise no input events will flow.
//
// Make sure resources are loaded for using fonts or backgrounds:
//
// 	LGC::Button::ReserveResources();
//
// Connect widgets to events:
//
// 	button->signal_button_down.connect( this, &ClassName::EventHandlerFunction );
//
// Check the API for each widget to see which events it supports.
//
// The class that contains the EventHandlerFunction must inherit from has_slots:
//
// 	class Foo: public has_slots<>
//
// Note that any child widgets are assumed to be from heap memory and
// will be deleted if the widget is destroyed. If you don't want children destroyed,
// make sure to call RemoveChild() on whatever children you want saved.


#ifndef BASIC_WIDGET_H
#define BASIC_WIDGET_H

#include <vector>
#include <string>
#include <stack>
#include "../../objects/PollingObject.h"
#include "../../core/Texture.h"
#include "../../core/Fonts.h"
#include "../../core/DrawingTools.h"
#include "../sigslot.h"
#include "../../etc/Interpolator.h"
#include "../../objects/PollingEngine.h"
#include "../../core/GameApplication.h"

namespace LGC {

using namespace sigslot;

class BasicWidget: public PollingObject, public has_slots<>  {
public:

	enum BackgroundMode {
		BG_STRETCH,
		BG_DECAL,
		BG_TILE
		};

	enum TextAlign {
		TEXT_LEFT,
		TEXT_RIGHT,
		TEXT_CENTER,
		TEXT_JUSTIFY
		};

	BasicWidget( float x, float y, float w, float h, BasicWidget* parent=NULL );

	virtual ~BasicWidget();

	virtual bool Poll();

	virtual void Draw();

	// Use this to designate an arbitrary widget a modal widget to take input.
	// This is usefull for a screen widget. Note that this should be done internally if possible
	// If you don't "modal" at least one widget, none of your widgets will accept any input events.
	void PushModal();
	void PopModal();


	void SetXPos( float x ) { xpos = x; }
	void SetYPos( float x ) { ypos = x; }
	void SetWidth( float x ) { width = x; }
	void SetHeight( float x ) { height = x; }
	void SetDims( float x, float y, float w, float h ) { xpos = x; ypos = y; width = w; height = h; }
	void SetText( std::string& str ) { text = str; }
	void SetText( const char* str ) { text = std::string(str); }
	void SetTextAlign( TextAlign x ) { text_align = x; }
	void SetTextColor( const ColorQuad& color ) { text_color = color; }
	void SetBGColor( const ColorQuad& color ) { bg_color = color; }
	void SetBGMode( BackgroundMode x ) { bg_mode = x; }
	void SetBGImage( const Texture& t ) { bg_image = t; }
	void SetBorderColor( const ColorQuad& color ) { border_color = color; }
	void SetBorderWidth( float x ) { border_width = x; }
	void SetBorderPadding( float x ) { border_padding = x; }
	void SetFont( Font& f ) { font = f; }
	void Hide();
	void Show( float opacity = 1 ); // also sets opacity
	void FadeIn( float time );
	void FadeOut( float time );
	inline void Mask() { masked = true; } // prevents parent from cascading Show()
	inline void Unmask() { masked = false; }

	void StartClipping();
	void StopClipping();

	inline float 		Xpos() { return xpos; }
	inline float 		Ypos() { return ypos; }
	inline float 		XFar() { return xpos + width; }
	inline float 		YFar() { return ypos + height; }
	inline float 		Width() { return width; }
	inline float 		Height() { return height; }
	float 			AbsoluteXpos();
	float 			AbsoluteYpos();
	inline float 		BorderWidth() { return border_width; };
	inline float 		BorderPadding() { return border_padding; };
	inline std::string 	Text() { return text; }
	inline TextAlign 	TextAlignment() { return text_align; }
	void 			BorderColor( ColorQuad& color ) { color = border_color; }
	void 			BGColor( ColorQuad& color ) { color = bg_color; }
	inline bool		Visible() { return opacity > 0; }
	inline bool		Masked() { return masked; }

	BasicWidget*		GetChild( unsigned int x );
	unsigned int 		NumChildren() { return children.size(); }
	void 			AddChild( BasicWidget* obj );
	void		 		RemoveChild( BasicWidget* obj );
	void				RemoveChildren();

	// SLOTS - default behaviour is just to pass command through to children
	virtual void HandleRightClickUp( int x, int y );
	virtual void HandleRightClickDown( int x, int y );
	virtual void HandleLeftClickUp( int x, int y );
	virtual void HandleLeftClickDown( int x, int y );
	virtual void HandleMouseMove( int x, int y );
	virtual void HandleTypeChar( char c );
	virtual void HandleCommand( std::string& s );

 	// STATIC CORE INTERFACE: (use this to enter commands from outside the widget hierarchy)
 	static void Event_RightClickUp( int x, int y );
	static void Event_RightClickDown( int x, int y );
	static void Event_LeftClickUp( int x, int y );
	static void Event_LeftClickDown( int x, int y );
	static void Event_MouseMove( int x, int y );
	static void Event_TypeChar( char c );
	static void Event_Command( std::string& s );

	static void ProcessEvent( SDL_Event& e );

protected:

	// Helper functions
	virtual void DrawFoundation();
	virtual void DrawBorder();
	virtual void DrawText( float x=0 , float y=0,  bool shadow=true );
	virtual void DrawTextCentered( bool shadow=true ); // middle-center
	void DoFading(); // returns opacity
	float MouseToWidgetXCoord( int x );
	float MouseToWidgetYCoord( int y );

	void BroadcastHandleRightClickUp( int x, int y );
	void BroadcastHandleRightClickDown( int x, int y );
	void BroadcastHandleLeftClickUp( int x, int y );
	void BroadcastHandleLeftClickDown( int x, int y );
	void BroadcastHandleMouseMove( int x, int y );
	void BroadcastHandleTypeChar( char c );
	void BroadcastHandleCommand( std::string& s );


	void	DeleteChild( BasicWidget* obj );
	void	DeleteChildren();


	// members
	std::vector<BasicWidget*> children;
	BasicWidget* parent;
	PollingEngine engine;

	float xpos;
	float ypos;
	float width;
	float height;
	std::string text;
	TextAlign text_align;
	float border_padding;
	float border_width;
	Texture bg_image;
	BackgroundMode bg_mode;
	ColorQuad bg_color;
	ColorQuad border_color;
	ColorQuad text_color;
	Font font;

	Interpolator fader;
	bool fading_in;
	bool fading_out;
	float opacity;
	bool masked;

	// STATIC CLASS MEMBERS:
	//static BasicWidget* widget_in_focus;
	static ScissorStack* scissorstack;
	static std::stack<BasicWidget*> modal_stack;

	// checks to see if args are inside widget area
	bool Contains(float x, float y);
	bool Contains(float x, float y, float w, float h);
	};








} // end namespace LGC


#endif








