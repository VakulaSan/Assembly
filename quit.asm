global quit_from_program

section .text 
quit_from_program:  mov eax, 1
                mov ebx, 0
                int 0x80
                ret
