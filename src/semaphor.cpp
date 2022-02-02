/*
 * semaphor.cpp
 *
 *  Created on: Sep 20, 2021
 *      Author: OS1
 */


#include "semaphor.h"

#include "../h/kernel.h"
#include "KernelS.h"

Semaphore::Semaphore(int init){
	//lock
	myImpl = new KernelSem(init);
	//KernelSem::allSem->put((void*)myImpl);
	//unlock

}
Semaphore::~Semaphore (){
	delete myImpl;
	myImpl = 0;
}
int Semaphore::wait (Time maxTimeToWait){
	return myImpl->wait(maxTimeToWait);
}
void Semaphore::signal(){
	myImpl->signal();
}
int Semaphore::val () const{
	return myImpl->val();
}
