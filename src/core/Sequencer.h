#ifndef SEQUENCER_H
#define SEQUENCER_H

#include <vector>
#include "../objects/PollingObject.h"
#include "../objects/PollingEngine.h"

// enum Layer {
// 	SUB_LAYER=0,
// 	BOTTOM_BACKGROUND_LAYER,
// 	MID_BACKGROUND_LAYER,
// 	TOP_BACKGROUND_LAYER,
// 	BOTTOM_SPRITE_LAYER,
// 	PLAYER_LAYER,
// 	MID_SPRITE_LAYER,
// 	TOP_SPRITE_LAYER,
// 	BOTTOM_FOREGROUND_LAYER,
// 	MID_FOREGROUND_LAYER,
// 	TOP_FOREGROUND_LAYER,
// 	BOTTOM_GUI_LAYER,
// 	TOP_GUI_LAYER,
// 	TRANSITION_LAYER
// 	};




namespace LGC {




class Sequencer {
public:

	Sequencer( int num_layers = 5);
	~Sequencer();

	/** Polls all objects in the registry. This is a per-frame function.*/
	void PollAll( float incoming_time );
	void DrawAll();


	/** Registers an object with the polling engine. */
	void Register( PollingObject* p, int layer = 0 );


	/** */
	void ModSpeed( float x );

	/** */
	void Pause();

	/** */
	void UnPause();

	void FreezeLayer( unsigned int i );
	void ThawLayer( unsigned int i );

	/** */
	bool Paused() { return paused; }

	/**
	level (1) will only erase and delete regular objects.
	level (2) will erase PROTECTED objects and erase+delete DELETE objects.
	level (3) will erase all objects and delete DELETE objects.
	*/
	void Flush( int level=1 );
	void FlushLayer( unsigned int layer, int level=1 );

	bool Unlink( PollingObject* obj );

private:

	// This is a vector of lists. The main vector represents the "layers".
	// The sub-lists are the lists of objects in that layer
	std::vector<  PollingEngine  > registry;

	bool paused;

	};



} // end namespace LGC




#endif




