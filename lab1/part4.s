/* Program that converts a binary number to decimal */
           
           .text               // executable code follows
           .global _start
_start:
            MOV    R4, #N
            MOV    R5, #Digits  // R5 points to the decimal digits storage location
            LDR    R4, [R4]     // R4 holds N
            MOV    R0, R4       // parameter for DIVIDE goes in R0
			MOV    R8, #1000    // R8 holds Thousands Divisor
			MOV    R9, #100     // R9 holds Hundreds Divisor
			MOV    R10, #10     // R10 holds Tens Divisor
            BL     DIVIDE
            STRB   R6, [R5, #3] // Thousands digit is now in R6
            STRB   R3, [R5, #2] // Hundreds digit is in R3
			STRB   R2, [R5, #1] // Tens digit is in R2
			STRB   R0, [R5]     // Ones digit is in R0
			
END:        B      END

DIVIDE:     MOV    R6, #0       // Set R6 to 0
			MOV    R3, #0       // Set R3to 0
			MOV    R2, #0       // Set R2 to 0
			
THOUSANDS:  CMP    R0, R8
            BLT    HUNDREDS
            SUB    R0, R8
            ADD    R6, #1
            B      THOUSANDS
			
HUNDREDS:   CMP    R0, R9
			BLT    TENS
            SUB    R0, R9
            ADD    R3, #1
            B      HUNDREDS

TENS:       CMP    R0, R10
			BLT    DIV_END
            SUB    R0, R10
            ADD    R2, #1
            B      TENS

DIV_END:    MOV    PC, LR

N:          .word  9999         // the decimal number to be converted
Digits:     .space 4          // storage space for the decimal digits

            .end
