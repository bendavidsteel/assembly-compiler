/*Author: Ben Steel*/
/*zxccmain.c*/
/*takes assembly code file, compiles it and writes result to a .exe file*/

#include <stdio.h>
#include <string.h>
#include "zx256.h"

int main(int argc, char *argv[])
{
	FILE* inputfile;
	FILE* outputfile;
	int success;
	if ((argc == 4) && !strcmp(argv[2], "-o"))
	{
		inputfile = fopen(argv[1], "rb");
		outputfile = fopen(argv[3], "wb");
	}
	else if ((argc == 3) && !strcmp(argv[1], "-o"))
	{
		inputfile = stdin;
		outputfile = fopen(argv[2], "wb");
	}
	else if (argc == 2)
	{
		inputfile = fopen(argv[1], "rb");
		outputfile = fopen("a.exe", "wb");
	}
	else if (argc == 1)
	{
		inputfile = stdin;
		outputfile = fopen("a.exe", "wb");
	}
	success = zxenc_dump(inputfile, 1, outputfile);
	if (success == 1)
	{
		fprintf(stderr, "Error writing to file.");
	}
	else if (success == 0)
	{
		fprintf(stdout, "Success!");
	}
	fclose(outputfile);
	fclose(inputfile);
	return 0;
}