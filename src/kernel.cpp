
#include "pcb.h"
#include <iostream.h>
#include <dos.h>

#include "../h/kernel.h"
#include "SCHEDULE.H"
#include "LoopT.h"
#include "KernelS.h"

volatile unsigned tsp = 0;
volatile unsigned tss = 0;
volatile unsigned tbp = 0;

volatile int Kernel::contextSwitch = 0;
volatile int Kernel::lck = 0;
volatile int Kernel::count = 1;
InterruptRoutine Kernel::oldTimer = 0;
volatile LoopThread* Kernel::loopThread = 0;
volatile Thread* Kernel::mainThread = 0;
extern void tick();

void Kernel::inic(){
	PCB::madeThreads = new List();
	Kernel::mainThread = new Thread(500,5);
	Kernel::mainThread->myPCB->mainFlag = 1;
	Kernel::mainThread->myPCB->started = 1;
	Kernel::loopThread = new LoopThread();
	Kernel::loopThread->myPCB->loopFlag = 1;
	Kernel::mainThread->myPCB->started = 1;
	PCB::madeThreads->put((void*)mainThread->myPCB);
	PCB::running = mainThread->myPCB;
	PCB::madeThreads->put((void*)loopThread->myPCB);
	count = mainThread->myPCB->timeSlice;
//lockASM
#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(0x08);
	setvect(0x60, oldTimer);
	setvect(0x08, timer);
#endif
//unlockASM

}

void Kernel::restore(){
	//asm cli
	lockASM
	setvect(0x08, oldTimer);
	delete loopThread;
	delete mainThread;
	delete PCB::madeThreads;
	//asm sti
	unlockASM
}

void interrupt Kernel::timer(...){
	if (!Kernel::contextSwitch){

		if (count >= 0) {
					if (count!=0)
						count--;
					asm int 60h;
					tick();

				}
		KernelSem::update();

	}

	if (Kernel::contextSwitch || (count == 0 && PCB::running->timeSlice!=0 && lck==0)){
#ifndef BCC_BLOCK_IGNORE
		asm {
						// cuva sp i bp
						mov tsp, sp
						mov tss, ss
						mov tbp, bp
					}
#endif

	if (PCB::running!=0){
		PCB::running->sp = tsp;
		PCB::running->ss = tss;
		PCB::running->bp = tbp;

		if(!PCB::running->finished && !PCB::running->blocked && !PCB::running->loopFlag && PCB::running->started){
						Scheduler::put((PCB*)PCB::running);
		}
	}

		PCB::running = Scheduler::get();
		if (PCB::running == 0){
			PCB::running = loopThread->myPCB;
		}
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;
		count = PCB::running->timeSlice;
		#ifndef BCC_BLOCK_IGNORE
			asm {
							mov sp, tsp   // restore sp i bp
							mov ss, tss
							mov bp, tbp;
						}
		#endif

				}
	Kernel::contextSwitch=0;
}

