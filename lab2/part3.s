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
		  BEQ     END
		  
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

END:      B       END             

TEST_NUM: .word   0x55555555
		  .word   0xffffffff
		  .word   0x00000001
		  .word   0x0  

ANUM1:    .word   0x55555555
ANUM2:	  .word   0xaaaaaaaa

          .end                            
