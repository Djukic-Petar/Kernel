#include "brojevi.h"
#include "slova.h"
#include <stdio.h>
#include <stdlib.h>


int userMain(int argc, char* argv[])
{
	Brojevi br;
	Slova slv;

	slv.start();
	br.start();

	br.waitToComplete();
	slv.waitToComplete();
	puts("userMain done!");
	return 0;
}