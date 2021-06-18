;======OLED Screen on SSD1306====
.device ATmega328P
.include "headers.inc"
.include "macro.inc"
          
          
init:   Initialization
          SendStart
          SendSLAW 
          ;-----Initialization of display
          SendCommand  0xA8          ;Set MUX ratio 3F
          SendCommand  0x3F
          SendCommand  0xD3          ;Set display offset
          SendCommand  0x00
          SendCommand  0x40          ;Display start line 40-7F
          SendCommand  0xA0          ;Set Segment Re-map
          SendCommand  0xC0          ;Set COM port output Scan detection
          SendCommand  0xDA          ;Set COM Pins Hardware Configuration
          SendCommand  0x12
          SendCommand  0x81          ;Set contrast control 00-FF
          SendCommand  0xFF
          SendCommand  0xA4          ;Entire display ON
          SendCommand  0xA7         ;Set normal inverse display
          SendCommand  0xD5          ;Set display clock divide ratio/Oscillator frequency
          SendCommand  0x80
          SendCommand  0x8D          ;Charge pump command table
          SendCommand  0x14
          
          SendCommand  0x20;       
          SendCommand  0x00
          SendCommand  0x21;
          SendCommand  0x00
          SendCommand  0x7F
          SendCommand  0x22;
          SendCommand  0x00
          SendCommand  0x07
          
          SendCommand  0xAF
          ;------Send DATA AND  !!!DON'T SEND STOP!!! IT WON'T WORK!!!
          ClearAllScreen

          ldi     temp,    1<<PC0 |1<<PC3
          out     DDRC,    temp
          ldi     temp,    1<<COM1A0
          sts     TCCR1A, temp
          ldi     temp,    1<<CS12 | 1<<CS10 
          sts     TCCR1B, temp
          ldi     temp,   0x7f
          sts     OCR1AH, temp
          ldi     temp,   0xFF
          sts     OCR1AL, temp
          ldi     temp,   1<<OCIE1A | 1<<TOIE1
          sts     TIMSK1, temp
          

;Display_img wind,0, 66, 59, 50; address,start page, start column, width, height
Display_img zero_big,1, 0, 27, 48
Display_img zero_big,1, 27, 27, 48
Display_img doubledot_big,2, 57, 10, 32
Display_img zero_big,1, 70, 27, 48
Display_img zero_big,1, 97, 27, 48
                    
          ldi     temp,  0xFF
          out     DDRC, temp
          ldi     dd,   8
          ldi     dv,   1
          rcall   divide
          out     PORTC, dres
          sei


END:      rjmp  END
;--------Timer1 COF interrupts
TIMER1_COMPA:
          SendCommand  0x21;
          SendCommand  57
          SendCommand  67
          SendCommand  0x22;
          SendCommand  2
          SendCommand  5
          ldi     r30,  low(dots)
          ldi     r31,  high(dots)
          ld      R25,  Z
          cpi     R25,   0
          brne show_dots
          ldi     R25,   1
          ldi     r30,  low(dots)
          ldi     r31,  high(dots)
          st      Z,    R25
          rcall clear_screen
          rjmp ext_int
 show_dots:  
          ldi     R25,  0
          ldi     r30,  low(dots)
          ldi     r31,  high(dots)
          st      Z,    R25
          Display_img doubledot_big,2, 57, 10, 32
 ext_int: reti
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

.include "Graphical_data.inc"		 