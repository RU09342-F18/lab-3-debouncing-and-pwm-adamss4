# Software PWM
The PWM method in this code uses the software of timers and interrupts to generate a PWM
signal to drive an LED. This is achieved using Multiple interrupt registers and seting the
clock to up/down mode to create the signal.
## Header file
In this code, the header, macros.h is used in both codes. This header file simply acts
as ifdef statements at the start of the main.c, so the code for each board can simply be
copied and pasted with either minimal or no changes required. To use macros.h, simply add
it to the project directory and include it in the start of the main.c.
## Implementation
The code between the two boards is nearly identical, thanks to the use of the header file
macros.h, but there is one main difference between the two. For the FR2311, instead of 
timer A0 being used, timer B0 was used, but the function remains the same. To start,
functions were written to initialize the button, led, and timer, to easily debug each 
if a problem arose. Along with aiding in debugging, it also makes the code easier to follow.
for both, SMCLK was used as the source for the timer. The LED starts at a 50% duty cycle, 
and increments by 10% each time the button is pressed. Once the cycle reaches 100% and the 
button is pressed again, the duty cycle resets to 0%.