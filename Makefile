all: zxpre.exe zxenc.exe zxcc.exe

zxpre.exe: zxpremain.c zx256.c zx256.h
	gcc -std=c99 -Wall zxpremain.c zx256.c -o zxpre.exe
	
zxenc.exe: zxencmain.c zx256.c zx256.h
	gcc -std=c99 -Wall zxencmain.c zx256.c -o zxenc.exe

zxcc.exe: zxccmain.c zx256.c zx256.h
	gcc -std=c99 -Wall zxccmain.c zx256.c -o zxcc.exe
	
zxpre: zxpre.exe

zxenc: zxenc.exe

zxcc: zxcc.exe

test: zxpre.exe zxenc.exe zxcc.exe
	python tests/runtests.py