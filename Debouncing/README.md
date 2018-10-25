# Software Debouncing
Instead of using a delay cycles command to debounce a switch, timers as well as interrupt vectors
are used to stop the bouncing.
## Implementation
To stop button bouncing, the code simply will delay for about 60 ms to ensure the button will be done bouncing
by the time the interrupt fires, due to the button bouncing for about half a microsecond, which gives the delay
more than enough time to stop the bouncing.
## Code
While initializing the inputs, outputs, and clocks of both boards has a very similar structure, 
initalizing the interrupt vectors varies greatly between the two boards. For the G2553, 
the interrupt vector is the same as found in other codes involving interrupt, using
the #pragma vector code. For the P401R however, interrupts and the ISRs are initialized 
using the code:
    __enable_irq(); // enables global interrupt
    NVIC->ISER[0] = 1 << ((TA0_0_IRQn) & 31); // enable ISRs
    NVIC->ISER[1] = 1 << ((PORT1_IRQn) & 31);
Instead of using a vector, the ISRs are enabled in the main, and the ISR themselves are 
written as a void function after the main. 
Example button ISR:
void PORT1_IRQHandler(void)
{
    P1OUT ^= BIT0; // toggles red led on and off
    P1IE &= ~(BIT1); // disables interrupt on port 1.1 for debouncing
    P1IFG &= ~(BIT1); // clears interrupt flag
    TA0CTL |= BIT4; // sets timer to up mode
}