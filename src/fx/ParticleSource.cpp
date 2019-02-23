
// #include <math.h>
// #include <SDL/SDL.h>
// #include <iostream>
//
// #include <deque>
// #include <vector>
// #include <list>

#include "ParticleSource.h"
#include "../core/CoreInterface.h"
#include "../core/extra_math.h"
#include "../etc/ColorCurve.h"
#include <math.h>

#include <GL/gl.h>





namespace LGC {





ParticleSource::~ParticleSource() {

	}



bool ParticleSource::Poll() {
	return engine.PollAll( TimeFromLast() );
	}

void ParticleSource::Draw() {
	glPushAttrib( GL_COLOR_BUFFER_BIT );
	glBlendFunc(GL_SRC_ALPHA,GL_ONE);
	engine.DrawAll();
	glPopAttrib();
	}




ParticleSourceRedExplosion::ParticleSourceRedExplosion( int x, int y, int num ) {
	for (int n=0; n < int(num/3*2); n++) {
		// slow red gusher shots
		SDL_Color c = { RandomNum(30)+225, RandomNum(10)+30, RandomNum(10)+30 };
		Particle* p = new Particle(x,y,c,RandomNum(300)+1, RandomNum(180)-180, 400, 5, RandomNum(200)+800, 25);
		engine.Register(p);
		}

	for (int n=0; n < int(num/3); n++) {
		// fast white/orange sparks
		SDL_Color c = { RandomNum(50)+205, RandomNum(30)+150, RandomNum(30)+150 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+900, RandomNum(180)-180, 200, 2, RandomNum(200)+100, 20);
		engine.Register(p);
		}
	}





ParticleSourceGreenExplosion::ParticleSourceGreenExplosion( int x, int y, int num ) {
	for (int n=0; n < int(num/3*2); n++) {
		// slow Green gusher shots
		SDL_Color c = { RandomNum(30)+100, RandomNum(40)+215, RandomNum(30)+100 };
		Particle* p = new Particle(x,y,c,RandomNum(300)+1, RandomNum(180)-180, 400, 5, RandomNum(200)+800, 25);
		engine.Register(p);
		}

	for (int n=0; n < int(num/3); n++) {
		// fast white/green sparks
		SDL_Color c = { RandomNum(30)+150, RandomNum(30)+225, RandomNum(30)+150 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+900, RandomNum(180)-180, 200, 2, RandomNum(200)+100, 20);
		engine.Register(p);
		}
	}





ParticleSourceSpark::ParticleSourceSpark( int x, int y, int num ) {
	for (int n=0; n < num; n++) {
		//SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+225 };
		SDL_Color c = { RandomNum(100)+155, RandomNum(100)+155, RandomNum(100)+155 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+1000, RandomNum(360), 0, 2, RandomNum(100)+40, 20);
		engine.Register(p);
		}
	}



ParticleSourceFirework::ParticleSourceFirework( int x, int y, int num ) {
	// fast orange
	for (int n=0; n < num/2-num/20; n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(150)+50, 0 };
		Particle* p = new Particle(x,y,c,RandomNum(3000)+1000, RandomNum(360), 20, 2, RandomNum(600)+600, 25 );
		engine.Register(p);
		}
	// slow orange
	for (int n=0; n < num/2-num/20; n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(150)+50, 0 };
		Particle* p = new Particle(x,y,c,RandomNum(500) + ( (int(RandomNum(2)) < 1) ? 300 : 0), RandomNum(360), 20, RandomNum(2)+3, RandomNum(1000)+1850, 40);
		engine.Register(p);
		}
	// slow hot white
	for (int n=0; n < num/10; n++) {
		SDL_Color c = { RandomNum(56)+200, RandomNum(56)+200, RandomNum(56)+200 };
		Particle* p = new Particle(x,y,c,RandomNum(500)+1, RandomNum(360), 20, 5, RandomNum(1000)+2000, 50.0);
		engine.Register(p);
		}
	}

ParticleSourceWeightlessFirework::ParticleSourceWeightlessFirework( int x, int y, int num ) {
	// fast orange
	for (int n=0; n < num/2-num/20; n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(150)+50, 0 };
		Particle* p = new Particle(x,y,c,RandomNum(3000)+1000, RandomNum(360), 0, 2, RandomNum(600)+600, 25 );
		engine.Register(p);
		}
	// slow orange
	for (int n=0; n < num/2-num/20; n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(150)+50, 0 };
		Particle* p = new Particle(x,y,c,RandomNum(500) + ( (int(RandomNum(2)) < 1) ? 300 : 0), RandomNum(360), 0, RandomNum(2)+3, RandomNum(1000)+1850, 40);
		engine.Register(p);
		}
	// slow hot white
	for (int n=0; n < num/10; n++) {
		SDL_Color c = { RandomNum(56)+200, RandomNum(56)+200, RandomNum(56)+200 };
		Particle* p = new Particle(x,y,c,RandomNum(500)+1, RandomNum(360), 0, 5, RandomNum(1000)+2000, 50.0);
		engine.Register(p);
		}
	}



ParticleSourceMachineExplosion::ParticleSourceMachineExplosion( int x, int y, int num ) {
	for (int n=0; n < int(num/6*2)-2; n++) {
		// fast white sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+225 };
		Particle* p = new Particle(x,y,c,RandomNum(1000)+600, RandomNum(360), 0, 2, RandomNum(280), 40);
		engine.Register(p);
		}
	for (int n=0; n < int(num/6)-2; n++) {
		// slow arcing hot spots
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+225 };
		Particle* p = new Particle(x,y,c,RandomNum(200)+100, RandomNum(360), 10, RandomNum(5)+2, RandomNum(700)+200, 45);
		engine.Register(p);
		}
	for (int n=0; n < int(num/6*2)-2; n++) {
		// slowish rainbow sparks
		SDL_Color c = { RandomNum(255), RandomNum(255), RandomNum(255) };
		Particle* p = new Particle(x,y,c,RandomNum(600)+1, RandomNum(360), 20, RandomNum(4)+2, RandomNum(600), 20);
		engine.Register(p);
		}
	for (int n=0; n < 6; n++) {
		// a coupla long range fast white sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+225 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+700, RandomNum(360), 0, 2, RandomNum(400)+175, 45);
		engine.Register(p);
		}
	}






ParticleSourceMassiveExplosion::ParticleSourceMassiveExplosion( int x, int y, int num ) {
	for (int n=0; n < int(num/6*2); n++) {
		// fast white/yellow sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+150 };
		Particle* p = new Particle(x,y,c,RandomNum(1200)+500, RandomNum(360), 0, 3, RandomNum(175)+100, RandomNum(18)+5);
		engine.Register(p);
		}
	for (int n=0; n < int(num/6); n++) {
		// fast orange sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+150, RandomNum(30)+80 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+300, RandomNum(360), 0, 3, RandomNum(300), 20);
		engine.Register(p);
		}
 	for (int n=0; n < int(num/6*2); n++) {
		// slower red sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+50, RandomNum(30)+50 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+1, RandomNum(360), 0, RandomNum(3)+1, RandomNum(700)+1, 40);
		engine.Register(p);
 		}
 	for (int n=0; n < RandomNum(5)+5; n++) {
		// arcing hot sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+225 };
		Particle* p = new Particle(x,y,c,RandomNum(100)+200, RandomNum(360), 15, RandomNum(5)+1, RandomNum(500)+500, 40);
		engine.Register(p);
 		}

	}







ParticleSourceSuperMassiveExplosion::ParticleSourceSuperMassiveExplosion( int x, int y, int num ) {
	startx = x;
	starty = y;
	basenum = num;
	for (int i=0; i < 6; i++) {
		triggers.push_back(false); // not fired
		}
	IgniteMain( startx+(RandomNum(40)-20), starty+(RandomNum(40)-20), basenum+(RandomNum(50)-25) );
	}

// Custom Poll() function for time-delayed sub explosions :-)
bool ParticleSourceSuperMassiveExplosion::Poll() {

	for (int i=0; i < 6; i++) {
		if (  Time() > BirthTick() + float(250*i)/1000.0  &&  triggers[i] == false ) {
			if ( i==5 || i==4 ) {
				IgniteRing( startx, starty, basenum);
				}
			else {
				IgniteMain( startx+(RandomNum(40)-20), starty+(RandomNum(40)-20), basenum+(RandomNum(50)-25) );
				}
			triggers[i] = true;
			break;
			}
		}

	return ParticleSource::Poll();
	}


void ParticleSourceSuperMassiveExplosion::IgniteMain( int x, int y, int num ) {
	for (int n=0; n < int(num/6*2); n++) {
		// fast white/yellow sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+150 };
		Particle* p = new Particle(x,y,c,RandomNum(1200)+500, RandomNum(360), 0, 3, RandomNum(175), RandomNum(25)+5);
		engine.Register(p);
		}
	for (int n=0; n < int(num/6); n++) {
		// fast orange sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+150, RandomNum(30)+80 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+300, RandomNum(360), 0, 3, RandomNum(300), 20);
		engine.Register(p);
		}
 	for (int n=0; n < int(num/6*2); n++) {
		// slower red sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+50, RandomNum(30)+50 };
		Particle* p = new Particle(x,y,c,RandomNum(400)+1, RandomNum(360), 0, RandomNum(3)+1, RandomNum(700), 40);
		engine.Register(p);
 		}
 	for (int n=0; n < RandomNum(5)+5; n++) {
		// arcing hot sparks
		SDL_Color c = { RandomNum(30)+225, RandomNum(30)+225, RandomNum(30)+225 };
		Particle* p = new Particle(x,y,c,RandomNum(100)+200, RandomNum(360), 15, RandomNum(5)+1, RandomNum(500)+500, 40);
		engine.Register(p);
 		}
	}

void ParticleSourceSuperMassiveExplosion::IgniteRing( int x, int y, int num ) {
	for (int n=0; n < num; n++) {
		// ring of sparks
		SDL_Color c = { RandomNum(60)+195, RandomNum(60)+195, RandomNum(30)+150 };
		Particle* p = new Particle(x,y,c,RandomNum(100)+1000, RandomNum(360), 0, 2, RandomNum(300), 10);
		engine.Register(p);
		}
	}





// ParticleSourceRain::ParticleSourceRain( int x, int y, int num ) {
// 	basenum = num;
// 	for (int n=0; n < num; n++) {
// 		SDL_Color c = { RandomNum(20)+120, RandomNum(20)+120, RandomNum(15)+200 };
// 		Particle* p = new Particle( RandomNum(gamespace->SpaceWidth()),0,c,RandomNum(500)+1500, RandomNum(5)+85, 0, RandomNum(40)+10, 1500, 2 );
// 		engine.Register(p);
// 		}
// 	}
//
//
// // Custom Poll() function for time-delayed rain drops
// bool ParticleSourceRain::Poll( /*int time*/ ) {
// 	for (int n=0; n < basenum; n++) {
// 		SDL_Color c = { RandomNum(20)+120, RandomNum(20)+120, RandomNum(15)+200 };
// 		Particle* p = new Particle( RandomNum(gamespace->SpaceWidth()),0,c,RandomNum(500)+1500, RandomNum(5)+85, 0, RandomNum(40)+10, 1500, 2 );
// 		engine.Register(p);
// 		}
// 	return ParticleSource::Poll( /*time*/ );
// 	}


ParticleSourcePartyBomb::ParticleSourcePartyBomb( int x, int y, int num ) {
	for (int n=0; n < num; n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(100)+155, RandomNum(100)+155 };
		Particle* p = new Particle(x,y,c,RandomNum(1500)+1, RandomNum(360), 50, 3, RandomNum(1000)+900, 8);
		engine.Register(p);
		}
	}


ParticleSourcePopExplosion::ParticleSourcePopExplosion( int x, int y, int num ) {
	// basic party shots
	for (int n=0; n < int(num/3)*2; n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(100)+155, RandomNum(100)+155 };
		Particle* p = new Particle(x,y,c,RandomNum(700)+1, RandomNum(360), 100, RandomNum(4)+1, RandomNum(1200)+50, 15);
		engine.Register(p);
		}
	// super-fast streakers
	for (int n=0; n < int(num/3); n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(100)+155, RandomNum(100)+155 };
		Particle* p = new Particle(x,y,c,RandomNum(1000)+1500, RandomNum(360), 100, RandomNum(2)+1, RandomNum(750)+250, 15);
		engine.Register(p);
		}
	}

/*

ParticleSourceImgExplosion::ParticleSourceImgExplosion( int x, int y, int num ) {
	// basic party shots
	for (int n=0; n < int(num/3)*2; n++) {
		SDL_Color c = { 0,0,0 };
		AnimParticle* p = new AnimParticle(x,y,c,RandomNum(700), RandomNum(360), 100, RandomNum(15)+1, RandomNum(1200)+50, 5);
		engine.Register(p);
		}
	// super-fast streakers
	for (int n=0; n < int(num/3); n++) {
		SDL_Color c = { RandomNum(100)+155, RandomNum(100)+155, RandomNum(100)+155 };
		AnimParticle* p = new AnimParticle(x,y,c,RandomNum(1000)+1500, RandomNum(360), 100, RandomNum(10)+15, RandomNum(750)+250, 1);
		engine.Register(p);
		}
	}





ParticleSourceRicochetExplosion::ParticleSourceRicochetExplosion( int x, int y, int num ) {
	for (int n=0; n < num; n++) {
		// ricochet particles
		SDL_Color c = { 0,0,0 };
		CollisionImgParticle* p = new CollisionImgParticle(x,y,c,RandomNum(100)+700, RandomNum(360), 5, 1, 3000, 2);
		engine.Register(p);
		}
	}*/





ParticleSourceMotionExplosion::ParticleSourceMotionExplosion( int x, int y, int num, float speed, float angle ) {
	// prep motion angle:
	angle = DegToRad( angle );

	for (int n=0; n < num; n++) {

		// particle vars:
		float particle_speed = float(RandomNum(400)+10);
		float particle_angle = DegToRad( (float)RandomNum(360) );

		// particle vector:
		float v1x = cos(particle_angle) * particle_speed;
		float v1y = sin(particle_angle) * particle_speed;

		// motion vector:
		float v2x = cos(angle) * speed;
		float v2y = sin(angle) * speed;

		// vector addition:
		float v3x = v1x + v2x;
		float v3y = v1y + v2y;

		// v3 new speed:
		float new_speed = sqrt( v3x*v3x + v3y*v3y );

		// v3 new angle
		float new_angle = atan( v3y / (v3x + 0.000001) ); // beware x/0

		// adjust new angle for quadrant rotation
		if ( v3x < 0 ) { new_angle += 3.1415926f; }
		else if (  v3x > 0  &&  v3y < 0  ) { new_angle += 3.1415926f * 2; }

		// create particle
		SDL_Color c = { 0,0,0 };
		if ( int( RandomNum(2) ) ) {
			c.r = RandomNum(20)+236; c.g = RandomNum(30)+30; c.b = RandomNum(30)+30;
			}
		else {
			c.r = RandomNum(50)+206; c.g = RandomNum(60)+150; c.b = RandomNum(30)+60;
			}
		Particle* p = new Particle( x,y,c, new_speed, (int)RadToDeg(new_angle), 80, 4, RandomNum(300)+1000, 40);
		engine.Register(p);

		}
	}





ParticleSourceMotionPartyBomb::ParticleSourceMotionPartyBomb( int x, int y, int num, float speed, float angle ) {
	// prep motion angle:
	angle = DegToRad( angle );

	for (int n=0; n < num; n++) {

		// particle vars:
		float particle_speed = float(RandomNum(1500)+1);
		float particle_angle = DegToRad( (float)RandomNum(360) );

		// particle vector:
		float v1x = cos(particle_angle) * particle_speed;
		float v1y = sin(particle_angle) * particle_speed;

		// motion vector:
		float v2x = cos(angle) * speed;
		float v2y = sin(angle) * speed;

		// vector addition:
		float v3x = v1x + v2x;
		float v3y = v1y + v2y;

		// v3 new speed:
		float new_speed = sqrt( v3x*v3x + v3y*v3y );

		// v3 new angle
		float new_angle = atan( v3y / (v3x + 0.000001) ); // beware x/0

		// adjust new angle for quadrant rotation
		if ( v3x < 0 ) { new_angle += 3.1415926f; }
		else if (  v3x > 0  &&  v3y < 0  ) { new_angle += 3.1415926f * 2; }

		// create particle
		SDL_Color c = { RandomNum(100)+155, RandomNum(100)+155, RandomNum(100)+155 };
		Particle* p = new Particle( x,y,c, new_speed, (int)RadToDeg(new_angle), 50, 3, RandomNum(500)+900, 8);
		engine.Register(p);

		}
	}





ParticleSourceSparkler_SparkShower::ParticleSourceSparkler_SparkShower( float x, float y, int num, int spread ) {
// 	float max_dist = sqrt( spread*spread + spread*spread );

	for (int n=0; n < num; n++) {
// 		float tempx = x + (  RandomNum(spread) - (spread / 2.0)  );
// 		float tempy = y + (  RandomNum(spread) - (spread / 2.0)  );
// 		if ( sqrt( tempx*tempx + tempy*tempy ) < max_dist ) { }
//
		ParticleSparkler* p = new ParticleSparkler(
			x + (  RandomNum(spread) - (spread / 2.0)  ),
			y + (  RandomNum(spread) - (spread / 2.0)  ),
			15, // speed
			90, // angle - straight down
			0, // no gravity
			float(1/(RandomNum(256)+1)),
			float(1/(RandomNum(256)+1)),
			float(1/(RandomNum(256)+1)),
			1.0
			);

		engine.Register(p);
		}

	}



ParticleSourceSparkler_GlitterCloud::ParticleSourceSparkler_GlitterCloud( float x, float y, int num ) {
	for (int n=0; n < num; n++) {
		ParticleSparkler* p = new ParticleSparkler(
			x,
			y,
			RandomNum(200), // speed
			RandomNum(361), // angle - straight down
			RandomNum(50), // no gravity
			float(1/(RandomNum(256)+1)),
			float(1/(RandomNum(256)+1)),
			float(1/(RandomNum(256)+1)),
			1.0,
			0.8,
			RandomNum(1000)
			);

		engine.Register(p);
		}
	}





RainbowParticleSource::RainbowParticleSource( float x, float y, int num ) {
	// fast orange
	for (int n=0; n < num/2-num/20; n++) {
		ColorCurve c;
		c.AddPoint( 0.0, 0.7, 0.4, 0.0, 0.6);
		c.AddPoint( 1.0, 1.0, 0.9, 0.7, 1.0);
		RainbowParticle* p = new RainbowParticle(x,y,RandomNum(3000)+1000, RandomNum(360), 20, c, RandomNum(600)+600, 2, 25 );
		engine.Register(p);
		}
	// slow orange
	for (int n=0; n < num/2-num/20; n++) {
		ColorCurve c;
		c.AddPoint( 0.0, 0.7, 0.4, 0.0, 0.8);
		c.AddPoint( 1.0, 0.8, 0.7, 0.3, 1.0);
		RainbowParticle* p = new RainbowParticle(x,y,RandomNum(500) + ( (int(RandomNum(2)) < 1) ? 300 : 0), RandomNum(360), 20, c, RandomNum(1000)+1850, RandomNum(2)+3, 40);
		engine.Register(p);
		}
	// slow hot white
	for (int n=0; n < num/10; n++) {
		ColorCurve c;
		c.AddPoint( 0.0, 1.0, 0.9, 0.7, 0.7);
		c.AddPoint( 1.0, 0.8, 0.7, 0.3, 1.0);
		RainbowParticle* p = new RainbowParticle(x,y,RandomNum(500), RandomNum(360), 20, c, RandomNum(1000)+2000, 5, 50.0);
		engine.Register(p);
		}
	}



} // end namespace LGC


