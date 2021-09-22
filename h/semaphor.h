/*
 * semaphore.h
 *
 *  Created on: Sep 20, 2021
 *      Author: OS1
 */
/*
#ifndef H_SEMAPHORE_H_
#define H_SEMAPHORE_H_

typedef unsigned int Time;

class KernelSem;
class Semaphore {
public:
	Semaphore (int init=1);
	virtual ~Semaphore ();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const; // Returns the current value of the semaphore
private:
	KernelSem* myImpl;
};
#endif



#endif /* H_SEMAPHORE_H_ */
