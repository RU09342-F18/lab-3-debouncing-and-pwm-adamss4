#include <msp430.h> 


/**
 * main.c
 */
int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer
	
	P1SEL = 0; // sets pin 1 to GPIO
	P1DIR |= BIT0; // set port 1.0 to output
	P1DIR &= ~(BIT3); // set port 1.3 to input
	P1OUT &= ~(BIT0); // led 1 starts as off

P

	BCSCTL3 = LFXT1S_2; // switches A clock to internal oscillator
	TACCTL0 = CCIE; // enables interrupt on timer A0
	TACCR0 = 1200; // clock counts up to 1200 for debouncing
	TACTL = TASSEL_1 + MC_0 + TACLR; // Sets timer to 0 with ACLK as source, timer is stopped

	__bis_SR_register(LPM0_bits + GIE); // sets low power mode 0, +and enables global interrupt
	// CPU and MCLK are disabled, SMCLK and ACLK are active
}
 // button ISR
#pragma vector = PORT1_VECTOR
__interrupt void Port_1(void)
{
    P1OUT ^= BIT0; // toggles led on and off
    P1IE &= ~(BIT3); // Disables interrupt on port 1.3 so the timer can debounce
    P1IFG &= ~(BIT3); // clears interrupt flag
    TACTL |= BIT4; // sets timer to up mode
}

// timer ISR
#pragma vector = TIMER0_A0_VECTOR
__interrupt void Timer_A(void)
{
    P1IE |= BIT3; // enables interrupt on port 1.3
    TACTL &= ~(BIT4); // stops timer
    TACTL |= BIT2; // reset the clock
    P1IFG &= ~(BIT3); // clears interrupt flag
}
