#include "System.h"
#include <Dos.h>
#include "PCBLists.h"
#include "PCB.h"
#include "Lock.h"
#include "IVTEntry.h"

int System::argCount;
char** System::argVect;
IdleThread System::idleThread;
InitThread System::initThread;
Time System::runningPCBTime = 0;
intrpt System::oldRout = 0;
int System::requestedContextSwitch = 0;

unsigned runningSP;
unsigned runningSS;
unsigned runningBP;

int System::lockFlag = 1;

void System::lockSys()
{
	lockFlag--;
	asm cli;
}

void System::unlockSys()
{
	lockFlag++;
	if(lockFlag == 1)
		asm sti;
}

void System::init()
{
	lock;

	asm{
		cli
		push es
		push ax

		mov ax,0
		mov es,ax

		mov ax, word ptr es:0x0022
		mov word ptr oldTimerSEG, ax	
		mov ax, word ptr es:0x0020	
		mov word ptr oldTimerOFF, ax	

		mov word ptr es:0x0022, seg timer
		mov word ptr es:0x0020, offset timer

		mov ax, oldTimerSEG
		mov word ptr es:0x0182, ax
		mov ax, oldTimerOFF
		mov word ptr es:0x0180, ax

		pop ax
		pop es
		sti
	}

	System::makeMainPCB();
	System::idleThread.start();
	System::initThread.start();

	unlock;
	System::initThread.waitToComplete();
}

//Make a PCB with no thread, default stack size and time, and id=-1
//This PCB will be used to restore context back to the main function when the user code is done
void System::makeMainPCB()
{
	PCBLists::running = new PCB(NULL, 4096, 2, -1);
	PCBLists::running->start();
}

void System::finalize()
{
	lock;

	//Restore the old timer routine
	asm{
		push ax;
		push es;

		mov ax, 0;
		mov es, ax;

		mov ax, word ptr es:0x0180;
		mov word ptr es:0x0020, ax;

		mov ax, word ptr es:0x0182;
		mov word ptr es:0x0022, ax

		pop es;
		pop ax;
	}

	IVTEntry::deleteIVTList();

	//Delete the main PCB (id=-1) we created in the init() function
	delete PCBLists::running;

	unlock;
}

//Timer function which does the context switch itself
void interrupt timer(...)
{
	if(System::lockFlag == 1)
	{
		lock;
		
		if(System::requestedContextSwitch == 0)
		{
			asm int 60h;
			PCBLists::sleepQ.refreshSleepQueue();
			System::runningPCBTime++;
			
			if(PCBLists::running->getTimeSlice() == 0)	//PCB is given unlimited time
			{
				unlock;
				return;
			}
			else
			{
				if(System::runningPCBTime == PCBLists::running->getTimeSlice())							
					System::requestedContextSwitch = 1;				
			}
		}
		
		if(System::requestedContextSwitch == 1)
		{
			System::requestedContextSwitch = 0;

			//If the running PCB is not done
			if(PCBLists::running->getStatus() != PCB::done)
			{	//Save the context of the current PCB
				
				//If the running PCB was blocked on a semaphore, don't put it in the scheduler
				if(PCBLists::running->getStatus() == PCB::running)	
				{		
					PCBLists::running->setStatus(PCB::ready);	

					//Don't put the idle thread into the scheduler!
					if(PCBLists::running->getId() != 0)	
						PCBLists::schedu.put(PCBLists::running);
				}

				asm{
					mov runningSS, ss;
					mov runningSP, sp;
					mov runningBP, bp;
				}

				PCBLists::running->setSS(runningSS);
				PCBLists::running->setSP(runningSP);
				PCBLists::running->setBP(runningBP);

			}
			//Get new PCB to run
			PCBLists::running = PCBLists::schedu.get();

			if(PCBLists::running == NULL)
			{
				PCBLists::running = PCBLists::idlePCB;
			}
			
			System::runningPCBTime = 0;		
			PCBLists::running->setStatus(PCB::running);

			//Restore the new PCB's context

			runningSS = PCBLists::running->getSS();
			runningSP = PCBLists::running->getSP();
			runningBP = PCBLists::running->getBP();
			
			asm{
				mov ss, runningSS;
				mov sp, runningSP;
				mov bp, runningBP;
			}		
		}
		unlock;
	}
	else
	{
		System::requestedContextSwitch = 0;
	}
}

int main(int argc, char *argv[])
{
	System::argVect = argv;
	System::argCount = argc;
	System::init();
	int retVal = System::initThread.getRetVal();
	System::finalize();
	return retVal;
}