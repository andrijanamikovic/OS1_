
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

#define PREPAREENTRY(num, callBack) \
void interrupt Routine##num(...); \
IVTEntry Entry##num(num, Routine##num); \
void interrupt Routine##num(...) { \
	Entry##num.signal(); \
	if(callBack==1) \
	Entry##num.callOldRoutine(); \
}

#endif /* H_EVENT_H_ */

