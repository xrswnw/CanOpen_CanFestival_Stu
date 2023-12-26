#ifndef _ANYID_CANOPEN_TIM_HL_H
#define _ANYID_CANOPEN_TIM_HL_H

#include "AnyID_CanOpen_Config.h"
#include "AnyId_Canopen_Slave.h"




#define TIM_PORT					TIM1
#define TIM_PSE_SYS_FREQ			(SYS_CLOCK_FREQ / 1000000) - 1				//Ö÷Æµ120Mhz

	
void setTimer(TIMEVAL value);
void Tim_InitInterface();
TIMEVAL getElapsedTime(void);


#endif 

