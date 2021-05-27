;======OLED Screen on SSD1306====
.device ATmega328P
.include "headers.inc"

		.cseg
		rjmp init
		wait:   push    temp
        check:  lds     temp,   TWCR
        sbrs    temp,   TWINT
		rjmp    check
		lds     temp,   TWSR 
 		sts     UDR0,   temp
		pop     temp
        ret 
		send_slaw:
        in      ZL,     SPL
		in      ZH,     SPH
		ldd     temp,   Z+1
		sts     TWDR,   temp
		ldi     temp,   1<<TWEN | 1<<TWINT
		sts     TWCR,   temp
		rcall wait
		ret	   
        SendSLAW_ 0x78
		
		  
init:   Initialization
		
		;-----Initialization of displa
		END: rjmp END