#include <msp430.h> 

#define LED BIT7
#define BUTTON1 BIT3 //start
#define BUTTON2 BIT7 //stop || LED unit change
#define FREQ 4096
#define TA1CMP 1024 // Control LED flash velocity

// Prototypes
void clear_global_variables();

// Global Variables
static long signal_count, time, timer_count, result;
int measuring = 0;

int result_digits[3];
unsigned int index, current_digit;

// Functions
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
    timer_count += 0xFFFF;
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
        TA1CCR0 = 0;
        P1OUT &= ~LED;
        measuring = 1;
        P1IE |= BIT0;
        clear_global_variables();
        TA0CCR0 = 1;
        return;
    }
    // BUTTON2 pressed
    else {
        P2IFG &= ~BUTTON2;
        if(measuring == 1){
            TA0CCR0 = 0;
            measuring = 0;
            P1IE &= ~BIT0;

            // Calculations
            timer_count += TA0R;
            time = timer_count/FREQ;
            // time to minutes
            time /= 60;

            result = signal_count / time;

            result_digits[2] = result % 10;
            result_digits[1] = (result / 10) % 10;
            result_digits[0] = (result / 100) % 10;
            current_digit = result_digits[index++];

            TA1CCR0 = TA1CMP;

            return;
        }

        // LED switch units
        else{
            if(index == 3) index = 0;
            current_digit = result_digits[index++];

        }

    }
}

void clear_global_variables(){
    signal_count = 0;
    time = 0;
    timer_count = 0;
    result = 0;
    index = 0;
    current_digit = 0;

}
