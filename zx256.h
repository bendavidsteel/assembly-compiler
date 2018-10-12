/*Author: Ben Steel*/
/*zx256.h*/
/*header file for functions*/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int preprocess_get_line(char label[], char instruction[], FILE* fp);

int encode(char instruction[], unsigned char ibytes[]);

int find_char(char character, int start, int end, char string[]);

int zxenc_dump(FILE* inputfile, int write, FILE* outputfile);

void print_ibytes(unsigned char ibytes[], int nbytes, char label[], char instruction[], int write, FILE* outputfile);