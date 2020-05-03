global 
section .bss
string resb 7
string_length equ $-string
string2 resb 7                          ; 

section .text
                push ebp                ;save the ebp 
                mov ebp, esp            ;get the link to esp. ebp stores just lik of its value
                mov dword [ebp-4], 0    ;set initial value to check if stack has any value
                xor edx, edx            ;edx:eax/ecx ; set 0 in edx because edx:eax will be the divisible
                mov eax, [ebp+8]        ;get the number transfered by the calling procedure
                mov ecx, 1000000        ;set the digit - YOU LATER CAN SET ANY NUMBER

division:       div ecx                 ;get the quotient of the number
                test eax, eax           ;check if the quotient zero
                jnz put_not_zero        ;if it is not so - push the value in stack - otherwise check the first value in stack 
                test dword [ebp-4], 0   ;check if initial value on stack is 0, it means we will not put the number in stack
                jnz push_in_stack       ;if first argument is not 0, push 0 in stack 0!+0 12...+0  
                jmp next_digit          ;if the first argument is 0 

put_not_zero:   test dword [ebp-4], 0   ;
                jnz push_in_stack
                mov [ebp-4], eax        ;clear the first initial value with normal value

push_in_stack:  push eax                ;save the digit in stack

next_digit:     cmp ecx, 10             ; check if divisor is 10 to finish the loop
                jne continue_division 
                push edx                ;if divisor is 10 we will not continue loop. We will put the remainder in stack 
                jmp read_to_create_string

continue_division:  
                mov ebx, edx            ;move the remainder to ebx to save it
                mov eax, ecx            ;decrement divisor by 10 lower
                xor edx, edx            ;zero the value of the edx, because edx is part of division operation 
                div 10  
                mov ecx, eax            ;set the new digit divisible
                mov eax, ebx            ;get the remainder for division operation 
                jmp division  

read_to_create_string:
                mov ecx, 0              ;calculate the length of the string
                cmp esp, ebp            ;check if we go to a start of the adress of procedure [return adress - 4] = ebp
                je create_ascii_symbols
                pop [string + ecx]      ;write the string from the end, because we have got reversed string in stack 123-> 321 from stack
                inc ecx                 ;!!!! we cannot get get correct eax, because we decrement it after pop operation, so we get eax-1  
                jmp read_to_create_string

create_ascii_symbols:
                [string - ecx]          ;start of the string
                [string + string_length];end of the string
                ;now you just have to display the string
                ;sorry, you have to add number 48 to create symbol ASCII

