#include "thread.h"
#include "pcb.h"
#include "karnel.h"
#include <iostream.h>
#include <dos.h>
#include "SCHEDULE.H"
#include "LoopT.h"
#include "syPrintf.h"



// Pomocne promenljive za prekid tajmera
unsigned tsp;
unsigned tss;
unsigned tbp;
volatile unsigned lockFlag = 1;
//volatile contextSwitch = 0;
//volatile int count = 20;

void interrupt timer(){
	if (!contextSwitch) {
		tick();
		count--;
	}
	if (count == 0 || contextSwitch) {
		if (lockFlag ==1 ){
			contextSwitch = 0;
			asm {
				// cuva sp i bp
				mov tsp, sp
				mov tss, ss
				mov tbp, bp
			}

			PCB::running->sp = tsp;
			PCB::running->ss = tss;
			PCB::running->bp = tbp;

		//	running= getNextPCBToExecute();	// Scheduler
				//Scheduler::put((PCB*)PCB::running);
			if(!PCB::running->finished && !PCB::running->blocked && PCB::running->started && PCB::running!=PCB::loopThread->loopPCB){
				Scheduler::put((PCB*)PCB::running);
				syncPrintf("stavljeno u scheduler");

			}
			PCB::running = Scheduler::get();


			if (PCB::running!=0)
				syncPrintf("Uzeo sam nesto iz schedulera glupog");


			if (PCB::running == 0){
				syncPrintf("Uzeo sam loop");
				PCB::running = PCB::loopThread->loopPCB;
			}

			tsp = PCB::running->sp;
			tss = PCB::running->ss;
			tbp = PCB::running->bp;

			count = PCB::running->kvant;

			asm {
				mov sp, tsp   // restore sp i bp
				mov ss, tss

				mov bp, tbp;
			}
		}
		//else contextSwitch = 1;
		if(!contextSwitch) asm int 60h;
	}
	contextSwitch = 0;
}

unsigned oldTimerOFF, oldTimerSEG;

void Karnel::inic(){
	asm{
			cli
			push es
			push ax

			mov ax,0   //  ; inicijalizuje rutinu za tajmer
			mov es,ax

			mov ax, word ptr es:0022h //; pamti staru rutinu
			mov word ptr oldTimerSEG, ax
			mov ax, word ptr es:0020h
			mov word ptr oldTimerOFF, ax

			mov word ptr es:0022h, seg timer	 //postavlja
			mov word ptr es:0020h, offset timer //novu rutinu

			mov ax, oldTimerSEG	 //	postavlja staru rutinu
			mov word ptr es:0182h, ax //; na int 60h
			mov ax, oldTimerOFF
			mov word ptr es:0180h, ax

			pop ax
			pop es
			sti
		}

	mainThread = new Thread();
	mainThread->myPCB->started = 1;
	PCB::running = mainThread->myPCB;

	PCB::loopThread = new LoopThread();
	//PCB::loopThread->start();
	timer(); //??????????????????/

}

void Karnel::restore(){
	asm {
		cli
		push es
		push ax

		mov ax,0
		mov es,ax


		mov ax, word ptr oldTimerSEG
		mov word ptr es:0022h, ax
		mov ax, word ptr oldTimerOFF
		mov word ptr es:0020h, ax

		pop ax
		pop es
		sti
	}
}

