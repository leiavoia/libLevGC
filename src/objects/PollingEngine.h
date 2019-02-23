#ifndef POLLINGENGINE_H
#define POLLINGENGINE_H

#include <list>

/**

INSTRUCTIONS FOR USE:

1) Create an object. It should be a class the inherits from PollingObject:

		SomeClass* thing = new SomeClass( params );

2) Register the object with the polling engine. This makes it update every frame:

		PollingEngine.Register( thing );

3) Make sure you poll the engine every frame. Somewhere in
   your game loop, you need to call:

   		PollingEngine.PollAll( time_delta  );
		PollingEngine.DrawAll();

That's it! "Thing" will now update every frame and draw too. When doing
the polling, you need to supply the engine with a float that represents
the time past from the last frame. The engine will do all the number crunching
on it's own.


Management:

Because there will be some special objects (like the player, GUI elements, etc)
that need finer manual control, there are three management modes provided
(documented below). If you want the engine to update the object but do not want it
deleted when it returns false, you can set the object to be either
PROTECTED or PRESERVED.


Important things to remember:

- The polling engine only accepts pointers to objects.
  It does this to preserve polymorphism. Beware that the engine will
  attempt to delete the object if it returns false ("is finished").
  If the object is not dynamically allocated with the NEW keyword, it
  will segfault on deletion.

- You should override the Poll() function for any inherited object. This
  Does the actual updating. What you do in Poll() is completely up to you.
  It makes no difference to the engine.

- Polling objects return TRUE from Poll() if they are still alive. They
  Return FALSE if they are finished and need to be deleted. DO NOT
  DELETE OBJECTS YOURSELF. The engine will do that for you.

- Most of the time, once the engine is set up, you'll want to follow a
  "fire and forget" policy for adding new objects to the engine. Suppose
  you want to fire a missile. You would do that like so:

  		PollingEngine.Register(   new Missile( whatever )   );

  Then just forget about it! It will take care of itself after that. You
  do not need to keep track of "Missile" in any way, shape, or form.

- You can register ANYTHING that needs time based updating. It need not
  be a graphical object. It can be a timer, a player or enemy, a world
  trigger, a time bomb, anything with delayed reactions, animations, etc.
  What you do in the Poll() function is completely up to you.

*/


namespace LGC {




// pre-dec
class PollingObject;




class PollingEngine {
public:

	PollingEngine();

	virtual ~PollingEngine();

	/** Performs a Poll() on all associated objects */
	virtual bool PollAll( float time_diff );

	virtual void DrawAll();


	/** Registers an object with the polling engine.
	    NOTE: To "unregister", just have the object return FALSE in it's Poll() function.
	    It will be erased and deleted on the engine's next polling cycle.
	    WARNING: Objects WILL be deleted, so they must be pointers to dynamically allocated memory.
	*/
	void Register( PollingObject* p );

	/** Forceably removes an object from the engine.
	    This is usefull if you need to specifically remove the object from it's current list,
	    such as when you want to transfer an object from one engine to another without
	    disrupting service to the object itself. Unlink() will work on any object,
	    regardless of it's protection status. Unlink does NOT *delete* memory.
	*/
	bool Unlink( PollingObject* obj );

	/** */
	void Freeze() { frozen = true; }

	/** */
	void Thaw() { frozen = false; }

	/**
	level (1) will only erase and delete regular objects.
	level (2) will erase PROTECTED objects and erase+delete DELETE objects.
	level (3) will erase all objects and delete DELETE objects.
	*/
	void Flush( int level=1 );

	/** */
	void ModSpeed( float x ) { time_mod = x; }



	/** The current total time */
	inline const float Time() { return time; }

	/** The previous total time */
	inline const float LastTime() { return last_time; }

	/** Time delta since the last frame */
	inline const float TimeFromLast() { return time_from_last; }




private:

	std::list<PollingObject*> registry;

	// the current frame time
	float time;

	// difference between this frame and the last frame
	float time_from_last;

	// the last frame time
	float last_time;

	// time modifier ( 0.0 = frozen, 1.0 = normal time, 1+ = speedup)
	float time_mod;

	bool frozen;


	};





} // end namespace LGC




#endif




