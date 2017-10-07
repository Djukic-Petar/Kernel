#ifndef _InitThr_h_
#define _InitThr_h_
#include "Thread.h"



class InitThread : public Thread{
public:
	int getRetVal() const;

protected:
	void run();



private:
	int retVal;

};

#endif