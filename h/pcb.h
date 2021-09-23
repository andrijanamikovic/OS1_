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
#include "LoopT.h"

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
    //da dodam i running???

	int kvant;

	StackSize stackSize;
	Time timeSlice;
	Thread *myThread;


	static List* madeThreads;
	List* waitList;
	//treba da def i ponasanje flaga rade to ljudi

	PCB(StackSize, Time , Thread*);
	~PCB();
	Thread* getByID(ID id);


    static void wrapper();
	void waitTocomplete();
	void start();
	static void run(); //mozda bez static?
    void unblock();
    void  initStack();

   // static volatile LoopThread* loopThread;
	static volatile PCB* running; //trenutna nit
	//static volatile PCB* mainPCB;
	//unsigned volatile counter; //globalni counter

	int waitingTime;
private:
	static int currentID;
	int id;
protected:
	friend class Thread;
	friend class LoopThread;
	friend class List;
	friend class Karnel;
	friend class KernelSem;
	friend class KernelEv;

};


#endif /* PCB_H_ */
