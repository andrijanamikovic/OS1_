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
  syncPrintf("Thread %d starts\n", myPCB->id);
  myPCB->start();
}
void Thread::waitToComplete(){
	myPCB->waitTocomplete();
}
Thread::~Thread(){
	//this->waitToComplete();
	//da li imam listu svih niti?
	myPCB->waitList->remove((void*)myPCB);
	delete myPCB;
	//dispatch();
}
void dispatch (){
	asm cli;
	contextSwitch = 1;
	asm sti;
	timer(); //treba li?
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
