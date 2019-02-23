#include "RenderingLayer.h"

#include <algorithm>
#include <iostream>
#include <GL/gl.h>
#include <GL/glu.h>

#include "../utilities/LogManager.h"



namespace LGC {


RenderingLayer* RenderingLayer::GetInstance() {
	static RenderingLayer* app = new RenderingLayer();
	return app;
	}


RenderingLayer::RenderingLayer() :
 	screen_width(800),
	screen_height(600),
	color_bitdepth(16),
	inited(false)
	{

	}

RenderingLayer::~RenderingLayer() {
	SDL_WM_ToggleFullScreen( screen );
	}



void RenderingLayer::Init( int width, int height, int bitdepth, bool fullscreen) {

	if (inited) { return; }

	/* initialize SDL */
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) {
		LogMgr::Write(LOGMSG_FATAL, "*** SDL Video Initialization Failed *** : %s", SDL_GetError( ));
		return;
		}

	/* Fetch the video info */
	const SDL_VideoInfo *vinfo = SDL_GetVideoInfo();
	if ( !vinfo ) {
		LogMgr::Write(LOGMSG_ERROR, "Video Info Query Failed: %s", SDL_GetError( ));
		return;
		}

	/* the flags to pass to SDL_SetVideoMode */
	videoFlags = SDL_OPENGL | SDL_GL_DOUBLEBUFFER | SDL_HWPALETTE /*| SDL_RESIZABLE*/;
	if (fullscreen) { videoFlags |= SDL_FULLSCREEN; }

	/* This checks to see if surfaces can be stored in memory */
	if ( vinfo->hw_available ) { videoFlags |= SDL_HWSURFACE; }
	else { videoFlags |= SDL_SWSURFACE; }

	/* This checks if hardware blits can be done */
	if ( vinfo->blit_hw ) { videoFlags |= SDL_HWACCEL; }

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* get an SDL surface */
	screen = SDL_SetVideoMode( width, height, bitdepth, videoFlags );

	/* Verify there is a screen */
	if ( !screen ) {
		LogMgr::Write(LOGMSG_ERROR, "Video Mode Set Failed: %s", SDL_GetError( ));
		return;
		}

	// turn off the mouse cursor:
	//SDL_ShowCursor( SDL_DISABLE );

	ResizeWindow( width, height );

// 	// VIDEO INFO:
// 	std::cout
// 	<< "hw_available: " << vinfo->hw_available << std::endl
// 	<< "wm_available: " << vinfo->wm_available << std::endl
// 	<< "blit_hw: " << vinfo->blit_hw << std::endl
// 	<< "blit_hw_CC: " << vinfo->blit_hw_CC << std::endl
// 	<< "blit_hw_A: " << vinfo->blit_hw_A << std::endl
// 	<< "blit_sw: " << vinfo->blit_sw << std::endl
// 	<< "blit_sw_CC: " << vinfo->blit_sw_CC << std::endl
// 	<< "blit_sw_A: " << vinfo->blit_sw_A << std::endl
// 	<< "blit_fill: " << vinfo->blit_fill << std::endl
// 	<< "video_mem: " << vinfo->video_mem << std::endl
// 	<< "hw_available: " << vinfo->hw_available << std::endl
// 	<< "hw_available: " << vinfo->hw_available << std::endl;


	// save variables for later if we've gotten this far (they obviously work)
	screen_width = width;
	screen_height = height;
	color_bitdepth = bitdepth;


	// OPENGL BEGINNING STATE:

	glShadeModel( GL_SMOOTH );
	glClearColor( 0.0f, 0.0f, 0.0f, 1.0f );
	glClearDepth( 1000.0f );
	glEnable( GL_DEPTH_TEST );
	glEnable(GL_TEXTURE_2D);
	glDepthFunc( GL_LEQUAL );
	//glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
	glLineWidth(1);
	glPointSize(1);
// 	glPolygonMode(GL_FRONT_AND_BACK, GL_SMOOTH);
// 	glEnable(GL_ALPHA_TEST);
// 	glAlphaFunc(GL_LEQUAL, 1);

	// don't use any texture if none are needed
	glBindTexture( GL_TEXTURE_2D, 0 );

	ClearScreen();

	inited = true;

	}

void RenderingLayer::ClearScreen() {
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	}

void RenderingLayer::SwapBuffers() {
	SDL_GL_SwapBuffers();
	}


void RenderingLayer::ToggleFullScreen() {
	SDL_WM_ToggleFullScreen( screen );
	}


void RenderingLayer::Shutdown() {
	if (!inited) { return; }
 	SDL_Quit();
	inited = false;
	}


void RenderingLayer::ResizeWindow( int width, int height ) {
	screen = SDL_SetVideoMode( width, height, color_bitdepth, videoFlags );
	if ( !screen ) {
		LogMgr::Write(LOGMSG_ERROR, "Could not get a surface after resize: %s", SDL_GetError( ));
		return;
		}
	if ( height == 0 ) { height = 1; } // protect against a divide by zero
	glViewport( 0, 0, ( GLsizei )width, ( GLsizei )height );
	glMatrixMode( GL_PROJECTION );
	glLoadIdentity();
	gluPerspective( 50.0f, (GLfloat)width / (GLfloat)height, 1, 1000 );
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	}



void RenderingLayer::Ortho( bool x ) {
	static bool on = false;
	glMatrixMode( GL_PROJECTION );
	if ( x && !on ) {
		// void glOrtho(GLdouble left, GLdouble right, GLdouble bottom, GLdouble top, GLdouble near, GLdouble far);
		// glOrtho(0.0f,screen_width,screen_height,0.0f,-1.0f,1000.0f);
		glPushMatrix();
		glLoadIdentity();
		gluOrtho2D( 0.0f, screen_width, screen_height, 0.0f );
		// NOTE: ortho2D assumes ( -1.0 < z < 1.0 )
		on = true;
		}
	else if ( !x && on )  {
		glPopMatrix();
		on = false;
		}
	glMatrixMode( GL_MODELVIEW );
	glLoadIdentity();
	}


void RenderingLayer::Register( RenderObject* x ) {
	if (!x) { return; }
	registry.push_back(x);
	}

void RenderingLayer::Unregister( RenderObject* x ) {
	if (!x) { return; }
	registry.erase(   std::find( registry.begin(), registry.end(), x )   );
	}


void RenderingLayer::Render() {
	// TODO:
	//	- culling / occlusion
	//	- material / lighting sorting

// 	std::cout << "rendering [" << registry.size() << "] objects\n";
	// render all objects
	for (std::list<RenderObject*>::iterator iter = registry.begin(); iter != registry.end(); iter++) {
		(*iter)->Render( this ); // this is a double-dispatch call which loops back to a specific render function
		}
	SwapBuffers();
	Renderer->ClearScreen();
	}




// OBJECT-SPECIFIC RENDERING FUNCTIONS ======================\/======================

void RenderingLayer::Render( RenderMesh* x ) {
	if ( x->hide ) { return; }

	// switch material outside of the PushAttrib() - it manages itself
	if ( x->material ) { x->material->Switch(); }

	glPushMatrix();
	glPushAttrib( GL_TEXTURE_BIT | GL_COLOR_BUFFER_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_LIGHTING_BIT | GL_POLYGON_BIT );

	if ( !x->shaded ) 	{ glShadeModel( GL_FLAT ); }
	if ( x->wireframe )	{ glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); }
	if ( x->light ) 	{ glEnable(GL_LIGHTING); }



	int last_texture = 0;
	glDisable(GL_TEXTURE_2D);

	// move object into place
	glTranslatef( x->X(), x->Y(), x->Z() );


	glBegin( GL_TRIANGLES );
	for ( TriangleList::iterator i = x->triangles.begin(); i != x->triangles.end(); i++ ) {

		Vertex& v1 = x->vertexes[i->p1];
		Vertex& v2 = x->vertexes[i->p2];
		Vertex& v3 = x->vertexes[i->p3];

		// switch texture for this triangle if needed (based on first vertex)
		if ( last_texture != v1.TextureIndex() ) {
			if ( last_texture == 0 ) {
				glEnd();
				glEnable(GL_TEXTURE_2D);
				glBindTexture( GL_TEXTURE_2D, v1.TextureIndex() );
				glBegin( GL_TRIANGLES );
// 				std::cout << "enabling [" << x->vertexes[i->p1].TextureIndex() << "]\n";
				}
			else if ( 0 == v1.TextureIndex() ) {
				glEnd();
				glDisable(GL_TEXTURE_2D);
				glBegin( GL_TRIANGLES );
// 				std::cout << "disabling\n";
				}
			else {
				glEnd();
				glBindTexture( GL_TEXTURE_2D, v1.TextureIndex() );
				glBegin( GL_TRIANGLES );
// 				std::cout << "switching to [" << v1.TextureIndex() << "]\n";
				}
			last_texture = v1.TextureIndex();
			}

		if ( !x->light ) { glColor4f(v1.R(),v1.G(),v1.B(),v1.A()  ); }
		glNormal3f( v1.normal.x, v1.normal.y, v1.normal.z );
		glTexCoord2f(v1.U(),v1.V() );
		glVertex3f(v1.X(),v1.Y(),v1.Z() );

		if ( !x->light ) { glColor4f(v2.R(),v2.G(),v2.B(),v2.A()  ); }
		glNormal3f( v2.normal.x, v2.normal.y, v2.normal.z );
		glTexCoord2f(v2.U(),v2.V() );
		glVertex3f(v2.X(),v2.Y(),v2.Z() );

		if ( !x->light ) { glColor4f(v3.R(),v3.G(),v3.B(),v3.A()  ); }
		glNormal3f( v3.normal.x, v3.normal.y, v3.normal.z );
		glTexCoord2f(v3.U(),v3.V() );
		glVertex3f(v3.X(),v3.Y(),v3.Z() );

		}
	glEnd();


// 	// draw normals
// 	glDisable(GL_TEXTURE_2D);
// 	glDisable(GL_LIGHTING);
// 	glColor4f( 0.4f, 0.9f, 0.4f, 1.0f );
// 	glBegin( GL_LINES );
// 	for ( unsigned int i = 0; i < x->vertexes.size(); ++i ) {
// 		glVertex3f( x->vertexes[i].X(), x->vertexes[i].Y(), x->vertexes[i].Z() );
// 		glVertex3f( x->vertexes[i].X() + x->vertexes[i].normal.x, x->vertexes[i].Y() + x->vertexes[i].normal.y, x->vertexes[i].Z() + x->vertexes[i].normal.z );
// 		}
// 	glEnd();

// 	std::cout << "last texture [" << last_texture << "]\n";

	glPopAttrib();
	glPopMatrix();
	}



} // end namespace LGC


