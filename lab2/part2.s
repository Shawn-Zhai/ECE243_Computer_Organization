/* Program that counts consecutive 1's */

          .text                   // executable code follows
          .global _start                  
_start:                             
          MOV     R1, #TEST_NUM   // load the data word ...
		  MOV     R5, #0		  // R0 will hold the result
		  
M_LOOP:   LDR     R2, [R1], #4    // into R2 and then increment R1
		  CMP     R2, #0
		  BEQ     END
		  BL      ONES
		  CMP     R5, R0
		  MOVLT   R5, R0
		  B		  M_LOOP
		  
ONES:     MOV     R0, #0          // R0 is the counter, reset to 0
LOOP:     CMP     R2, #0          // loop until the data contains no more 1's
          MOVEQ   PC, LR             
          LSR     R3, R2, #1      // perform SHIFT, followed by AND
          AND     R2, R2, R3      
          ADD     R0, #1          // count the string length so far
          B       LOOP            

END:      B       END             

TEST_NUM: .word   0x00000001
		  .word   0x00000003
		  .word   0x00000007  
		  .word   0x0000000f  
		  .word   0x0000001f  
		  .word   0x0000003f  
		  .word   0x0000007f 
		  .word   0x00000fff  
		  .word   0x103fe00f  
		  .word   0x000007ff 
		  .word   0x0  

          .end                            
