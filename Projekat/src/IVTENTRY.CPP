#include "IVTEntry.h"
#include <dos.h>
#include "KernelEv.h"
#include "Lock.h"
#define NULL 0

IVTEntry** IVTEntry::ivtEntryList = NULL;

void IVTEntry::setIVTEntry(int ivtNo, IVTEntry *newEntry)
{
	if(ivtEntryList == NULL)
	{
		ivtEntryList = new IVTEntry*[256];
		for(int i = 0; i < 256; i++)
			ivtEntryList[i] = NULL;
	}
	ivtEntryList[ivtNo] = newEntry;
}

void IVTEntry::deleteIVTList()
{
	//If the user applicaton doesn't use events, the entry list won't be initialized
	if(!ivtEntryList)
		return;

	delete [] ivtEntryList;
	ivtEntryList = NULL;
}

IVTEntry::IVTEntry(int entryNo, pInterrupt f)
{
	lock;
	ivtNo = entryNo;
	IVTEntry::setIVTEntry(ivtNo, this);
	oldRout = getvect(ivtNo);
	setvect(ivtNo, f);
	unlock;
}

IVTEntry::~IVTEntry()
{
	lock
	if(ivtEntryList)
		ivtEntryList[ivtNo] = NULL;
	unlock
}

void IVTEntry::eventDestroyed()
{
	lock
	setvect(ivtNo, oldRout);
	ivtEvent = NULL;
	unlock
}

void IVTEntry::signal()
{
	ivtEvent->signal();
}

IVTEntry* IVTEntry::getIVTEntry(int ivtNo)
{
	return ivtEntryList[ivtNo];
}

void IVTEntry::setEvent(KernelEv *event)
{
	ivtEvent = event;
}

