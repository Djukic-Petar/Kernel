#include "IdleThr.h"
#include "Thread.h"

IdleThread::IdleThread() : Thread(4096, 1){}

void IdleThread::run()
{
	while(1){}
}

