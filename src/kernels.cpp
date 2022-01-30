/*
 * kernels.cpp
 *
 *  Created on: Sep 21, 2021
 *      Author: OS1
 */

#include "../h/kernel.h"
#include "KernelS.h"
#include "pcb.h"
#include "AList.h"
#include "SCHEDULE.H"
#include "syPrintf.h"

volatile  List* KernelSem::allSem = new List();

KernelSem::KernelSem(int init){
	lock
	Listblocked = new List();
	value = init;
	allSem->put(this);
	unlock
}
KernelSem::~KernelSem(){
	lock
	allSem->remove((void*)this);
	Listblocked->unblock();
	delete Listblocked;
	value = 0;
	unlock
}

int  KernelSem::val () const{
	return value;
}

int KernelSem::wait (Time maxTimeToWait){
	lock
	value--;
		if (value<0){
			PCB::running->waitingTime = maxTimeToWait;
			if (maxTimeToWait == 0){
				 PCB::running->NotwaitingSemaphore = 1;
			} else {
				PCB::running->NotwaitingSemaphore = 0;
			}
			block();
			if ( PCB::running->NotwaitingSemaphore == 0 && PCB::running->waitingTime == 0){
				unlock
				return 0;
			}
		}
		PCB::running->waitingTime = 0;
		unlock
		return  1;
}

void KernelSem::signal(){
	lock
		if (++value <= 0){
				unblock();
		}
	unlock
}

void KernelSem::block(){
	PCB::running->blocked = 1;
	if (Listblocked!=0)
		Listblocked->put((void*)PCB::running);
	dispatch();
}

void KernelSem::unblock(){
	lock
	PCB* temp =(PCB*)Listblocked->pop();
	if (temp!=0 && !temp->loopFlag){
		temp->blocked = 0;
		Scheduler::put(temp);
	}
	unlock
}

void KernelSem::update(){
	lock
	if (allSem == 0) return;
	List::Elem* temp = allSem->first;
	while (temp!=0){
		((KernelSem*)(temp->pcb))->updateSemaphore();
		temp = temp->next;
	}
	unlock
}

void KernelSem::updateSemaphore(){
		List::Elem* temp2 =this->Listblocked->first;
		while (temp2 != 0){
			if (!((PCB*)(temp2->pcb))->NotwaitingSemaphore){
					((PCB*)(temp2->pcb))->waitingTime--;
				if (((PCB*)(temp2->pcb))->waitingTime==0){
						((PCB*)(temp2->pcb))->blocked = 0;
						Scheduler::put(((PCB*)(temp2->pcb)));
						this->value++;
					PCB* del =((PCB*)(temp2->pcb));
					this->Listblocked->remove((void*)(del));
				}
		}

			temp2 = temp2->next;
	}
}
