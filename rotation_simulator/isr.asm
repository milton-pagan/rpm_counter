; isr.asm

 		.cdecls C,LIST,"msp430.h", "macros.h"

 		.text

 		.ref timer_count, arr_pntr, aux, index

;---------------------------------------------------------
;	TIMER ISR
;---------------------------------------------------------
TIMER0_A0_ISR
	xor.b #LED, &P1OUT
	mov timer_count, &TA0CCR0

	reti

;---------------------------------------------------------
;	BUTTON ISR
;---------------------------------------------------------
BUTTON_ISR

	incd index
	cmp aux, index
	jeq RESET_INDEX

	jmp END

RESET_INDEX:
	mov #0, index

END:
	mov arr_pntr, r5
	add index, r5

	mov @r5, timer_count			; Copy next rpm to timer_count
	bic.b #BUTTON, &P2IFG			; Clear interrupt flag

	reti

;------------------------------------------------------------------------------
;           Interrupt Vectors
;------------------------------------------------------------------------------
	.sect   TIMER0_A0_VECTOR		; Timer A0 ISR
	.short  TIMER0_A0_ISR
	.sect 	PORT2_VECTOR			; Button vector
	.short 	BUTTON_ISR

	.end
