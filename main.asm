;======OLED Screen on SSD1306====
.device ATmega328P
.include "headers.inc"

		.cseg
          
init:   Initialization
		SendStart
          SendSLAW 0x78
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
		SendCommand  0xA6          ;Set normal inverse display
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
		ClearScreen 0x78
proceed:
          ldi     R23,    2
		ldi     temp,   0
		mov     counter,temp
animation:
          
          ldi     temp,   0
		cpi     R23,    0
		breq    END		
		dec     R23
		ldi     ZH,     high(array<<1)
		ldi     ZL,     low(array<<1)
		add     ZL,     counter
		inc     counter
		inc     counter
          lpm     R26,    Z+
		lpm     R27,    Z
		ldi     temp,   2
		mul     R26,    temp
		mov     R26,    R0
		mov     R19,    R1
		mul     R27,    temp
		mov     R27,    R0
		add     R27,    R19
		mov     ZH,     R27
		mov     ZL,     R26
          rcall   delay
		SendStart
		SendSLAW 0x78
		ldi     r18,    9         ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send:     dec     r18
          breq    animation
          ldi     r19,    129
lp:		dec     r19
          breq    send
          ldi     temp,   0xC0
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
          rcall wait
		lpm     temp,   Z+
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT 
		sts     TWCR,   temp
		rcall wait
		rjmp    lp
		  	      

		END:    rjmp  END



        
wait:     lds     temp,   TWCR
          sbrs    temp,   TWINT
		rjmp    wait
		lds     temp,   TWSR 
		sts     UDR0,   temp
          ret
		  
		   
delay:    ldi     temp1,  2
lp0:      dec     temp1
          breq    exit
		ldi     temp2,  5
lp2:	     dec     temp2
		breq    lp0
		ldi     temp3,  255
lp3:	     dec     temp3 
		breq    lp2
		rjmp    lp3
exit:     ret 
.include "Graphical_data.inc"		 