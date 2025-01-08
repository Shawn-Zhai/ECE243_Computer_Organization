			.global _start
_start:     
			MOV   R2, #0		   // Counter
			LDR   R3, =0xff200020  // HEX Display
			LDR   R0, =0xff200050  // KEY Reg
			LDR   R0, [R0]
			B     START
			
MAIN:		LDR   R3, =0xff200020  // HEX Display
			LDR   R0, =0xff200050  // KEY Reg
			LDR   R0, [R0]
			
     		CMP   R0, #1
			BEQ   KEY0
			
			CMP   R0, #2
			BEQ   KEY1
			
			CMP   R0, #4
			BEQ   KEY2
			
			CMP   R0, #8
			BEQ   KEY3
			
			B     MAIN

KEY0:		LDR   R0, =0xff200050  // WAIT
			LDR   R0, [R0]		   // Until
			CMP   R0, #0		   // Key
		    BNE   KEY0			   // Release
			MOV   R2, #0
			BL    SEG7_CODE
			STR   R0, [R3]
			B     MAIN

KEY1:		LDR   R0, =0xff200050  // WAIT
			LDR   R0, [R0]		   // Until
			CMP   R0, #0		   // Key
			BNE   KEY1			   // Release
			CMP   R2, #9
			ADDLT R2, #1
			BL    SEG7_CODE
			STR   R0, [R3]
			B     MAIN

KEY2:		LDR   R0, =0xff200050  // WAIT
			LDR   R0, [R0]		   // Until
			CMP   R0, #0		   // Key
			BNE   KEY2			   // Release
			CMP   R2, #0
			SUBNE R2, #1
			BL    SEG7_CODE
			STR   R0, [R3]
			B     MAIN

KEY3:		LDR   R0, =0xff200050  // WAIT
			LDR   R0, [R0]		   // Until
			CMP   R0, #0		   // Key
			BNE   KEY3			   // Release
START:		MOV   R1, #BLANK
			LDRB  R1, [R1]
			STR   R0, [R3]
			
KEY3WaitK:	LDR   R0, =0xff200050  // WAIT
			LDR   R0, [R0]		   // Until
			CMP   R0, #8		   // Key
			BEQ   KEY3             // Pressed
			CMP   R0, #0
			BEQ   KEY3WaitK		   
			
			B     KEY0

SEG7_CODE:  MOV   R1, #BIT_CODES
		    ADD   R1, R2 		  // index into the BIT_CODES "array"
	   	    LDRB  R0, [R1]		  // load the bit pattern (to be returned)
		    MOV   PC, LR
			
BIT_CODES: .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
		   .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111
		   
BLANK:     .byte 0b00000000
	
	