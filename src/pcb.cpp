/*
 * pcb.cpp
 *
 *  Created on: Jul 30, 2021
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include <iostream.h>
#include <dos.h>

#include "LoopT.h"
#include "SCHEDULE.H"
#include "AList.h"
#include "syPrintf.h"
#include "karnel.h"


int PCB::currentID = 0;
volatile PCB *PCB::running = 0;
volatile LoopThread* PCB::loopThread = 0;
List* PCB::madeThreads = new List();
List* PCB::waitList = new List();
PCB::PCB(StackSize stackSize, Time timeSlice, Thread* thread){
		lock
		if (loopThread->made == 0  ) {
			//loopThread = new LoopThread();
			loopThread->made = 1;
			loopThread->loopPCB = thread->myPCB;
			//syncPrintf("Loop thread made \n");
		}
	    myThread = thread;
		//DA LI JE MENI KVANT TIME SLICE??
		if (timeSlice != 0 ){
			kvant = timeSlice;
		} else {
			kvant = 0;
		}
		//DE ZNAM ...
			this->timeSlice = timeSlice;
			this->id = currentID++;
			this->finished = 0;
			this->blocked = 0;
			this->started = 0;
			madeThreads->put(this);
		unlock
}
void PCB::initStack(){
	this->stackSize = stackSize / sizeof(unsigned);
	if (stackSize > defaultStackSize) stackSize = defaultStackSize;
	if (stackSize < 16) stackSize = 16;
	this->stack = new unsigned[this->stackSize];
#ifndef BCC_BLOCK_IGNORE		//12 je sta?
			/*ss = FP_SEG(&stack[stackSize - 12]);
			sp = FP_OFF(&stack[stackSize - 12]);
			bp = FP_OFF(&stack[stackSize - 12]);*/

			//ovako radio brat iz onog upustva
			/*stack[stackSize-1] = FP_SEG(myThread);
			stack[stackSize-2] = FP_OFF(myThread);

			//I=0 u PSWu
			stack[stackSize-5] = 0x200;

			//PC
			stack[stackSize-6] = FP_SEG(&(wrapper));
			stack[stackSize-7] = FP_SEG(&(wrapper));

			//SS, SP i BP
			ss = FP_SEG(stack + stackSize - 16);
			sp = FP_OFF(stack + stackSize - 16);*/

			stack[stackSize-1] = 0x200;
			stack[stackSize-2] = FP_SEG(&(wrapper));
			stack[stackSize-3] = FP_SEG(&(wrapper));
			ss = FP_SEG(stack + stackSize - 12);
			sp = FP_OFF(stack + stackSize - 12);
			bp = sp;
	#endif
}

PCB::PCB(Thread *thread){
	PCB(defaultStackSize, defaultTimeSlice, thread);
}

void PCB:: waitTocomplete(){
	//syncPrintf("PCB %d wait3complete\n", this->id);
	lock
	if (this->finished || this == PCB::loopThread || this->blocked){
		unlock
		return;
	}
	if (PCB::running!=this){
		running->blocked = 1;
		waitList->put((void*)this);
		dispatch();
		unlock
	}
}
void PCB::start(){
	 //if (!this) this = new Thread();
	lock
	 if (this->started == 0){
		 initStack();
		 Scheduler::put(this);
		 this->started = 1;
	}
	unlock
}
/*void PCB::run(){
	//if (!finished)
	//lock();
	syncPrintf("PCB %d run\n", &id);
	//unlock();
		PCB::wrapper();
}*/

PCB::~PCB() {
	// waitToComplite();
	delete[] stack;
	delete waitList;
}

void PCB::unblock(){
	List::Elem* temp = waitList->first;
	List::Elem* del = waitList->first;
	while (temp){
		((PCB*)(temp->pcb))->blocked = 0;
		Scheduler::put((PCB*)temp->pcb);
		del = temp;
		temp = temp->next;
		delete del;
	}
	waitList->first = 0;
	waitList->last = 0;


}

void PCB::wrapper() {
	//lock();
	syncPrintf("PCB wrapper ??????????? alooo\n");
	PCB::running->myThread->run();
	running->finished = 1;
	lock
	PCB::running->myThread->myPCB->unblock(); //ovo je debilno
	unlock
	//kad dodje ovde nit se zavrsila
	//this->finished = 1;
	dispatch();
	//unlock();
}

void PCB::setKvant(int kvant){
	this->kvant = kvant;
}


Thread* PCB::getByID(ID i){
	List::Elem* temp = madeThreads->first;
	while (temp){
		if (((PCB*)(temp->pcb))->id == i){
			return ((PCB*)temp->pcb)->myThread;
		}
		temp = temp->next;
	}
	return 0;
}
