/*
 * thread.cpp
 *
 *  Created on: Jul 30, 2021
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "syPrintf.h"
#include <dos.h>
#include "../h/kernel.h"

class Karnel;

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock
	myPCB = new PCB(stackSize, timeSlice,this);
	//ubacim ovde u listu PCBa?
	unlock
}

void Thread::start(){
  myPCB->start();
}
void Thread::waitToComplete(){
	// syncPrintf("Thread %d waits\n", myPCB->id);
		//lock
		if (myPCB == 0 || PCB::running == 0) return;
		if (myPCB->finished  || myPCB->loopFlag || !myPCB->started || this->myPCB==PCB::running ){ //|| this == running || !this->started || running->blocked || this == Karnel::mainThread->myPCB){
			//unlock
			return;
		}
			//syncPrintf("Nit sa id = %d blocked = %d, hoce da blokira nit sa id = %d blocked = %d\n a u scheduleru je %d \n", this->myPCB->id,this->myPCB->blocked, PCB::running->id, PCB::running->blocked, Karnel::inScheduler);
		PCB::running->blocked = 1;
			if (myPCB->waitList!=0)
				myPCB->waitList->put((void*)PCB::running);
		dispatch();
}
Thread::~Thread(){
	if (this->myPCB!=0){
	this->waitToComplete();
	lock
	myPCB->myThread = 0;
	myPCB->madeThreads->remove((void*)myPCB);
	myPCB->waitList->remove((void*)myPCB);
	delete this->myPCB;
	myPCB = 0;
	unlock
	}
}
void dispatch (){
	//syncPrintf("Thread dispatch \n");
	Karnel::contextSwitch = 1;
	Karnel::timer();
}

ID Thread::getId(){
	return (ID)myPCB->id;
}

ID Thread::getRunningId(){
	return (ID)PCB::running->id;
}

Thread* Thread::getThreadById(ID id){
	return PCB::running->getByID(id);
}
