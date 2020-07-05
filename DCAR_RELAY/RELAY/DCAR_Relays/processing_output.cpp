#include <Arduino.h>
#include "processing_output.h"
#include "config.h"


#ifdef DCAR_RELAYS

#define   RELAY_1_CODE    0xF0
#define   RELAY_2_CODE    0xF1
#define   RELAY_3_CODE    0xF2


void Output_Initialization(void){

  pinMode(RELAY_M1, OUTPUT);
  pinMode(RELAY_M2, OUTPUT);
  pinMode(RELAY_M3, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M3_PWM, OUTPUT);
  
  digitalWrite(M1_PWM, 0);
  digitalWrite(M2_PWM, 0);
  digitalWrite(M3_PWM, 0);
  digitalWrite(RELAY_M1, 0);
  digitalWrite(RELAY_M2, 0);
  digitalWrite(RELAY_M3, 0);
}

void relayControl(int _relay, int _dir, int _speed)
{
  switch (_relay)
  {
    case RELAY_1_CODE:
    analogWrite(M1_PWM, _speed);
    delay(100);
    digitalWrite(RELAY_M1, _dir);
    break;
    
  case RELAY_2_CODE:
    analogWrite(M2_PWM, _speed);
    delay(100);
    digitalWrite(RELAY_M2, _dir);
    break;
    
  case RELAY_3_CODE:
    analogWrite(M3_PWM, _speed);
    delay(100);
    digitalWrite(RELAY_M3, _dir);
    break;
  default:
      break;
  }
}

#endif
