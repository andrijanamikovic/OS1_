#ifndef _karnel_h_
#define _karnel_h_

#define lock {asm{pushf; cli;}}
#define unlock {asm popf;}

class PCB;
class Thread;
class LoopThread;

typedef void interrupt (*InterruptRoutine)(...);


extern void tick();
//static volatile Thread* mainThread;
class Karnel{
public:
    static void inic();
	static void restore();
	static void dispatch();
	static void interrupt timer(...);
	static InterruptRoutine oldTimer;
	//static volatile int forcedDispatch;
	static volatile int count;
	static volatile int contextSwitch;

	//volatile unsigned lockFlag = 1;
	static volatile LoopThread* loopThread;
	static volatile Thread* mainThread;

protected:
	friend class PCB;
	friend class Thread;
};

#endif
