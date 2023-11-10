#ifndef _ANYID_CANOPEN_PERIPH_HL_H
#define _ANYID_CANOPEN_PERIPH_HL_H

#include "AnyID_CanOpen_Config.h"






extern const PORT_INF PERIPH_RUN_LED_1;
#define PeriphLed1_Open()			PERIPH_RUN_LED_1.Port->BSRR = PERIPH_RUN_LED_1.Pin
#define PeriphLed1_Close()			PERIPH_RUN_LED_1.Port->BRR = PERIPH_RUN_LED_1.Pin

extern const PORT_INF PERIPH_RUN_LED_2;
#define PeriphLed2_Open()			PERIPH_RUN_LED_2.Port->BSRR = PERIPH_RUN_LED_2.Pin
#define PeriphLed2_Close()			PERIPH_RUN_LED_2.Port->BRR = PERIPH_RUN_LED_2.Pin

extern const PORT_INF PERIPH_RUN_LED_3;
#define PeriphLed3_Open()			PERIPH_RUN_LED_3.Port->BSRR = PERIPH_RUN_LED_3.Pin
#define PeriphLed3_Close()			PERIPH_RUN_LED_3.Port->BRR = PERIPH_RUN_LED_3.Pin


void Periph_InitInterface();
u8 Periph_GetAddr();
u8 Periph_GetKeyValue();
#endif 

