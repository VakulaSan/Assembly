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
;=============Send I2C STOP command==================== 
send_stop:
        push    temp
        ldi     temp,   1<<TWEN | 1<<TWINT | 1<<TWSTO
		sts     TWCR,   temp
        rcall   wait
		pop     temp
		ret		
;============Create a delay======================
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
;============Output image data to a display== 
display_img:
		push    temp
		push    YL
		push    YH
		push    temp
		in      YH,     SPH
		in      YL,     SPL
		
          rcall   send_start
		rcall   send_slaw
		ldi     temp,   0x22      
          push    temp
		rcall   send_command 
		pop     temp              ;   send set page command
		ldd     temp,   Y+9
          push    temp
          rcall   send_command      ;   send start page command
          pop     temp
          ldd     temp,   Y+12
		push    temp1 
		push    temp2
          push    temp3
		clr     temp1
		clr     temp2
		clr     temp3
		lsr     temp
		brcc    skip
          inc     temp1
skip:     lsr     temp
		brcc    skip1
		inc     temp1
skip1:    lsr     temp
          brcc    skip2
		inc     temp1
skip2:    dec     temp    
		
next:     ldd     temp1, Y+9
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

		ldd     temp,   Y+10      
		push    temp
		rcall   send_command     ;  send start colummn command
		pop     temp

		ldd     temp1,   Y+11
		dec     temp        
		add     temp,   temp1
		cpi     temp,   128
		brlo    sk
    	     ldi     temp,   127     
sk:		push    temp
		rcall   send_command       ;  send stop colummn command
		pop     temp
        
		rcall   send_start
		rcall   send_slaw
          
		ldd     ZH,     Y+8
		ldd     ZL,     Y+7
		ldd     r19,    Y+11
		ldd     r18,    Y+12
		lsr     r18
		lsr     r18
		lsr     r18
          cpi     r18,    0
		brne    stp
		ldi     r18,    1
stp:		mul     r18,    r19
		movw    r18,    r0
                    
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
;============

