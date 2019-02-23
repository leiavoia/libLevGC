#include "Fonts.h"

#include <GL/gl.h>
#include <sstream>
#include <queue>
#include <FTGL/ftgl.h>
#include "../utilities/LogManager.h"


namespace LGC {

Font::~Font() {
	for ( std::map< std::string, int >::iterator i = display_lists.begin(); i != display_lists.end(); i++ ) {
		FreeCompressedText( i->second );
		}
	}

void Font::RenderFontToScreen( const std::string& str, float x, float y ) {
	if ( !font ) { return; }
	glPushMatrix();
	glLoadIdentity();
	float min, max, dummy;
	font->BBox( "A|kljygh", dummy, min, dummy, dummy, max, dummy );
	glTranslatef( x, y, 0 );
	glScalef(1, -1, 1);
	glTranslatef( 0, ((min-max) * 0.78), 0 );
	font->Render(str);
	glPopMatrix();
	}

void Font::RenderFont( const std::string& str ) {
	if ( !font ) { return; }
	glPushMatrix();
	glScalef(1, -1, 1);
	float min, max, dummy;
	font->BBox( "A|kljygh", dummy, min, dummy, dummy, max, dummy );
	glTranslatef(0,(min-max) * 0.78 ,0);
	font->Render(str);
	glPopMatrix();
	}



void Font::CompressText(
	std::vector< std::string >& strings,
	std::string reference_name,
	float h_clip,
	TextFormat format )
	{

	if ( !font ) { return; }

	if (h_clip < 0) { h_clip = 2000; } // surely your screen isn't bigger than this!

	// prep work:
	float xmin, xmax, ymin, ymax, dummy;
	float h_track = 0; // end of last word, in pixels
	//float v_track = 0; // vertical depth, in pixels

	font->BBox( "W", xmin, ymin, dummy, xmax, ymax, dummy );
	const float line_spacing = (ymax - ymin) * 1.5;
	const float paragraph_spacing = line_spacing * 0.8;

	// break the text up into words
	std::string buffer;
	std::vector< std::queue< std::string > > words( strings.size() );
	for( unsigned int i=0; i < strings.size(); i++ ) { // foreach paragraph
		std::stringstream ss( strings[i] );
	    	while (ss >> buffer) {
			words[i].push(buffer);
			}
		}

	// start recording the display list here:
	int disp_list = glGenLists(1);
 	glNewList( disp_list, GL_COMPILE );

	// format left
	if ( format == FORMAT_LEFT ) {
		for( unsigned int i=0; i < strings.size(); i++ ) { // foreach paragraph

			std::stringstream render_str;
			bool first_word = true;

			while ( !words[i].empty() ) {

				// get next word width (includes any space)
				std::string nextword;
				if (!first_word) { nextword = " "; }
				nextword += words[i].front();
				font->BBox( nextword, xmin, ymin, dummy, xmax, ymax, dummy );
				float word_w = (xmax - xmin);

				// pack into the rendering string if we have space
				if ( word_w + h_track <= h_clip ) {
					//LogMgr::Write( LOGMSG_DEBUG, "Adding word [%s] { word + track = %f, clip = %f}", words[i].front().c_str(), word_w + h_track, h_clip);
					h_track += word_w;
					render_str << nextword;
					words[i].pop();
					first_word = false;
					}
				// check to see if the whole word is bigger than the h_clip (draw it anyway)
				else if ( word_w > h_clip ) {
					LogMgr::Write( LOGMSG_DEBUG, "Adding word [%s - TOO LARGE FOR LINE] { word + track = %f, clip = %f}", words[i].front().c_str(), word_w + h_track, h_clip);
					RenderFont( words[i].front() );
					words[i].pop();
					glTranslatef( 0, line_spacing, 0); // move down
					first_word = true;
					render_str.str("");
					h_track = 0;
					}
				// otherwise render and move line down
				else {
					//LogMgr::Write( LOGMSG_DEBUG, "Rendering. word [%s] did not fit { word + track = %f, clip = %f}", words[i].front().c_str(), word_w + h_track, h_clip);
					RenderFont( render_str.str() );
					glTranslatef( 0, line_spacing, 0); // move down
					first_word = true;
					render_str.str("");
					h_track = 0;
					}

				}

			// render whatever is left in the buffer
			RenderFont( render_str.str() );
			glTranslatef( 0, line_spacing, 0); // move down
			first_word = true;
			render_str.str("");
			h_track = 0;

			// move down for next paragraph
			if ( i < words.size()-1 ) {
				glTranslatef( 0, paragraph_spacing, 0); // move down
				}
			}
		}


	glEndList();

	this->display_lists[ reference_name ] = disp_list;
	}

void Font::UseStoredText( std::string name, float x, float y ) {
// 	if ( !font ) { return; }
	glPushMatrix();
// 	glLoadIdentity();
// 	float min, max, dummy;
// 	font->BBox( "A|kljygh", dummy, min, dummy, dummy, max, dummy );
	glTranslatef( x, y, 0 );
// 	glScalef(1, -1, 1);
// 	glTranslatef( 0, ((min-max) * 0.78), 0 );
	glCallList( display_lists[name] );
	glPopMatrix();
	}

void Font::FreeCompressedText( int x ) {
	glDeleteLists(x,1);
	}

void Font::DrawTextCentered( const std::string& text, float x, float y ) {
	if ( !font ) { return; }
	float ymin, ymax, xmin, xmax, dummy, h, w;
	font->BBox( text, xmin, ymin, dummy, xmax, ymax, dummy );
	h = ymax - ymin;
	w = xmax - xmin;
	glPushMatrix();
	glTranslatef( x - (w*0.5), y - (h*0.5), 0 );
	RenderFont( text );
	glPopMatrix();
	}

void Font::GetStringSize( float& w, float& h, const std::string& str ) {
	if ( !font ) { return; }
	float ymin, ymax, xmin, xmax, dummy;
	font->BBox( str, xmin, ymin, dummy, xmax, ymax, dummy );
	h = ymax - ymin;
	w = xmax - xmin;
	}









#ifdef LGC_USE_FTGL

FTGLFontImpl::FTGLFontImpl( const std::string& filename ) {
	font = new FTTextureFont( filename.c_str() );
	}

FTGLFontImpl::~FTGLFontImpl() {
	delete font;
	font = NULL;
	}

// void FTGLFontImpl::Open( const std::string& filename ) {
// 	font->Open( filename.c_str() );
// 	}

void FTGLFontImpl::BBox( const std::string& string, float& xmin, float& ymin, float& zmin, float& xmax, float& ymax, float& zmax ) {
	font->BBox( string.c_str(), xmin, ymin, zmin, xmax, ymax, zmax );
	}

void FTGLFontImpl::Render( const std::string& string ) {
	font->Render( string.c_str() );
	}

void FTGLFontImpl::Advance( const std::string& string ) {
	font->Advance( string.c_str() );
	}

int FTGLFontImpl::Error() { return font->Error(); }

void FTGLFontImpl::SetFaceSize( int size ) {
	font->FaceSize( size );
	}

// int FTGLFontImpl::FaceSize() {
// 	return font->FaceSize();
// 	}

#endif






} // end namespace LGC




