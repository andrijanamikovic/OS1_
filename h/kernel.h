#ifndef _Kernel_h_
#define _Kernel_h_

#include "thread.h"
class PCB;
class LoopThread;


#define lockASM {asm{pushf; cli;}}
#define unlockASM {asm popf;}
#define lock {Kernel::lck=1;}
#define unlock {Kernel::lck=0;}

typedef void interrupt (*InterruptRoutine)(...);


extern void tick();
//static volatile Thread* mainThread;
class Kernel{
public:
	static volatile int lck;
    static void inic();
	static void restore();
	static void dispatch();
	static void interrupt timer(...);
	static InterruptRoutine oldTimer;
	//static volatile int forcedDispatch;
	static volatile int count; //msm da ni to
	static volatile int contextSwitch;

	//volatile unsigned lockFlag = 1;
	static volatile LoopThread* loopThread;
	static volatile Thread* mainThread;

protected:
	friend class PCB;
	friend class Thread;
	friend class KernelSem;
};

#endif
