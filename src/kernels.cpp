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
#include "syPrintf.h"

volatile  List* KernelSem::allSem = 0;

KernelSem::KernelSem(int init){
	lock
	Listblocked = new List();
	value = init;
	n = 0;
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
					block();
					dispatch();
					if ( PCB::running->waitingSemaphore == 1 && PCB::running->waitingTime == 0){
						unlock
						return 0;
					}
				}
			PCB::running->waitingTime = 0;
			unlock
			return 1;
}

void KernelSem::signal(){
	lock
	int i = 0;
		if (++value <= 0){
			//i= -1 * value;
			//for (int j =0; j<i; j++)
				unblock();
		}
	unlock
}

void KernelSem::block(){
	PCB::running->blocked = 1;
	if (Listblocked!=0)
		Listblocked->put((void*)PCB::running);
	n++;
	dispatch();
}

void KernelSem::unblock(){
	PCB* temp =(PCB*)Listblocked->pop();
	if (temp!=0 && !temp->loopFlag){
		temp->blocked = 0;
		temp->deblocked = 1;
		Karnel::inScheduler++;
		n--;
		Scheduler::put(temp);
	}
}

void KernelSem::update(){
	if (allSem == 0) return;
	List::Elem* temp = allSem->first;
	while (temp!=0){
		((KernelSem*)(temp->pcb))->updateSemaphore();
		temp = temp->next;
	}
}

void KernelSem::updateSemaphore(){
		List::Elem* temp2 =this->Listblocked->first;
		while (temp2 != 0){
			if (((PCB*)(temp2->pcb))->waitingSemaphore){
				((PCB*)(temp2->pcb))->waitingTime--;
				if (((PCB*)(temp2->pcb))->waitingTime==0){
					//if (!((PCB*)(temp2->pcb))->loopFlag){
						((PCB*)(temp2->pcb))->blocked = 0;
						((PCB*)(temp2->pcb))->deblocked = 1;
						//((KernelSem*)(temp->pcb))->n--;
						//syncPrintf("Deblokirao sam niti iz liste cekanja semafora \n");
						Karnel::inScheduler++;
						Scheduler::put(((PCB*)(temp2->pcb)));
					PCB* del =((PCB*)(temp2->pcb));
					this->Listblocked->remove((void*)(del));}
				temp2 = temp2->next;
					//syncPrintf("Deblokirao sam jednu niti iz liste cekanja semafora \n");
				}// else {
					//temp2 = temp2->next;
				//syncPrintf("Usao sam da Deblokiram niti iz liste cekanja semafora \n");
			//}
			temp2 = temp2->next;
	}

}
