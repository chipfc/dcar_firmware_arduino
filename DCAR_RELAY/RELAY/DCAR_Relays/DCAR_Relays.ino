
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
  //    timerInitialize();
}

void loop()
{
  WatchDog_Clear();
  FSM();
  Fault_Tolerant_Board_ID();
  delay(10);
}
