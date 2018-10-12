small:    db "too small\n"
big:      db "big enough\n"

          ; Compute 1 + 2 + 3 + 4 + 5
main:     mov %a,0x00             ; total = 0
          mov %b,0x01             ; i = 1
lbegin:   add %a,%b              ; total += i
          add %b,0x01             ; i++
          cmp %b,0x05             ; if(b < 5)
          jl  lbegin             ;   goto lbegin

          ; Print appropriate mesage
          mov %a,0x04             ; sys_write
          mov %b,0x01             ; stdout
          cmp %a,0x0b             ; if(total <= 11)
          jle small              ;    goto psmall
          mov %c,big             ; big string
          mov %d,0x0b             ; length 11
          jmp print              ; goto print
psmall:   mov %c,small           ; small string
          mov %d,0x0a             ; length 10
print:    syscall                ; actually print!

          ; Clean exit
          mov %a,0x01             ; sys_exit
          syscall                ; invoke sys call
