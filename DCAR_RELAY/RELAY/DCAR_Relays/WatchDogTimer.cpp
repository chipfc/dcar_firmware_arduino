#include <Arduino.h>
#include <avr/wdt.h>
#include "LED.h"



void WatchDog_Inititalization(void){
    wdt_enable(WDTO_1S);
}


void WatchDog_Clear(void){
    wdt_reset();    
}