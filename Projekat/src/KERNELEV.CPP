#include "KernelEv.h"
#include "Schedule.h"
#include "PCBLists.h"
#include "lock.h"


KernelEv::KernelEv(IVTNo ivtNo)
{
	lock;
	val = 0;
	myIVTNo = ivtNo;
	myCreator = PCBLists::running;
	blocked = NULL;
	IVTEntry::getIVTEntry(ivtNo)->setEvent(this);//Entry should be prepared at this point
	unlock;
}

KernelEv::~KernelEv()
{
	if(blocked)
		signal();
	IVTEntry::getIVTEntry(myIVTNo)->eventDestroyed();
}

void KernelEv::wait()
{
	lock
	if(PCBLists::running == myCreator)
	{
		if(val == 0)
		{
			PCBLists::running->setStatus(PCB::blocked);
			blocked = PCBLists::running;
			unlock;
			dispatch();
		}
		else
		{
			val = 0;
			unlock;
		}
	}
}

void KernelEv::signal()
{
	lock
	if(val == 0)
	{
		if(blocked != NULL)
		{
			blocked->setStatus(PCB::ready);
			Scheduler::put(blocked);
			blocked = NULL;
		}
		else
			val = 1;
	}
	unlock
}