#include "PCBLists.h"
#include "Lock.h"
#include "PCB.h"

sleepQueue PCBLists::sleepQ;
PCB* PCBLists::running;
Scheduler PCBLists::schedu;
PCB* PCBLists::idlePCB;

PCBList::PCBList() 
{ 
	first = NULL;
	last = NULL;
}

void PCBList::put(PCB *pcb)
{
	lock;
	if(last)
	{
		last->next = new pcbElem();
		last = last->next;
	}
	else
	{
		first = new pcbElem();
		last = first;
	}
	last->pcb = pcb;
	last->next = NULL;
	unlock;
}

PCB* PCBList::getFirst()
{
	lock;
	PCB *ret;
	pcbElem *dispose;
	if(!first)
	{
		unlock;
		return NULL;
	}
	ret = first->pcb;
	dispose = first;
	first = first->next;
	if(dispose == last)
		last = NULL;
	delete dispose;
	unlock;
	return ret;

}

PCBList::~PCBList()
{
	lock;
	pcbElem *cur = first;
	while(cur)
	{
		first = first->next;
		delete cur->pcb;
		cur = cur->next;
	}
	unlock;
}

void PCBList::remove(PCB *pcb)
{
	lock
	pcbElem *prev = NULL;
	for(pcbElem *current = first; current != NULL; current = current->next)
	{
		if(current->pcb == pcb)//found it!
		{
			if(prev == NULL)	//if the running PCB is the first in the ready list
			{
				first = current->next;
				delete current;
				break;
			}
			prev->next = current->next;
			if(current == last)
				last = prev;
			delete current;
			break;
		}
		prev = current;
	}
	unlock;
}

sleepElem::sleepElem(PCB *_pcb, Time _sleepingTime, sleepElem *_next)
{
	pcb = _pcb;
	sleepingTime = _sleepingTime;
	next = _next;
}

sleepElem::sleepElem(PCB *_pcb, Time _sleepingTime)
{
	pcb = _pcb;
	sleepingTime = _sleepingTime;
	next = NULL;
}

//Function that puts a pcb into the sleep queue.
//The timeToSleep argument is the absolute time for which
//the thread will sleep. The elements in the queue contain a
//sleepingTime variable, which indicates the time needed
//for the thread to be woken after it's predecessors are
//removed from the queue.
void sleepQueue::put(PCB *_pcb, Time timeToSleep)
{
	lock
	if(PCBLists::sleepQ.first)
	{
		sleepElem *cur = PCBLists::sleepQ.first;
		sleepElem *prev = NULL;
		
		//Loop finds the new PCB's spot in the list
		while(cur)
		{
			if(cur->sleepingTime <= timeToSleep)
			{
				timeToSleep -= cur->sleepingTime;
			}
			else
			{
				if(cur == first)
				{
					first = new sleepElem(_pcb, timeToSleep, cur);
					break;
				}
				else
				{
					prev->next = new sleepElem(_pcb, timeToSleep, cur);
					prev = prev->next;
					break;
				}
			}

			prev = cur;
			cur = cur->next;
		}
		if(!cur)
			prev->next = new sleepElem(_pcb, timeToSleep);

		//The pcb next to the newly inserted one needs it's sleeping time adjusted
		if(cur)
			cur->sleepingTime -= timeToSleep;	
	}
	else
	{
		first = new sleepElem(_pcb, timeToSleep);
	}
	unlock
}

//Function called by the timer to refresh the sleep times of PCBs
void sleepQueue::refreshSleepQueue()
{
	if(!first) return;
	sleepElem *cur = first;

	cur->sleepingTime--;
	while(cur && cur->sleepingTime == 0)
	{	
		first = first->next;
		PCBLists::schedu.put(cur->pcb);
		delete cur;
		cur = first;
	}
}

sleepQueue::sleepQueue()
{
	first = NULL;
}

sleepQueue::~sleepQueue()
{
	sleepElem *cur = first;
	while(cur)
	{
		first = first->next;
		delete cur;
		cur = first;
	}
}