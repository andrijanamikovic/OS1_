/*
 * IdleT.cpp
 *
 *  Created on: Aug 31, 2021
 *      Author: OS1
 */

#include "../h/LoopT.h"
#include "thread.h"
#include "pcb.h"
#include "syPrintf.h"

LoopThread::LoopThread():Thread(500,5){
	this->made = 0;
}
//volatile int LoopThread::made = 0;
void LoopThread::run(){syncPrintf("Ran loopa pobratime");while(1);}

void LoopThread::start(){
	syncPrintf("Loop pokrenut ");
	myPCB->start();
}
