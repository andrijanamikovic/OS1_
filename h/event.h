
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

#endif /* H_EVENT_H_ */

