global write_on_display
extern msg1
extern msg1_length

section .text
write_on_display: pop esi
again:          mov eax, 4
                mov ebx, 1
                mov ecx, msg1
                mov edx, msg1_length
                int 80h
                dec esi
                cmp esi, 0
                jne again
                xor esi, esi
                ret               
