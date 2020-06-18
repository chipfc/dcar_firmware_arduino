#include <SoftwareSerial.h>
#include <avr/wdt.h>

#define SSerialRX        3  //Serial Receive pin
#define SSerialTX        4  //Serial Transmit pin

#define SSerialTxControl 2   //Kiểm soát hướng cho rs485

#define RS485Transmit    HIGH
#define RS485Receive     LOW
SoftwareSerial Data(SSerialRX, SSerialTX); // RX, TX

int byteReceived;
void setup()
{
  wdt_enable(WDTO_1S);
  Serial.begin(9600);
  pinMode(SSerialTxControl, OUTPUT);
  digitalWrite(SSerialTxControl, RS485Receive);
  Data.begin(9600);
}

// 040300000004445C READ PH
// 040300040004059D READ TEMP
// 040300080004C59E SANILITY

void loop() {
  wdt_reset();
  if (Serial.available()) {
    byteReceived = Serial.read();
    digitalWrite(SSerialTxControl, RS485Transmit);
    Data.write(byteReceived);
    digitalWrite(SSerialTxControl, RS485Receive);
    //   serialData[index] = byteReceived;
    //   index++;
    //   if (index == 8) index = 0;
  }
  if (Data.available())
  {
    byteReceived = Data.read();
    Serial.write(byteReceived);
  }
}
