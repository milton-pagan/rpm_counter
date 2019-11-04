#include <msp430.h> 

#define LED BIT7
#define BUTTON1 BIT3 //start
#define BUTTON2 BIT7 //stop || LED unit change
#define FREQ 4096
#define TA1CMP 2048 // Control LED flash velocity

// Prototypes
void clear_global_variables();

// Global Variables
static long signal_count, time, timer_count;
int measuring = 0;

int result_digits[3];
unsigned int index, current_digit, timer_digit, timer_delay, delay_on, result;

// Functions
void main()
{
    WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

    PM5CTL0 &= ~LOCKLPM5;

    clear_global_variables();

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

    // Initialize timer A0
    TA0CCR0 = 0;
    TA0CCTL0 |= CCIE;
    TA0CTL |= TASSEL__ACLK + ID_3 + MC__CONTINUOUS;

    // Initialize timer A1
    TA1CCR0 = 0;
    TA1CCTL0 |= CCIE;
    TA1CTL |= TASSEL__ACLK + ID_3 + MC__UP;

    _BIS_SR(CPUOFF + GIE);

    while(1);
}

// Timer A0 ISR
#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer0_A_CC0_ISR(){
    timer_count += 0xFFFF;
}

// Timer A1 ISR
#pragma vector=TIMER1_A0_VECTOR
__interrupt void Timer1_A_CC0_ISR() {
    if(!delay_on && timer_digit == 0) {
        timer_digit = 2 * current_digit;
        P1OUT &= ~LED;
    }

    if(delay_on && timer_delay == 0) {
        P1OUT |= LED;
        timer_delay = 6; // Changed from 20 -Dionel
    }

    if(timer_delay == 0) {
        P1OUT ^= LED;

        if(timer_digit - 1 == 0) delay_on = 1;
        timer_digit--;
    }

    if(delay_on && timer_delay != 0) {

        if(timer_delay - 1 == 0) delay_on = 0;
        timer_delay--;
    }

    TA1CCR0 = TA1CMP;
}

//Input ISR
#pragma vector=PORT1_VECTOR
__interrupt void input_ISR() {
    signal_count++;
    P1IFG &= ~BIT0;
}

// Buttons ISR
#pragma vector=PORT2_VECTOR
__interrupt void Buttons_ISR() {

    long temp = TA0R;

    // Both buttons pressed
    if(P2IFG == (BUTTON1 | BUTTON2)) {
        P2IFG &= 0;
        return;
    }
    // BUTTON1 pressed
    if(P2IFG == BUTTON1) {
        P2IFG &= ~BUTTON1;

        if(measuring){
            return;
        }
        TA1CCR0 = 0;
        P1OUT &= ~LED;
        measuring = 1;
        clear_global_variables();
        P1IE |= BIT0;
        TA0CCR0 = 1;
        return;
    }
    // BUTTON2 pressed
    else {
        P2IFG &= ~BUTTON2;
        if(measuring){
            P1IE &= ~BIT0;
            TA0CCR0 = 0;
            measuring = 0;

            // Calculations
            timer_count += temp;
            time = timer_count/FREQ;
            double aux = signal_count / time;
            aux *= 60;

            result = (int) aux;
            //            result = 123;

            result_digits[2] = result % 10;
            result_digits[1] = (result / 10) % 10;
            result_digits[0] = (result / 100) % 10;

            current_digit = result_digits[0];

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
    timer_digit = 0;
    delay_on = 0;

}
