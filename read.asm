global read_from_keyboard
extern number

section .text 
read_from_keyboard: mov eax, 3
                mov ebx, 0
                mov ecx, number
                mov edx, 3
                int 0x80
                ret 
