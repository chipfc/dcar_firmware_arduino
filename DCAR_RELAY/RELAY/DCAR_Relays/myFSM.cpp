#include <Arduino.h>

#include "projectHeader.h"
#include "myFSM.h"
#include "config.h"
#include "processing_input.h"
#include "processing_output.h"

tBYTE serialData[8];
tBYTE serialReceivedBufferIndex = 0;
int dir;
int pwmValue;

////////////////////////////////////////////////////////////
static void Send_ACK_To_Master(void);
static void Can_Transmit_Enable(void);
static void Can_Receive_Enable(void);
static void Clear_Received_Buffer(void);
////////////////////////////////////////////////////////////

void FSM_Initialization(void){
    Serial.begin(9600);
    
    pinMode(SSerialTxControl, OUTPUT);
    digitalWrite(SSerialTxControl, 0);
  
}
static void Send_ACK_To_Master(void){
    Serial.write(serialData[0]);
    Serial.write(serialData[1]);
    Serial.write(serialData[2]);
}

static void Can_Transmit_Enable(void){
  digitalWrite(SSerialTxControl, RS485Transmit);
}
static void Can_Receive_Enable(void){
  digitalWrite(SSerialTxControl, RS485Receive);
}

static void Clear_Received_Buffer(void){
  serialReceivedBufferIndex = 0;
  serialData[0] = 0x00;
  // serialData[1] = 0x00;
  // serialData[2] = 0x00;
}

void FSM(){    
    if (Serial.available()){
        serialData[serialReceivedBufferIndex] = Serial.read();
        serialReceivedBufferIndex++;
    }
   
  if (isBoardIDMatched(serialData[0]) && (serialReceivedBufferIndex == 3)){
    
    Can_Transmit_Enable();
    Send_ACK_To_Master();
    delay(3);
    Can_Receive_Enable();
    
    pwmValue = serialData[2];
    dir = 1;
 
    if (pwmValue < 10){
      dir = 0;
      pwmValue = 0;
    }
    relayControl(serialData[1], dir, pwmValue);
    Clear_Received_Buffer();
  }
  else if (isBoardIDMatched(serialData[0]) == 0){
    Clear_Received_Buffer();
  }

}
