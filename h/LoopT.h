#ifndef _idlet_h_
#define _idlet_h_

#include "thread.h"


class LoopThread: public Thread{
public:
	//volatile int made;
	LoopThread();
	PCB* loopPCB;
	void start();
protected:
	virtual void run();

};
#endif
