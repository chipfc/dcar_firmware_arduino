#include <avr/wdt.h>
#include <SoftwareSerial.h>

#define SSerialRX 0 //Serial Receive pin
#define SSerialTX 1 //Serial Transmit pin
#define RS485Transmit HIGH
#define RS485Receive LOW
#define SSerialTxControl 2
#define Pin13LED 13

#define RELAY_1_M1 10
#define RELAY_2_M1 11
#define M1_PWM 5

#define RELAY_1_M2 7
#define RELAY_2_M2 8
#define M2_PWM 6

#define RELAY_1_M3 3
#define RELAY_2_M3 4
#define M3_PWM 9

#define SW1 A5
#define SW2 A4
#define SW3 A3
#define SW4 A2
#define SW5 12

#define NORMAL_STATE 1
#define PRESSED_STATE 0

#define TIME_OUT 120000
//SoftwareSerial Data(SSerialRX, SSerialTX); // RX, TX
int byteReceived;
int byteSend;
byte serialData[8];
int index = 0;
int relay_1 = 0;
int relay_2 = 0;

int relay_3 = 0;
int relay_4 = 0;

int relay_5 = 0;
int relay_6 = 0;
int ID = 255;
int dir;
int dir1 = 3;
int dir2 = 3;
int dir3 = 3;
int pwm;
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

int KeyReg0, KeyReg1, KeyReg2, KeyReg3;
int TimeOutForKeyPress;

void getKeyInput(){
  KeyReg2 = KeyReg1;  
  KeyReg1 = KeyReg0;  
  KeyReg0 = readID();
  if ((KeyReg1 == KeyReg0) && (KeyReg1 == KeyReg2)){
    if (KeyReg2 != KeyReg3){
      KeyReg3 = KeyReg2;  

      if (KeyReg2 == PRESSED_STATE){      
        TimeOutForKeyPress = 50;
      } 
    }else{
      TimeOutForKeyPress --;     
      ID = KeyReg3; 
    }
  }
}



void setup()
{
  wdt_enable(WDTO_1S);
  Serial.begin(9600);
  pinMode(RELAY_1_M1, OUTPUT);
  pinMode(RELAY_2_M1, OUTPUT);
  pinMode(RELAY_1_M2, OUTPUT);
  pinMode(RELAY_2_M2, OUTPUT);
  pinMode(RELAY_1_M3, OUTPUT);
  pinMode(RELAY_2_M3, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M3_PWM, OUTPUT);
  pinMode(Pin13LED, OUTPUT);
  
  digitalWrite(M1_PWM, 0);
  digitalWrite(M2_PWM, 0);
  digitalWrite(M3_PWM, 0);
  digitalWrite(RELAY_1_M1, 0);
  digitalWrite(RELAY_2_M1, 0);
  digitalWrite(RELAY_1_M2, 0);
  digitalWrite(RELAY_2_M2, 0);
  digitalWrite(RELAY_1_M3, 0);
  digitalWrite(RELAY_2_M3, 0);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  


  for(int i = 0; i < 100; i++){
    wdt_reset();
    getKeyInput();
    delay(10);
  }
  
  digitalWrite(Pin13LED, 0);
  delay(500);
  
  for(int i = 0; i < ID; i++){
    wdt_reset();
    digitalWrite(Pin13LED, 1);
    delay(50);
    digitalWrite(Pin13LED, 0);
    delay(700);
  }

}

void loop()
{
  // Serial.println(analogRead(A0));
  wdt_reset();
  overLoad();
  
  getKeyInput();
  
  if (Serial.available())
  {
    byteReceived = Serial.read();
    serialData[index] = byteReceived;
    index++;
  }
  
  TimeOutControl();
  if ((serialData[0] == ID) && (index == 3))
  {
    digitalWrite(SSerialTxControl, RS485Transmit);
    Serial.write(serialData[0]);
    Serial.write(serialData[1]);
    Serial.write(serialData[2]);
    delay(3);
    digitalWrite(SSerialTxControl, RS485Receive);
    int data = (128 - serialData[2]) * 2;
    if (data < 0)
    {
      dir = 0;
      pwm = 1 - data;
    }
    else
    {
      dir = 1;
      pwm = data - 1;
    }
    motorControl(serialData[1], dir, pwm);

    index = 0;
    serialData[0] = 0x00;
    digitalWrite(Pin13LED, LOW);
  }
  else if ((serialData[0] != ID))
  {
    index = 0;
    serialData[0] = 0x00;
  }
  delay(10);
}
int readID(){
  
  int _ID = 0;
  if (digitalRead(SW1) == 0)
    _ID += 1;
  if (digitalRead(SW2) == 0)
    _ID += 2;
  if (digitalRead(SW3) == 0)
    _ID += 4;
  if (digitalRead(SW4) == 0)
    _ID += 8;
  if (digitalRead(SW5) == 0)
    _ID += 16;
  
  return _ID;
}

void overLoad()
{
  int M1 = analogRead(A0);
  int M2 = analogRead(A1);
  int M3 = analogRead(A7);
  if ((M1 > 632) || (M1 < 392))
  {
    m1OverLoad = 1;
    motorControl(0xf0, dir1, 10);
  }
  if ((M2 > 632) || (M2 < 392))
  {
    m2OverLoad = 1;
    motorControl(0xf1, dir2, 10);
  }
  if ((M3 > 632) || (M3 < 392))
  {
    m3OverLoad = 1;
    motorControl(0xf2, dir3, 10);
  }
}
void motorControl(int _motor, int _dir, int _speed)
{
  switch (_motor)
  {
    case 0xf0:
      if ((dir1 == _dir) && (pwm1 == _speed))
        break;
      if (_speed == 10)
      {
        digitalWrite(M1_PWM, 0);
        pwm1 = 0;
        delay(100);
        digitalWrite(RELAY_1_M1, 0);
        digitalWrite(RELAY_2_M1, 0);
      }
      else if (_speed < 10)
      {
        digitalWrite(M1_PWM, 0);
        pwm1 = 0;
        delay(100);
        digitalWrite(RELAY_1_M1, 0);
        digitalWrite(RELAY_2_M1, 0);
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
          M1isRun = 1;
          timeOut1 = millis();
          digitalWrite(M1_PWM, 1); //them sau
          dir1 = _dir;
          pwm1 = _speed;
        }
      }
      break;
    case 0xf1:
      if ((dir2 == _dir) && (pwm2 == _speed))
        break;
      if (_speed == 10)
      {
        digitalWrite(M2_PWM, 0);
        pwm2 = 0;
        delay(100);
        digitalWrite(RELAY_1_M2, 0);
        digitalWrite(RELAY_2_M2, 0);
      }
      else if (_speed < 10)
      {
        digitalWrite(M2_PWM, 0);
        pwm2 = 0;
        delay(100);
        digitalWrite(RELAY_1_M2, 0);
        digitalWrite(RELAY_2_M2, 0);
      }
      else
      {
        if (((m2OverLoad == 1) && (dir2 != _dir)) || (m2OverLoad == 0))
        {
          m2OverLoad = 0;
          if (_dir == 1)
          {
            digitalWrite(M2_PWM, 0);
            delay(50);
            digitalWrite(RELAY_1_M2, 1);
            digitalWrite(RELAY_2_M2, 0);
            delay(50);
          }
          if (_dir == 0)
          {
            digitalWrite(M2_PWM, 0);
            delay(50);
            digitalWrite(RELAY_1_M2, 0);
            digitalWrite(RELAY_2_M2, 1);
            delay(50);
          }
          M2isRun = 1;
          timeOut2 = millis();
          digitalWrite(M2_PWM, 1); //them sau
          dir2 = _dir;
          pwm2 = _speed;
        }
      }
      break;
    case 0xf2:
       if ((dir3 == _dir) && (pwm3 == _speed))
        break;
      if (_speed == 10)
      {
        digitalWrite(M3_PWM, 0);
        pwm3 = 0;
        delay(100);
        digitalWrite(RELAY_1_M3, 0);
        digitalWrite(RELAY_2_M3, 0);
      }
      else if (_speed < 10)
      {
        digitalWrite(M3_PWM, 0);
        pwm3 = 0;
        delay(100);
        digitalWrite(RELAY_1_M3, 0);
        digitalWrite(RELAY_2_M3, 0);
      }
      else
      {
        if (((m3OverLoad == 1) && (dir3 != _dir)) || (m3OverLoad == 0))
        {
          m3OverLoad = 0;
          if (_dir == 1)
          {
            digitalWrite(M3_PWM, 0);
            delay(50);
            digitalWrite(RELAY_1_M3, 1);
            digitalWrite(RELAY_2_M3, 0);
            delay(50);
          }
          if (_dir == 0)
          {
            digitalWrite(M3_PWM, 0);
            delay(50);
            digitalWrite(RELAY_1_M3, 0);
            digitalWrite(RELAY_2_M3, 1);
            delay(50);
          }
          M3isRun = 1;
          timeOut3 = millis();
          digitalWrite(M3_PWM, 1); //them sau
          dir3 = _dir;
          pwm3 = _speed;
        }
      }
      break;
    default:
      break;
  }
}
void TimeOutControl()
{
  if (M1isRun == 1)
  {
    if ((millis() - timeOut1) >= TIME_OUT)
    {
      motorControl(0xf0, dir1, 10);
      M1isRun = 0;
    }
  }
  if (M2isRun == 1)
  {
    if ((millis() - timeOut2) >= TIME_OUT)
    {
      motorControl(0xf1, dir2, 10);
      M2isRun = 0;
    }
  }
  if (M3isRun == 1)
  {
    if ((millis() - timeOut3) >= TIME_OUT)
    {
      motorControl(0xf2, dir3, 10);
      M3isRun = 0;
    }
  }
}
