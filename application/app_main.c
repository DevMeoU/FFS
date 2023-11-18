#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "app_main.h"

int main(int argc, char ** argv)
{
	printf("Start...\n");
	APPMIDW_MenuInit(argv[1]);
	// APPMIDW_MenuInit("floppyff.img");
	return 0;
}
