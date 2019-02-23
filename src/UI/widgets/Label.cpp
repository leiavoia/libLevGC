#include "Label.h"
#include "../../core/DrawingTools.h"
#include "../../core/ResourceMgr.h"

namespace LGC {

Font Label::default_label_font = Font();



Label::Label( float x, float y, float w, float h, BasicWidget* parent, std::string text) :
	BasicWidget(x,y,w,h,parent)
	{
	SetTextColor( ColorQuad(1,1,1,1) );
	SetFont( default_label_font );
	SetTextAlign(TEXT_LEFT);
	SetText(text);
	draw_centered = false;
	}

Label::Label( float x, float y, BasicWidget* parent, std::string text, Font& font ) :
	BasicWidget(x,y,10,10,parent)
	{
	SetTextColor( ColorQuad(1,1,1,1) );
	SetFont( font );
	SetTextAlign(TEXT_LEFT);
	SetText(text);
	if ( this->font.Valid() && text != "" ) {
		float w, h;
		this->font.GetStringSize(w,h,text);
		SetWidth( w + (BorderPadding()*2) + (BorderWidth()*2) );
		SetHeight( h + (BorderPadding()*2) + (BorderWidth()*2) );
		}
	else {
		SetFont( default_label_font );
		}
	draw_centered = false;
	}

void Label::ReserveResources() {
	default_label_font = RM->GetFont("default label font", "Label");
	}

void Label::DumpResources() {
	RM->DumpFont(default_label_font, "Label");
	}

void Label::Draw() {
// 	DrawFoundation();
 	if ( draw_centered ) { DrawTextCentered(); } else { DrawText(); }
// 	DrawBorder();
	}




REGISTER_CLASS_AS_RESOURCE_CLIENT(Label)


} // end namespace LGC

