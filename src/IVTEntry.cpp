/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 23, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelE.h"
#include <dos.h>
#include "karnel.h"
//KernelEv* IVEntery::events[256] = 0;
KernelEv* IVEntry::events[256] = {0};
IVEntry::IVEntry(IVTNo ivtNo, InterruptRoutine newRoutine){
	lock
	this->ent=ivtNo;
	this->event = 0;
#ifndef BCC_BLOCK_IGNORE
	oldRoutine = getvect(this->ent);
	setvect(this->ent, newRoutine);
#endif
	unlock
}
IVEntry::~IVEntry(){
	lock
#ifndef BCC_BLOCK_IGNORE
	setvect(ent, oldRoutine);
#endif
	unlock
}
void IVEntry::signal(){
	if (events[ent] != 0)
		events[ent]->signal();
}

IVTNo IVEntry::getNo(){
	return ent;
}

void IVEntry::callOldRoutine(){
	oldRoutine();
}
