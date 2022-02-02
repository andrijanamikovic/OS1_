/*
 * event.cpp
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */


#include "event.h"

#include "../h/kernel.h"
#include "KernelE.h"

Event::Event (IVTNo ivtNo){
	//lock
	myImpl = new KernelEv(ivtNo);
	//unlock
}
Event::~Event (){
	delete myImpl;
	myImpl = 0;
}

void Event::wait(){
	if (myImpl == 0) return;
	myImpl->wait();
}

void Event::signal(){
	if (myImpl == 0) return;
	myImpl->signal();
}

