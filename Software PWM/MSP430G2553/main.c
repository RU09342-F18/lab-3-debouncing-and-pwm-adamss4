#include <msp430.h> 
#include <macros.h>

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
    L2OUT &= ~(LED2); // selects led2 to output timer signal
}


volatile unsigned int PWM = 5000; // starting PWM
void  initTA0(void)
{
    TA0CCTL0 = CCIE; // enables interrupt on CCR0
    TA0CCR0 = 10000-1; // 1kHz wave
    TA0CTL = TASSEL_2 + MC_2 + TACLR; // uses SMCLK, stops timer, and clears
    TA0CCTL1 = CCIE; // enables interrupt on CCR1
    TA0CCR1 = PWM; // sets duty cycle
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // ueses SMCLK, sets timer A to up mode, and clears

}



/**
 * main.c
 */

int main(void)
{
	WDTCTL = WDTPW | WDTHOLD;	// stop watchdog timer

	initButtons(); // initializes buttons with interrupt
	initLEDS(); // initializes buttons with interrupt
	initTA0(); // initializes timer A0


	__bis_SR_register(LPM0_bits + GIE); // enables low power mode 0 and enables global interupt

}

// button ISR
#pragma vector=PORT1_VECTOR
__interrupt void Port_1(void)
{
    __delay_cycles(5000); // Debouncin'
    TA0CCR1 += 1000;
    TA0CTL = TASSEL_2 + MC_1 + TACLR; // stops and clears timer
    B1IFG &= ~(B1); // clears interrupt flag
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void Timer_A0(void)
{
    if(TA0CCR1 != 1000)
    {
        L1OUT &= ~(LED1); // turns off led
    }
    else if(TA0CCR1 == 1100)
    {
        TA0CCR1 = 0;
    }
    TA0CCTL1 &= ~(CCIFG); // clears CCR1 interrupt
}


