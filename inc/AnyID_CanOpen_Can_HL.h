#ifndef _ANYID_CAN_HL_H
#define _ANYID_CAN_HL_H

#include "AnyID_CanOpen_Config.h"
#include "AnyId_Canopen_Slave.h"		//协议栈中直接映射此文件部分函数	
#define CAN_HARDPORT					CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	
#define CAN_SEND_TIMEOUT    ((uint32_t)0x0000FFFF)

typedef struct canFrame{
	u8 state;
	u32 waitTimes;
	CanRxMsg rxMasg;
	CanTxMsg txMasg;
}CAN_FRAME;

extern CAN_FRAME g_sCanFrame ;

BOOL Can_InitInterface();
u8 canSend(CAN_PORT notused, Message *TxMessage);
void Can_Receive_Msg();
#endif 

