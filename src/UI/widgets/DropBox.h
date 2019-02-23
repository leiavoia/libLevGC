#ifndef DROPBOX_H
#define DROPBOX_H

#include <string>
#include <vector>
#include "../sigslot.h"
#include "../../core/Fonts.h"
#include "BasicWidget.h"
#include "Slider.h"


namespace LGC {

using namespace sigslot;

class DropBox: public BasicWidget {
public:

	DropBox( float x, float y, BasicWidget* parent, unsigned int num_options=1 );

	virtual ~DropBox() {}

	virtual bool Poll();

	virtual void Draw();

	void AddEntry( const std::string& str );
	void RemoveEntry( const std::string& str );
	void SelectEntry( const std::string& str );

	// SIGNALS
 	signal1<BasicWidget*> signal_select;
 	signal1<BasicWidget*> signal_focus;
 	signal1<BasicWidget*> signal_unfocus;

	// SLOTS (for core interface)
	virtual void HandleLeftClickUp( int x, int y );
	virtual void HandleLeftClickDown( int x, int y );
	virtual void HandleMouseMove( int x, int y );

	virtual void HandleDropButton( Button* obj );
	virtual void HandleSelect( Button* obj );
	virtual void HandleScroll( float value, Slider* obj );

  	static void ReserveResources();
	static void DumpResources();

	inline bool SelectedItem() { return selected; }


protected:

	unsigned int num_options;
	float option_height;
	float option_width;
	bool open;

	Button* selected;
	std::vector< Button* > items;
	BasicWidget* pane;
	VSlider* slider;
	Button* drop_button;



	static Font default_dropbox_font;

	void OpenList( float time = 0 );
	void CloseList( float time = 0 );
	void ResizeAllButtons();
	};


} // end namespace LGC





#endif


