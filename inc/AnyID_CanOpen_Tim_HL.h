#ifndef _ANYID_CANOPEN_TIM_HL_H
#define _ANYID_CANOPEN_TIM_HL_H

#include "AnyID_CanOpen_Config.h"



	
void setTimer(TIMEVAL value);
void Tim_InitInterface();
TIMEVAL getElapsedTime(void);
#endif 

