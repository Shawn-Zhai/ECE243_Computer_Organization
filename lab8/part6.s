.define LED_ADDRESS 0x10
.define HEX_ADDRESS 0x20
.define SW_ADDRESS 0x30

counter:	mv r3, #0
start:	mv r4, =0xFF		//counter

SWset:	mvt r6, #SW_ADDRESS
		ld r5, [r6]			//load swich value
		mv r2, =0x1FF
		and r5, r2
		add r5, #1
		
bigLoop:	sub r5, #1			//decrement switch value
		mv r4, =0xFF

DelayLoop: 	sub r4, #1			//decrement fixed counter
		bne DelayLoop

		sub r5, #0
		bne bigLoop
		
LED:		mvt r6, #LED_ADDRESS
		st r3, [r6]
		add r3, #1

		mv r0, =0x200	//check max
		sub r0, r3
		beq counter

		b start
		
		