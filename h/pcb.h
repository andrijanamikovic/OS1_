/*
 * PCB.H
 *
 *  Created on: Jul 28, 2021
 *      Author: OS1
 */

#ifndef PCB_H_
#define PCB_H_

#include "AList.h"
#include "thread.h"


//class LoopThread;
const StackSize maxStackSize = 65536;
class PCB{
public:
	unsigned *stack;
	unsigned bp;
	unsigned sp;
	unsigned ss;
	StackSize size;
	int finished;
	int blocked;
    int started;
    int mainFlag;
    int loopFlag;
    int NotwaitingSemaphore;

	StackSize stackSize;
	Time timeSlice;
	Thread *myThread;


	static List* madeThreads;
	List* waitList;


	PCB(StackSize, Time , Thread*);
	~PCB();
	Thread* getByID(ID id);


    static void wrapper();
	void start();
	static void run();
    void unblock();
    void  initStack();

	static volatile PCB* running; //trenutna nit

	int waitingTime;
private:
	static int currentID;
	int id;
protected:
	friend class Thread;
	friend class LoopThread;
	friend class List;
	friend class Kernel;
	friend class KernelSem;
	friend class KernelEv;

};


#endif /* PCB_H_ */
