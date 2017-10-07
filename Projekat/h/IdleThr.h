#ifndef _IdleThr_h_
#define _IdleThr_h_
#include "Thread.h"

class IdleThread : public Thread{
public:
	IdleThread();

protected:
	void run();

};


#endif