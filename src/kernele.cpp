/*
 * kernele.cpp
 *
 *  Created on: Sep 22, 2021
 *      Author: OS1
 */


#include "../h/kernel.h"
#include "KernelE.h"
#include "pcb.h"
#include "IVTEntry.h"
#include "SCHEDULE.H"


KernelEv::KernelEv(IVTNo ivtNo){
	lock
	flag = 0;
	maker = PCB::running;
	ThreadIsBlocked = 0;
	this->ivtNo = ivtNo;
	(IVTEntry::get(ivtNo))->event = this;
	unlock
}

KernelEv::~KernelEv(){
	lock
	if (ThreadIsBlocked){
		maker->blocked = 0;
		Scheduler::put((PCB*)maker);
	}
	(IVTEntry::get(this->ivtNo))->event = 0;
	unlock
}
void KernelEv::signal(){
	lock
	if (!ThreadIsBlocked)
		flag = 1;
	else {
		//flag = 0;
		maker->blocked = 0;
		ThreadIsBlocked = 0;
		Scheduler::put((PCB*)maker);
	}
	unlock
}
void KernelEv::wait(){
	lock
	if (maker!=PCB::running){
		unlock
		return;
	}
		if (flag)
			flag = 0;
		else {
			if (PCB::running->mainFlag){
				unlock
				return;
			}
			ThreadIsBlocked = 1;
			PCB::running->blocked = 1;
			unlock
			dispatch();
		}
	unlock
}

