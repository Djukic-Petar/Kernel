#ifndef _kernelEv_h_
#define _kernelEv_h_

#include "IVTEntry.h"
#include "PCBLISTS.h"
#include "PCB.h"

class KernelEv
{
public:
	KernelEv(IVTNo ivtNo);

	~KernelEv();

	void wait();

	void signal();

private:
	int val;	//0 - blocked, 1 - signalled
	IVTNo myIVTNo;

	PCB *blocked;
	PCB *myCreator;
};

#endif