#include "slova.h"
#include <conio.h>
#include <stdlib.h>

void Slova::run()
{
	int j = 100;
	int random=2;
	for(int k=0; k<j; k++)
	{
		cprintf("3 %d\r\n", k);
		random = rand() % 10 + 5;
		cprintf("Sleeping for %d slices\r\n", random);
		sleep(random);
	}
}