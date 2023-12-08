#ifndef _ANYID_CANOPEN_RS485_
#define _ANYID_CANOPEN_RS485_

#include "AnyID_CanOpen_RS485_HL.h"
#include "AnyID_CanOpen_Uart_Receive.h"




extern UART_RCVFRAME g_sRS485RcvFrame;
extern UART_RCVFRAME g_sUartRcvTempFrame;
void RS485_Init(u32 bud);

#endif
