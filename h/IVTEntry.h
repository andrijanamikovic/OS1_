/*
 * IVTEntery.h
 *
 *  Created on: Sep 23, 2021
 *      Author: OS1
 */

#ifndef H_IVTENTRY_H_
#define H_IVTENTRY_H_

#include "KernelE.h"

typedef void interrupt (*InterruptRoutine)(...);

class IVEntry{
public:
	static KernelEv* events[256];
	IVEntry(IVTNo ivtNo, InterruptRoutine newRoutine);
	~IVEntry();
	void signal();
	IVTNo getNo();
	InterruptRoutine oldRoutine;
	void callOldRoutine();
protected:
	friend class KernelEv;
private:
	IVTNo ent;
	KernelEv* event;
	friend class KernelEv;
	//void callOld();
};

#define PREPAREENTRY(num, callBack) \
void interrupt Routine##num(...); \
IVTEntry Entry##num(num, Routine##num); \
void interrupt Routine##num(...) { \
	Entry##num.signal(); \
	if(callBack==1) \
	Entry##num.callOldRoutine(); \
}

#endif /* H_IVTENTRY_H_ */