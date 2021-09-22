

#include "pcb.h"
#include "karnel.h"
#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "LoopT.h"
#include "syPrintf.h"
//#include "KernelS.h"

volatile unsigned tsp = 0;
volatile unsigned tss = 0;
volatile unsigned tbp = 0;
//volatile int Karnel::forcedDispatch = 0;
volatile int Karnel::contextSwitch = 0;
volatile int Karnel::count = 0;
InterruptRoutine Karnel::oldTimer = 0;
volatile LoopThread* Karnel::loopThread = 0;
volatile Thread* Karnel::mainThread = 0;
extern void tick();
//volatile unsigned lockFlag = 1;
//volatile contextSwitch = 0;
//volatile int count = 20;
void Karnel::inic(){
	//asm cli
	//mainThread = new Thread();
	//mainThread->myPCB->start();
	//PCB::running = mainThread->myPCB;
	PCB::madeThreads = new List();
	Karnel::mainThread = new Thread();
	Karnel::mainThread->myPCB->mainFlag = 1;
	Karnel::mainThread->myPCB->id = 0;
	Karnel::mainThread->myPCB->start();
	//Karnel::mainThread->myPCB->started = 1;
	PCB::running = mainThread->myPCB;
	Karnel::loopThread = new LoopThread();
	Karnel::loopThread->start();

	//PCB::running = Karnel::loopThread->loopPCB;
	//asm sti


	//PCB::running->myThread = new Thread();
	//PCB::running->start();

#ifndef BCC_BLOCK_IGNORE
	oldTimer = getvect(0x08);
	setvect(0x60, oldTimer);
	setvect(0x08, timer);
#endif

	//unlock

	//syncPrintf("jel udje on u new tajmer??\n");
}

void Karnel::restore(){
	asm cli
	setvect(0x08, oldTimer);
	//syncPrintf("Gotovo bajo");
	delete loopThread;
	delete mainThread;
	delete PCB::madeThreads;
	asm sti
}
int flag = 0;

void interrupt Karnel::timer(...){
	if (!Karnel::contextSwitch){
		//if (Karnel::count > 0)
			//Karnel::count--;
		tick();
		//KernelSem::update();
		//if (PCB::running->kvant == 0){
		if (count > 0) {
			count--;
			asm int 60h;
			tick();
		}
		if (!Karnel::contextSwitch && count == 0){
			asm int 60h;
			return;
		}

		/*if (PCB::running->kvant >0)
			PCB::running->kvant--;*/

	}
	if ((Karnel::contextSwitch || count == 0) && PCB::running->kvant!=0 ){
		//(PCB::running->timeSlice != 0 && PCB::running->kvant== 0)){ //(PCB::running->timeSlice != 0 && Karnel::count == 0)){
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

		if(!PCB::running->finished && !PCB::running->blocked && PCB::running!=loopThread->loopPCB){
						Scheduler::put((PCB*)PCB::running);
					//	syncPrintf("tajmer stavljeno u scheduler  %d  \n", PCB::running->id);

		}
	}

		PCB::running = Scheduler::get();
		if (PCB::running == 0){
			PCB::running = loopThread->loopPCB;
		//	syncPrintf("u tajmeru je loop sp = %d, bp = %d, ss = %d  id = %d \n", PCB::running->ss, PCB::running->sp ,PCB::running->bp, PCB::running->id);
		}
		//syncPrintf("Restoring context!\n");
		tsp = PCB::running->sp;
		tss = PCB::running->ss;
		tbp = PCB::running->bp;

			//Karnel::count = PCB::running->kvant;
			count = PCB::running->kvant;
		#ifndef BCC_BLOCK_IGNORE
			asm {
							mov sp, tsp   // restore sp i bp
							mov ss, tss
							mov bp, tbp;
						}
		#endif

				}

	// OVAJ deo sa flagom je ubacen samo da mi ogranicen broj puta ispise sp,bp i ostatak
	if (flag < 5){
	//syncPrintf("u tajmeru sp = %d, bp = %d, ss = %d  id = %d \n", PCB::running->sp, PCB::running->bp ,PCB::running->ss, PCB::running->id);
	flag++;
	}
	//if(!Karnel::contextSwitch) asm int 60h;
	Karnel::contextSwitch=0;
}

