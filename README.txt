Author: Ben Steel
Candidate ID: 24561
Date last modified: 09/03/2016
Description: 
zxpre takes an assembly code file, removes comments and whitespace, does rudimentary searches for invalid code,
and then prints the cleaned up code to stdout

zxenc has two modes. it can a single line of assembly code and return the corresponding machine code,
or it can take an assembly code file and return the machine code in a readable way

zxcc takes an assembly code file and writes the corresponding machine code to an executable file

fibonacci calculates the first 14 fibonacci numbers and stores them in memory, and then prints its success

Instructions for compiling program:
running 'make' in the project directory compiles all c files at once
running 'make' and then the name of the program compiles that program only i.e. 'make zxpre'

Examples of use:
$ ./zxpre.exe tests/zxpre/hello.asm
name:    db "Hello world!\n"
main:    mov %a,0x04
         mov %b,0x01
         mov %c,name
         mov %d,0x0d
         syscall
         mov %a,0x01
         syscall
		 
$ ./zxenc.exe -c 'mov %a,(0x21)'
0x01 0xc8 0x21

$ ./zxenc.exe tests/zxpre/hello.asm
0x48 0x65 0x6c 0x6c  ; name      db "Hello world!\n"
0x6f 0x20 0x77 0x6f
0x72 0x6c 0x64 0x21
0x0a
0x01 0xc0 0x04       ; main      mov %a,0x04
0x01 0xc1 0x01       ;           mov %b,0x01
0x01 0xc2 0x00       ;           mov %c,0x00
0x01 0xc3 0x0d       ;           mov %d,0x0d
0x30                 ;           syscall
0x01 0xc0 0x01       ;           mov %a,0x01
0x30                 ;           syscall
0x0d                 ;           main

$ ./zxcc.exe tests/zxpre/hello.asm -o hello.exe
Success!

Changelog:
06/03/2016: completed zxpremain.c
07/03/2016: completed first mode of zxencmain.c
08/03/2016: completed second mode of zxencmain.c
09/03/2016: completed zxccmain.c
            completed fibonacci.asm