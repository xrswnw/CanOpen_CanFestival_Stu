  
#ifndef _ANYID_CAN_HL_H
#define _ANYID_CAN_HL_H

#include "AnyID_CanOpen_Config.h"


#define CAN_PORT1					CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	

#define CAN_SEND_TIMEOUT    ((uint32_t)0x0000FFFF)

//典型通信参数
#define CAN_BUD_1000K_PSCVALUE			4
#define CAN_BUD_800K_PSCVALUE			5
#define CAN_BUD_500K_PSCVALUE			8
#define CAN_BUD_250K_PSCVALUE			16
#define CAN_BUD_125K_PSCVALUE			32
#define CAN_BUD_50K_PSCVALUE			80
#define CAN_BUD_20K_PSCVALUE			200
#define CAN_BUD_10K_PSCVALUE			400





typedef struct canParamenterInfo
{
	u8 mode;
	u8 tsjw;
    u8 tbs1;
    u8 tbs2;
    u16 psc;
    u32 canId;   
}CAN_PARAMENTERINFO;

typedef struct canFrame{
	u8 state;
	u32 waitTimes;
	CanRxMsg rxMasg;
	CanTxMsg txMasg;
}CAN_FRAME;

extern CAN_FRAME g_sCanFrame ;

BOOL Can_InitInterface(CAN_PARAMENTERINFO *pParamInfo);

void Can_Receive_Msg();

u8 canSend(CAN_PORT notused, Message *TxMessage);

#endif 

