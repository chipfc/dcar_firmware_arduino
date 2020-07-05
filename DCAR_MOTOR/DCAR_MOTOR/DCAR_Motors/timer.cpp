#include <Arduino.h>
#include <TimerOne.h>
#include "projectHeader.h"
#include "timer.h"
#include "LED.h"
#include "processing_input.h"

void timerIsr();

void timerInitialize(void){
    Timer1.initialize(TIMER_INTERRUPT_PERIOD); // set a timer of length 100000 microseconds (or 0.1 sec - or 10Hz => the led will blink 5 times, 5 cycles of on-and-off, per second)
    Timer1.attachInterrupt( timerIsr ); // attach the service routine here
}

/// --------------------------
/// Custom ISR Timer Routine
/// --------------------------
void timerIsr()
{
}
