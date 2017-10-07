#ifndef _IVTEntry_h_
#define _IVTEntry_h_
#include "Event.h"

#define PREPAREENTRY(intNo, oldR)\
	void interrupt inter##intNo(...);\
	IVTEntry newEntry##intNo(intNo, inter##intNo);\
	void interrupt inter##intNo(...)\
	{\
		newEntry##intNo.signal();\
		if(oldR == 1)\
			newEntry##intNo.oldRout();\
	}\

typedef void interrupt (*pInterrupt)(...);

class KernelEv;

class IVTEntry
{
public:

	void interrupt (*oldRout)(...);

	static IVTEntry* getIVTEntry(int ivtNo);

	static void deleteIVTList();

	~IVTEntry();
	IVTEntry(int entryNo, pInterrupt f);

	void setEvent(KernelEv *event);

	void signal();

	void eventDestroyed();

	static IVTEntry **ivtEntryList;

private:
	KernelEv *ivtEvent;

	static void setIVTEntry(int ivtNo, IVTEntry *newEntry);
	
	int ivtNo;
};



#endif
