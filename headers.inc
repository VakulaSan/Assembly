.def   temp       = R16
.def   graph_data = R20
.def   temp1      = R21
.def   temp2      = R22
.def   temp3      = R23
.def   counter    = R7

          .cseg
		rjmp init
wait:     push    temp
check:    lds     temp,   TWCR
          sbrs    temp,   TWINT
	     rjmp    check
		lds     temp,   TWSR 
 		sts     UDR0,   temp
		pop     temp
          ret  
send_slaw:
          in      ZL,     SPL
		in      ZH,     SPH
		ldd     temp,   Z+3
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall wait
		ret
send_command:
       
          ldi     temp,   0x80
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
          rcall   wait 
		
		in      ZL,     SPL
		in      ZH,     SPH
		ldd     temp,   Z+3
		sts     TWDR,   temp
		ldi     temp,   1<<TWINT | 1<<TWEN
		sts     TWCR,   temp
          sbrs    temp,   TWINT
		rcall   wait
		ret 
send_start:
          ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTA
		sts     TWCR,   temp
          rcall   wait
		ret 
send_stop:
          ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTO
		sts     TWCR,   temp
          rcall   wait
		ret 

.MACRO Initialization
          ldi     temp,   HIGH(RAMEND)
          out     SPH,    temp
		ldi     temp,   LOW(RAMEND)
		out     SPL,    temp
		;-------I2C init
		ldi     temp,   0x0C
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
          rcall   send_start
.ENDMACRO      

.MACRO  SendSLAW
          ldi     temp,   @0
          push    temp
          rcall send_slaw
		pop     temp
.ENDMACRO   
.MACRO  SendCommand
          ldi     temp,   @0
		push    temp
		rcall   send_command
		pop     temp
 		
.ENDMACRO

.MACRO SendStop
          rcall   send_stop
.ENDMACRO  

.MACRO ClearScreen
clear_screen:
          rcall send_start
		ldi     temp,  0x78,
		push    temp
		rcall send_slaw
		pop     temp 
		ldi     r18,    9             ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send1:    dec     r18
          breq    END
          ldi     r19,    129
lp1:	     dec     r19
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

