#include "Semaphor.h"
#include "KernelS.h"

Semaphore::Semaphore(int init)
{
	myImpl = new KernelSem(init);
}

Semaphore::~Semaphore()
{
	delete myImpl;
}

void Semaphore::wait()
{
	myImpl->wait();
}

void Semaphore::signal()
{
	myImpl->signal();
}

int Semaphore::val() const
{
	return myImpl->getVal();
}