.MACRO Initialization
          ldi     temp,   HIGH(RAMEND)
          out     SPH,    temp
		ldi     temp,   LOW(RAMEND)
		out     SPL,    temp
		;-------I2C init
		ldi     temp,   0x06	
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
        rcall send_slaw
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
		rcall send_slaw
		ldi     r18,    9             ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send1:    dec     r18
          breq    proceed
          ldi     r19,    129
lp1:	     dec     r19
          breq    send1
          ldi     temp,   send_data
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
          rcall wait
		ldi     temp,   0xFF
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
		rcall wait
		rjmp    lp1        
.ENDMACRO   

.MACRO Display_img ; address,start page, start column, width, height
          push    temp
		ldi     temp,   @4
		push    temp
		ldi     temp,   @3
		push    temp
		ldi     temp,   @2
		push    temp
		ldi     temp,   @1
		push    temp
		ldi     temp,   HIGH(@0<<1)
		push    temp
		ldi     temp,   LOW(@0<<1)
		push    temp
		rcall   display_img
		pop     temp
		pop     temp
		pop     temp
		pop     temp
		pop     temp
		pop     temp
		pop     temp 
.ENDMACRO              
