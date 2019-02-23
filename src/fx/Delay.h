#ifndef DELAY_H
#define DELAY_H

#include "../objects/PollingObject.h"

namespace LGC {

class Delay: public PollingObject {
public:

	Delay( PollingObject* object, float time, int layer = 0 );

	~Delay();

	virtual bool Poll();

	virtual void Draw();

protected:
	PollingObject* object;
	float delay_time;
	int layer;
	};


} // end LGC

#endif





