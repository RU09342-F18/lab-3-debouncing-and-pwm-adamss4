# Hardware PWM
Along with software PWM methods, one can also generate a PWM singal using the hardware
of the microcontrollers themselves. Instead of using multiple timers, the OUTMOD function,
specifially OUTMOD_7, or output mode 7, which is reset/set mode.
## Header file
This code also uses the macros.h file found in the code forn software PWM for both sets
of code. To see how to use it, view the header file section in the README found for software
PWM.
## Implementation
Between the two boards, the code for hardware PWM is identical, thanks to the header file
macros.h. Seperate functions were used to initilaize the timers, LED, and the button, to aid
with any debugging that may need to be completed. The timer used for setting PWM was Timer A, 
which was initialized with SMCLK in up mode. Due to this method being hardware PWM, the 
OUTMOD function explained before was used. For reset/set mode, the CCTL output signal is 
flipped when the TA0CCR1 interrupt flag occurs, and the value is set when the TA0CCR0 interrupt
flag occurs. This method creates a PWM waveform, with duty cycle varying based on the value in 
CCR1. The duty cycle was changed by incrementing the value of CCR1 by 100, or 10 percent.
When the duty cycle was 100 percent and the button was pressed again, the duty cycle was 
reset to 0 percent. 