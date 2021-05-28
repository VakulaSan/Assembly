;======OLED Screen on SSD1306====
.device ATmega328P
.include "headers.inc"
         
init:     Initialization
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
		