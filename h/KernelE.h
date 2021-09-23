/*
 * KernelE.h
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */

#ifndef H_KERNELE_H_
#define H_KERNELE_H_

#include "event.h"

class PCB;

class KernelEv{
public:
	KernelEv(IVTNo ivtNo);
	~KernelEv();
	void signal();
	void wait();
private:
	//kako da ih spregnem???
	unsigned flag;
	unsigned ThreadIsBlocked;
	volatile PCB* maker;
	IVTNo ivtNo;
	friend class Event;
};


#endif /* H_KERNELE_H_ */
