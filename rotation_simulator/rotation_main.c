#include <msp430.h> 
#include "macros.h"

/**
 * main.c
 */

int timer_count;
int rpm_arr[5] = {60, 120, 180, 240, 300};
int arr_size = 5;

int* arr_pntr;
int index = 0;
int aux;

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	PM5CTL0 &= ~LOCKLPM5;

	// Set-up 1.0 as output
	P1DIR |= LED;
	P1OUT &= ~LED;

	// Set-up 2.3 as input
	P2DIR &= ~BUTTON;
	P2OUT |= BUTTON;
	P2REN |= BUTTON;
	P2IE |= BUTTON;
	P2IES |= BUTTON;

	// Set-up timer
	TA0CTL |= TASSEL__ACLK | ID__8 | MC__UP;
	TA0CCTL0 |= CCIE;

	// Initialize rpm values
	arr_pntr = &rpm_arr[0];

	aux = 2 * arr_size;         // Auxiliary variable to compare index

	int i;
	for(i = 0; i < arr_size; i++) {
	    rpm_arr[i] = (int) ((60.0 / rpm_arr[i]) * (FREQ / 2));
	}

	timer_count = rpm_arr[index];

	TA0CCR0 = timer_count;

	_BIS_SR(CPUOFF + GIE);

	while(1) {}
}
