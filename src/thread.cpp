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
	if (this->myPCB!=0)
	this->waitToComplete();
	//da li imam listu svih niti?
	lock
	myPCB->myThread = 0;
	//syncPrintf("Thread %d delet\n", myPCB->id);
	myPCB->madeThreads->remove((void*)myPCB);
	myPCB->waitList->remove((void*)myPCB);
	//if (myPCB!=0)
	//	delete myPCB;
	unlock
	//dispatch();
}
void dispatch (){
	lock
	Karnel::contextSwitch = 1;
	 //syncPrintf("Thread %d dispatch\n", myPCB->id);
	//PCB::running->finished = 1;
	Karnel::timer(); //treba li?
	unlock
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
