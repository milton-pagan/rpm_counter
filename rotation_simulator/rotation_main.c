#include <msp430.h> 
#include "macros.h"

/**
 * main.c
 */

int count;
double temp;
int rpm = 120;


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= LED;
	P1OUT &= ~LED;

	TA0CTL |= TASSEL__ACLK | ID__8 | MC__UP;
	TA0CCTL0 |= CCIE;

	temp = rpm / 60;
	temp = 1 / temp;
	count  = (int) (temp * (FREQ / 2));

	TA0CCR0 = count;

	_BIS_SR(CPUOFF + GIE);

	while(1) {}
}
