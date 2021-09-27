/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 23, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelE.h"
#include <dos.h>

#include "../h/kernel.h"
//KernelEv* IVEntery::events[256] = 0;
KernelEv* IVTEntry::events[256] = {0};
IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine newRoutine){
	lock
	this->ent=ivtNo;
	this->event = 0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(this->ent);
	setvect(this->ent, newRoutine);
#endif
	unlock
}
IVTEntry::~IVTEntry(){
	lock
#ifndef BCC_BLOCK_IGNORE
	setvect(ent, oldRoutine);
#endif
	unlock
}
void IVTEntry::signal(){
	if (events[ent] != 0)
		events[ent]->signal();
}

IVTNo IVTEntry::getNo(){
	return ent;
}


void IVTEntry::callOldRoutine(){
	oldRoutine();
}
