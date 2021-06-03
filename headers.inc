.def   temp          = R16
.def   inc_adresses  = R9
.def   num_of_frames = R10
.def   temp1   = R21
.def   temp2   = R22
.def   temp3   = R23
.equ send_cmd  = 0x80
.equ send_data = 0xC0
.equ SLA_addr  = 0x3C		

          .cseg
		rjmp init
;=================================
wait:     push    temp
check:  lds     temp,   TWCR
        sbrs    temp,   TWINT
	    jmp   check
		pop     temp
        ret  
;=================================
send_slaw:
        push    temp         
        ldi     temp,   SLA_addr<<1
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall wait
		pop     temp
		ret
;=================================
send_command:
        ldi     temp,   send_cmd
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
        rcall   wait 
		
		push    ZL
		push    ZH
		push    temp
		in      ZL,     SPL
		in      ZH,     SPH	
		ldd     temp,   Z+6
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp	
        rcall   wait
		pop     temp
		pop     ZH
		pop     ZL
		ret 
;=================================
send_start:
        push    temp
        ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTA
		sts     TWCR,   temp
        rcall   wait
		pop     temp
		ret
;================================= 
send_stop:
        push    temp
        ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTO
		sts     TWCR,   temp
        rcall   wait
		pop     temp
		ret		

delay:  push    temp1
        push    temp2
		push    temp3

        ldi     temp1,  17
lp0:    dec     temp1
        breq    exit
		ldi     temp2,  255
lp2:	dec     temp2
		breq    lp0
		ldi     temp3,  255
lp3:	dec     temp3 
		breq    lp2
		rjmp    lp3
exit:   pop     temp3
        pop     temp2
		pop     temp1
        ret
		;============== 

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
		ldi     temp,  @0
		push    temp
		rcall send_slaw
		pop     temp
		ldi     r18,    9             ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send1:  dec     r18
        breq    proceed
        ldi     r19,    129
lp1:	dec     r19
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
exit1:    ret         
.ENDMACRO   
.MACRO Display_img ; address,start page, start column, width, height
        rcall   send_start
		rcall   send_slaw
		ldi     temp,   0x22
        push    temp
		rcall   send_command 
		pop     temp   ;   send set page command
		ldi     temp,   @1
        push    temp
        rcall   send_command    ;   send start page command
        pop     temp

		push    temp1
		push    temp2
        push    temp3
		clr     temp1
		clr     temp2
		clr     temp3
		ldi     temp,   @4
		lsr     temp
		adc     temp1,  temp2   
		lsr     temp
		adc     temp1,  temp2
		lsr     temp            ; dividing by eight /8
        adc     temp1,  temp2
		ldi     temp3,  @1
		add     temp,   temp3   ; add start and divided value for end page
		cpi     temp1,  0
		breq    next
        inc     temp
next:     
		
        pop     temp2
		pop     temp1

.ENDMACRO
