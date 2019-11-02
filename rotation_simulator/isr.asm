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

;---------------------------------------------------------
;	BUTTON ISR
;---------------------------------------------------------
BUTTON_ISR

	cmp #0, shifts
	jne DECREASE

	mov #SHIFTS, shifts
	mov #MAX_COUNT, count

	jmp END_ISR

DECREASE:
	rra count

END_ISR:
	dec shifts
	bic.b #BUTTON,&P2IFG

	reti

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
	.sect   TIMER0_A0_VECTOR		; Timer A0 ISR
	.short  TIMER0_A0_ISR
	.sect 	PORT2_VECTOR			; Button 2.3 ISR
	.short 	BUTTON_ISR

	.end
