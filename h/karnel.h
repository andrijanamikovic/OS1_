#ifndef _karnel_h_
#define _karnel_h_


/*static void lock() {
	asm cli;
}

static void unlock() {
	asm sti;
}*/
#define lock asm cli;
#define unlock asm sti;

class PCB;
class Thread;



static volatile int contextSwitch = 0;
static volatile int count = 20;
void interrupt timer();
void tick();
static volatile Thread* mainThread;
class Karnel{
public:

    static void inic();
	static void restore();



	//volatile unsigned lockFlag = 1;



protected:
	friend class PCB;
	friend class Thread;
};

#endif
