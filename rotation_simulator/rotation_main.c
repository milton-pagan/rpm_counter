#include <msp430.h> 

#define LED BIT0
#define BUTTON BIT3
#define MAX_COUNT 4096
#define SHIFTS 3

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

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A(void) {
    P1OUT ^= LED;
    TA0CCR0 = count;
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT_2(void) {
    if(shifts == 0) {
        shifts = SHIFTS;
        count = MAX_COUNT;
    }

    else {
        count--;
        count >>= 1;
    }

    shifts--;
    P2IFG &= ~BUTTON;
}
