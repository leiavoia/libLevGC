
#include <math.h>
#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <iostream>

#include "Particle.h"
#include "ParticleSource.h"
#include "../core/Texture.h"
#include "../core/extra_math.h"


#include <GL/gl.h>


// TODO:

// change x,y to xpos,ypos to integrate classes better. particle isn't really moving!




// KILLME
//SDL_Surface* ImgParticle::img = IMG_Load("graphics/ball8px.png");
//SDL_Surface* CollisionImgParticle::img = IMG_Load("graphics/ball8px.png");
//std::vector<SDL_Surface*> AnimParticle::imgs;
//bool AnimParticle::images_loaded = false;





namespace LGC {





Particle::Particle(
	int x,
	int y,
	SDL_Color c,
	float speed,
	int angle,
	float gravity,
	int fadeout,
	float lifespan,
	float fade_interval
	) :PhysicalObject(x,y,gravity) {

	startx = x;
	starty = y;
	color = c;
	fade = fadeout;
	ls = lifespan / 1000;
	interval = fade_interval;
	this->speed = speed;

	// correctly rotate angle - no negs
	if (angle < 0) { angle -= int(angle / 360) * 360; }
	if (angle > 360) { angle -= int(angle / 360) * 360; }

	// precompute the vector by measuring out 1000 pixels
	vx = cos( DegToRad(angle) ) * 1000.0 ;
	vy = sin( DegToRad(angle) ) * 1000.0 ;

	//std::cout << "angle: " << this->angle << "\n";
	}




bool Particle::Poll() {
	// am i dead?
	return ( TimeFromBirth() > ls ) ? false : true ;
	}



void Particle::Draw() {

	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_LINE_STRIP );


	// foreach of the last X pixels in the flow of time
	for (int n=0; n < fade; n++) {

		// FIXME casting

		// get time and number of pixels to add since the last time
		float time_from_birth = TimeFromBirth() - (float(interval*n) / 1000.0);
		float pixels = speed * time_from_birth;

		float ratio_of_1000 = pixels / 1000.0;

		if (pixels < 0) { break; }

		float yoff = pow( Gravity(), time_from_birth );

		float x = startx + (ratio_of_1000 * vx);
		float y = (starty + (ratio_of_1000 * vy)) + yoff;

		// only adjust the particals actual position if this is the head
		// NOTE: technically we are braking the poll/draw partition here
		if (!n) { MoveTo(x,y); }

// 		// OOB - do not draw
// 		if ( x < 1 || y < 1 || x > gamespace->SpaceWidth()-2 || y > gamespace->SpaceHeight()-2 ) {
// 			continue;
// 			}

		glColor4f(
			(GLfloat)color.r/255.0,
			(GLfloat)color.g/255.0,
			(GLfloat)color.b/255.0,
			(   (1.0  - (GLfloat)n/(GLfloat)fade) * (1.2 - (  (TimeFromBirth()) / ls ) )   )
			);
		glVertex2f( x, y);


		}

	glEnd( );
	}





ParticleSparkler::ParticleSparkler(
	float x,
	float y,
	float speed,
	float angle,
	float gravity,
	float R,
	float G,
	float B,
	float A,
	float color_deviation,
	float life_span )

	:PhysicalObject(x,y,gravity),
	R(R),
	G(G),
	B(G),
	A(A),
	color_deviation(color_deviation),
	life_span(life_span/1000),
	startx(x),
	starty(y),
	speed(speed)

	{


	// correctly rotate angle - no negs
	if (angle < 0) { angle -= int(angle / 360) * 360; }
	if (angle > 360) { angle -= int(angle / 360) * 360; }

	// precompute the vector by measuring out 1000 pixels
	vx = cos( DegToRad(angle) ) * 1000.0 ;
	vy = sin( DegToRad(angle) ) * 1000.0 ;

	}

bool ParticleSparkler::Poll() {

	// MOVE ----------------


	// am i dead?
	if ( TimeFromBirth() > life_span ) {  return false; }

	float pixels = speed * TimeFromBirth();

	float ratio_of_1000 = pixels / 1000.0;

	float yoff = pow( Gravity(), TimeFromBirth() );
	float x = startx + (ratio_of_1000 * vx);
	float y = (starty + (ratio_of_1000 * vy)) + yoff;

	MoveTo(x,y);

	return true;
	}

void ParticleSparkler::Draw() {

	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_POINTS );

		glColor4f(
			(float)RandomNum(1000) / 1000.0f,
			(float)RandomNum(1000) / 1000.0f,
			(float)RandomNum(1000) / 1000.0f,
			1.0 //(1.2 - (  (float(Time()) - float(birth_tick))  / life_span ) )

			//1,1,1,1

// 			R + (  ( float( RandomNum( int(color_deviation*1000.0) )+1.0 ) / 1000.0 )  - (color_deviation / 2.0)  ),
// 			G + (  ( float( RandomNum( int(color_deviation*1000.0) )+1.0 ) / 1000.0 )  - (color_deviation / 2.0)  ),
// 			B + (  ( float( RandomNum( int(color_deviation*1000.0) )+1.0 ) / 1000.0 )  - (color_deviation / 2.0)  ),
// 			A + (  ( float( RandomNum( int(color_deviation*1000.0) )+1.0 ) / 1000.0 )  - (color_deviation / 2.0)  )
// 				* (1.2 - (  (float(Time()) - float(birth_tick))  / life_span ) )
			);

		glVertex2f( Xpos(), Ypos() );

	glEnd();
	}











RainbowParticle::RainbowParticle(
	float x,
	float y,
	float speed,
	float angle,
	float gravity,
	ColorCurve& curve,
	float life_span,
	int num_nodes,
	float node_interval
	)

	:

	PhysicalObject(x,y,gravity),
	lifespan(life_span/1000),
	startx(x),
	starty(y),
	node_interval(node_interval),
	speed(speed),
	nodes(num_nodes)

	{

	cc = curve;

	// correctly rotate angle - no negs
	if (angle < 0) { angle -= int(angle / 360) * 360; }
	if (angle > 360) { angle -= int(angle / 360) * 360; }

	// precompute the vector by measuring out 1000 pixels
	vx = cos( DegToRad(angle) ) * 1000.0 ;
	vy = sin( DegToRad(angle) ) * 1000.0 ;
	}




bool RainbowParticle::Poll() {
	// am i dead?
	return ( TimeFromBirth() > lifespan ) ? false : true ;
	}



void RainbowParticle::Draw() {

	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_LINE_STRIP );


	// foreach of the last X pixels in the flow of time
	for (int n=0; n < nodes; n++) {

		// FIXME casting

		// get time and number of pixels to add since the last time
		float time_from_birth = TimeFromBirth() - float(node_interval*n);
		float pixels = speed * time_from_birth;

		float ratio_of_1000 = pixels / 1000.0;

		if (pixels < 0) { break; }

		float yoff = pow( Gravity(), time_from_birth );
		float x = startx + (ratio_of_1000 * vx);
		float y = (starty + (ratio_of_1000 * vy)) + yoff;

		// only adjust the particals actual position if this is the head
		// NOTE: technically we are braking the poll/draw partition here
		if (!n) { MoveTo(x,y); }

// 		// OOB - do not draw
// 		if ( x < 1 || y < 1 || x > gamespace->SpaceWidth()-2 || y > gamespace->SpaceHeight()-2 ) {
// 			continue;
// 			}

		float age = time_from_birth / lifespan;
		float point = float(n) / float(nodes);
		glColor4f(
			cc.R( point ),
			cc.G( point ),
			cc.B( point ),
			cc.A( 1.0 - age )
			);
		glVertex2f( x, y);


		}

	glEnd( );
	}





/*






ImgParticle::ImgParticle(int x, int y, SDL_Color c, float speed, int angle, float gravity, int fadeout, int lifespan, float fade_interval)
	: Particle(x,y,c,speed,angle,gravity,fadeout,lifespan,fade_interval) {
	;;
	}

bool ImgParticle::Poll(  ) {
	// get time and number of pixels to add since the last time
	float time_from_birth = time - birth_tick;
	int pixels =  int( (int)speed * (int)time_from_birth ) / 1000;

	// am i dead?
	if ( time_from_birth > ls ) {
		return false;
		}

	if (pixels < 0) { return false; }

	// basic right-angle trig
	int xoff = int( cos( DegToRad(angle) ) * pixels );
	int yoff = int( sin( DegToRad(angle) ) * pixels );

	// adjust for "gravity"
	yoff += int (  pow( (float)weight, time_from_birth/1000.0 )  );

	// move position
	xpos = float(startx + xoff);
	ypos = float(starty + yoff);

	// OOB
	if ( xpos < 1 || ypos < 1 || xpos > gamespace->SpaceWidth()-2 || ypos > gamespace->SpaceHeight()-2 ) {
		return false;
		}

	// draw
	//DrawImage(img,x,y);
	//gamespace->DrawImageToSpace(img, (int)xpos, (int)ypos);

	return true;
	}





AnimParticle::AnimParticle(int x, int y, SDL_Color c, float speed, int angle, float gravity, int fadeout, int lifespan, float fade_interval)
	: Particle(x,y,c,speed,angle,gravity,fadeout,lifespan,fade_interval) {
	index = rand()%6;
	frame_delay = 0;
	if (!images_loaded) {
		imgs.push_back( IMG_Load("graphics/ball8px-1.png") );
		imgs.push_back( IMG_Load("graphics/ball8px-2.png") );
		imgs.push_back( IMG_Load("graphics/ball8px-3.png") );
		imgs.push_back( IMG_Load("graphics/ball8px-4.png") );
		imgs.push_back( IMG_Load("graphics/ball8px-5.png") );
		imgs.push_back( IMG_Load("graphics/ball8px-2.png") );

// 		imgs.push_back( IMG_Load("graphics/flotsom8px-1.png") );
// 		imgs.push_back( IMG_Load("graphics/flotsom8px-2.png") );
// 		imgs.push_back( IMG_Load("graphics/flotsom8px-3.png") );
// 		imgs.push_back( IMG_Load("graphics/flotsom8px-4.png") );

// 		imgs.push_back( IMG_Load("graphics/flotsom-B-8px-1.png") );
// 		imgs.push_back( IMG_Load("graphics/flotsom-B-8px-2.png") );
// 		imgs.push_back( IMG_Load("graphics/flotsom-B-8px-3.png") );
// 		imgs.push_back( IMG_Load("graphics/flotsom-B-8px-4.png") );

		images_loaded = true;
		}
	}

bool AnimParticle::Poll(  ) {
	// get time and number of pixels to add since the last time
	float time_from_birth = time - birth_tick;
	int pixels =  int( (int)speed * (int)time_from_birth ) / 1000;

	// am i dead?
	if ( time_from_birth > ls ) {
		return false;
		}

	if (pixels < 0) { return false; }

	// basic right-angle trig
	int xoff = int( cos( DegToRad(angle) ) * pixels );
	int yoff = int( sin( DegToRad(angle) ) * pixels );

	// adjust for "gravity"
	yoff += int (  pow( (float)weight, time_from_birth/1000.0 )  );

	// move position
	xpos = float(startx + xoff);
	ypos = float(starty + yoff);

	// OOB
	if ( xpos < 1 || ypos < 1 || xpos > gamespace->SpaceWidth()-2 || ypos > gamespace->SpaceHeight()-2 ) {
		return false;
		}

	// draw
	//DrawImage(imgs[index],x,y);
	//gamespace->DrawImageToSpace( imgs[index], (int)xpos, (int)ypos );


	if ( ++frame_delay > 5 ) { ++index; frame_delay = 0; }
	if ( index >= imgs.size() ) { index = 0; }

	return true;
	}





CollisionImgParticle::CollisionImgParticle(int x, int y, SDL_Color c, float speed, int angle, float gravity, int fadeout, int lifespan, float fade_interval)
	: Particle(x,y,c,speed,angle,gravity,fadeout,lifespan,fade_interval) {
	col_box.x1 = x;
	col_box.y1 = y;
	col_box.x2 = x+8;
	col_box.y2 = y+8;
	killme = false;
	//std::cout << "> particle created \n";
	}

bool CollisionImgParticle::Poll(  ) {

	if (killme) { return false; }

	// get time and number of pixels to add since the last time
	float time_from_birth = time - birth_tick;
	int pixels =  int( (int)speed * (int)time_from_birth ) / 1000;

	// am i dead?
	if ( time_from_birth > ls ) {
		return false;
		}

	if (pixels < 0) { return false; }

	// basic right-angle trig
	int xoff = int( cos( DegToRad(angle) ) * pixels );
	int yoff = int( sin( DegToRad(angle) ) * pixels );

	// adjust for "gravity"
	yoff += int (  pow( (float)weight, time_from_birth/1000.0 )  );
	xpos = float(startx + xoff);
	ypos = float(starty + yoff);

	// OOB
	if ( xpos < 1 || ypos < 1 || xpos > gamespace->SpaceWidth()-2 || ypos > gamespace->SpaceHeight()-2 ) {
		return false;
		}

	// update location
	col_box.x1 = int(xpos);
	col_box.y1 = int(ypos);
	col_box.x2 = int(xpos+8);
	col_box.y2 = int(ypos+8);

	//std::cout << col_box.x1 << "," << col_box.y1 << " -> " << col_box.x2 << "," << col_box.y2 << "\n";

	glBindTexture( GL_TEXTURE_2D, 0 );
	glBegin( GL_POINT );
		glColor4f( 1.0, 1.0, 1.0, 0.0 ); //(GLfloat)color.r/255.0, (GLfloat)color.g/255.0, (GLfloat)color.b/255.0
 		glVertex3f( xpos, ypos, 0 );
	glEnd();

	return true;
	}

void CollisionImgParticle::IntersectWorld(CollisionObject* obj) {
	Core->RegisterObject(  new ParticleSourceMachineExplosion( (int)xpos, (int)ypos, 100 )  );
	killme = true;
 	}*/


} // end namespace LGC


