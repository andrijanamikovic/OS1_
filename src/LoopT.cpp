/*
 * IdleT.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: OS1
 */

#include "LoopT.h"

#include "../h/kernel.h"
#include "pcb.h"
#include "syPrintf.h"

LoopThread::LoopThread():Thread(defaultStackSize,1){
	myPCB->started = 1;
	myPCB->loopFlag = 1;
}

void LoopThread::run(){
	volatile int i = 1;
	while(i>0){
		i = 2;
	}
}
void LoopThread::start(){
	if (!myPCB) return;
	if (!myPCB->started)
		myPCB->started = 1;
}
