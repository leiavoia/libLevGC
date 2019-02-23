#ifndef POLLINGOBJECT_H
#define POLLINGOBJECT_H

#include "PollingEngine.h"


namespace LGC {





/*
PollingObjects have two main functions: Poll() and Draw().
This is used to update the object and draw it every frame.
PollingObjects are specifically designed to work with
PollingEngines and are a very tightly coupled pair of classes.
The object's Poll and Draw functions are defined by some inherited
class and must be defined by you. The good news is that you can define
those functions however you want. You can put absolutely ANYTHING in
the Poll() function including movement, AI, physics, updating internal
mechanisms, whatever. You can even leave it completely blank. Draw()
obviously is supposed to draw something, but how you do that in your
system is also up to you.

If you need an object to update and draw, simply inherit from this class
and feed the object into a PollingEngine.

See the header for PollingEngine for more details.
*/
class PollingObject {
public:

	enum Management {
		DELETE, // delete and erase the object when it stops polling
		PROTECT, // erase but do not delete
		PRESERVE // do not erase or delete the object ever.
		};

	PollingObject( Management mode = DELETE ) :
		killme(false),
		mode(mode),
		engine(NULL),
		birth_tick(0)
		{ }

	virtual ~PollingObject() {}

	/** Performs an action on polling. Returns FALSE if the object is finished
	    with it's lifespan and should be deleted. Returns TRUE if the object is
	    still active and requires further polling. */
	virtual bool Poll() = 0;

	/** */
	virtual void Draw() = 0;



	/** See above mode for management options */
	inline void SetPollingMode( Management m ) { mode = m; }
	inline void Protect() { mode = PROTECT; }
	inline void Preserve() { mode = PRESERVE; }

	/** Returns the object's protected status. */
	inline bool Protected() { return mode == PROTECT; }

	/** Returns the object's preserved status. */
	inline bool Preserved() { return mode == PRESERVE; }



	/** Forces an object to return FALSE on it's next poll. */
	inline void StopPolling() { killme = true; }

	/** Returns TRUE if an object is flagged with StopPolling() */
	inline bool StoppedPolling() { return killme; }

	/** Assigns an engine as an "observer subject" to get times and info from. */
	void AssignEngine( PollingEngine* pe );

	/** Removes the object from it's current engine, if any. Does not delete the object */
	void Unlink();



	/** The time the object as created */
	inline const float BirthTick() { return birth_tick; }

	/** The total time since the object was created */
	inline const float TimeFromBirth() { return (engine) ? engine->Time() - birth_tick : 0; }

	/** The current universal total time according to the polling engine */
	inline const float Time() { return (engine) ? engine->Time() : 0 ; }

	/** The previous total time according to the polling engine */
	inline const float LastTime() { return (engine) ? engine->LastTime() : 0; }

	/** Time delta since the last frame */
	inline const float TimeFromLast() { return (engine) ? engine->TimeFromLast() : 0; }


private:

	bool killme;

	Management mode;

	PollingEngine* engine;

	float birth_tick;

	};





} // end namespace LGC



#endif












