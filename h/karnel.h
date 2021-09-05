#ifndef _karnel_h_
#define _karnel_h_


/*static void lock() {
	asm cli;
}

static void unlock() {
	asm sti;
}*/
#define lock {asm{pushf; cli;}}
#define unlock {asm popf;}

class PCB;
class Thread;

typedef void interrupt (*InterruptRoutine)(...);
typedef unsigned char IVTNo;

static volatile int contextSwitch = 0;
static volatile int count = 20;
void interrupt timer();
void tick();
static volatile Thread* mainThread;
class Karnel{
public:

    static void inic();
	static void restore();
	static void dispatch();
	static void interrupt newTimer(...);
	static InterruptRoutine oldTimer;
	static InterruptRoutine swap(IVTNo enteryNumber, InterruptRoutine newIntRoutine);
	static volatile int forcedDispatch;


	//volatile unsigned lockFlag = 1;



protected:
	friend class PCB;
	friend class Thread;
};

#endif
