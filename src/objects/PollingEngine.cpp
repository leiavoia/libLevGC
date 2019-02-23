
#include "../utilities/LogManager.h"

#include <algorithm>
#include "PollingEngine.h"
#include "PollingObject.h"


namespace LGC {




PollingEngine::PollingEngine( )  :
	time(0),
	time_from_last(0),
	last_time(0),
	time_mod(1.0),
	frozen(false)
	{}


/*
Cycle through and Poll() all objects. The basic idea here is to iterate
through each layer list, and any objects that return FALSE on their Poll(),
( the "kill me" signal ) are deleted immediately and the next object is polled.
*/
bool PollingEngine::PollAll( float time_diff ) {

	if (frozen) { return false; }

	// recalc time for everybody
	time_diff *= time_mod;
	last_time = time;
	time += time_diff;
	time_from_last = time_diff;

	// poll all objects and kill FALSE returners
	for (std::list<PollingObject*>::iterator iter = registry.begin(); iter != registry.end();) {
		if (  (*iter)->StoppedPolling()  ||  !(*iter)->Poll()  ) {
			if ( (*iter)->Preserved() ) { continue; }
			if ( !(*iter)->Protected() ) { delete *iter; }
			iter = registry.erase(iter);
			}
		else { ++iter; }
		}

	if ( registry.size() == 0 ) { return false; }

	return true;
	}


void PollingEngine::DrawAll() {
	for (std::list<PollingObject*>::iterator iter = registry.begin(); iter != registry.end(); iter++) {
		(*iter)->Draw();
		}
	}

// register an object with the engine. Offset the layer number to a vector index.
void PollingEngine::Register( PollingObject* p ) {
	if (!p) { return; }
	p->AssignEngine( this );
	registry.push_back(p);
	}

bool PollingEngine::Unlink( PollingObject* obj ) {
	// look for the object and remove it
	std::list<PollingObject*>::iterator i = std::find( registry.begin(), registry.end(), obj );
	if ( i != registry.end() ) {
		registry.erase( i );
		return true;
		}
	return false;
	}

PollingEngine::~PollingEngine() {
	Flush();
	if ( registry.size() > 0 ) {
	     LogMgr::Write( LOGMSG_WARNING, "PollingEngine had [%d] protected or preserved objects upon destruction.", registry.size() );
		}
	}


void PollingEngine::Flush( int level ) {
	for (std::list<PollingObject*>::iterator iter = registry.begin(); iter != registry.end();) {
		if ( (*iter)->Preserved() && level == 3 ) {
			iter = registry.erase(iter);
			}
		else if ( (*iter)->Protected() && (level != 1)  ) {
			iter = registry.erase(iter);
			}
		else if ( !(*iter)->Protected() && !(*iter)->Preserved() ) {
			delete *iter;
			iter = registry.erase(iter);
			}
		else { iter++; }
		}
	}





} // end namespace LGC



