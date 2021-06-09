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
          rcall   send_start
		rcall   send_slaw
		;=======Send command page ...from - to
		ldi     temp,   0x22
          push    temp
		rcall   send_command 
		pop     temp              ;   send set page command
		ldi     temp,   @1
          push    temp
          rcall   send_command      ;   send start page command
          pop     temp

		push    temp1
		push    temp2
          push    temp3
		clr     temp1
		clr     temp2
		clr     temp3
		ldi     temp,   @4
		
		lsr     temp
		brcc    skip
          inc     temp1
skip:     lsr     temp
		brcc    skip1
		inc     temp1
skip1:    lsr     temp
          brcc    skip2
		inc     temp1
skip2:    ;cpi     temp1,  0
          ;brne    next
		dec     temp    
		
next:     ldi     temp1, @1
          add     temp,  temp1
		cpi     temp,  8
		brlo    next1
         	ldi     temp,  7	
next1:    push    temp
          rcall   send_command       ; send stop page command 
	     pop     temp
    
		pop     temp3
          pop     temp2
		pop     temp1
        
		ldi     temp,   0x21    ;  set colummn command 
		push    temp
          rcall   send_command
		pop     temp 

		ldi     temp,   @2       
		push    temp
		rcall   send_command     ;  send start colummn command
		pop     temp

		ldi     temp1,  @3-1        
		add     temp,   temp1
		cpi     temp,   128
		brlo    sk
    	     ldi     temp,   127     
sk:		push    temp
		rcall   send_command       ;  send stop colummn command
		pop     temp
        
		rcall   send_start
		rcall   send_slaw

		ldi     ZL,     LOW((@0)<<1)
		ldi     ZH,     HIGH((@0)<<1)
		push    ZL
		push    ZH
		ldi     temp,   @3
		push    temp
          ldi     temp,   @4
          push    temp
          rcall   send_img
		pop     temp
		pop     temp
		pop     ZH
		pop     ZL
.ENDMACRO              
send_img:
        push    r18
		push    r19
		push    temp
		push    temp1
        in      YH,     SPH
        in      Yl,     SPL
		ldd     ZH,     Y+9
		ldd     ZL,     Y+10
		ldd     r19,    Y+8
		ldd     r18,    Y+7
		lsr     r18
		lsr     r18
		lsr     r18
          cpi     r18,    0
		brne    stp
		ldi     r18,    1
stp:		mul     r18,    r19
		movw    r18,    r0
          

;         ldi     temp1,  0
; 		lsr     r19
;         ror     r18
; 		brcc    nxt
;         inc     temp1
; nxt:      lsr     r19
;           ror     r18
; 		brcc    nxt1
; 		inc     temp1
; nxt1:     lsr     r19
;           ror     r18
;           brcc    nxt2
; 		inc     temp1
; nxt2:     cpi     temp1,  0
;           breq    send_
; 		ldi     temp1,  0
; 		inc     r18
; 		adc     r19,    temp1
		 
          
send_:    subi    r18,    1
          sbci    r19,    0
          brcs    exit1
          
          ldi     temp,   send_data
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
          rcall   wait
		
        
		lpm     temp,   Z+
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
		rcall   wait
		rjmp    send_
		
exit1:    pop     temp1
          pop     temp
          pop     r19
          pop     r18
          ret