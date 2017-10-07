#ifndef _System_h_
#define _System_h_
#include "Thread.h"
#include "Idlethr.h"
#include "initthr.h"

typedef void interrupt (*intrpt)(...);

static unsigned oldTimerSEG, oldTimerOFF;

static char debugStr[10];

void interrupt timer(...);

class System
{
public:
	static intrpt oldRout;
	static Time runningPCBTime;
	static int requestedContextSwitch;	

	static void init();
	static void makeMainPCB();
	static void finalize();

	static int argCount;
	static char **argVect;

	static IdleThread idleThread;
	static InitThread initThread;

	static int lockFlag; //1 - Unlocked, <1 - Locked
	static void lockSys();
	static void unlockSys();

};



#endif