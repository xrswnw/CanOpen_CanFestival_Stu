  
#ifndef _ANYID_CAN_HL_H
#define _ANYID_CAN_HL_H

#include "AnyID_CanOpen_Config.h"
#include "demo.h"

#define CAN_PORT1					CAN1
#define CAN_INT_CHANNEL				USB_LP_CAN1_RX0_IRQn
#define CAN_IRQHandler   			USB_LP_CAN1_RX0_IRQHandler
	

#define CAN_SEND_TIMEOUT    ((uint32_t)0x0000FFFF)
typedef struct canParamenterInfo
{
	u8 mode;
	u8 tsjw;
    u8 tbs1;
    u8 tbs2;
    u16 psc;
    u32 canId;   
}CAN_PARAMENTERINFO;



BOOL Can_InitInterface(CAN_PARAMENTERINFO *pParamInfo);

u8 Med_Can_Send_Msg (u8* msg,u8 len);

u8 canSend (CAN_PORT notused, Message *TxMessage);

#endif 

