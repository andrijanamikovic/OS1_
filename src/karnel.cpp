
#include "pcb.h"
#include "karnel.h"
#include <iostream.h>
#include <dos.h>
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
		tick();
		KernelSem::update();
		if (count > 0) {
			count--;
			asm int 60h;
			tick();
		}
		if (!Karnel::contextSwitch && count == 0){
			asm int 60h;
			return;
		}

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
					//	syncPrintf("tajmer stavljeno u scheduler  %d  \n", PCB::running->id);

		}
	}

		PCB::running = Scheduler::get();
		if (PCB::running == 0){
			PCB::running = loopThread->myPCB;
			//syncPrintf("U tajmeru je inSch = %d \n", Karnel::inScheduler);
			//PCB::running->loopFlag=1;
		//syncPrintf("u tajmeru je loop sp = %d, bp = %d, ss = %d  id = %d \n", PCB::running->ss, PCB::running->sp ,PCB::running->bp, PCB::running->id);
		} else {
			Karnel::inScheduler--;
		}

		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

			//Karnel::count = PCB::running->kvant;
			count = PCB::running->timeSlice;
			//count = PCB::running->kvant;
		#ifndef BCC_BLOCK_IGNORE
			asm {
							mov sp, tsp   // restore sp i bp
							mov ss, tss
							mov bp, tbp;
						}
		#endif

				}
	//syncPrintf("U tajmeru je running podesen na %d a inScheduler = %d \n", PCB::running->id, Karnel::inScheduler);
	//if(!Karnel::contextSwitch) asm int 60h;
	Karnel::contextSwitch=0;
}

