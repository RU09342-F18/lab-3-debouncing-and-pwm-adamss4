#include "msp.h"


/**
 * main.c
 */
void main(void)
{
	WDT_A->CTL = WDT_A_CTL_PW | WDT_A_CTL_HOLD;		// stop watchdog timer
    P1SEL0 = 0; // selects pin 1 as gpio
    P1SEL1 = 0; // selects pin 1 as gpio
    P1DIR |= ~(BIT1); // selects port 1.1 as input (button)
    P1DIR |= BIT0; // set port 1.0 as output (ledr)

    P1REN |= BIT1; // enables pull-up resistor for port 1.1
    P1OUT |= BIT1; // sets port 1.1 to have pull-up resistor
    P1IE |= BIT1; // enable interrupt on port 1.1
    P1IES |= BIT1; // sets interrupt flag on falling edge

    TA0CCTL0 = CCIE; // enables interrupt on timer A0
    TA0CCR0 = 12000; // clock counts to 12000 for debouncing
    TA0CTL = TASSEL_1 + MC_0 + TACLR; // Sets timer to 0 with ACLK as source, timer is stopped

    __enable_irq(); // enables global interrupt
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31); // enable ISRs
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);



}
// button ISR
void PORT1_IRQHandler(void)
{
    P1OUT ^= BIT0; // toggles red led on and off
    P1IE &= ~(BIT1); // disables interrupt on port 1.1 for debouncing
    P1IFG &= ~(BIT1); // clears interrupt flag
    TA0CTL |= BIT4; // sets timer to up mode
}

// timer ISR
void TA0_0_IRQHandler(void)
{
    P1IE |= BIT1; // enables interrupt on port 1.1
    TA0CTL &= (~BIT4); // stops timer
    TA0CTL |= BIT2; // resets the clock
    P1IFG &= ~(BIT1); // clears interrupt flag
}


