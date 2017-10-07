#include "Thread.h"
#include "PCB.h"
#include "Lock.h"
#include "PCBLists.h"
#include "System.h"
#include "Schedule.h"

ID Thread::idGen = 0;

Thread::Thread(StackSize stackSize, Time timeSlice)
{
	id = idGen++;
	myPCB = new PCB(this, stackSize, timeSlice, id);
}

void Thread::start()
{
	myPCB->start();
}

void Thread::waitToComplete()
{
	myPCB->waitToComplete();
}

void Thread::sleep(Time timeToSleep)
{
	PCBLists::running->sleep(timeToSleep);
}

Thread::~Thread()
{
	waitToComplete();
	delete myPCB;
}

void dispatch()
{
	System::requestedContextSwitch = 1;
	timer();
}