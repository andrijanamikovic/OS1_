/*
 * thread.cpp
 *
 *  Created on: Jul 30, 2021
 *      Author: OS1
 */

#include "thread.h"
#include "pcb.h"
#include "karnel.h"
#include "syPrintf.h"

class Karnel;

Thread::Thread(StackSize stackSize, Time timeSlice){
	lock
	myPCB = new PCB(stackSize, timeSlice,this);
	//ubacim ovde u listu PCBa?
	unlock
}

void Thread::start(){
  //syncPrintf("Thread %d starts\n", myPCB->id);
  myPCB->start();
}
void Thread::waitToComplete(){
	 //syncPrintf("Thread %d waits\n", myPCB->id);
	myPCB->waitTocomplete();
}
Thread::~Thread(){
	if (this->myPCB!=0){
	this->waitToComplete();
	lock
	myPCB->myThread = 0;
	myPCB->madeThreads->remove((void*)myPCB);
	myPCB->waitList->remove((void*)myPCB);
	unlock
	}
}
void dispatch (){
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
