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

class IVTEntry{
public:
	static IVTEntry* IVT[256];
	IVTEntry(IVTNo ivtNo, InterruptRoutine newRoutine);
	~IVTEntry();
	void signal();
	IVTNo getNo();
	InterruptRoutine oldRoutine;
	static IVTEntry* get(IVTNo ivtNo);
	void callOldRoutine();
	//IVTEntry* getEntry(IVTNo ivtNo);
protected:
	friend class KernelEv;
private:
	IVTNo ent;
	KernelEv* event;
	//void callOld();
};


#endif /* H_IVTENTRY_H_ */
