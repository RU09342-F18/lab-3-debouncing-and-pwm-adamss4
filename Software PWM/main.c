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
    L2DIR |= LED2; // sets led2 to output
    P1SEL |= LED2; // selects led2 to output timer signal
}


volatile unsigned int PWM = 500; // starting PWM
void  initTA0(void)
{
    TACCR0 = 1000-1; // timer A0 counts to 999 to make a 1kHz wave
    TACCR1 = PWM;
    TACCTL1 = OUTMOD_7;
    TACTL = TASSEL_2 + MC_1; // sets timer A to up mode
}


int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	initButtons(); // initializes buttons with interrupt
	initLEDS(); // initializes buttons with interrupt
	initTA0(); // initializes timer A0

	__bis_SR_register(LPM0_bits + GIE); // enables low power mode 0

}

// button ISR
#pragma vector=PORT1_VECTOR         // Button interrupt
__interrupt void Port_1(void)
{

    __delay_cycles(5000);          // Debouncin'
    TACCR1 += 100;               // Increases PWM

    if(TACCR1 == 1100)
    {        // Resets PWM
        TACCR1 = 0;
    }

    B1IFG &= ~(B1);           // Clears interrupt on button
}
