#include "KernelS.h"
#include "PCBLists.h"
#include "PCB.h"
#include "Lock.h"

KernelSem::KernelSem(int _val)
{
	lock
	val = _val;
	blocked = new PCBList();
	unlock
}

//If there are blocked threads waiting on the semaphore about to be destroyed, the threads will unblock
KernelSem::~KernelSem()
{
	lock
	while(val < 0)
		signal();

	delete blocked;
	unlock
}


void KernelSem::wait()
{
	lock;
	if(val-- <= 0)
	{
		blocked->put(PCBLists::running);
		PCBLists::running->setStatus(PCB::blocked);
		unlock;
		dispatch();
	}
	else
	{
		unlock;
	}
}

void KernelSem::signal()
{
	lock
	PCB *pcb = blocked->getFirst();
	if(pcb)
	{
		pcb->setStatus(PCB::ready);
		PCBLists::schedu.put(pcb);
	}
	val++;
	
	unlock
}

int KernelSem::getVal() const
{
	return val;
}