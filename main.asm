;======OLED Screen on SSD1306====
.device ATmega328P
.include "macro.inc"
.include "headers.inc"

          
          
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
          

; Display_img wind,0, 66, 59, 50; address,start page, start column, width, height
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


   

.include "Graphical_data.inc"		 