#ifndef _idlet_h_
#define _idlet_h_

#include "thread.h"


class LoopThread: public Thread{
public:
	//volatile int made;
	LoopThread();
	void start();

	PCB* loopPCB;
protected:
	virtual void run();
private:
	volatile static int started;

};
#endif
