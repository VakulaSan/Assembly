.def   temp    = R16
.def   temp1   = R21
.def   temp2   = R22  
.def   temp3   = R23
.def   drem    = R15
.def   dres    = R16
.def   dd      = R16
.def   dv      = R17
.def   dcnt    = R18
.def   hours   = R23
.def   minutes = R24
.equ send_cmd  = 0x80
.equ send_data = 0xC0
.equ SLA_addr  = 0x3C		
.equ I2C_speed = 0x06
          .dseg
hours_:    .byte 1
minutes_:  .byte 1
dots:      .byte 1
seconds_:    .byte 1

          .cseg
          .org 0
          rjmp init
          .org 0x0016
          rjmp TIMER1_COMPA
          .org 0x001A
          rjmp TIMER1_OVF
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
send_command:                     ;1 parameter - FE 0x22 -command
        ldi      temp,   send_cmd
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
;============Clear display===================
clear_all_screen:
          rcall send_start
          rcall send_slaw
          ldi     temp,   0x20
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x00
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x21
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x00
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x7F
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x22
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x00
          push    temp
          rcall   send_command
          pop     temp
          ldi     temp,   0x07
          push    temp
          rcall   send_command
          pop     temp
          ldi     r18,    9             ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
clear_screen:
send1:    dec     r18
          breq    ext
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
ext:		ret        
;============Output image data to a display== 
display_img:                  ; address,start page, start column, width, height
          push    YL
          push    YH
          in      YH,     SPH
          in      YL,     SPL
          push    temp

          rcall   send_start
          rcall   send_slaw
          ldi     temp,   0x22      
          push    temp
          rcall   send_command 
          pop     temp              ;   send set page command
          ldd     temp,   Y+7
          push    temp
          rcall   send_command      ;   send start page command
          pop     temp
          ldd     temp,   Y+10
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
          
next:     ldd     temp1, Y+7
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

          ldd     temp,   Y+8      
          push    temp
          rcall   send_command     ;  send start colummn command
          pop     temp

          ldd     temp1,   Y+9
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
          
          ldd     ZH,     Y+6
          ldd     ZL,     Y+5
          ldd     r19,    Y+9
          ldd     r18,    Y+10
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
          ; address,start page, start column, width, height
exit1:    pop     temp
          pop     YH
          pop     YL
          ret
;============Divide
divide:   sub     drem,   drem
          ldi     dcnt,   9
dv1:      rol     dd
          dec     dcnt
          brne    dv2
          rjmp    exit2
dv2:      rol     drem
          sub     drem,   dv 
          brcc    dv3
          add     drem,   dv
          clc
          rjmp    dv1
dv3:      sec
          rjmp    dv1  
exit2:    ret
;------------Compare digits
compare_digits:
          cpi     dres,    0
          brne cp_dg1
          ldi     R17,   LOW(zero_big<<1)
          ldi     R18,   HIGH(zero_big<<1)
          rjmp    ext4
cp_dg1:   cpi     dres,    1
          brne cp_dg2
          ldi     R17,   LOW(one_big<<1)
          ldi     R18,   HIGH(one_big<<1)
          rjmp    ext4
cp_dg2:   cpi     dres,    2
          brne cp_dg3
          ldi     R17,   LOW(two_big<<1)
          ldi     R18,   HIGH(two_big<<1)
          rjmp    ext4
cp_dg3:   cpi     dres,    3
          brne cp_dg4
          ldi     R17,   LOW(three_big<<1)
          ldi     R18,   HIGH(three_big<<1)
          rjmp    ext4
cp_dg4:   cpi     dres,    4
          brne cp_dg5
          ldi     R17,   LOW(four_big<<1)
          ldi     R18,   HIGH(four_big<<1)
          rjmp    ext4
cp_dg5:   cpi     dres,    5
          brne cp_dg6
          ldi     R17,   LOW(five_big<<1)
          ldi     R18,   HIGH(five_big<<1)
          rjmp    ext4
cp_dg6:   cpi     dres,    6
          brne cp_dg7
          ldi     R17,   LOW(six_big<<1)
          ldi     R18,   HIGH(six_big<<1)
          rjmp    ext4
cp_dg7:   cpi     dres,    7
          brne cp_dg8
          ldi     R17,   LOW(seven_big<<1)
          ldi     R18,   HIGH(seven_big<<1)
          rjmp    ext4
cp_dg8:   cpi     dres,    8
          brne cp_dg9
          ldi     R17,   LOW(eight_big<<1)
          ldi     R18,   HIGH(eight_big<<1)
          rjmp    ext4
cp_dg9:   ldi     R17,   LOW(nine_big<<1)
          ldi     R18,   HIGH(nine_big<<1)
ext4:     ret 
;---------Timer 1 OVERflow Interrupt 
TIMER1_OVF:
          ldi     r30,  low(seconds_)
          ldi     r31,  high(seconds_)
          ld      temp,  Z
          inc     temp
          cpi     temp,  15
          brsh    nxt7
          ldi     r30,  low(seconds_)
          ldi     r31,  high(seconds_) 
          st      Z,    temp
          reti
nxt7:     clr    temp
          ldi     r30,  low(seconds_)
          ldi     r31,  high(seconds_) 
          st      Z,    temp

          ldi     r30,  low(hours_)
          ldi     r31,  high(hours_)
          ld      hours,  Z
          ldi     r30,  low(minutes_)
          ldi     r31,  high(minutes_)
          ld      minutes,  Z

          cpi     minutes,  60
          brlo    nxt4
          ldi     minutes,  0
          inc     hours   
nxt4:     cpi     hours,  23
          brlo    nxt5
          ldi     hours,  0   
nxt5:     mov     dd,     hours
          ldi     r30,  low(hours_)
          ldi     r31,  high(hours_) 
          st      Z,    hours
          ldi     dv,     10
          rcall   divide
          rcall   compare_digits
          ldi     temp,   48
          push    temp
          ldi     temp,   27
          push    temp
          ldi     temp,   0   
          push    temp
          ldi     temp,   1
          push    temp
          push    R18
          push    R17
          rcall   display_img
          pop     R17
          pop     R18
          pop     temp
          pop     temp
          pop     temp
          pop     temp
          mov     dres,    drem
          rcall   compare_digits
          ldi     temp,   48
          push    temp
          ldi     temp,   27
          push    temp
          ldi     temp,   27   
          push    temp
          ldi     temp,   1
          push    temp
          push    R18
          push    R17
          rcall   display_img
          pop     R17
          pop     R18
          pop     temp
          pop     temp
          pop     temp
          pop     temp
          ;-----------------
          mov     dd,     minutes
          inc     minutes
          ldi     r30,  low(minutes_)
          ldi     r31,  high(minutes_)
          st      Z,    minutes
          ldi     dv,     10
          rcall   divide
          rcall   compare_digits
          ldi     temp,   48
          push    temp
          ldi     temp,   27
          push    temp
          ldi     temp,   70   
          push    temp
          ldi     temp,   1
          push    temp
          push    R18
          push    R17
          rcall   display_img
          pop     R17
          pop     R18
          pop     temp
          pop     temp
          pop     temp
          pop     temp
          mov     dres,    drem
          rcall   compare_digits
          ldi     temp,   48
          push    temp
          ldi     temp,   27
          push    temp
          ldi     temp,   97   
          push    temp
          ldi     temp,   1
          push    temp
          push    R18
          push    R17
          rcall   display_img
          pop     R17
          pop     R18
          pop     temp
          pop     temp
          pop     temp
          pop     temp 
          reti     
;-----------------