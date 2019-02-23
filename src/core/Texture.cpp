#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <GL/gl.h>

#include "Texture.h"


namespace LGC {




BlendMode Texture::global_blend_mode = BLEND_NORMAL;
float Texture::screen_x = 0;
float Texture::screen_y = 0;



void Texture::Blit( float x, float y, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( Left(), Bottom() );
		glVertex2f( x, y+Height() );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( Right(), Bottom() );
		glVertex2f(  x+Width(), y+Height() );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( Right(), Top() );
		glVertex2f( x+Width(), y );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( Left(), Top() );
		glVertex2f( x, y );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}


void Texture::BlitFromCenter( float x, float y, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float half_w = Width() * 0.5;
	float half_h = Height() * 0.5;

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( Left(), Bottom() );
		glVertex2f( x-half_w, y+half_h );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( Right(), Bottom() );
		glVertex2f(  x+half_w, y+half_h );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( Right(), Top() );
		glVertex2f( x+half_w, y-half_h );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( Left(), Top() );
		glVertex2f( x-half_w, y-half_h );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}

void Texture::BlitFlip( float x, float y, bool flip_horz, bool flip_vert, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float left = 	(flip_horz) ? Right() 	: Left();
	float right = 	(flip_horz) ? Left() 	: Right();
	float bottom = (flip_vert) ? Top() 	: Bottom();
	float top = 	(flip_vert) ? Bottom() 	: Top();

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( left, bottom );
		glVertex2f( x, y+Height() );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( right, bottom );
		glVertex2f(  x+Width(), y+Height() );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( right, top );
		glVertex2f( x+Width(), y );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( left, top );
		glVertex2f( x, y );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}


void Texture::BlitFlipFromCenter( float x, float y, bool flip_horz, bool flip_vert, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float half_w = Width() * 0.5;
	float half_h = Height() * 0.5;

	float left = 	(flip_horz) ? Right() 	: Left();
	float right = 	(flip_horz) ? Left() 	: Right();
	float bottom = (flip_vert) ? Top() 	: Bottom();
	float top = 	(flip_vert) ? Bottom() 	: Top();

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( left, bottom );
		glVertex2f( x-half_w, y+half_h );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( right, bottom );
		glVertex2f(  x+half_w, y+half_h );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( right, top );
		glVertex2f( x+half_w, y-half_h );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( left, top );
		glVertex2f( x-half_w, y-half_h );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}




void Texture::BlitRotoZoom( float x, float y, float rot, float scale, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float adjust_x = Width() * scale;
	float adjust_y = Height() * scale;

	glPushMatrix();
	glLoadIdentity();

	// TRANSLATE FIRST, ROTOZOOM SECOND!
	glTranslatef( x - screen_x + adjust_x, y - screen_y + adjust_y, 0  );
	if (scale != 1.0) { glScalef( scale, scale, 0); }
	if (rot) { glRotatef( rot, 0, 0, 1 ); }
	//glTranslatef( adjust_x, adjust_y, 0  );

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( Left(), Bottom() );
		glVertex2f( 0, Height() );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( Right(), Bottom() );
		glVertex2f(  Width(), Height() );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( Right(), Top() );
		glVertex2f( Width(), 0 );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( Left(), Top() );
		glVertex2f( 0, 0 );
	glEnd();
	glPopMatrix();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}

void Texture::BlitRotoZoomFromCenter( float x, float y, float rot, float scale, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	glPushMatrix();
	glLoadIdentity();

	// TRANSLATE FIRST, ROTOZOOM SECOND!
	glTranslatef(  x - screen_x,  y - screen_y, 0   );
	if (scale != 1.0) 	{ glScalef( scale, scale, 0); }
	if (rot != 0.0) 	{ glRotatef( rot, 0, 0, 1 ); }
	glTranslatef(  -(Width() * 0.5),  -(Height() * 0.5), 0   );

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( Left(), Bottom() );
		glVertex2f( 0, Height() );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( Right(), Bottom() );
		glVertex2f(  Width(), Height() );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( Right(), Top() );
		glVertex2f( Width(), 0 );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( Left(), Top() );
		glVertex2f( 0, 0 );
	glEnd();
	glPopMatrix();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}


void Texture::BlitRotoZoomFlip( float x, float y, float rot, float scale, bool flip_horz, bool flip_vert, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float adjust_x = Width() * scale;
	float adjust_y = Height() * scale;

	float left = 	(flip_horz) ? Right() 	: Left();
	float right = 	(flip_horz) ? Left() 	: Right();
	float bottom = (flip_vert) ? Top() 	: Bottom();
	float top = 	(flip_vert) ? Bottom() 	: Top();

	glPushMatrix();
	glLoadIdentity();

	// TRANSLATE FIRST, ROTOZOOM SECOND!
	glTranslatef( x - screen_x + adjust_x, y - screen_y + adjust_y, 0  );
	if (scale != 1.0) { glScalef( scale, scale, 0); }
	if (rot) { glRotatef( rot, 0, 0, 1 ); }
	//glTranslatef( adjust_x, adjust_y, 0  );

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( left, bottom );
		glVertex2f( 0, Height() );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( right, bottom );
		glVertex2f(  Width(), Height() );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( right, top );
		glVertex2f( Width(), 0 );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( left, top );
		glVertex2f( 0, 0 );
	glEnd();
	glPopMatrix();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}

void Texture::BlitRotoZoomFlipFromCenter( float x, float y, float rot, float scale, bool flip_horz, bool flip_vert, BlendMode blend ) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float left = 	(flip_horz) ? Right() 	: Left();
	float right = 	(flip_horz) ? Left() 	: Right();
	float bottom = (flip_vert) ? Top() 	: Bottom();
	float top = 	(flip_vert) ? Bottom() 	: Top();

	glPushMatrix();
	glLoadIdentity();

	// TRANSLATE FIRST, ROTOZOOM SECOND!
	glTranslatef(  x - screen_x,  y - screen_y, 0   );
	if (scale != 1.0) 	{ glScalef( scale, scale, 0); }
	if (rot != 0.0) 	{ glRotatef( rot, 0, 0, 1 ); }
	glTranslatef(  -(Width() * 0.5),  -(Height() * 0.5), 0   );

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( left, bottom );
		glVertex2f( 0, Height() );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( right, bottom );
		glVertex2f(  Width(), Height() );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( right, top );
		glVertex2f( Width(), 0 );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( left, top );
		glVertex2f( 0, 0 );
	glEnd();
	glPopMatrix();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}

void Texture::BlitCustomStretch ( float x, float y, float w, float h, BlendMode blend) {
	if (!id) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( Left(), Bottom() );
		glVertex2f( x, y+h );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( Right(), Bottom() );
		glVertex2f(  x+w, y+h );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( Right(), Top() );
		glVertex2f( x+w, y );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( Left(), Top() );
		glVertex2f( x, y );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}

void Texture::TileAcross( float x, float y, float w, float h, BlendMode blend ) {
	if ( !id || !is_tilable ) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float x_ratio = w / (float)Width();
	float y_ratio = h / (float)Height();

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( 0, y_ratio );
		glVertex2f( x, y+h );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( x_ratio, y_ratio );
		glVertex2f(  x+w, y+h );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( x_ratio, 0 );
		glVertex2f( x+w, y );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( 0, 0 );
		glVertex2f( x, y );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}


void Texture::TileAcrossWithOffset( float x, float y, float w, float h, float xoff, float yoff, BlendMode blend ) {
	if ( !id || !is_tilable ) { return; }

	glBindTexture( GL_TEXTURE_2D, id );

	if ( blend != global_blend_mode ) { SwitchBlendMode( blend ); }

	float x_ratio = w / (float)Width();
	float y_ratio = h / (float)Height();

	glBegin(GL_QUADS);
		/* Bottom Left Of The Texture and Quad */
		glTexCoord2f( xoff, y_ratio + yoff );
		glVertex2f( x, y+h );
		/* Bottom Right Of The Texture and Quad */
		glTexCoord2f( x_ratio + xoff, y_ratio + yoff );
		glVertex2f(  x+w, y+h );
		/* Top Right Of The Texture and Quad */
		glTexCoord2f( x_ratio + xoff, yoff );
		glVertex2f( x+w, y );
		/* Top Left Of The Texture and Quad */
		glTexCoord2f( xoff, yoff );
		glVertex2f( x, y );
	glEnd();

	// FIXME
	if ( blend != BLEND_NORMAL ) { SwitchBlendMode( BLEND_NORMAL ); }
	}



void Texture::SwitchBlendMode(BlendMode blend) {
	switch(blend) {
		case BLEND_NORMAL:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		case BLEND_NONE:
			glDisable(GL_BLEND);
			break;
		case BLEND_ADD:
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA,GL_ONE);
			break;
		case BLEND_SUBTRACT:
			// TODO: NOT IMPLEMENTED YET
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
			break;
		}
	global_blend_mode = blend;
	}




} // end namespace LGC























