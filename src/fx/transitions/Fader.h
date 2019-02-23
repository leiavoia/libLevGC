#ifndef FADER_H
#define FADER_H

#include "../../objects/PollingObject.h"
#include "../../etc/Interpolator.h"
#include "../../core/GameState.h"
#include "../../core/Texture.h"

//predec
class Sequencer;



namespace LGC {




// Fades the previous screen out to black and the new screen in
class FadeToBlackTransition: public PollingObject {
public:
	// can either fade in or out.
     FadeToBlackTransition(Sequencer* s);
	~FadeToBlackTransition();
	virtual bool Poll();
	virtual void Draw();
	void SetFadeTime( float t );
private:
	Sequencer* sequencer;
	Interpolator curve;
	float fade_time;
	Texture img;
	};



// Crossfades two screens together
class CrossFadeTransition: public PollingObject {
public:
     CrossFadeTransition(Sequencer* s);
	~CrossFadeTransition();
	virtual bool Poll();
	virtual void Draw();
	void SetFadeTime( float t );
protected:
	Sequencer* sequencer;
	Interpolator curve;
	float fade_time;
	Texture img;
	};


class BreakBoxTransition: public CrossFadeTransition {
public:
     BreakBoxTransition(Sequencer* s);
	virtual ~BreakBoxTransition();
	virtual void Draw();
private:
	std::vector<Texture> boxes;
	};

// Fades a screen in from black when there is no previous screen.
class FadeInFromBlackTransition: public PollingObject {
public:
	// can either fade in or out.
     FadeInFromBlackTransition(Sequencer* s);
	~FadeInFromBlackTransition();
	virtual bool Poll();
	virtual void Draw();
	void SetFadeTime( float t );
private:
	Sequencer* sequencer;
	Interpolator curve;
	float fade_time;
	Texture img;
	};




} // end namespace LGC





#endif





