#include <msp430.h> 

#define LED BIT7
#define BUTTON1 BIT3 //start
#define BUTTON2 BIT7 //stop || led unit change
#define FREQ 4096

long signal_count, time, timer_count;

int measuring = 0;
int lastDigit = 0;

int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

    // Initialize 1.0 as input
    P1DIR &=  ~BIT0;
    P1IN &= ~BIT0;
    P1IE &= ~BIT0;
    P1IES |= BIT0;

    // Initialize 1.7 as output
    P1DIR |= LED;
    P1OUT &= ~LED;

    // Initialize 2.3 and 2.7 as pull-up inputs
    P2DIR &= ~BUTTON1 & ~BUTTON2;
    P2REN |= BUTTON1 | BUTTON2;
    P2OUT |= BUTTON1 | BUTTON2;
    P2IES |= BUTTON1 | BUTTON2;
    P2IE  |= BUTTON1 | BUTTON2;

    // Initialize timer 0A
    TA0CCR0 = 0;
    TA0CCTL0 |= CCIE;
    TA0CTL |= TASSEL__ACLK + ID_3 + MC__CONTINUOUS;

    // Initialize timer 1A
    TA1CCR0 = 0;
    TA1CCTL0 |= CCIE;
    TA1CTL |= TASSEL__ACLK + ID_3 + MC__UP;


    return 0;
}

// Timer A0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A_CC0_ISR(){
    timerCount += 0xFFFF;
}

// Timer A1 ISR
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A_CC0_ISR() {

}

//Input ISR
#pragma vector=PORT1_VECTOR
__interrupt void input_ISR() {

}

// Buttons ISR
#pragma vector=PORT2_VECTOR
__interrupt void Buttons_ISR() {
    if(P2IFG == (BUTTON1 | BUTTON2)) {
        P2IFG &= 0;
        return;
    }
    // BUTTON1 pressed
    if(P2IFG == BUTTON1) {
        P2IFG &= ~BUTTON1;
        if(measuring == 1){
            return;
        }
        measuring = 1;
        P1IE |= BIT0;
        signal_count = 0;

        return;
    }
    // BUTTON2 pressed
    else {
        P2IFG &= ~BUTTON2;

        if(measuring == 1){
            measuring = 0;
            return;
        }


    }
}
