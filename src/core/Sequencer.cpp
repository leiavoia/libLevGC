

#include "CoreInterface.h"



namespace LGC {





Sequencer::Sequencer( int num_layers ): paused(false) {
	// create all layers before using the Polling system
	for (int i=0; i < num_layers; i++) {
		registry.push_back( PollingEngine() );
		}
	}

Sequencer::~Sequencer() {
	Flush();
	}


/*
Cycle through and Poll() all objects. Poll layers in order with the
back layers first, front layers last. The basic idea here is to iterate
through each layer list, and any objects that return FALSE on their Poll(),
( the "kill me" signal ) are deleted immediately and the next object is polled.
*/
void Sequencer::PollAll( float incoming_time ) {
	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		registry[i].PollAll( incoming_time  );
		}
	}


bool Sequencer::Unlink( PollingObject* obj ) {
 	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		if ( registry[i].Unlink( obj ) ) {
			return true;
			}
		}
	return false;
	}

void Sequencer::DrawAll() {
 	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		registry[i].DrawAll();
		}
	}


void Sequencer::Register( PollingObject* p, int layer ) {
	if (!p) { return; }
	if (layer >= (int)registry.size()) { layer = (int)registry.size()-1; }
	if (layer < 0) { layer = 0; }
	registry[ layer ].Register(p);
	}

void Sequencer::ModSpeed( float x ) {
	if (x < 0) { x = 0;}
 	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		registry[i].ModSpeed(x);
		}
	}

void Sequencer::Pause() {
 	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		registry[i].Freeze();
		}
	paused = true;
	}


void Sequencer::UnPause() {
 	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		registry[i].Thaw();
		}
	paused = false;
	}

void Sequencer::FreezeLayer( unsigned int i ) {
 	if ( i < registry.size() ) {
 		registry[i].Freeze();
 		}
	}


void Sequencer::ThawLayer( unsigned int i ) {
 	if ( i < registry.size() ) {
 		registry[i].Thaw();
 		}
	}

void Sequencer::Flush( int level ) {
 	for (unsigned int i=0; i < registry.size(); i++) { // foreach layer
		registry[i].Flush(level);
		}
	}

void Sequencer::FlushLayer( unsigned int layer, int level ) {
	if ( layer < registry.size() ) {
		registry[layer].Flush(level);
		}
	}


} // end namespace LGC






