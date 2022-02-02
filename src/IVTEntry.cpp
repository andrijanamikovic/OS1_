/*
 * IVTEntry.cpp
 *
 *  Created on: Sep 23, 2021
 *      Author: OS1
 */

#include "IVTEntry.h"
#include "KernelE.h"
#include <dos.h>


#include "syPrintf.h"

#include "../h/kernel.h"
IVTEntry* IVTEntry::IVT[256] = {0};
//KernelEv* IVTEntry::events[256] = {0};
IVTEntry::IVTEntry(IVTNo ivtNo, InterruptRoutine newRoutine){
	lockASM
	this->ent=ivtNo;
	this->event = 0;
#ifndef BCC_BLOCK_IGNORE
	this->oldRoutine = getvect(ivtNo);
	setvect(ivtNo, newRoutine);
#endif
	IVT[ivtNo] = this;
	unlockASM
}
IVTEntry::~IVTEntry(){
	lockASM
#ifndef BCC_BLOCK_IGNORE
	callOldRoutine();
	setvect(ent, oldRoutine);
#endif
	event = 0;
	unlockASM
}
void IVTEntry::signal(){
	if (event != 0)
		event->signal();
}

IVTNo IVTEntry::getNo(){
	return ent;
}

IVTEntry* IVTEntry::get(IVTNo ivtNo){
	return IVT[ivtNo];
}

void IVTEntry::callOldRoutine(){
	lockASM
	oldRoutine();
	unlockASM
}
