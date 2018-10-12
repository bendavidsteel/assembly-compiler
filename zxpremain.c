/*Author: Ben Steel*/
/*zxpremain.c*/
/*takes an into of assembly code and cleans it up*/
/*removes comments and whitespace, and checks for invalid code*/

#include <stdio.h>
#include <string.h>

#include "zx256.h"

int main(int argc, char *argv[])
{
	char label[9];
	char instruction[256];
	int read;
	int label_len = 0;
	FILE* inputfile = stdin;
	if (argc > 2)
	{
		fprintf(stderr, "Usage: ./zxpre.exe INPUTFILE OR ./zxpre.exe < INPUTFILE");
		return 1;
	}
	else if (argc == 2)
	{
		inputfile = fopen(argv[1], "rb");
	}
	if(inputfile == NULL)
	{
		fprintf(stderr, "Unable to open file: %s\n", argv[1]);
		return 1;
	}
	while(1)
	{
		read = preprocess_get_line(label, instruction, inputfile);
		label_len = strlen(label);
		if (read == 1)
		{
			if (label_len > 0)
			{
				fprintf(stdout, "%s:", label);
				for (int i = 0; i<9-label_len-1; i++)
				{
					fprintf(stdout, " ");
				}
			}
			else
			{
				for (int i = 0; i<9; i++)
				{
					fprintf(stdout, " ");
				}
			}
			fprintf(stdout, "%s\n", instruction);
		}
		else if (read == 2)
			return 1;
		else if (read == EOF)
		{
			break;
		}
	}
	fclose(inputfile);
	return 0;
}
