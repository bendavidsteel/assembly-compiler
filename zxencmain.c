/*Author: Ben Steel*/
/*zxencmain.c*/
/*encodes assembly instructions into machine code*/
#include <stdio.h>
#include <string.h>
#include "zx256.h"

int main(int argc, char *argv[])
{
	int num_bytes = 0;
	int success;
	FILE* inputfile;
	if (!strcmp(argv[1], "-c"))
	{
		unsigned char ibytes[40] = {
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0};
		char instruction[64] = "";
		strcpy(instruction, argv[2]);
		num_bytes = encode(instruction, ibytes);
		for (int i = 0; i<num_bytes; i++)
		{
			fprintf(stdout, "0x%02x ", ibytes[i]);
		}
		fprintf(stdout, "\n");
	}
	else
	{
		if (argc == 2)
		{
			inputfile = fopen(argv[1], "rb");
		}
		else if (argc == 1)
		{
			inputfile = stdin;
		}
		success = zxenc_dump(inputfile, 0, NULL);
		if (success == 1)
		{
			fprintf(stderr, "Error compiling file");
		}
		fclose(inputfile);
	}
	return 0;
}