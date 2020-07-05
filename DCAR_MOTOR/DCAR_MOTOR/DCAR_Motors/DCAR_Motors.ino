
#include "LED.h"
#include "timer.h"
#include "myFSM.h"
#include "processing_input.h"
#include "processing_output.h"
#include "WatchDogTimer.h"
#include "myFSM.h"

void setup()
{
  WatchDog_Inititalization();
  Led_Initialization();
  Input_Initialization();
  Output_Initialization();
  FSM_Initialization();

  pinMode(2, OUTPUT);
  digitalWrite(2, 0);
  //    timerInitialize();
}

void loop()
{
  WatchDog_Clear();
  OverLoad_Checking();
  FSM();
  Fault_Tolerant_Board_ID();
  delay(10);
}
