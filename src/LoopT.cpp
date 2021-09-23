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

//volatile int LoopThread::started = 0;
LoopThread::LoopThread():Thread(defaultStackSize,1){
	Karnel::loopThread->loopPCB = this->myPCB;
	//LoopThread::started = 1;
}
//volatile int LoopThread::made = 0;
void LoopThread::run(){//syncPrintf("Ran loopa pobratime");
	volatile int i = 1;
	while(i>0){
		i++;
		i--;
	}
}
void LoopThread::start(){
	if (!loopPCB) return;
	if (!loopPCB->started)
		loopPCB->started = 1;
}
