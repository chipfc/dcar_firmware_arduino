#ifndef _PROCESSING_OUTPUT_H
#define	_PROCESSING_OUTPUT_H

#include "projectHeader.h"

void Output_Initialization(void);
#ifdef DCAR_RELAYS
void relayControl(int _relay, int _dir, int _speed);
#endif 

#ifdef DCAR_MOTORS
void motorControl(int _motor, int _dir, int _speed);
void TimeOutControl(void);
void OverLoad_Checking(void);


void Turn_M1_Off(void);
void Turn_M2_Off(void);
void Turn_M3_Off(void);

tBYTE isM1Run(void);
tBYTE isM2Run(void);
tBYTE isM3Run(void);

void Clear_M1isRun(void);
void Clear_M2isRun(void);
void Clear_M3isRun(void);
#endif  /* DCAR_MOTORS */
#endif	/* _PROCESSING_OUTPUT_H */
