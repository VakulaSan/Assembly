global _start   
global msg1 
global msg1_length
global number
extern write_on_display
extern read_from_keyboard
extern quit_from_program

section .bss
number resb 10

section .data 
msg1 db "Hello", 10
msg1_length equ $-msg1
msg2 db "You have entered number - "
msg2_length equ $-msg2

section .text
_start:         push 5
again:          call write_on_display
                
                              
                

               

                mov eax, 1
                mov ebx, 0
                int 80h
