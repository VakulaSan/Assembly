.MACRO Initialization
          ldi     temp,   HIGH(RAMEND)
          out     SPH,    temp
          ldi     temp,   LOW(RAMEND)
          out     SPL,    temp
          ;-------I2C init
          ldi     temp,   I2C_speed	
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
          ;-------Timer/Counter0 initialization 
          ldi     r30,  low(hours_)
          ldi     r31,  high(hours_)
          ldi     temp,  0
          st      Z,     temp
          ldi     r30,  low(minutes_)
          ldi     r31,  high(minutes_)
          ldi     temp,  0
          st      Z,     temp
          ldi     r30,  low(dots)
          ldi     r31,  high(dots)
          ldi     temp,  1
          st      Z,    temp
          ldi     r30,  low(seconds_)
          ldi     r31,  high(seconds_)
          ldi     temp,  0
          st      Z,    temp

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

.MACRO ClearAllScreen 
         rcall   clear_all_screen
.ENDMACRO   

.MACRO Display_img ; address,start page, start column, width, height
          push    temp
          ldi     temp,   @4
          push    temp
          ldi     temp,   @3
          push    temp
          ldi     temp,   @2
          push    temp
          ldi     temp,   @1
          push    temp
          ldi     temp,   HIGH(@0<<1)
          push    temp
          ldi     temp,   LOW(@0<<1)
          push    temp
          rcall   display_img
          pop     temp
          pop     temp
          pop     temp
          pop     temp
          pop     temp
          pop     temp
          pop     temp 
.ENDMACRO              

