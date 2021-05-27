
.def   temp       = R16
.def   graph_data = R20
.def   temp1      = R21
.def   temp2      = R22
.def   temp3      = R23
.def   counter    = R7
.def   lengh_of_array = R10
;.equ   time       = (16000000/     
.MACRO Initialization
        ldi     temp,   HIGH(RAMEND)
        out     SPH,    temp
		ldi     temp,   LOW(RAMEND)
		out     SPL,    temp
		;-------I2C init
		ldi     temp,   0x07
		sts     TWBR,   temp
        ldi     temp,   0x00
		sts     TWSR,   temp
        ;-------UART init
		clr     temp
		sts     UBRR0H, temp
		ldi     temp,   0x67
		sts     UBRR0L, temp
		ldi     temp,   1<<TXEN0
		sts     UCSR0B, temp
		    
.ENDMACRO 

.MACRO SendStart
 send_start:
        ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTA
		sts     TWCR,   temp
        rcall   wait
		ret 
.ENDMACRO      

.MACRO  SendSLAW
send_slaw:
        in      ZL,     SPL
		in      ZH,     SPH
		ldd     temp,   Z+1
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall wait
		ret	   
.ENDMACRO          
		  

.MACRO  SendCommand
send_command:
       
        ldi     temp,   0x80
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
        rcall   wait 
		
		in      ZL,     SPL
		in      ZH,     SPH
		ldd     temp,   Z+1
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall   wait
		ret
.ENDMACRO

 .MACRO SendStop
send_stop:
        ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTO
		sts     TWCR,   temp
        rcall   wait
		ret 
.ENDMACRO  

.MACRO ClearScreen
clear_screen:
        rcall send_start
		rcall send_slaw 0x78
		ldi     r18,    9             ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send1:  dec     r18
        breq    END
        ldi     r19,    129
lp1:	dec     r19
        breq    send1
        ldi     temp,   0xC0
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
        rcall wait
		ldi     temp,   0x00
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
		rcall wait
		rjmp    lp1
        ret         
.ENDMACRO   
.MACRO Wait   
wait:   push    temp
check:  lds     temp,   TWCR
        sbrs    temp,   TWINT
		rjmp    check
		lds     temp,   TWSR 
 		sts     UDR0,   temp
		pop     temp
        ret  
.ENDMACRO		   
.MACRO SendStart_
        rcall send_start
.ENDMACRO
.MACRO SendSLAW_
       ldi     temp,   @0
	   push    temp
       rcall send_slaw
	   pop     temp
.ENDMACRO
