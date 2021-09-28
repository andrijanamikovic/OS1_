
#include "pcb.h"
#include <iostream.h>
#include <dos.h>

#include "../h/kernel.h"
#include "SCHEDULE.H"
#include "LoopT.h"
#include "syPrintf.h"
#include "KernelS.h"

volatile unsigned tsp = 0;
volatile unsigned tss = 0;
volatile unsigned tbp = 0;

volatile long Karnel::inScheduler = 0;
volatile int Karnel::contextSwitch = 0;
volatile int Karnel::count = 1;
InterruptRoutine Karnel::oldTimer = 0;
volatile LoopThread* Karnel::loopThread = 0;
volatile Thread* Karnel::mainThread = 0;
extern void tick();

void Karnel::inic(){
	PCB::madeThreads = new List();
	Karnel::mainThread = new Thread(500,5);
	Karnel::mainThread->myPCB->mainFlag = 1;
	Karnel::mainThread->myPCB->started = 1;
	Karnel::loopThread = new LoopThread();
	Karnel::loopThread->myPCB->loopFlag = 1;
	Karnel::mainThread->myPCB->started = 1;
	PCB::madeThreads->put((void*)mainThread->myPCB);
	PCB::running = mainThread->myPCB;
	PCB::madeThreads->put((void*)loopThread->myPCB);
	count = mainThread->myPCB->timeSlice;

#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(0x08);
	setvect(0x60, oldTimer);
	setvect(0x08, timer);
#endif

}

void Karnel::restore(){
	asm cli
	setvect(0x08, oldTimer);
	delete loopThread;
	delete mainThread;
	delete PCB::madeThreads;
	asm sti
}

void interrupt Karnel::timer(...){
	if (!Karnel::contextSwitch){

		if (count > 0) {
					count--;
					asm int 60h;
					tick();

				}
		KernelSem::update();

	}
	if (Karnel::contextSwitch || (count == 0 && PCB::running->timeSlice!=0 )){
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
						Karnel::inScheduler++;
		}
	}

		PCB::running = Scheduler::get();
		if (PCB::running == 0){
			PCB::running = loopThread->myPCB;
		} else {
			Karnel::inScheduler--;
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
	Karnel::contextSwitch=0;
}

