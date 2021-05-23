.device ATmega328P
.def   temp       = R16
.def   graph_data = R20
.def   temp1      = R21
.def   temp2      = R22
.def   temp3      = R23
;.equ   time       = (16000000/     
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
		sts     UBRR0L,  temp
		ldi     temp,   1<<TXEN0
		sts     UCSR0B, temp     
.ENDMACRO 

.MACRO SendStart
          ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTA
		sts     TWCR,   temp
          rcall   wait 
.ENDMACRO      

.MACRO  SendSLAW
          ldi     temp,   @0 
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall   wait	   
.ENDMACRO          
		  

.MACRO  SendCommand
          ldi     temp,   0x80
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
          rcall   wait 
		ldi     temp,   @0
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall   wait
.ENDMACRO

 .MACRO SendStop
          ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTO
		sts     TWCR,   temp
          rcall   wait 
.ENDMACRO  

.MACRO ClearScreen
          ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTA
		sts     TWCR,   temp
          rcall   wait
		ldi     temp,   @0 
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		call   wait

		ldi     r18,    9             ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send1:    dec     r18
          breq    proceed
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
                 
.ENDMACRO           