global write_on_display
extern msg1
extern msg1_length

section .text
write_on_display: mov eax, 4
                mov ebx, 1
                mov ecx, msg1
                mov edx, msg1_length
                int 80h
                ret               
