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

class PCB{
public:
	unsigned *stack;
	unsigned bp;
	unsigned sp;
	unsigned ss;
	int finished;
	int blocked;
    int started;
    //da dodam i running???


	int id;
	int kvant;

	StackSize stackSize;
	Time timeSlice;
	Thread *myThread;


	static List* madeThreads;
	static List* waitList;
	//treba da def i ponasanje flaga rade to ljudi

	PCB(StackSize, Time , Thread*);
	PCB(Thread* thread); //imaju deaufaltne?
	~PCB();
	Thread* getByID(ID id);


    static void wrapper();
	void waitTocomplete();
	void start();
	static void run(); //mozda bez static?
    void setKvant(int kvant);
    void unblock();
    void initStack();

    static volatile LoopThread* loopThread;
	static volatile PCB* running; //trenutna nit
	unsigned volatile counter; //globalni counter
private:
	static int currentID;
protected:
	friend class Thread;
	friend class LoopThread;
	friend class List;
	friend class Karnel;
};


#endif /* PCB_H_ */
