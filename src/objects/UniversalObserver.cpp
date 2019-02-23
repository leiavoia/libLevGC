#include "UniversalObserver.h"


namespace LGC {




UniversalObserver::UniversalObserver() {
	subjects = std::list<UniversalSubject*>();
	}

UniversalObserver::~UniversalObserver() {
	// unregister with all subjects
	for (subject_iter i = subjects.begin(); i != subjects.end(); i++) {
		(*i)->UnregisterObserver( this );
		}
	}	

void UniversalObserver::RegisterAsObserverWith( UniversalSubject* subject ) {
     if (!subject) { return; }
	subject->RegisterObserver( this );
	subjects.push_back( subject );
	}
	
void UniversalObserver::UnregisterAsObserverWith( UniversalSubject* subject ) {
	if (!subject) { return; }
	subject->UnregisterObserver( this );
	for (subject_iter i = subjects.begin(); i != subjects.end(); ) {
		if (*i == subject) { i = subjects.erase(i); } else { i++; }
		}	
	}
	




UniversalSubject::UniversalSubject() {
	observers = std::list<UniversalObserver*>();
	}

UniversalSubject::~UniversalSubject() {
	// notify observers that we are dead
	// NOTE: doing this modifies our own list because UnregisterAsObserverWith()
	// also calls our own UnregisterObserver() via the other object.
	while ( observers.size() != 0 ) {
		(*observers.begin())->UnregisterAsObserverWith( this );
		}
	}

void UniversalSubject::RegisterObserver( UniversalObserver* obs ) {
	if (!obs) { return; }
	observers.push_back( obs );
	}

void UniversalSubject::UnregisterObserver( UniversalObserver* obs ) {
	if (!obs) { return; }
	for (observer_iter i = observers.begin(); i != observers.end(); ) {
		if (*i == obs) { i = observers.erase(i); } else { i++; }
		}
	}

void UniversalSubject::BroadcastSignal( observer_signal sig ) {
	for (observer_iter i = observers.begin(); i != observers.end(); i++) {
		(*i)->ReceiveSignal( sig );
		}
	}





	
} // end namespace LGC	





