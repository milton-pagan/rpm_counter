; isr.asm

 		.cdecls C,LIST,"msp430.h", "macros.h"

 		.text

 		.ref shifts, count

;---------------------------------------------------------
;	TIMER ISR
;---------------------------------------------------------
TIMER0_A0_ISR
	xor.b #LED, &P1OUT
	mov count, &TA0CCR0

	reti

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
	.sect   TIMER0_A0_VECTOR		; Timer A0 ISR
	.short  TIMER0_A0_ISR

	.end
