#include <msp430.h> 
#include "macros.h"

/**
 * main.c
 */

int shifts;
int count;

int main(void)
{
    shifts = SHIFTS;
    count = MAX_COUNT;

	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= LED;
	P1OUT &= ~LED;

	P2DIR &= ~BUTTON;
	P2REN |= BUTTON;
	P2OUT |= BUTTON;
	P2IES |= BUTTON;
	P2IE  |= BUTTON;

	TA0CTL |= TASSEL__ACLK | ID__8 | MC__UP;
	TA0CCTL0 |= CCIE;

	TA0CCR0 = count;

	_BIS_SR(CPUOFF + GIE);

	while(1) {}
}
