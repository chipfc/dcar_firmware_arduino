
#ifndef _PROCESSING_INPUT_H
#define	_PROCESSING_INPUT_H
#include "projectHeader.h"


void Input_Initialization(void);
void getKeyInput();
tBYTE getBoardID(void);
tBYTE isBoardIDMatched(tBYTE receivedID);
void Fault_Tolerant_Board_ID(void);

#endif	/* _PROCESSING_INPUT_H */
