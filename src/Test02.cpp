#include "../h/thread.h"
#include "syPrintf.h"

/*
	Test: asinhrono preuzimanje
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

	//syncPrintf("Thread %d: loop2 starts\n",getId());

	for (int k = 0; k < 20000; k++);

	syncPrintf("Thread %d: loop2 ends\n",getId());

}



void tick(){}

int userMain(int argc, char** argv)
{
	syncPrintf("User main starts\n");
	TestThread t1,t2;
	t1.start();
	t2.start();
	syncPrintf("User main ends\n");
	return 16;
}


