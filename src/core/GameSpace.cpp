#include "GameSpace.h"


#include "Texture.h"
#include "ResourceMgr.h"
#include "../fx/backgrounds/BackgroundImage.h"
#include "CoreInterface.h"
#include <GL/gl.h>
// #include "../objects/TestCritters.h"
#include "../objects/CollisionObjects.h"



namespace LGC {




int GameSpace::screen_w = 800; 
int GameSpace::screen_h = 600;



GameSpace::GameSpace( std::string name ): xoff(0), yoff(0) {
		space_width = 3000;
		space_height = 2000;
		
		// FIXME do we really need these?
		xoff_limit = space_width - screen_w -1;
		yoff_limit = space_height - screen_h -1;
		
		// partition the quadtree collision space:
		CollisionObject::ResetQuadtree( space_width, space_height, 250 );
		
		// start a new one global position
		glPushMatrix(); 
		glLoadIdentity();	
		

// 	if ( name == "ROOM1" ) {	
// 		space_width = 3000;
// 		space_height = 2000;
// 		
// 		// FIXME do we really need these?
// 		xoff_limit = space_width - screen_w -1;
// 		yoff_limit = space_height - screen_h -1;
// 		
// 		// partition the quadtree collision space:
// 		CollisionObject::ResetQuadtree( space_width, space_height, 250 );
// 		
// 		// start a new one global position
// 		glPushMatrix(); 
// 		glLoadIdentity();	
// 		
// 		// Reserve resources		
// 		reglist.push_back( "BasicLaserShot" );
// 		reglist.push_back( "LightSphere" );
// 		reglist.push_back( "LightFlare" );
// 		reglist.push_back( "PlayerFace" );
// 		reglist.push_back( "WorldBlock" );
// 		//reglist.push_back( "MovingBlock" );
// 		reglist.push_back( "Explosion1" );
// 		reglist.push_back( "GameSpaceMap" );
// 		reglist.push_back( "Player" );
// 		reglist.push_back( "SlantyThingy" );
// 		reglist.push_back( "Collectable" );
// 		RM->ReserveResourcesFor( reglist );
// 	
// 		// build world
// 		Core->RegisterObject(  new WorldBlock( 10,20,2760,20 ), MID_SPRITE_LAYER   );
// 		Core->RegisterObject(  new WorldBlock( 2770,20,20,1070 ), MID_SPRITE_LAYER  );
// 		Core->RegisterObject(  new WorldBlock( 10,1070,2760,20 ), MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 20,670,1000,20 ), MID_SPRITE_LAYER  );	
// 		Core->RegisterObject(  new WorldBlock( 1400,670,960,20 ), MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 1000,900,500,20 ), MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 10,20,20,1050 ) , MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 200,200,300,20 ), MID_SPRITE_LAYER   ); 	
// 		Core->RegisterObject(  new WorldBlock( 500,450,200,20 ), MID_SPRITE_LAYER   );
// 	
// 		// create bg images:
// 		BackgroundImage* bg1 = new BackgroundImage("test_bg", this, screen_w, screen_h );
// 		bg1->SetParallax(1.0);
// 		bgs.push_back( bg1 );
// 		Core->RegisterObject( bg1, BOTTOM_BACKGROUND_LAYER );	
// 		
// 		}
// 			
// 	else if ( name == "ROOM2" ) {
// 		space_width = 3000;
// 		space_height = 2000;
// 		
// 		// FIXME do we really need these?
// 		xoff_limit = space_width - screen_w -1;
// 		yoff_limit = space_height - screen_h -1;
// 		
// 		// partition the quadtree collision space:
// 		CollisionObject::ResetQuadtree( space_width, space_height, 250 );
// 		
// 		// start a new one global position
// 		glPushMatrix(); 
// 		glLoadIdentity();	
// 		
// 		// Reserve resources		
// 		reglist.push_back( "BasicLaserShot" );
// 		reglist.push_back( "LightSphere" );
// 		reglist.push_back( "LightFlare" );
// 		reglist.push_back( "PlayerFace" );
// 		reglist.push_back( "WorldBlock" );
// 		//reglist.push_back( "MovingBlock" );
// 		reglist.push_back( "Explosion1" );
// 		reglist.push_back( "GameSpaceMap" );
// 		reglist.push_back( "Player" );
// 		reglist.push_back( "SlantyThingy" );
// 		reglist.push_back( "Collectable" );
// 		RM->ReserveResourcesFor( reglist );
// 	
// 		// build world
// 		Core->RegisterObject(  new WorldBlock( 10,20,2760,20 ), MID_SPRITE_LAYER   );
// 		Core->RegisterObject(  new WorldBlock( 2770,20,20,1070 ), MID_SPRITE_LAYER  );
// 		Core->RegisterObject(  new WorldBlock( 10,1070,2760,20 ), MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 20,670,1000,20 ), MID_SPRITE_LAYER  );	
// 		Core->RegisterObject(  new WorldBlock( 1400,670,960,20 ), MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 1000,900,500,20 ), MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 10,20,20,1050 ) , MID_SPRITE_LAYER  );		
// 		Core->RegisterObject(  new WorldBlock( 200,200,300,20 ), MID_SPRITE_LAYER   ); 	
// 		Core->RegisterObject(  new WorldBlock( 500,450,200,20 ), MID_SPRITE_LAYER   );
// 	
// 		// create bg images:
// 		BackgroundImage* bg1 = new BackgroundImage( "test_bg2", this, screen_w, screen_h );
// 		bg1->SetParallax(1.0);
// 		bgs.push_back( bg1 );
// 		Core->RegisterObject( bg1, BOTTOM_BACKGROUND_LAYER );			
// 		}
	}





GameSpace::~GameSpace() {
	// mark all background images for destruction
	for (unsigned int i=0; 1 < bgs.size(); i++) {
		bgs[i]->StopPolling();
		}

	// release resources
	RM->DumpResourcesFor( reglist );
	}



void GameSpace::Move(int x, int y) {
	xoff += x;
	yoff += y;
	if (xoff > xoff_limit) { xoff = xoff_limit; }
	else if (xoff < 0) { xoff = 0; }
	if (yoff > yoff_limit) { yoff = yoff_limit; }
	else if (yoff < 0) { yoff = 0; }

	RecalcOffsetMatrix();
	}



void GameSpace::MoveTo( int x, int y ) {
	xoff = x;
	yoff = y;
	if (xoff > xoff_limit) { xoff = xoff_limit; }
	else if (xoff < 0) { xoff = 0; }
	if (yoff > yoff_limit) { yoff = yoff_limit; }
	else if (yoff < 0) { yoff = 0; }

	RecalcOffsetMatrix();
	}


void GameSpace::CenterOn( float x, float y ) {
	xoff = int(x - screen_w /2);
	yoff = int(y - screen_h /2);
	
	if (xoff > xoff_limit) { xoff = xoff_limit; }
	else if (xoff < 0) { xoff = 0; }
	if (yoff > yoff_limit) { yoff = yoff_limit; }
	else if (yoff < 0) { yoff = 0; }

	RecalcOffsetMatrix();	
	}




void GameSpace::RecalcOffsetMatrix() {
	glPopMatrix(); // dump the old one
	glPushMatrix(); // get a new one
	glLoadIdentity();
	glTranslatef( -(GLfloat)xoff, -(GLfloat)yoff, 0 );
	Texture::SetScreenOffset( xoff, yoff );
	}







bool GameSpace::Touches( int x, int y, int w, int h ) {
	if (
		x > xoff+screen_w   || // left of object to right of screen area
		x+w < xoff 		|| // right of object to left of screen area
		y > yoff+screen_h   || // bottom of object to top of screen area
		y+h < yoff  		   // top of object to bottom of screen area
		) { return false; }
	return true;
	}

bool GameSpace::Touches( float x, float y, float w, float h ) {
	if (
		x > xoff+screen_w   || // left of object to right of screen area
		x+w < xoff 		|| // right of object to left of screen area
		y > yoff+screen_h   || // bottom of object to top of screen area
		y+h < yoff  		   // top of object to bottom of screen area
		) { return false; }
	return true;
	}

bool GameSpace::Touches( CollisionObject* obj ) {
	return Touches(
			obj->XMin(),
			obj->YMin(),
			obj->XMax(),
			obj->YMax()
			);
	}

	
	
	
} // end namespace LGC	

