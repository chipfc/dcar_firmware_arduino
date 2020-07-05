#include <Arduino.h>
#include "processing_input.h"
#include "config.h"
#include "WatchDogTimer.h"
#include "LED.h"

#define NORMAL_STATE 1
#define PRESSED_STATE 0


tBYTE Board_ID = 0;
tBYTE Board_ID_1 = 0;
tBYTE Board_ID_2 = 0;

tWORD KeyReg0, KeyReg1, KeyReg2, KeyReg3;
tWORD TimeOutForKeyPress;

tBYTE readID();

void Input_Initialization(void){
  pinMode(SW1, INPUT_PULLUP);
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);

  for(int i = 0; i < 100; i++){
    WatchDog_Clear();
    getKeyInput();
    delay(10);
  }
  
  Turn_Led_Off();
  delay(500);
  
  for(int i = 0; i < Board_ID; i++){
    WatchDog_Clear();
    Turn_Led_On();
    delay(50);
    Turn_Led_Off();
    delay(700);
  }
}

tBYTE readID(){
  
  tBYTE _ID = 0;
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
      Board_ID = KeyReg3; 
      Board_ID_1 = Board_ID;
      Board_ID_2 = Board_ID;
    }
  }
}

tBYTE getBoardID(void){
  return Board_ID;
}
void Fault_Tolerant_Board_ID(void){
  if(Board_ID != Board_ID_1 && Board_ID == Board_ID_2){
    Board_ID_1 = Board_ID;
  } else if(Board_ID == Board_ID_1 && Board_ID != Board_ID_2){
    Board_ID_2 = Board_ID;
  } else if(Board_ID != Board_ID_1 && Board_ID_1 == Board_ID_2){
    Board_ID = Board_ID_1;
  } else if(Board_ID == Board_ID_1 && Board_ID_1 == Board_ID_2){
      Board_ID = Board_ID_1;
      Board_ID_1 = Board_ID_2;
      Board_ID_2 = Board_ID;
  } else {
    getKeyInput();
  }
}
tBYTE isBoardIDMatched(tBYTE receivedID){
  return (receivedID == Board_ID);
}