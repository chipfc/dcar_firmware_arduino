#include <Arduino.h>
#include "LED.h"



void Led_Initialization(void){
  pinMode(Pin13LED, OUTPUT);
}

void Turn_Led_Off(void){
  digitalWrite(Pin13LED, 0);
}
void Turn_Led_On(void){
  digitalWrite(Pin13LED, 1);
}
