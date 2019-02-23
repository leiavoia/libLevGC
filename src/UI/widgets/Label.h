#ifndef LABEL_H
#define LABEL_H

#include "../../core/Fonts.h"
#include "BasicWidget.h"
#include <string>


namespace LGC {

using namespace sigslot;

class Label: public BasicWidget {
public:

	Label( float x, float y, float w, float h, BasicWidget* parent, std::string text = "");
	Label( float x, float y, BasicWidget* parent, std::string text = "", Font& font = Label::default_label_font ); // fit to text

	virtual ~Label() {}

// 	virtual bool Poll() { return true; }

	virtual void Draw();

	inline void DrawCentered( bool x ) { draw_centered = x; }

  	static void ReserveResources();
	static void DumpResources();

protected:
	bool draw_centered;
	static Font default_label_font;
	};








} // end namespace LGC


#endif



