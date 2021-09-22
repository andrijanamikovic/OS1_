/*
 * KernelSem.h
 *
 *  Created on: Sep 20, 2021
 *      Author: OS1
 */

#ifndef H_KERNELS_H_
#define H_KERNELS_H_

#include "AList.h"
#include "semaphor.h"

class KernelSem{
public:
	KernelSem(int init);
	virtual ~KernelSem();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;

	volatile static List* allSem;
	static  List* blocked;
	void block();
	void unblock();
	static void update();
protected:
	friend class Semaphore;
	friend class List;
private:
	int value;


};




#endif /* H_KERNELS_H_ */
