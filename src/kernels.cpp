/*
 * kernels.cpp
 *
 *  Created on: Sep 21, 2021
 *      Author: OS1
 */

#include "karnel.h"
#include "KernelS.h"
#include "pcb.h"
#include "AList.h"
#include "SCHEDULE.H"


volatile  List* KernelSem::allSem = 0;
List* KernelSem::blocked = 0;

KernelSem::KernelSem(int init){
	lock
	//allSem = new List();
	//da li tu treba nesto posto pise da moze biti negativna????
	blocked = new List();
	value = init;
	unlock
}
KernelSem::~KernelSem(){
	lock
	delete[] blocked;
	unlock
}

int  KernelSem::val () const{
	return value;
}

int KernelSem::wait (Time maxTimeToWait){
	lock
	if (--value<0){
		if (maxTimeToWait!=0){
			PCB::running->waitingTime = maxTimeToWait;
			block();
			unlock
			return 1;
		} else {
			return 1;
		}
	}
	value--;
	unlock
	return 0;

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
	if (blocked!=0)
		blocked->put((void*)PCB::running);
	dispatch();
}

void KernelSem::unblock(){
	PCB* temp =(PCB*)blocked->pop();
	temp->blocked = 0;
	Scheduler::put(temp);
}

void KernelSem::update(){
	List::Elem* temp = allSem->first;
	while (temp){
		List::Elem* temp2 =((KernelSem*)(temp->pcb))->blocked->first;
		while (temp2){
			if (((PCB*)(temp2->pcb))->waitingTime!=0){
				((PCB*)(temp2->pcb))->waitingTime--;
				if (((PCB*)(temp2->pcb))->waitingTime==0){
					((PCB*)(temp2->pcb))->blocked = 0;
					Karnel::inScheduler++;
					Scheduler::put(((PCB*)(temp2->pcb)));
					PCB* del =((PCB*)(temp2->pcb));
					temp2 = temp2->next;
					((KernelSem*)(temp->pcb))->blocked->remove((void*)(del));
				}
			}
			temp2 = temp2->next;
		}
		temp = temp->next;
	}
}
