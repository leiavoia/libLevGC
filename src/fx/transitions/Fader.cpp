#include "Fader.h"

#include "../../core/DrawingTools.h"
#include <GL/gl.h>
#include "../../core/Sequencer.h"
#include "../../utilities/LogManager.h"
#include "../../core/GameApplication.h"
#include "../../core/extra_math.h"

namespace LGC {




static const float FADE_TIME_FTB = 0.6;
static const float FADE_TIME_FIFB = 0.25;
static const float FADE_TIME_CF = 0.3;


// Generic function to grab screen image:
void GetScreenCapture( Texture& img ) {
	glReadBuffer(GL_FRONT);

	// create a texture from raw memory
	int required_texture_size = NextPowerOfTwo( App->ScreenHeight() );
	GLuint texture_num = 0;
	glGenTextures( 1, &texture_num );
	glBindTexture( GL_TEXTURE_2D, texture_num );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR );
	glCopyTexImage2D(GL_TEXTURE_2D,0,GL_RGB,0,0,required_texture_size,required_texture_size,0);

	// create a proper game Texture
	AreaRect<int> one(0,0,required_texture_size,required_texture_size);
	AreaRect<int> two(0,0,App->ScreenWidth(),App->ScreenHeight());
	img = Texture( std::string("screen"), (int)texture_num, one, two );
	}




FadeToBlackTransition::FadeToBlackTransition(Sequencer* s):
	sequencer(s),
	curve( Interpolator(Interpolator::NO_REPEAT) ),
	fade_time(FADE_TIME_FTB)
	{
	// FIXME <<<*******************************************************
	// sequencer->FreezeNonGUI();
	curve.AddPoint(0,0);
	curve.AddPoint(fade_time*0.5,1);
	curve.AddPoint(fade_time,0);
	GetScreenCapture( img );
	}

FadeToBlackTransition::~FadeToBlackTransition() {
	const GLuint x = img.ID();
	glDeleteTextures( 1, &x );
	}


bool FadeToBlackTransition::Poll() {
	if ( TimeFromBirth() > fade_time ) {
		// FIXME <<<*******************************************************
		// sequencer->ThawNonGUI();
		return false;
		}
	return true;
	}

void FadeToBlackTransition::Draw() {
	float x = curve.Value( TimeFromBirth() );
	glPushMatrix();
	glLoadIdentity();
	if ( TimeFromBirth() < fade_time*0.5 ) {
		glColor4f( 1,1,1,1 );
		img.BlitRotoZoomFlipFromCenter( App->ScreenWidth()/2, App->ScreenHeight()/2, 0, 1.0,false,true );
		}
	glColor4f( 0,0,0,x );
	DrawQuad(0,0,App->ScreenWidth(),App->ScreenHeight());
	glPopMatrix();
	}

void FadeToBlackTransition::SetFadeTime( float t ) {
	curve.Clear();
	curve.AddPoint(0,0);
	curve.AddPoint(t*0.5,1);
	curve.AddPoint(t,0);
	fade_time = t;
	}

// =================================================================




CrossFadeTransition::CrossFadeTransition(Sequencer* s):
	sequencer(s),
	curve( Interpolator(Interpolator::NO_REPEAT) ),
	fade_time(FADE_TIME_CF)
	{
	// FIXME <<<*******************************************************
	// sequencer->FreezeNonGUI();
	curve.AddPoint(0,1);
	curve.AddPoint(fade_time,0);
	GetScreenCapture( img );
	}

CrossFadeTransition::~CrossFadeTransition() {
	const GLuint x = img.ID();
	glDeleteTextures( 1, &x );
	}


bool CrossFadeTransition::Poll() {
	if ( TimeFromBirth() > fade_time ) {
		return false;
		}
	return true;
	}

void CrossFadeTransition::Draw() {
	float x = curve.Value( TimeFromBirth() );
	glPushMatrix();
	glLoadIdentity();
	glColor4f( 1,1,1,x );
	img.BlitRotoZoomFlipFromCenter(App->ScreenWidth()/2, App->ScreenHeight()/2,0,1.0,false,true);
	glPopMatrix();
	}

void CrossFadeTransition::SetFadeTime( float t ) {
	curve.Clear();
	curve.AddPoint(0,1);
	curve.AddPoint(t,0);
	fade_time = t;
	}

// =================================================================




BreakBoxTransition::BreakBoxTransition(Sequencer* s):
	CrossFadeTransition(s)
	{
	// we should have an image now. Split it up into separate textures.
	unsigned int across = 10;
	unsigned int down = 8;
	int required_texture_size = NextPowerOfTwo( App->ScreenHeight() );
	AreaRect<int> texture_coords(0,0,required_texture_size,required_texture_size);
	int w = App->ScreenWidth() / across;
	int h = App->ScreenHeight() / down;
	for( unsigned int x = 0; x < across; x++ ) {
		for( unsigned int y = 0; y < down; y++ ) {
			AreaRect<int> area_coords( x*w, y*h, x*w + w, y*h + h );
			boxes.push_back( Texture( std::string("box"), img.ID(), texture_coords, area_coords ) );
			}
		}
	}

BreakBoxTransition::~BreakBoxTransition() {

	}

void BreakBoxTransition::Draw() {
	float x = curve.Value( TimeFromBirth() );
	glPushMatrix();
	glLoadIdentity();
	float screenh = float( App->ScreenHeight() );
	float boxwidth_half = (boxes[0].RightPx() - boxes[0].LeftPx()) * 0.5;
	float boxheight_half = (boxes[0].BottomPx() - boxes[0].TopPx()) * 0.5;
	glColor4f( 1,1,1,x );
	for ( unsigned int i = 0; i < boxes.size(); i++ ) {
		// remember that the image is upside down,
		// so we have to rearrange the whole matrix when rendering
		float xcoord = boxes[i].LeftPx() + boxwidth_half;
		float ycoord = screenh - (boxes[i].TopPx() + boxheight_half);
		boxes[i].BlitRotoZoomFlipFromCenter( xcoord, ycoord,  360.0f * x, x, false, true );
		}
	glPopMatrix();
	}



// =================================================================



FadeInFromBlackTransition::FadeInFromBlackTransition(Sequencer* s):
	sequencer(s),
	curve( Interpolator(Interpolator::NO_REPEAT) ),
	fade_time(FADE_TIME_FIFB)
	{

	// FIXME <<<*******************************************************
	// sequencer->FreezeNonGUI();

	curve.AddPoint(0,1);
	curve.AddPoint(fade_time,0);
	}

FadeInFromBlackTransition::~FadeInFromBlackTransition() {
	}


bool FadeInFromBlackTransition::Poll() {
	if ( TimeFromBirth() > fade_time ) {
		// FIXME <<<*******************************************************
		// sequencer->ThawNonGUI();
		return false;
		}
	return true;
	}

void FadeInFromBlackTransition::Draw() {
	// FIXME fixed Screen size?
	float x = curve.Value( TimeFromBirth() );
	glPushMatrix();
	glLoadIdentity();
	glColor4f( 0,0,0,x );
	DrawQuad(0,0,App->ScreenWidth(),App->ScreenHeight());
	glPopMatrix();
	}

void FadeInFromBlackTransition::SetFadeTime( float t ) {
	curve.Clear();
	curve.AddPoint(0,1);
	curve.AddPoint(t,0);
	fade_time = t;
	}


} // end namespace LGC












