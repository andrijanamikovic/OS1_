/*
 * IdleT.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: OS1
 */

#include "LoopT.h"
#include "pcb.h"
#include "syPrintf.h"
#include "karnel.h"

volatile int LoopThread::started = 0;
LoopThread::LoopThread():Thread(defaultStackSize,1){
	Karnel::loopThread->loopPCB = this->myPCB;
}
//volatile int LoopThread::made = 0;
void LoopThread::run(){//syncPrintf("Ran loopa pobratime");
	while(1);}

void LoopThread::start(){
	if (!started){
	if (!myPCB->started){
	//	syncPrintf("Loop pokrenut \n ");
		PCB::madeThreads->put(this->myPCB);
	}
	myPCB->started = 1;
	//syncPrintf("uslo u loop start \n ");
	}
	started = 1;
}
