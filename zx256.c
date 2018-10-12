/*Author: Ben Steel*/
/*zx256.c*/
/*contains functions used by zx compiler programs*/

#include "zx256.h"

int find_char(char character, int start, int end, char char_string[])
{
	for (int i = start; i<end; i++)
	{
		if (char_string[i] == character)
		{
			return i;
		}
	}
	return -1;
}

int preprocess_get_line(char label[], char instruction[], FILE* inputfile)
{
	char string[256];
	int loc_nullbyte = 0;
	int loc_first_quo = 0;
	int loc_sec_quo = 0;
	int loc_semicolon = 0;
	int loc_colon = 0;
	int instr_start = -1;
	int instr_end = -1;
	int instr_plchldr = 0;
	for (int i = 0; i<10; i++)
	{
		label[i] = ' ';
	}
	for (int i = 0; i<256; i++)
	{
		instruction[i] = ' ';
	}
	if (fgets(string, 256, inputfile) == NULL)
	{
		return EOF;
	}
	loc_nullbyte = find_char('\0', 0, 256, string);  //looking for nullbyte char
	loc_first_quo = find_char('"', 0, loc_nullbyte, string);   //looking for open quote
	if (loc_first_quo != -1)  //checking for second quotation mark
	{
		loc_sec_quo = find_char('"', loc_first_quo+1, loc_nullbyte, string);
		if (loc_sec_quo == -1)
		{
			fprintf(stderr, "Unclosed string.\n");
			return 2;
		}
		for (int i = 0; i<=loc_nullbyte;i++)   //looking for semicolon where string to print
		{
			if ((i<loc_first_quo) && (i>loc_sec_quo) && (string[i] == ';'))
			{
				loc_semicolon = i;
				break;
			}
		}
	}
	else  //checking for semicolon where no string to print
	{
		loc_semicolon = find_char(';', 0, 256, string);
	}
	for (int i = 0; i<=9; i++)   //checking for colon
	{
		if (string[i] == ':')
		{
			loc_colon = i;
			break;
		}
	}
	if (loc_colon == 0)
	{
		loc_colon = -1;
	}
	else
	{
		if (find_char(':', loc_colon+1, loc_nullbyte, string) != -1)
		{
			fprintf(stderr, "Double label.\n");
			return 2;
		}
	}
	if (loc_colon != -1) //if valid colon
	{
		for (int i = 0; i<loc_colon; i++)
		{
			label[i] = string[i];
		}
		label[loc_colon] = '\0';
	}
	else
	{
		label[0] = '\0';
	}
	for (int i = loc_colon+1; i<loc_nullbyte; i++)  //looking for start of instruction
	{
		if ((string[i] == ';'))
		{
			return 0;
		}
		else if (string[i] != ' ')
		{
			instr_start = i;
			break;
		}
	}
	if (instr_start == -1)
	{
		return 0;
	}
	for (int i = instr_start; i<=instr_start+8; i++)   //recording instruction
	{
		if ((string[i] >= 'a') && (string[i] <= 'z'))
		{
			instruction[i-instr_start] = string[i];
		}
		else
		{
			if (i == instr_start)
			{
				return 0;
			}
			instruction[i-instr_start] = string[i];
			instr_end = i;
			break;
		}
	}
	if ((find_char(' ', 0, 6, instruction) == -1) && (loc_first_quo == -1))
	{
		instruction[7] = '\0';
		return 1;
	}
	if (loc_first_quo != -1)   //returning if full instruction is string
	{
		for (int i = loc_first_quo; i<=loc_sec_quo; i++)
		{
			instruction[i-loc_first_quo+3] = string[i];
		}
		instruction[loc_sec_quo-instr_start+1] = '\0';
		return 1;
	}
	else
	{
		if (instr_end != -1)
		{
			instr_plchldr = instr_end+1 - instr_start;
			for (int i = instr_end; i<loc_nullbyte; i++)
			{
				if ((string[i] >= '!') && (string[i] <= '~') && (string[i] != ';'))
				{
					instruction[instr_plchldr] = string[i];
					instr_plchldr++;
				}
				else if (loc_semicolon != -1)
				{
					if (i == loc_semicolon)
					{
						break;
					}
				}
			}
			instruction[instr_plchldr] = '\0';
			return 1;
		}
		else
		{
			return 0;
		}
	}
}



int encode(char instruction[], unsigned char ibytes[])
{
	char first_mem[5] = "";
	char second_mem[5] = "";
	char first_copy[5] = "";
	int l_per = 0;
	int r_per = 0;
	int l_x = 0;
	int r_x = 0;
	int l_br = 0;
	int r_br = 0;
	int l_q = 0;
	int r_q = 0;
	int newline_char = -1;
	int len = 64;
	int space_place = -1;
	int comma_place = -1;
	char *opcode[50];
	for (int i = 0; i<50; i++)
	{
		opcode[i] = "";
	}
	opcode[1] = "mov";
	opcode[4] = "add";
	opcode[5] = "sub";
	opcode[6] = "mul";
	opcode[7] = "div";
	opcode[8] = "and";
	opcode[9] = "or";
	opcode[10] = "xor";
	opcode[11] = "cmp";
	opcode[20] = "not";
	opcode[32] = "jmp";
	opcode[33] = "jle";
	opcode[34] = "jl";
	opcode[35] = "je";
	opcode[36] = "jne";
	opcode[37] = "jge";
	opcode[38] = "jg";
	opcode[48] = "syscall";
	opcode[49] = "db";
	
	const char *reg[8];
	reg[0] = "a";
	reg[1] = "b";
	reg[2] = "c";
	reg[3] = "d";
	reg[4] = "ip";
	reg[5] = "sp";
	reg[6] = "bp";
	reg[7] = "flags";
	
	ibytes[0] = 0;
	ibytes[1] = 0;
	ibytes[2] = 0;

	/*len = find_char('\0', 0, 256, instruction);*/
	space_place = find_char(' ', 0, len, instruction);   //finding the space
	char instr[space_place];
	if (space_place == -1)
	{
		ibytes[0] += 0x30;
		return 1;
	}
	else
	{
		for (int i = 0; i<space_place; i++)   //recording the instruction in instr
		{
			instr[i] = instruction[i];
		}
		for (int i = 0; i<=49; i++)  //finding hex for instruction in instr
		{
			if (!strcmp(instr, opcode[i]))
			{
				ibytes[0] += i;
				break;
			}
		}
	}
	if ((ibytes[0] >= 32) && (ibytes[0] < 40))  //method for simplest case unary instruction
	{
		l_x = find_char('x', space_place, len, instruction);
		if (l_x == -1)
		{
			return 2; //hopefully its a label
		}
		for (int i = l_x+1; i<len; i++)   //recording register
		{
			if (instruction[i] == ' ')
			{
				break;
			}
			else
			{
				first_mem[i-l_x-1] = instruction[i];
			}
		}
		ibytes[1] = (char)strtol(first_mem, NULL, 16);
		return 2;
	}
	else if (ibytes[0] == 20)
	{
		l_per = find_char('%', space_place, len, instruction);
		for (int i = l_per + 1; i<len; i++)   //recording register
		{
			if (instruction[i] == ' ')
			{
				break;
			}
			else
			{
				first_mem[i-l_per-1] = instruction[i];
			}
		}
		for (int i = 0; i<8; i++)
		{
			if (!strcmp(first_mem, reg[i]))
			{
				ibytes[1] += i;
				return 2;
			}
		}
		return 0;
		
	}
	else if (ibytes[0] < 16)   //case of binary instruction
	{
		comma_place = find_char(',', space_place, len, instruction);  //finding the comma
		l_per = find_char('%', space_place, comma_place, instruction);  //checking for percentage before the comma
		r_per = find_char('%', comma_place, len, instruction);   //checking for percentage after the comma
		l_x = find_char('x', space_place, comma_place, instruction);   //checking for hex code before comma
		r_x = find_char('x', comma_place, len, instruction);   //checking for hex code after comma
		l_br = find_char('(', space_place, comma_place, instruction);   //checking for indirect before comma
		r_br = find_char('(', comma_place, len, instruction);   //checking for indirect after comma
		if (l_per != -1)
		{
			for (int i = l_per + 1; i<comma_place; i++)    //recording first memory location
			{
				if (instruction[i] == ')')
				{
					break;
				}
				first_mem[i-l_per-1] = instruction[i];
			}
		}
		else if (l_x != -1)
		{
			for (int i = l_x + 1; i<comma_place; i++)    //recording first memory location
			{
				if (instruction[i] == ')')
				{
					break;
				}
				first_mem[i-l_x-1] = instruction[i];
			}
		}
		else
		{
			return 0;
		}
		strcpy(first_copy, first_mem);   //workaround due to weird vanishing memory bug
		if (r_per != -1)
		{
			for (int i = r_per + 1; i<len; i++)    //recording second memory location
			{
				if (instruction[i] == ')')
				{
					break;
				}
				second_mem[i-r_per-1] = instruction[i];
			}
		}
		else if (r_x != -1)
		{
			for (int i = r_x + 1; i<len; i++)    //recording second memory location
			{
				if (instruction[i] == ')')
				{
					break;
				}
				second_mem[i-r_x-1] = instruction[i];
			}
		}
		else
		{
			return 3;  //hopefully label is here, therefore will always be case 4
		}
		if ((l_per != -1) && (r_per != -1))  //checking for cases 1,2, and 3
		{
			if ((l_br == -1) && (r_br != -1))  //case 2
			{
				ibytes[1] += 64;
			}
			else if ((l_br != -1) && (r_br == -1))  //case 3
			{
				ibytes[1] += 128;
			}
			for (int i = 0; i<8; i++)
			{
				if (!strcmp(first_copy, reg[i]))
				{
					ibytes[1] += (i*8);
				}
			}
			for (int i = 0; i<8; i++)
			{
				if (!strcmp(second_mem, reg[i]))
				{
					ibytes[1] += i;
				}
			}
			return 2;
		}
		else   //checking for cases 4,5, and 6
		{
			ibytes[1] += 192;
			if ((l_br == -1) && (r_br != -1))  //case 5
			{
				ibytes[1] += 8;
			}
			else if ((l_br != -1) && (r_br == -1))   //case 6
			{
				ibytes[1] += 16;
				for (int i = 0; i<8; i++)
				{
					if (!strcmp(second_mem, reg[i]))
					{
						ibytes[1] += i;
					}
				}
				ibytes[2] = (char)strtol(first_copy, NULL, 16);
				return 3;
			}
			for (int i = 0; i<8; i++)
			{
				if (!strcmp(first_copy, reg[i]))
				{
					ibytes[1] += i;
				}
			}
			ibytes[2] = (char)strtol(second_mem, NULL, 16);
			return 3;
		}
	}
	else if (ibytes[0] == 49)
	{
		l_q = find_char('"', space_place, len, instruction);
		r_q = find_char('"', l_q + 1, len, instruction);
		newline_char = find_char('\\', l_q+1, r_q, instruction);
		if (newline_char != -1)
		{
			if (find_char('n', newline_char+1, r_q, instruction) != newline_char+1)
			{
				newline_char = -1;
			}
		}
		for (int i = l_q+1; i<=r_q; i++)
		{
			if (i == newline_char)
			{
				ibytes[i-l_q-1] = '\n';
				i++;
			}
			else
			{
				ibytes[i-l_q-1] = instruction[i];
			}
		}
		if (newline_char != -1)
		{
			return r_q-l_q-2;
		}
		else
		{
			return r_q-l_q-1;
		}
	}
	return 0;
}



int zxenc_dump(FILE* inputfile, int write, FILE* outputfile)
{
	struct LabelPosition
	{
		char label[9];
		int label_pos;
	} typedef LabelPos;
	LabelPos label_dictionary[32];
	char potential_label[9];
	char instruction[256];
	char hex[4];
	char instruction_dictionary[128][256];
	char instruction_edit[256];
	unsigned char ibytes[40] = {
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0,
		0,0,0,0,0,0,0,0,0,0};
	char *embed_label;
	int read;
	int current_bytes = 0;
	int label_len;
	int no_of_labels = 1;
	int no_of_instructions = 0;
	int nbytes;
	int pointer_index;
	while(1)
	{
		read = preprocess_get_line(potential_label, instruction, inputfile);
		if (read == EOF)
		{
			break;
		}
		else if (read == 2)
		{
			return 1;
		}
		else if (read == 1)
		{
			strcpy(instruction_dictionary[no_of_instructions], instruction);
			no_of_instructions++;
			label_len = strlen(potential_label);
			if (label_len > 0)
			{
				strcpy(label_dictionary[no_of_labels].label, potential_label);
				label_dictionary[no_of_labels].label_pos = current_bytes;
				no_of_labels++;
				/*printf("%d\n", label_dictionary[no_of_labels].label_pos);*/
			}
			nbytes = encode(instruction, ibytes);
			current_bytes += nbytes;
		}
	}
	for (int i = 0; i<no_of_labels; i++)      //rearranging labels in list so labels like psmall and small don't get mixed up
	{
		for (int j = i+1; j<no_of_labels; j++)
		{
			embed_label = strstr(label_dictionary[j].label, label_dictionary[i].label);
			if (!(!embed_label))
			{
				LabelPos temp = label_dictionary[i];
				label_dictionary[i] = label_dictionary[j];
				label_dictionary[j] = temp;
			}
		}
	}
	for (int i = 0; i<no_of_instructions; i++)      //replacing labels in code with addresses
	{
		for (int j = 1; j<no_of_labels; j++)
		{
			embed_label = strstr(instruction_dictionary[i], label_dictionary[j].label);
			if (!(!embed_label) && (find_char('"', 0, 4, instruction_dictionary[i]) == -1))
			{
				pointer_index = embed_label - instruction_dictionary[i];
				sprintf(hex, "0x%02x", label_dictionary[j].label_pos);
				strcpy(instruction_edit, instruction_dictionary[i]);
				for (int i = 0; i<=4; i++)
				{
					if (i != 4)
					{
						instruction_edit[pointer_index+i] = hex[i];
					}
					else
					{
						instruction_edit[pointer_index+i] = '\0';
					}
				}
				strcpy(instruction_dictionary[i], instruction_edit);
			}
		}
	}
	current_bytes = 0;
	for (int i = 0; i<no_of_instructions; i++)
	{
		potential_label[0] = '\0';
		for (int j = 0; j<no_of_labels; j++)
		{
			if (current_bytes == label_dictionary[j].label_pos)
			{
				strcpy(potential_label, label_dictionary[j].label);
			}
		}
		nbytes = encode(instruction_dictionary[i], ibytes);
		current_bytes += nbytes;
		print_ibytes(ibytes, nbytes, potential_label, instruction_dictionary[i], write, outputfile);
	}
	for (int i = 0; i<no_of_labels; i++)
	{
		if (!strcmp("main", label_dictionary[i].label))
		{
			potential_label[0] = '\0';
			strcpy(instruction, "main\0");
			nbytes = 1;
			ibytes[0] = label_dictionary[i].label_pos;
			print_ibytes(ibytes, nbytes, potential_label, instruction, write, outputfile);
		}
	}
	return 0;
}




void print_ibytes(unsigned char ibytes[], int nbytes, char label[], char instruction[], int write, FILE* outputfile)
{
	if (write == 0)
	{
		int byte_counter = nbytes;
		int label_len = 0;
		for (int i = 0; (i<4) && (i<nbytes); i++)
		{
			fprintf(stdout, "0x%02x ", ibytes[i]);
		}
		if (nbytes < 4)
		{
			for (int i = 0; i<(5*(4-nbytes)); i++)
			{
				fprintf(stdout, " ");
			}
		}
		fprintf(stdout, " ; ");
		label_len = strlen(label);
		if (label_len > 0)
		{
			fprintf(stdout, "%s", label);
		}
		for (int i = 0; i<10-label_len; i++)
		{
			fprintf(stdout, " ");
		}
		fprintf(stdout, "%s\n", instruction);
		byte_counter -= 4;
		while (byte_counter > 0)
		{
			for (int i = 0; (i<4) && (i<byte_counter); i++)
			{
				fprintf(stdout, "0x%02x ", ibytes[i+(nbytes-byte_counter)]);
			}
			fprintf(stdout, "\n");
			byte_counter -= 4;
		}
	}
	else
	{
		for (int i = 0; i<nbytes; i++)
		{
			if (fwrite(&ibytes[i], 1, 1, outputfile) != 1)
			{
				fprintf(stderr, "Unable to write bytes to file.");
			}
		}
	}
}