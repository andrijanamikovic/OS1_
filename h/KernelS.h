/*
 * KernelSem.h
 *
 *  Created on: Sep 20, 2021
 *      Author: OS1
 */
/*
#ifndef H_KERNELS_H_
#define H_KERNELS_H_

class KernelSem{
	KarnelSem(int init);
	virtual ~KarnelSem();
	virtual int wait (Time maxTimeToWait);
	virtual void signal();
	int val () const;

	volatile static KernelSem* allSem;

protected:
	friend class Semaphore;
	friend class List;



};




#endif /* H_KERNELS_H_ */
