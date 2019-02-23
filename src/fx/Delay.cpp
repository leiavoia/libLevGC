#include "Delay.h"
#include "../core/CoreInterface.h"

namespace LGC {


Delay::Delay( PollingObject* object, float time, int layer ) {
	delay_time = time;
	this->object = object;
	this->layer = layer;
	}

Delay::~Delay() {
	}


bool Delay::Poll() {
	if ( TimeFromBirth() >= delay_time ) {
		Core->RegisterObject( object, layer );
		return false;
		}
	return true;
	}

void Delay::Draw() {
	// no drawing
	}


} // end LGC

