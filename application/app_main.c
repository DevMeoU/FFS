#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "app_main.h"

int main(int argc, char ** argv)
{
	printf("Start...\n");
	APPMIDW_MenuInit(argv[1]);
	// APPMIDW_MenuInit("E:\\F-SOFT\\Fresher_FullTime\\FFS\\floppy.img");
	return 0;
}
