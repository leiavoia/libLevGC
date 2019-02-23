#ifndef UNIVERSAL_OBSERVER_H
#define UNIVERSAL_OBSERVER_H

#include <list>


namespace LGC {




/*
Observer signals are signals that are broadcast by a Subject 
and received by an Observer. The signal reflects the status 
or change in status of the Subject, not the Observer. 
The Observer may deal with the Subject's broadcast signals 
however it wishes. Observers respond to this "stimulus" in 
a generic fashion. When they receive a signal, they perform 
an action.  The signal is not tied to a specific object but 
the response to that signal may be.
*/

/*
NOTE: if you want a more flexible system, at the expense of being
a bit slower, you can use strings instead of enums.
*/

enum observer_signal {

	// PLAYER SIGNALS
	SIGNAL_UP = 0,
	SIGNAL_DOWN,
	SIGNAL_LEFT,
	SIGNAL_RIGHT,
	SIGNAL_FIRE,
	SIGNAL_CHARGE,
	SIGNAL_CHARGE_RELEASE,
	SIGNAL_JUMP,
	SIGNAL_DEATH,
	SIGNAL_DAMAGE,
	SIGNAL_CONTACT,
	SIGNAL_MOVE, // doubles as a GUI signal
	SIGNAL_STOP,
	
	// GUI SIGNALS
	SIGNAL_BUTTONCLICK,
	SIGNAL_MOUSECLICK,
	SIGNAL_FOCUS,
	
	// ANYTHING ELSE 
	SIGNAL_CUSTOM_1 = 101,
	SIGNAL_CUSTOM_2 = 102,
	SIGNAL_CUSTOM_3 = 103,
	SIGNAL_CUSTOM_4 = 104,
	SIGNAL_CUSTOM_5 = 105,
	SIGNAL_CUSTOM_6 = 106,
	SIGNAL_CUSTOM_7 = 107,
	SIGNAL_CUSTOM_8 = 108,
	SIGNAL_CUSTOM_9 = 109
	
	};
	
	
	
	
	
// predec
class UniversalSubject;



class UniversalObserver {

public:

	UniversalObserver();
	
	virtual ~UniversalObserver(); // report unregister to subject
	
	// This is where all the real signal-response meat goes:
	virtual void ReceiveSignal( observer_signal sig ) = 0;
	
	// Use these functions so that the class will record the Subject. 
	// When we die, we automatically unregister ourself.
	void RegisterAsObserverWith( UniversalSubject* subject );
	void UnregisterAsObserverWith( UniversalSubject* subject );
	
private:

	std::list<UniversalSubject*> subjects;
	
	typedef std::list<UniversalSubject*>::iterator subject_iter;
	
	};

	
	
		
class UniversalSubject {

public:

	UniversalSubject();
	
	virtual ~UniversalSubject(); 
	
	void RegisterObserver( UniversalObserver* obs );
	
	void UnregisterObserver( UniversalObserver* obs );
	
protected:

	void BroadcastSignal( observer_signal sig );
	
private:

	std::list<UniversalObserver*> observers;
	
	typedef std::list<UniversalObserver*>::iterator observer_iter;
	
	};
	
	


	
} // end namespace LGC	



	
#endif





