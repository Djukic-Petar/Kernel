#include "InitThr.h"
#include "System.h"

extern int userMain(int argCount, char *argVect[]);

int InitThread::getRetVal() const { return retVal; }

void InitThread::run()
{
	retVal = userMain(System::argCount, System::argVect);
}