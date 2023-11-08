
//#include "canfestival.h"
//#include "timer.h"
//#include "data.h"
#include "AnyID_CanOpen_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};

BOOL Can_InitInterface(CAN_PARAMENTERINFO *pParamInfo)
{
	BOOL bOk = FALSE;
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	CAN_InitTypeDef  CAN_InitStructure = {0};
	CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PORT.Pin;
	
	GPIO_Init(CAN_RX_PORT.Port, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PORT.Pin;
	
	GPIO_Init(CAN_TX_PORT.Port, &GPIO_InitStructure);

	CAN_InitStructure.CAN_ABOM = DISABLE;   			// ����Զ����߹���
	CAN_InitStructure.CAN_AWUM = DISABLE;   			// ˯��ģʽͨ���������(���CAN->MCR��SLEEPλ)
	CAN_InitStructure.CAN_BS1 = pParamInfo->tbs1;
	CAN_InitStructure.CAN_BS2 = pParamInfo->tbs2;
	CAN_InitStructure.CAN_Mode = pParamInfo->mode;
	CAN_InitStructure.CAN_NART = ENABLE;   				// ʹ�ñ����Զ����� 
	CAN_InitStructure.CAN_Prescaler = pParamInfo->psc;
	CAN_InitStructure.CAN_RFLM = DISABLE;   			// ���Ĳ�����,�µĸ��Ǿɵ� 
	CAN_InitStructure.CAN_SJW = pParamInfo->tsjw;		//ͬ����ת���
	CAN_InitStructure.CAN_TTCM = DISABLE;				// ��ʱ�䴥��ͨ��ģʽ  
	CAN_InitStructure.CAN_TXFP = DISABLE;   			// ���ȼ��ɱ��ı�ʶ������ 
	
	if(CAN_Init(CAN_PORT1, &CAN_InitStructure) == CANINITOK)		// ��ʼ��CAN1
	{
		CAN_FilterInitStructure.CAN_FilterNumber = 0;  					 	// ������0
		CAN_FilterInitStructure.CAN_FilterMode = CAN_FilterMode_IdMask;   	// ����ģʽ
		CAN_FilterInitStructure.CAN_FilterScale = CAN_FilterScale_32bit;   	// 32λ 
		CAN_FilterInitStructure.CAN_FilterIdHigh = 0x0000;   				// 32λID
		CAN_FilterInitStructure.CAN_FilterIdLow = 0x0000;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh = 0x0000;   			// 32λMASK
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = 0x0000;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;   // ������0������FIFO0
		CAN_FilterInitStructure.CAN_FilterActivation = ENABLE;  				 // ���������0
		CAN_FilterInit(&CAN_FilterInitStructure);  			 				// ��������ʼ��
		
		NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN1_RX0_IRQn;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_RX >> 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_RX & 0x03;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		bOk = FALSE;
	}
	else
	{
		bOk = TRUE;
	}
	return bOk;
}



u8 Med_Can_Send_Msg (u8* msg,u8 len)
{	
	u8 mailBox = 0, relust = 0;
	u32 waitTimes = 0, index = 0;
	
	CanTxMsg TxMessage;   
	TxMessage.StdId = 0x12;   			// ��׼��ʶ��
	TxMessage.ExtId = 0x12;  		 	// ��չ��ʶ��
	TxMessage.IDE = CAN_ID_STD;   		// ʹ�ñ�׼��ʶ��
	TxMessage.RTR = 0;   				// ��Ϣ����Ϊ����֡��һ֡8λ
	TxMessage.DLC = len;
	
	for(index = 0;index < len;index ++)
	{
		TxMessage.Data[index] = msg[index];   // ���֡���ݶ�
	}
	
	mailBox = CAN_Transmit(CAN_PORT1, &TxMessage);   // ���ͱ���   

	// �ȴ����ͽ���
    while(CAN_TransmitStatus(CAN_PORT1, mailBox) != CANTXOK && waitTimes < CAN_SEND_TIMEOUT)
    {
        waitTimes++;
		relust = CAN_TransmitStatus(CAN_PORT1, mailBox);
    }
    
    if(waitTimes < CAN_SEND_TIMEOUT)
    {
       relust = CANTXOK;
    }
	
	return relust;		
}


u8 canSend (CAN_PORT notused, Message *TxMessage)
{	
	u8 mailBox = 0, relust = 0;
	u32 waitTimes = 0;
	CanTxMsg txMasg = {0};
   
	txMasg.StdId = TxMessage->cob_id;   			// ��׼��ʶ��
	txMasg.IDE = CAN_ID_STD;   						// ʹ�ñ�׼��ʶ��
	txMasg.RTR = (TxMessage->rtr == CAN_RTR_DATA ? 0 : 2);	
	txMasg.DLC = TxMessage->len;
	memcpy(txMasg.Data, TxMessage->data, TxMessage->len);
	mailBox = CAN_Transmit(CAN_PORT1, &txMasg);   // ���ͱ���   
	// �ȴ����ͽ���
    while(CAN_TransmitStatus(CAN_PORT1, mailBox) != CANTXOK && waitTimes < CAN_SEND_TIMEOUT)
    {
        waitTimes++;
    }
    
    if(waitTimes < CAN_SEND_TIMEOUT)
    {
       relust = CANTXOK;
    }
	
	return relust;		
}


u8 Med_Can_Receive_Msg (u8 *buf)
{		   		   
 	u32 i;
	CanRxMsg RxMessage;   // ������ձ��Ľṹ��
	
	// û�н��յ�����,ֱ���˳� 
	if( CAN_MessagePending(CAN_PORT1,CAN_FIFO0) == 0)
	{
		return 0;
	}
	CAN_Receive(CAN_PORT1, CAN_FIFO0, &RxMessage);   // ��ȡ����	
	
	for(i = 0;i < RxMessage.DLC;i ++)
	{
		buf[i] = RxMessage.Data[i];
	}
	
	return RxMessage.DLC;	
}




