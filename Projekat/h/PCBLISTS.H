#ifndef _PCBLists_h_
#define _PCBLists_h_
#include "Thread.h"
#include "Schedule.h"
#define NULL 0

class PCB;

struct pcbElem
{
	PCB *pcb;
	pcbElem *next;
};

struct PCBList
{
	pcbElem *first, *last;

	PCBList();

	//Puts a PCB into the last place of the linked list
	void put(PCB *pcb);

	//Removes the first PCB from the linked list
	PCB* getFirst();

	~PCBList();

	//Remove the pcbElem containing this pcb from the list
	void remove(PCB *pcb); 
};

struct sleepElem
{
	PCB *pcb;
	Time sleepingTime;
	sleepElem *next;

	sleepElem(PCB *_pcb, Time _sleepingTime, sleepElem *_next);
	sleepElem(PCB *_pcb, Time _sleepingTime);
};

struct sleepQueue
{
	sleepElem *first;

	sleepQueue();

	~sleepQueue();

	void put(PCB *_pcb, Time timeToSleep);

	void refreshSleepQueue();
};

class PCBLists
{
public:
	static sleepQueue sleepQ;
	static PCB *running;
	static Scheduler schedu;
	static PCB *idlePCB;

	
};

#endif