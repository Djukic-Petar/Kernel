#ifndef _PCB_h_
#define _PCB_h_
#include "thread.h"
#include "Semaphor.h"

class PCBLists;

typedef char byte;

class PCB
{
public:

	PCB(Thread *_myThr, StackSize stackSize, Time timeSlice, ID _id);

	int getStatus() const;

	void setStatus(int stat);

	Time getTimeSlice() const;

	//Thread* getThread() const;

	unsigned getSP() const;

	unsigned getSS() const;

	unsigned getCS() const;

	unsigned getIP() const;

	unsigned getBP() const;

	void setBP(unsigned newBP);

	void setSP(unsigned newSP);

	void setSS(unsigned newSS);

	void setCS(unsigned newCS);

	void setIP(unsigned newIP);

	void sleep(Time timeToSleep);

	void waitToComplete();

	void start();

	void startIdle();

	ID getId() const;

	~PCB();

	static void runWrapper();

	//Possible PCB states
	static const int running;
	static const int ready;
	static const int sleeping;
	static const int done;
	static const int blocked;

private:

	unsigned sPoint;
	unsigned sSeg;
	unsigned basePoint;

	Thread *myThr;
	
	StackSize stackSize;
	StackSize stackIndexSize;
	unsigned *stack;

	ID id;
	int status;
	
	Time timeSlice;
	friend class Thread;
	

	Semaphore *waitToCmplSem;
};

#endif