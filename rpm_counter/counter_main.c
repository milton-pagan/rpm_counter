#include <msp430.h> 

#define LED BIT7
#define BUTTON1 BIT3 //start
#define BUTTON2 BIT7 //stop || led unit change
#define FREQ 4096

long count, time, timer_count;

void finalResult();
void initTimerA();
/**
 *
 * Dionel
 * Cuenta
 *
 * Milton
 * LED's
 */

//time = timer_count/freq

//final = count/time


/**
 * main.c
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

    P1DIR &=  ~BIT0;
    P1IN &= ~BIT0;

    P1DIR |= LED;
    P1OUT &= ~LED;
    P1IES |= BIT0;

    P2DIR &= ~BUTTON1 & ~BUTTON2;
    P2REN |= BUTTON1 | BUTTON2;
    P2OUT |= BUTTON1 | BUTTON2;
    P2IES |= BUTTON1 | BUTTON2;
    P2IE  |= BUTTON1 | BUTTON2;

    initTimer0A();
    TA0CCR0 = 1;
//    if(BUTTON2.isPressed()){
//        TA0CCR0 = 0;
//        timeCount += TA0R;
//    }


    return 0;
}

void initTimer0A(){
    TA0CCR0 = 0; //Initially, Stop the Timer
    TA0CCTL0 |= CCIE; //Enable interrupt for CCR0.
    TA0CTL |= TASSEL__ACLK + ID_3 + MC__CONTINUOUS; //Select TACLK, TACLK/1, Continuous Mode
}

void initTimer1A(){
    TA1CCR0 = 0; //Initially, Stop the Timer
    TA1CCTL0 |= CCIE; //Enable interrupt for CCR0.
    TA1CTL |= TASSEL__ACLK + ID_3 + MC__UP; //Select TACLK, TACLK/1, Continuous Mode
}


void finalResult(){

}

#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer0_A_CC0_ISR(){
    timerCount += 0xFFFF;
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A_CC0_ISR() {

}

