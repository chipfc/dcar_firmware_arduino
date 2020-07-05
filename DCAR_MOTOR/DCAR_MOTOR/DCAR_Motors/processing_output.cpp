#include <Arduino.h>
#include "processing_output.h"
#include "config.h"



#ifdef DCAR_RELAYS
  #define   RELAY_1_CODE    0xF0
  #define   RELAY_2_CODE    0xF1
  #define   RELAY_3_CODE    0xF2
#endif

#ifdef DCAR_MOTORS
#define   MOTOR_1_CODE    0xF0
#define   MOTOR_2_CODE    0xF1
#define   MOTOR_3_CODE    0xF2
#endif



int dir1 = 3;
int dir2 = 3;
int dir3 = 3;
int pwm1 = 0;
int pwm2 = 0;
int pwm3 = 0;
int m1OverLoad = 0;
int m2OverLoad = 0;
int m3OverLoad = 0;
long int timeOut1;
long int timeOut2;
long int timeOut3;
int M1isRun = 0;
int M2isRun = 0;
int M3isRun = 0;




void Output_Initialization(void){
#ifdef DCAR_RELAYS
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
#endif

#ifdef DCAR_MOTORS
  pinMode(RELAY_1_M1, OUTPUT);
  pinMode(RELAY_2_M1, OUTPUT);
  pinMode(RELAY_1_M2, OUTPUT);
  pinMode(RELAY_2_M2, OUTPUT);
  pinMode(RELAY_1_M3, OUTPUT);
  pinMode(RELAY_2_M3, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M3_PWM, OUTPUT);
  digitalWrite(M1_PWM, 0);
  digitalWrite(M2_PWM, 0);
  digitalWrite(M3_PWM, 0);
  digitalWrite(RELAY_1_M1, 1);
  digitalWrite(RELAY_2_M1, 1);
  digitalWrite(RELAY_1_M2, 1);
  digitalWrite(RELAY_2_M2, 1);
  digitalWrite(RELAY_1_M3, 1);
  digitalWrite(RELAY_2_M3, 1);

#endif

}

#ifdef DCAR_RELAYS
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

#ifdef DCAR_MOTORS
void motorControl(int _motor, int _dir, int _speed)
{
  switch (_motor)
  {
    case MOTOR_1_CODE:

      if ((dir1 == _dir) && (pwm1 == _speed))
        break;
      if (_speed == 10)
      {
        digitalWrite(M1_PWM, 0);
        pwm1 = 0;
        delay(100);
        digitalWrite(RELAY_1_M1, 1);
        digitalWrite(RELAY_2_M1, 1);
      }
      else if (_speed < 10)
      {
        /*  for (int i = pwm1; i > 0; i--) {
              analogWrite(M1_PWM,  i);
              delay(2);
            }*/
        digitalWrite(M1_PWM, 0);
        pwm1 = 0;
        delay(100);
        digitalWrite(RELAY_1_M1, 1);
        digitalWrite(RELAY_2_M1, 1);
      }
      else
      {
        if (((m1OverLoad == 1) && (dir1 != _dir)) || (m1OverLoad == 0))
        {
          m1OverLoad = 0;
          if (_dir == 1)
          {
            digitalWrite(M1_PWM, 0);
            delay(50);
            digitalWrite(RELAY_1_M1, 1);
            digitalWrite(RELAY_2_M1, 0);
            delay(50);
          }
          if (_dir == 0)
          {
            digitalWrite(M1_PWM, 0);
            delay(50);
            digitalWrite(RELAY_1_M1, 0);
            digitalWrite(RELAY_2_M1, 1);
            delay(50);
          }
          /*  for (int i = 0; i < _speed; i++) {
                analogWrite(M1_PWM,  i);
                delay(2);
              }*/
          M1isRun = 1;
          timeOut1 = millis();
          digitalWrite(M1_PWM, 1); //them sau
          dir1 = _dir;
          pwm1 = _speed;
        }
      }
      break;
    case MOTOR_2_CODE:
      if ((dir2 == _dir) && (pwm2 == _speed))
        break;
      if (_speed == 10)
      {
        digitalWrite(M2_PWM, 0);
        pwm2 = 0;
        delay(100);
        digitalWrite(RELAY_1_M2, 1);
        digitalWrite(RELAY_2_M2, 1);
      }
      else if (_speed < 10)
      {
        /*  for (int i = pwm2; i > 0; i--) {
              analogWrite(M2_PWM,  i);
              delay(2);
            }*/
        digitalWrite(M2_PWM, 0);
        pwm2 = 0;
        delay(100);
        digitalWrite(RELAY_1_M2, 1);
        digitalWrite(RELAY_2_M2, 1);
      }
      else
      {
        if (_dir > 0)
        {
          digitalWrite(M2_PWM, 0);
          delay(50);
          digitalWrite(RELAY_1_M2, 1);
          digitalWrite(RELAY_2_M2, 0);
          delay(50);
        }
        else
        {
          digitalWrite(M2_PWM, 0);
          delay(50);
          digitalWrite(RELAY_1_M2, 0);
          digitalWrite(RELAY_2_M2, 1);
          delay(50);
        }
        /*  for (int i = 0; i < _speed; i++) {
              analogWrite(M2_PWM,  i);
              delay(2);
            }*/
      }
      M2isRun = 1;
      timeOut2 = millis();
      digitalWrite(M2_PWM, 1);
      dir2 = _dir;
      pwm2 = _speed;
      break;
    case MOTOR_3_CODE:
      if ((dir3 == _dir) && (pwm3 == _speed))
        break;
      if (_speed == 10)
      {
        digitalWrite(M3_PWM, 0);
        pwm3 = 0;
        delay(100);
        digitalWrite(RELAY_1_M3, 1);
        digitalWrite(RELAY_2_M3, 1);
      }
      else if (_speed < 10)
      {
        /* for (int i = pwm3; i > 0; i--) {
             analogWrite(M3_PWM,  i);
             delay(2);
            }*/
        digitalWrite(M3_PWM, 0);
        pwm3 = 0;
        delay(100);
        digitalWrite(RELAY_1_M3, 1);
        digitalWrite(RELAY_2_M3, 1);
      }
      else
      {
        if (_dir > 0)
        {
          digitalWrite(M3_PWM, 0);
          delay(50);
          digitalWrite(RELAY_1_M3, 1);
          digitalWrite(RELAY_2_M3, 0);
          delay(50);
        }
        else
        {
          digitalWrite(M3_PWM, 0);
          delay(50);
          digitalWrite(RELAY_1_M3, 0);
          digitalWrite(RELAY_2_M3, 1);
          delay(50);
        }
        /* for (int i = 0; i < _speed; i++) {
             analogWrite(M3_PWM,  i);
             delay(2);
            }*/
      }
      M3isRun = 1;
      timeOut3 = millis();
      digitalWrite(M3_PWM, 1);
      dir3 = _dir;
      pwm3 = _speed;
      break;
    default:
      break;
  }
}


void Turn_M1_Off(void){
    motorControl(MOTOR_1_CODE, dir1, 10);
}
void Turn_M2_Off(void){
    motorControl(MOTOR_2_CODE, dir2, 10);
}
void Turn_M3_Off(void){
    motorControl(MOTOR_3_CODE, dir3, 10);
}

tBYTE isM1Run(void){
  return (M1isRun == 1);
}

tBYTE isM2Run(void){
  return (M2isRun == 1);
}

tBYTE isM3Run(void){
  return (M3isRun == 1);
}

void Clear_M1isRun(void){
  M1isRun = 0;
}

void Clear_M2isRun(void){
  M2isRun = 0;
}
void Clear_M3isRun(void){
  M3isRun = 0;
}


void TimeOutControl(void)
{
  if (isM1Run())
  {
    if ((millis() - timeOut1) >= TIME_OUT)
    {
      Turn_M1_Off();
      Clear_M1isRun();
    }
  }
  if (isM2Run())
  {
    if ((millis() - timeOut2) >= TIME_OUT)
    {
      Turn_M2_Off();
      Clear_M2isRun();
    }
  }
  if (isM3Run())
  {
    if ((millis() - timeOut3) >= TIME_OUT)
    {
      Turn_M3_Off();
      Clear_M3isRun();
    }
  }
}

void OverLoad_Checking(void)
{
  int M1 = analogRead(A0);
  int M2 = analogRead(A1);
  int M3 = analogRead(A7);
  if ((M1 > 632) || (M1 < 392))
  {
    m1OverLoad = 1;
    Turn_M1_Off();
  }
  if ((M2 > 632) || (M2 < 392))
  {
    m2OverLoad = 1;
    Turn_M2_Off();
  }
  if ((M3 > 632) || (M3 < 392))
  {
    m3OverLoad = 1;
    Turn_M3_Off();
  }
}

#endif