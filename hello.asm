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
msg1 db "Please, enter a number from 1 to 99", 10
msg1_length equ $-msg1
msg2 db "You have entered number - "
msg2_length equ $-msg2

section .text
_start:         mov eax, 4
                mov ebx, 1
                mov ecx, msg1
                mov edx, msg1_length
                int 80h

                call read_from_keyboard
                add esp, 4

                call quit_from_program
                add esp, 4
