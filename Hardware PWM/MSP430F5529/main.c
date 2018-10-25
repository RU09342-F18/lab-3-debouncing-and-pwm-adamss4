#include <msp430.h>
#include <macros.h>

/**
 * main.c
 */

void initButtons(void)
{
    B1DIR &= ~(B1); // set button 1 as input
    B1REN |= B1; // enable resistor on button 1
    B1OUT |= B1; // pull-up resistor on button 1
    B1IE |= B1; // enable interrupt on button 1
    B1IES |= B1; // set interrupt flag on falling edge
    B1IFG &= ~(B1); // clear interrupt flag

}

void initLEDS(void)
{
    P1DIR |= LED1; // sets led1 to output
    P1SEL |= LED1; // selects led1 to output timer signal
}


volatile unsigned int PWM = 500; // starting PWM
void  initTA0(void)
{
    TA0CCR0 = 1000 - 1; // 1kHz wave
    TA0CCTL1 = OUTMOD_7; // sets outmod to reset/set for pwm
    TA0CCR1 = PWM; // sets CCR1 to variable PWM
    TA0CTL = TASSEL_2 + MC_1+ TACLR; // sets timer A to up mode
}


int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;   // stop watchdog timer

    initButtons(); // initializes buttons with interrupt
    initLEDS(); // initializes buttons with interrupt
    initTA0(); // initializes timer A0

    __bis_SR_register(LPM0_bits + GIE); // enables low power mode 0

}

// button ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{

    __delay_cycles(500); // lazy deboune

    if(TA0CCR1 == 1100) // when pwm is 100%, resets pwm
    {
        TA0CCR1 = 0;
    }
    else
    {
        TA0CCR1 += 100; // increases pwm
    }

    B1IFG &= ~(B1);           // Clears interrupt on button
}
