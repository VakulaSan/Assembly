global 

section .text
                push ebp                ;save the ebp 
                mov ebp, esp            ;get the link to esp. ebp stores just lik of its value
                mov dword [ebp-4], 0    ;set initial value to check if stack has any value
                xor edx, edx            ;edx:eax/ecx ; set 0 in edx because edx:eax will be the divider
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

next_digit:     mov ebx, edx            ;move the remainder to ebx to save it
                mov eax, ecx            ;decrement divider by 10 lower
                div 10  
                mov ecx, eax            ;set the new digit 
                mov eax, ebx            ;get the remainder of divide operation 
                

