

// AKA "The Explosions File" !


#ifndef PARTICLESOURCE_H
#define PARTICLESOURCE_H


#include <math.h>
#include <SDL/SDL.h>
#include <iostream>
#include <vector>


#include "Particle.h"
#include "../objects/PollingObject.h"
#include "../objects/PollingEngine.h"




namespace LGC {




/** */
class ParticleSource: public PollingObject {
public:
	ParticleSource() {;;}

	virtual ~ParticleSource();

	virtual bool Poll();

	virtual void Draw();

protected:
	PollingEngine engine;
	};





/** */
class ParticleSourceRedExplosion: public ParticleSource {
public:
	ParticleSourceRedExplosion( int x, int y, int num=10 );
	virtual ~ParticleSourceRedExplosion() {;;}
	};


/** */
class ParticleSourceGreenExplosion: public ParticleSource {
public:
	ParticleSourceGreenExplosion( int x, int y, int num=10 );
	virtual ~ParticleSourceGreenExplosion() {;;}
	};


/** */
class ParticleSourceSpark: public ParticleSource {
public:
	ParticleSourceSpark( int x, int y, int num=10 );
	virtual ~ParticleSourceSpark() {;;}
	};

/** */
class ParticleSourceFirework: public ParticleSource {
public:
	ParticleSourceFirework( int x, int y, int num=10 );
	virtual ~ParticleSourceFirework() {;;}
	};

/** */
class ParticleSourceWeightlessFirework: public ParticleSource {
public:
	ParticleSourceWeightlessFirework( int x, int y, int num=10 );
	virtual ~ParticleSourceWeightlessFirework() {;;}
	};

/** */
class ParticleSourceMachineExplosion: public ParticleSource {
public:
	ParticleSourceMachineExplosion( int x, int y, int num=150 );
	virtual ~ParticleSourceMachineExplosion() {;;}
	};


/** */
class ParticleSourceMassiveExplosion: public ParticleSource {
public:
	ParticleSourceMassiveExplosion( int x, int y, int num=500 );
	virtual ~ParticleSourceMassiveExplosion() {;;}
	};


/** */
class ParticleSourceSuperMassiveExplosion: public ParticleSource {
public:
	ParticleSourceSuperMassiveExplosion( int x, int y, int num=500 );
	virtual ~ParticleSourceSuperMassiveExplosion() {;;}
	bool Poll();
	void IgniteMain( int x, int y, int num );
	void IgniteRing( int x, int y, int num );
private:
	int startx;
	int starty;
	int basenum;
	std::vector<bool> triggers;
	};


/** */
class ParticleSourcePartyBomb: public ParticleSource {
public:
	ParticleSourcePartyBomb( int x, int y, int num=500 );
	virtual ~ParticleSourcePartyBomb() {;;}
	};


/** */
class ParticleSourcePopExplosion: public ParticleSource {
public:
	ParticleSourcePopExplosion( int x, int y, int num=25 );
	virtual ~ParticleSourcePopExplosion() {;;}
	};



/** */
class ParticleSourceMotionExplosion: public ParticleSource {
public:
	ParticleSourceMotionExplosion( int x, int y, int num=300, float speed=0, float angle=0 );
	virtual ~ParticleSourceMotionExplosion() {;;}
	};



/** */
class ParticleSourceMotionPartyBomb: public ParticleSource {
public:
	ParticleSourceMotionPartyBomb( int x, int y, int num=500, float speed=0, float angle=0 );
	virtual ~ParticleSourceMotionPartyBomb() {;;}
	};




// /** */
// class ParticleSourceRain: public ParticleSource {
// public:
// 	ParticleSourceRain( int x, int y, int num=2 );
// 	virtual ~ParticleSourceRain() {;;}
// 	bool Poll( /*int time*/ );
// 	void Draw();
// private:
// 	int basenum;
// 	//int last_tick;
// 	};

/*

class ParticleSourceImgExplosion: public ParticleSource {
public:
	ParticleSourceImgExplosion( int x, int y, int num=500 );
	virtual ~ParticleSourceImgExplosion() {;;}
	};


class ParticleSourceRicochetExplosion: public ParticleSource {
public:
	ParticleSourceRicochetExplosion( int x, int y, int num=10 );
	virtual ~ParticleSourceRicochetExplosion() {;;}
	};

*/


/** */
class ParticleSourceSparkler_SparkShower: public ParticleSource {
public:
	ParticleSourceSparkler_SparkShower( float x, float y, int num=50, int spread=25 );
	~ParticleSourceSparkler_SparkShower() {;;}
	};


/** */
class ParticleSourceSparkler_GlitterCloud: public ParticleSource {
public:
	ParticleSourceSparkler_GlitterCloud( float x, float y, int num=50 );
	~ParticleSourceSparkler_GlitterCloud() {;;}

	};



/** */
class RainbowParticleSource: public ParticleSource {
public:
	RainbowParticleSource( float x, float y, int num=100 );
	~RainbowParticleSource() {;;}

	};




} // end namespace LGC



#endif





















