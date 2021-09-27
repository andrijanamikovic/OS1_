/*
 * pcb.cpp
 *
 *  Created on: Jul 30, 2021
 *      Author: OS1
 */

//#include "thread.h"
#include "pcb.h"
#include <dos.h>
#include "LoopT.h"
#include "SCHEDULE.H"
#include "AList.h"
#include "syPrintf.h"
#include "karnel.h"
#include "KernelS.h"
#include "syPrintf.h"

int PCB::currentID = 0;
volatile PCB *PCB::running = 0;
List* PCB::madeThreads = 0;



PCB::PCB(StackSize size, Time time, Thread* thread){
		lock
			myThread = thread;
			this->timeSlice = time;
			this->id = currentID++;
			finished = blocked = started = mainFlag = loopFlag =  deblocked = waitingSemaphore = 0;
			this->stack = 0;
			this->waitingTime = 0;
			waitList = new List();
			this->stackSize = size / sizeof(unsigned);
			if (stackSize > maxStackSize) this->stackSize = maxStackSize/sizeof(unsigned);
			if (stackSize < 16) this->stackSize = 16;
			sp = ss = bp = 0;
			initStack();
			madeThreads->put(this);
		unlock
}
void  PCB::initStack(){
	this->stack = new unsigned[this->stackSize];
#ifndef BCC_BLOCK_IGNORE
			stack[this->stackSize-1] = 0x200;
			stack[this->stackSize-2] = FP_SEG(&(wrapper));
			stack[this->stackSize-3] = FP_OFF(&(wrapper));
			ss = FP_SEG(stack + this->stackSize - 12);
			sp = FP_OFF(stack + this->stackSize - 12);
			bp = sp;
#endif
}



void PCB::start(){
	lock
	if (this!=0){
	 if (this->started == 0){
		 if (this->finished == 0){
			 Karnel::inScheduler++;
			 Scheduler::put(this);
		 }
		 this->started = 1;
	 }
	}
	unlock
}

PCB::~PCB() {
	lock
	if (stack!=0)
		delete[] stack;
	if (waitList!=0)
		delete waitList;
	unlock
}

void PCB::unblock(){
	waitList->unblock();
}

void PCB::wrapper() {
	if (running!=0){
	running->myThread->run();
	running->finished = 1;
	lock
	PCB::running->myThread->myPCB->unblock();
	unlock
	dispatch();
	}
}


Thread* PCB::getByID(ID i){
	List::Elem* temp = madeThreads->first;
	while (temp!=0){
		if (((PCB*)(temp->pcb))->id == i){
			return ((PCB*)temp->pcb)->myThread;
		}
		temp = temp->next;
	}
	return 0;
}
