.define LED_ADDRESS 0x10
.define HEX_ADDRESS 0x20
.define SW_ADDRESS 0x30

		mv r5, =0x100	

counter:	mv r3, #0
start:	mv r1, =0x1		//counter

SWset:	mvt r6, #SW_ADDRESS
		ld r4, [r6]			//load swich value
		mv r2, =0x1FF
		and r4, r2
		add r4, #1
		
bigLoop:	sub r4, #1			//decrement switch value
		mv r1, =0x1

DelayLoop: 	sub r1, #1			//decrement fixed counter
		bne DelayLoop

		sub r4, #0
		bne bigLoop
		
HEX:		mvt r2, #HEX_ADDRESS
		mv r0, r3

		//hex0
		mv r6, r7
		mv r7, #DIV10
		//mvt r6, #LED_ADDRESS
		//st r0, [r6]
		mv r4, #HEX7
		add r4, r0
		ld r4, [r4]		//r4 = bitcode
		st r4, [r2]
		add r2, #1
		mv r0, r1

		//hex1
		mv r6, r7
		mv r7, #DIV10
		mv r4, #HEX7
		add r4, r0
		ld r4, [r4]		//r4 = bitcode
		st r4, [r2]
		add r2, #1
		mv r0, r1

		//hex2
		mv r6, r7
		mv r7, #DIV10
		mv r4, #HEX7
		add r4, r0
		ld r4, [r4]		//r4 = bitcode
		st r4, [r2]
		add r2, #1
		mv r0, r1

		//hex3
		mv r6, r7
		mv r7, #DIV10
		mv r4, #HEX7
		add r4, r0
		ld r4, [r4]		//r4 = bitcode
		st r4, [r2]
		add r2, #1
		mv r0, r1

		//hex4
		mv r6, r7
		mv r7, #DIV10
		mv r4, #HEX7
		add r4, r0
		ld r4, [r4]		//r4 = bitcode
		st r4, [r2]
		add r2, #1
		mv r0, r1

		add r3, #1
		mv r0, =0xFFFF	//check max
		sub r0, r3
		beq counter

		b start
		
	

DIV10:	
		sub r5, #1 		// save registers that are modified
		st r2, [r5] 	// save on the stack

		mv r1, #0 		// init Q
DLOOP: 	mv r2, #9 		// check if r0 is < 10 yet
		sub r2, r0
		bpl RETDIV 		// if so, then return

INC:		add r1, #1 		// but if not, then increment Q
		sub r0, #10 	// r0 -= 10
		b DLOOP 		// continue loop
RETDIV:
		ld r2, [r5] 	// restore from the stack
		add r5, #1
		add r6, #1 		// adjust the return address
		mv r7, r6 		// return results

HEX7:			.word 0b00111111			// '0'
			.word 0b00000110			// '1'
			.word 0b01011011			// '2' 	 
			.word 0b01001111			// '3'
			.word 0b01100110			// '4'  
			.word 0b01101101			// '5'
			.word 0b01111101			// '6'
			.word 0b00000111			// '7'
			.word 0b01111111			// '8' 
			.word 0b01101111			// '9' 