#include "PCB.h"
#include "PCBLists.h"
#include "Lock.h"
#include <dos.h>

const int PCB::running = 0;
const int PCB::ready = 1;
const int PCB::sleeping = 2;
const int PCB::done = 3;
const int PCB::blocked = 4;


PCB::PCB(Thread *_myThr, StackSize stackSize, Time timeSlice, ID _id)
{
	lock
	waitToCmplSem = new Semaphore(0);
	id = _id;
	myThr = _myThr;
	this->stackSize = stackSize;
	this->timeSlice = timeSlice;
	status = PCB::ready;
	if(id == 0)	//We won't wait for the idle thread to complete
		waitToCmplSem->signal();
	unlock
	
}

int PCB::getStatus() const { return status; }

void PCB::setStatus(int stat) { status = stat; }

ID PCB::getId() const { return id; }

PCB::~PCB()
{
	lock
	if(id != -1)
		delete stack;

	delete waitToCmplSem;
	unlock
}

void PCB::waitToComplete()
{
	if(status != PCB::done)
	{
		waitToCmplSem->wait();
	}
}



void PCB::start()
{
	lock;
	if(id >= 0) //If not the main PCB
	{
		status = PCB::ready;

		stackIndexSize = stackSize/sizeof(unsigned);
		stack = new unsigned[stackIndexSize];
		
		//Push the psw and code entry point onto the stack
		stack[stackIndexSize-1] = 0x200; //PSWI <= 1
		stack[stackIndexSize-2] = FP_SEG(PCB::runWrapper);	//entry point segment
		stack[stackIndexSize-3] = FP_OFF(PCB::runWrapper);	//entry point offset


		sSeg = FP_SEG(stack+stackIndexSize-12);
		sPoint = FP_OFF(stack+stackIndexSize-12);
		basePoint = sPoint;


		if(id != 0)	//id == 0 means it's the idle thread
		{
			PCBLists::schedu.put(this);
		}
		else
			PCBLists::idlePCB = this;
	}
	else
	{
		status = PCB::running;
		
		stack = NULL; 
	}
	unlock;
}

void PCB::runWrapper()
{
	PCBLists::running->myThr->run();
	PCBLists::running->setStatus(PCB::done);
	while(PCBLists::running->waitToCmplSem->val() < 0)
		PCBLists::running->waitToCmplSem->signal();

	dispatch();
}

//Thread* PCB::getThread() const {return myThr;}

Time PCB::getTimeSlice() const {return timeSlice;}

unsigned PCB::getSP() const { return sPoint; }

void PCB::setSP(unsigned newSP)
{
	sPoint = newSP;
}

void PCB::setSS(unsigned newSS) {sSeg = newSS;}

unsigned PCB::getSS() const {return sSeg;}

void PCB::setBP(unsigned newBP) {basePoint = newBP;}

unsigned PCB::getBP() const {return basePoint;}

void PCB::sleep(Time timeToSleep)
{
	lock;
	if(timeToSleep > 0)
	{
		PCBLists::sleepQ.put(this, timeToSleep);
		setStatus(PCB::sleeping);
	}
	unlock;
	dispatch();
}