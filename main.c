#include <stdio.h>

#include "flv.h"


int main(int argc, char *argv[])
{
	if(argc == 1)
	{
		printf("Usage: \n"
 			   "   %s avfile/test1.flv\n"
			   "   %s avfile/test2.flv\n",
			   argv[0], argv[0]);
		return -1;
	}

	flv_demux_h264_aac(argv[1]);

	printf("\033[32m""Success!\n""\033[0m");

	return 0;
}

