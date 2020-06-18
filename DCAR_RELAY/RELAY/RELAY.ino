#include <avr/wdt.h>
#include <SoftwareSerial.h>

#define SSerialRX 0 //Serial Receive pin
#define SSerialTX 1 //Serial Transmit pin
#define RS485Transmit HIGH
#define RS485Receive LOW
#define SSerialTxControl 2
#define Pin13LED 13

#define RELAY_M1 8
#define M1_PWM 5

#define RELAY_M2 10
#define M2_PWM 6

#define RELAY_M3 7
#define M3_PWM 9

#define SW1 A5
#define SW2 A4
#define SW3 A3
#define SW4 A2
#define SW5 A1

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

void setup()
{
  wdt_enable(WDTO_1S);
  Serial.begin(9600);
  pinMode(RELAY_M1, OUTPUT);
  pinMode(RELAY_M2, OUTPUT);
  pinMode(RELAY_M3, OUTPUT);
  pinMode(M1_PWM, OUTPUT);
  pinMode(M2_PWM, OUTPUT);
  pinMode(M3_PWM, OUTPUT);
  pinMode(Pin13LED, OUTPUT);
  digitalWrite(Pin13LED, 1);
  digitalWrite(M1_PWM, 0);
  digitalWrite(M2_PWM, 0);
  digitalWrite(M3_PWM, 0);
  digitalWrite(RELAY_M1, 0);
  digitalWrite(RELAY_M2, 0);
  digitalWrite(RELAY_M3, 0);
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, 0);
  //  Data.begin(9600);
  digitalWrite(Pin13LED, 0);

  readID();
}

void loop()
{
  // Serial.println(analogRead(A0));
  wdt_reset();
  //  overLoad();
  if (Serial.available())
  {
    byteReceived = Serial.read();
    serialData[index] = byteReceived;
    index++;
  }
  readID(); // ID == 3 ID == 4 có time out port 1+2
  if (ID < 6) {
    TimeOutControl();
  }
  if ((serialData[0] == ID) && (index == 3))
  {
    digitalWrite(SSerialTxControl, RS485Transmit);
    Serial.write(serialData[0]);
    Serial.write(serialData[1]);
    Serial.write(serialData[2]);
    delay(3);
    digitalWrite(SSerialTxControl, RS485Receive);
    int data = serialData[2];
    if (data < 10)
    {
      dir = 0;
      pwm = 0;
    }
    else
    {
      dir = 1;
      pwm = data;
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
}
void readID()
{
  ID = 0;
  if (digitalRead(SW1) == 0)
    ID += 1;
  if (digitalRead(SW2) == 0)
    ID += 2;
  if (digitalRead(SW3) == 0)
    ID += 4;
  if (digitalRead(SW4) == 0)
    ID += 8;
  if (digitalRead(SW5) == 0)
    ID += 16;
  //Serial.println(ID);
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
void motorControl(int _relay, int _dir, int _speed)
{
  switch (_relay)
  {
    case 0xf0:

      if ((dir1 == _dir) && (pwm1 == _speed))
        break;
      else if (_dir == 0)
      {
        if (ID > 5) {
          for (int i = pwm1; i > 0; i--) {
            analogWrite(M1_PWM,  i);
            delay(2);
          }
        }
        analogWrite(M1_PWM, 0);
        pwm1 = 0;
        delay(100);
        digitalWrite(RELAY_M1, 0);
      }
      else
      {
        if (_dir == 1)
        {
          analogWrite(M1_PWM, 0);
          delay(50);
          digitalWrite(RELAY_M1, 1);
          delay(50);
        }
        if (ID > 5) {
          for (int i = 0; i <= _speed; i++) {
            analogWrite(M1_PWM,  i);
            delay(2);
          }
        } else analogWrite(M1_PWM,  255);
        M1isRun = 1;
        timeOut1 = millis();
        //   digitalWrite(M1_PWM, 1); //them sau
        dir1 = _dir;
        pwm1 = _speed;
      }
      break;
    case 0xf1:
      if ((dir2 == _dir) && (pwm2 == _speed))
        break;
      else if (_dir == 0)
      {
        if (ID > 5) {
          for (int i = pwm2; i > 0; i--) {
            analogWrite(M2_PWM,  i);
            delay(2);
          }
        }
        analogWrite(M2_PWM, 0);
        pwm2 = 0;
        delay(100);
        digitalWrite(RELAY_M2, 0);
      }
      else
      {
        if (_dir == 1)
        {
          analogWrite(M2_PWM, 0);
          delay(50);
          digitalWrite(RELAY_M2, 1);
          delay(50);
        }
        if (ID > 5) {
          for (int i = 0; i <= _speed; i++) {
            analogWrite(M2_PWM,  i);
            delay(2);
          }
        } else analogWrite(M2_PWM,  255);
        M2isRun = 1;
        timeOut2 = millis();
        //   digitalWrite(M1_PWM, 1); //them sau
        dir2 = _dir;
        pwm2 = _speed;
      }
      break;
    case 0xf2:
      if ((dir3 == _dir) && (pwm3 == _speed))
        break;
      else if (_dir == 0)
      {
        for (int i = pwm3; i > 0; i--) {
          analogWrite(M3_PWM,  i);
          delay(2);
        }
        analogWrite(M3_PWM, 0);
        pwm3 = 0;
        delay(100);
        digitalWrite(RELAY_M3, 0);
      }
      else
      {
        if (_dir == 1)
        {
          analogWrite(M3_PWM, 0);
          delay(50);
          digitalWrite(RELAY_M3, 1);
          delay(50);
        }
        for (int i = 0; i <= _speed; i++) {
          analogWrite(M3_PWM,  i);
          delay(2);
        }
        //   digitalWrite(M1_PWM, 1); //them sau
        dir3 = _dir;
        pwm3 = _speed;
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
      motorControl(0xf0, 0, 0);
      M1isRun = 0;
    }
  }
  if (M2isRun == 1)
  {
    if ((millis() - timeOut2) >= TIME_OUT)
    {
      motorControl(0xf1, 0, 0);
      M2isRun = 0;
    }
  }
}
