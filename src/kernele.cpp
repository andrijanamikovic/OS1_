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
	IVTEntry::events[ivtNo] = this;
	unlock
}

KernelEv::~KernelEv(){
	lock
	IVTEntry::events[ivtNo] = 0;
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
		Karnel::inScheduler++;
		Scheduler::put((PCB*)maker);
	}
	unlock
}
void KernelEv::wait(){	//mozda ti tu treba lock i unlock
	lock
	if (maker!=PCB::running) return;
		if (flag)
			flag = 0;
		else {
			ThreadIsBlocked = 1;
			PCB::running->blocked = 1;
			dispatch();
		}
	unlock
}

