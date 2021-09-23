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
//volatile PCB *PCB::mainPCB = 0;
//volatile LoopThread* PCB::loopThread = 0;
List* PCB::madeThreads = 0;



PCB::PCB(StackSize size, Time time, Thread* thread){
		lock
	    myThread = thread;
		if (time != 0 ){
			kvant = time;
		} else {
			kvant = 0;
		}
		//syncPrintf ("U pcb kvant = %d \n", this->kvant);
			this->timeSlice = time;
			//syncPrintf ("U pcb timeSlice = %d \n", this->timeSlice);
			this->id = currentID++;
			finished = blocked = started = mainFlag = loopFlag =  0;
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
			//syncPrintf("sp = %d, bp = %d, ss = %d  id = %d \n", sp, bp ,ss, id);
}

void PCB:: waitTocomplete(){
	lock
	if (this == 0) return;
	if (this->finished  || this->loopFlag || this->mainFlag || !this->started ){ //|| this == running || !this->started || running->blocked || this == Karnel::mainThread->myPCB){
		unlock		//ako zabode mozda fali this==running
		return;
	}
	if (running !=0){
		if (this == running){
			unlock
			return;
		}
		running->blocked = 1;
		if (waitList!=0)
			waitList->put((void*)running);
			unlock
			dispatch();
	}
}

void PCB::start(){
	lock
	if (this!=0){
	 if (this->started == 0){
		 if (this->finished == 0){
			 Scheduler::put(this);
		 }
		 this->started = 1;
	 }
	}
	unlock
}

PCB::~PCB() {
	// waitToComplite();
	lock
	if (stack!=0)
		delete[] stack;
	if (waitList!=0)
		delete waitList;
	unlock
}

void PCB::unblock(){
	List::Elem* temp = waitList->first;
	List::Elem* del = waitList->first;
	while (temp!=0){
		((PCB*)(temp->pcb))->blocked = 0;
		if (temp->pcb!=0){
			lock
			Scheduler::put((PCB*)temp->pcb);
			unlock
		}
		del = temp;
		temp = temp->next;
		if (del!=0)
			delete del;
	}
	waitList->first = 0;
	waitList->last = 0;
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
