#ifndef _KernelS_h_
#define _KernelS_h_
#include "Semaphor.h"

#include "PCBLists.h"

class Semaphore;

class KernelSem
{
public:
	
	KernelSem(int _val);
	~KernelSem();

	void wait();
	void signal();

	int getVal() const;

private:
	int val;
	
	PCBList *blocked;
};

#endif