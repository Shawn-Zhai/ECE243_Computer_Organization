			.global _start
_start:     
			MOV   	R7, #0		     // Counter
			LDR     R10, =0xfffec600 // Timer
			LDR     R11, =50000000   // Load Value
			STR     R11, [R10]
			MOV     R11, #0b011       // bit pattern (A = 1, E = 1)
			STR     R11, [R10, #0x8]
			
MAIN:		CMP 	R7, #100
			MOVEQ   R7, #0
			LDR     R2, =0xFF20005C   // EdgeBit
			LDR     R3, [R2]

DELAY:	 	LDR     R12, [R10, #0xc]  // Polling the F bit
			CMP     R12, #0
			BEQ     DELAY
			STR     R12, [R10, #0xc]
			
			CMP     R3, #0      // Check EdgeBit to determine whether stop
			BEQ     DISPLAY
	        MOV     R3, #15
			STR     R3, [R2]    // Reset EdgeBit to 0, display stops
			SUB     R7, #2
WaitStart:	LDR     R3, [R2]
			CMP     R3, #0
			BEQ     WaitStart   // Wait press and release to start
			MOV     R3, #15
			STR     R3, [R2]    // Reset EdgeBit to 0, display stops
			
DISPLAY:	MOV     R0, R7
			BL      DIVIDE
			
			MOV     R9, R1      // Save tens digit
			MOV     R2, R0
			BL		SEG7_CODE
			MOV     R5, R0      // R5 holds ones bitcode
			MOV 	R2, R9
			BL		SEG7_CODE
			MOV     R6, R0      // R6 holds tens bitcode
			
			LSL     R6, #8
			ORR     R5, R6
			
			LDR     R6, =0xFF200020 // base address of HEX3-HEX0
			STR     R5, [R6]
			
			ADD     R7, #1
			B       MAIN
			


DIVIDE:   	MOV     R1, #0
CONT:     	CMP     R0, #10
          	BLT     DIV_END
          	SUB     R0, #10
          	ADD     R1, #1
          	B       CONT
DIV_END:  	MOV     PC, LR     	  // tens in R1 , ones in R0

SEG7_CODE:	MOV  	R1, #BIT_CODES
		  	ADD  	R1, R2 		  // index into the BIT_CODES "array"
		  	LDRB    R0, [R1]		  // load the bit pattern (to be returned)
		  	MOV     PC, LR	

BIT_CODES: .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
		   .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111