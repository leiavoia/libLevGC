#include "PollingObject.h"
#include "PollingEngine.h"


namespace LGC {




/** Assigns an engine as an "observer subject" to get times and info from. */
void PollingObject::AssignEngine( PollingEngine* pe ) { 
	engine = pe; 
	birth_tick = pe->Time();
	}
		
void PollingObject::Unlink() {
	if (engine) {
		engine->Unlink(this);
		}
	}



	
} // end namespace LGC	



	
	
	
	
	
	
	
	









