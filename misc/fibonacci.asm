success: db "Fibonacci sequence stored in memory 0x01 to 0x0e\n"
main:    mov %a,0x00   ;moving 1 to a
         mov %b,0x01   ;moving 1 to b
		 mov %c,0x00   ;storing memory counter in c
		 mov (%c),%a
		 jmp store
adder:   add %b,%a     ;adding a to b
		 jmp store
store:   mov %a,(%c)
         add %c,0x01   ;interating counter
		 cmp %c,0x0e
		 jne print
		 mov (%c),%b   ;moving b to memory
		 jmp adder
print:   mov %a,0x04
         mov %b,0x01
		 mov %c,success
		 mov %d,0x0d
		 syscall