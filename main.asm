;======OLED Screen on SSD1306====
.device ATmega328P
.include "headers.inc"

		.cseg
          
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
		SendCommand  0xA7          ;Set normal inverse display
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
          clr     num_of_frames
		inc     num_of_frames  
		clr     inc_adresses
animation:
          mov     temp,    num_of_frames
          cpi     temp,    0
		breq    END		
		dec     num_of_frames
	     ldi     ZH,     high(array<<1)
		ldi     ZL,     low(array<<1)
		add     ZL,     inc_adresses
		inc     inc_adresses
		inc     inc_adresses
	     push    inc_adresses
		push    num_of_frames  
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
		pop     num_of_frames
		pop     inc_adresses
		SendStart
		SendSLAW 
		
		ldi     r18,    9         ; Set pages x = x-1. To set 8 pages you should set 9 e.g.
send:     dec     r18
          breq    animation
          ldi     r19,    129
lp:		dec     r19
          breq    send
          ldi     temp,   send_data
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

.include "Graphical_data.inc"		 