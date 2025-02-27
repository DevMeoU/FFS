#include "app_main.h"

int main(int argc, char ** argv)
{
	printf("Start...\n");
	// APPMIDW_MenuInit(argv[1]);
	APPMIDW_MenuInit("floppy.img");
	return 0;
}
