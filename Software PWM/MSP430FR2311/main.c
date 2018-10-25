#include <msp430.h> 
#include <macros.h>

void initButtons(void)
{
    B1DIR &= ~(B1); // set button 1 as input
    B1REN |= B1; // enable resistor on button 1
    B1OUT |= B1; // pull-up resistor on button 1
    B1IE |= B1; // enable interrupt on button 1
    B1IES |= B1; // trigger flag on negative edge
    B1IFG &= ~(B1); // clear interrupt flag

}

void initLEDS(void)
{
    L2DIR = LED2; // sets led2 to output
}


void  initTB0(void)
{
    TB0CCTL0 |= CCIE; // enables interrupt on CCR0
    TB0CCR0 = 10000-1; // 1 kHz frequency
    TB0CCTL1 |= CCIE; // enables interrupt on CCR1
    TB0CCR1 = 5000; // starts at 50% duty cycle
    TB0CTL = TBSSEL_2 + MC_1; // uses SMCLK, sets to up mode
}

void initTB1(void) // timer for debouncing
{
    TB1CCTL0 = CCIE; // enables interrupt on CCR1
    TB1CCR0 = 60000; // sets clock period
    TB1CTL = TBSSEL_2 + ID_3 + MC_1 + TBCLR; // uses SMCLK, divides clock by 8, sets timer A to up mode, and clears
}



/**
 * main.c
 */

int main(void)
{
    HIGHZ;
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer
    initButtons(); // initializes buttons with interrupt
    initLEDS(); // initializes buttons with interrupt
    initTB0(); // initializes timer B0
    initTB1(); // initializes timer B1


    __bis_SR_register(LPM0_bits + GIE); // enables low power mode 0 and enables global interupt

}

// button ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    B1IE &= ~(B1); // disable interrupt on button 1
    TB1CTL |= TBSSEL_2 + MC_1 + TBCLR; // sets timer to up mode and clears timer
    TB0CCR1 += 1000; // increments CCR1 by 100 for 10% duty cycle increase
    if (TB0CCR1 == 11000) // resets CCR1 to 0 when max pwm exceeded
    {
        TB0CCR1 = 0;
    }
    B1IFG &= ~(B1); // clears interrupt flag
}
// ISR for CCTL0
#pragma vector=TIMER0_B0_VECTOR
__interrupt void Timer_B0(void)
{
    L2OUT |= LED2; // turns on led2
    TB0CCTL0 &= ~(CCIFG); // clears interrupt flag on ccr0
}
//ISR for LED PWM and CCTL1
#pragma vector=TIMER0_B1_VECTOR
__interrupt void TimerB1(void)
{
    L2OUT &= ~LED2; // turns off led2
    TB0CCTL1 &= ~(CCIFG); // clears interrupt flag on ccr1
}
//ISR for debounce timer
#pragma vector=TIMER1_B0_VECTOR
__interrupt void Timer_1B1(void)
{
    TB1CTL = MC_0; // stops timer B1
    B1IE |= B1; // enables interrupt on button 1
    TB1CCTL0 &= ~(CCIFG); // clears interrupt flag on Timer B1

}

