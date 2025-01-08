/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R1, #TEST_NUM   // load the data word ...
		  MOV     R5, #0		  // R5 will hold the ONES
		  MOV     R6, #0		  // R6 will hold the ZEROS
		  MOV     R7, #0		  // R7 will hold the ALTERNATE
		  
M_LOOP:   LDR     R2, [R1], #4    // into R2 and then increment R1
		  CMP     R2, #0
		  MVN     R4, R2		  // R2 for ONES, R4 for ZEROS
		  LDR     R3, #ANUM1
		  EOR     R8, R2, R3      // R8 for ALTERNATE
		  LDR     R3, #ANUM2
		  EOR     R9, R2, R3      // R9 for ALTERNATE
		  BEQ     DISPLAY
		  
		  BL      ONES
		  CMP     R5, R0
		  MOVLT   R5, R0
		  
		  BL      ZEROS
		  CMP     R6, R0
		  MOVLT   R6, R0
		  
		  BL      ALTERNATE
		  CMP     R7, R0
		  MOVLT   R7, R0
		  
		  MOV     R8, R9
		  
		  BL      ALTERNATE
		  CMP     R7, R0
		  MOVLT   R7, R0
		  
		  B		  M_LOOP
		  
ONES:     MOV     R0, #0          // R0 is the counter, reset to 0
LOOPO:    CMP     R2, #0          // loop until the data contains no more 1's
          MOVEQ   PC, LR             
          LSR     R3, R2, #1      // perform SHIFT, followed by AND
          AND     R2, R2, R3      
          ADD     R0, #1          // count the string length so far
          B       LOOPO
		  
ZEROS:    MOV     R0, #0          // R0 is the counter, reset to 0
LOOPZ:    CMP     R4, #0          // loop until the data contains no more 1's
          MOVEQ   PC, LR             
          LSR     R3, R4, #1      // perform SHIFT, followed by AND
          AND     R4, R4, R3      
          ADD     R0, #1          // count the string length so far
          B       LOOPZ

ALTERNATE:MOV     R0, #0          // R0 is the counter, reset to 0
LOOPA:    CMP     R8, #0          // loop until the data contains no more 1's
          MOVEQ   PC, LR             
          LSR     R3, R8, #1      // perform SHIFT, followed by AND
          AND     R8, R8, R3      
          ADD     R0, #1          // count the string length so far
          B       LOOPA

DISPLAY:  LDR     R8, =0xFF200020 // base address of HEX3-HEX0
		  MOV     R0, R5          // display R5 on HEX1-0
		  BL      DIVIDE          // ones digit will be in R0; tens
				                  // digit in R1
		  MOV     R9, R1          // save the tens digit
		  BL 	  SEG7_CODE
		  MOV 	  R4, R0  		  // save bit code
		  MOV 	  R0, R9 		  // retrieve the tens digit, get bit code

		  BL 	  SEG7_CODE
		  LSL 	  R0, #8
		  ORR 	  R4, R0
		  
		  MOV     R0, R6          // display R6 on HEX1-0
		  BL      DIVIDE          // ones digit will be in R0; tens
				                  // digit in R1
		  MOV     R9, R1          // save the tens digit
		  BL 	  SEG7_CODE
		  MOV 	  R3, R0  		  // save bit code
		  MOV 	  R0, R9 		  // retrieve the tens digit, get bit code

		  BL 	  SEG7_CODE
		  LSL 	  R0, #8
		  ORR 	  R3, R0
		  LSL     R3, #16
		  
		  ORR     R2, R3, R4
		  STR 	  R2, [R8] 		  // display the numbers from R6 and R5
		  
	 	  LDR R8, =0xFF200030 	  // base address of HEX5-HEX4
		  
		  MOV     R0, R7          // display R5 on HEX1-0
		  BL      DIVIDE          // ones digit will be in R0; tens
				                  // digit in R1
		  MOV     R9, R1          // save the tens digit
		  BL 	  SEG7_CODE
		  MOV 	  R4, R0  		  // save bit code
		  MOV 	  R0, R9 		  // retrieve the tens digit, get bit code

		  BL 	  SEG7_CODE
		  LSL 	  R0, #8
		  ORR 	  R4, R0
		  
		  STR     R4, [R8] 		  // display the number from R7
		  
		  B      END

DIVIDE:   MOV     R2, #0
CONT:     CMP     R0, #10
          BLT     DIV_END
          SUB     R0, #10
          ADD     R2, #1
          B       CONT
DIV_END:  MOV     R1, R2     	  // quotient in R1 (remainder in R0)
          MOV     PC, LR

SEG7_CODE:MOV  	  R1, #BIT_CODES
		  ADD  	  R1, R0 		  // index into the BIT_CODES "array"
		  LDRB    R0, [R1]		  // load the bit pattern (to be returned)
		  MOV     PC, LR

END:      B       END             

TEST_NUM: .word   0x55555555
		  .word   0xffffffff
		  .word   0x00000001
		  .word   0x0  

ANUM1:    .word   0x55555555
ANUM2:    .word   0xaaaaaaaa

BIT_CODES: .byte 0b00111111, 0b00000110, 0b01011011, 0b01001111, 0b01100110
		   .byte 0b01101101, 0b01111101, 0b00000111, 0b01111111, 0b01100111

          .end                            
