




#ifndef PARTICLE_H
#define PARTICLE_H

#include <SDL/SDL.h>
#include "../objects/PollingObject.h"
#include "../objects/CollisionObjects.h"
#include "../etc/ColorCurve.h"




namespace LGC {




class Particle: public PollingObject, public PhysicalObject {
public:
	/** */
	Particle(
		int x, 
		int y, 
		SDL_Color c, 
		float speed=100, 
		int angle=360, 
		float gravity=0, 
		int fadeout=300, 
		float lifespan=5, 
		float fade_interval=15.0f
		);

	/** */
	virtual ~Particle() {}

	/** */
	virtual bool Poll();
	
	virtual void Draw();




protected:
	int startx;
	int starty;
	float speed;
	int fade; // fade out steps
	float ls; // lifespan
	float interval; //
	SDL_Color color;
	float vx;
	float vy;
	};


class ParticleSparkler: public PollingObject, public PhysicalObject {
public:

	ParticleSparkler( 
		float x, 
		float y, 
		float speed,
		float angle,
		float gravity,
		float R=1.0, 
		float G=1.0, 
		float B=1.0, 
		float A=1.0, 
		float color_deviation=0.4, 
		float life_span=3
		);
	
	virtual ~ParticleSparkler() {}
	
	virtual bool Poll();
	
	virtual void Draw();
	
private:
	float R, G, B, A, color_deviation, life_span, startx, starty, speed;
	float vx;
	float vy;
	
	};
	

	
	
	
class RainbowParticle: public PollingObject, public PhysicalObject {
public:

	RainbowParticle( 
		float x, 
		float y, 
		float speed,
		float angle,
		float gravity,
		ColorCurve& curve, 
		float life_span=3,
		int num_nodes=3,
		float node_interval=3.0
		);
	
	virtual ~RainbowParticle() {}
	
	virtual bool Poll();
	
	virtual void Draw();
	
private:
	float lifespan, startx, starty, node_interval, speed;
	float vx;
	float vy;
	int nodes;
	ColorCurve cc;
	
	};
	
	
/*
class ImgParticle: virtual public Particle {
public:
	ImgParticle(int x, int y, SDL_Color c, float speed=100, int angle=360, float gravity=0, int fadeout=300, int lifespan=5000, float fade_interval=15.0f);
	virtual ~ImgParticle() {}

	virtual bool Poll(  );

private:
	static SDL_Surface* img;
	};





class AnimParticle: virtual public Particle {
public:
	AnimParticle(int x, int y, SDL_Color c, float speed=100, int angle=360, float gravity=0, int fadeout=300, int lifespan=5000, float fade_interval=15.0f);
	virtual ~AnimParticle() {}

	virtual bool Poll( );

private:
	static std::vector<SDL_Surface*> imgs;
	static bool images_loaded;
	unsigned int index;
	unsigned int frame_delay;
	};





class CollisionImgParticle: virtual public Particle, virtual public CollisionObjectUnowned {
public:
	CollisionImgParticle(int x, int y, SDL_Color c, float speed=100, int angle=360, float gravity=0, int fadeout=300, int lifespan=5000, float fade_interval=15.0f);
	virtual ~CollisionImgParticle() {}

	virtual void IntersectWorld(CollisionObject* obj);

	virtual bool Poll(  );

private:
	static SDL_Surface* img;
	bool killme;
	};


*/

	
} // end namespace LGC	



#endif
