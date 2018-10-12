name: db "Hello world!\n"    ; message

      ; Print hello world
main: mov %a,0x04            ; sys_write
      mov %b,0x01            ; stdout
      mov %c,name            ; string to print
      mov %d,0x0d            ; length of string
      syscall                ; invoke sys call

      ; Clean exit
      mov %a,0x01            ; sys_exit
      syscall                ; invoke sys call
