#include "thread.h"
//#include "declare.h"
/*
	Test: cekanje niti
*/

#include "karnel.h"
#include "syPrintf.h"
/*#include <DOS.H>
#include <STDIO.H>
#include <STDARG.H>
#include "karnel.h"

// Zabranjuje prekide //OVAKO SE NE RADI U PROJEKTU
#define lock asm cli

// Dozvoljava prekide
#define unlock asm sti
*/
/*int syncPrintf(const char *format, ...)
{
//#ifndef BCC_BLOCK_IGNORE
	int res;
	va_list args;
	asm cli
		va_start(args, format);
	res = vprintf(format, args);
	va_end(args);
	asm sti
	return res;
//#endif
}
*/
class TestThread : public Thread
{
public:

	TestThread(): Thread() {};
	~TestThread()
	{
		waitToComplete();
	}
protected:

	void run();

};

void TestThread::run()
{
	syncPrintf("Thread %d: loop1 starts\n", getId());

	for(int i=0;i<32000;i++)
	{
		for (int j = 0; j < 32000; j++);
	}

	syncPrintf("Thread %d: loop1 ends, dispatch\n",getId());

	dispatch();

	syncPrintf("Thread %d: loop2 starts\n",getId());

	for (int k = 0; k < 20000; k++);

	syncPrintf("Thread %d: loop2 ends\n",getId());


}

class WaitThread: public Thread
{
private:
	TestThread *t1_,*t2_;

public:
	WaitThread(TestThread *t1, TestThread *t2): Thread()
	{
		t1_ = t1;
		t2_ = t2;
	};

	~WaitThread()
		{
			waitToComplete();
		}

protected:

	void run()
	{
		syncPrintf("Starting tests...\n");
		t1_->waitToComplete();
		syncPrintf("Test 1 completed!\n");
		t2_->waitToComplete();
		syncPrintf("Test 2 completed!\n");
	}
};

void tick() {}

int Usermain(int argc, char** argv)
//int doSomething()
{
	syncPrintf("User main starts\n");
	TestThread t1,t2;
	WaitThread w(&t1,&t2);
	t1.start();
	t2.start();
	w.start();

	syncPrintf("User main ends\n");
	return 16;
}
/*int userMain(int argc, char** argv){
	syncPrintf("User main starts\n");
		TestThread t1,t2;
		WaitThread w(&t1,&t2);
		t1.start();
		syncPrintf("t1 main starts\n");
		t2.start();
		syncPrintf("t2 main starts\n");
		w.start();
		syncPrintf("w main starts\n");
		syncPrintf("User main ends\n");
		return 16;
}*/

void userMain(int argc, char** argv){
//int main(){
//int main(int argc, char** argv){
  Karnel::inic();
  syncPrintf("Main starts\n");
  Usermain(argc,argv);
//  doSomething();
  Karnel::restore();

 // return 0;
}

