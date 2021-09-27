
// File: event.h
#ifndef _event_h_
#define _event_h_


typedef unsigned char IVTNo;

//class IVEntery;
class KernelEv;

class Event {
public:
	Event (IVTNo ivtNo);
	~Event ();
	void wait ();

protected:
	friend class KernelEv;
	void signal(); // can call KernelEv
private:
	KernelEv* myImpl;
};

#include "IVTEntry.h"

#define PREPAREENTRY(numEntry, callOld)\
void interrupt inter##numEntry(...);\
IVTEntry newEntry##numEntry(numEntry, inter##numEntry);\
void interrupt inter##numEntry(...) {\
	newEntry##numEntry.signal();\
	if (callOld)\
		newEntry##numEntry.callOldRoutine();\
}

#endif /* H_EVENT_H_ */




