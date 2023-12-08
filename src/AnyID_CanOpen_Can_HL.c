
//#include "canfestival.h"
//#include "timer.h"
//#include "data.h"
#include "AnyID_CanOpen_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};


CAN_FRAME g_sCanFrame = {0};
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
		
		NVIC_InitStructure.NVIC_IRQChannel = CAN_INT_CHANNEL;
		NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = INT_PRIORITY_UART_RX >> 2;
		NVIC_InitStructure.NVIC_IRQChannelSubPriority = INT_PRIORITY_UART_RX & 0x03;
		NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
		NVIC_Init(&NVIC_InitStructure);
		
		CAN_ITConfig(CAN1, CAN_IT_FMP0, ENABLE);
		
		bOk = FALSE;
	}
	else
	{
		bOk = TRUE;
	}
	return bOk;
}


u8 canSend(CAN_PORT notused, Message *TxMessage)
{	
	u8 mailBox = 0, relust = 0;

	g_sCanFrame.waitTimes = 0;
	g_sCanFrame.txMasg.StdId = TxMessage->cob_id;   			// ��׼��ʶ��
	g_sCanFrame.txMasg.IDE = CAN_ID_STD;   						// ʹ�ñ�׼��ʶ��
	g_sCanFrame.txMasg.RTR = (TxMessage->rtr == CAN_RTR_DATA ? 0 : 2);	
	g_sCanFrame.txMasg.DLC = TxMessage->len;
	memcpy(g_sCanFrame.txMasg.Data, TxMessage->data, TxMessage->len);
	mailBox = CAN_Transmit(CAN_PORT1, &g_sCanFrame.txMasg);   // ���ͱ���   
	// �ȴ����ͽ���
    while(CAN_TransmitStatus(CAN_PORT1, mailBox) != CANTXOK && g_sCanFrame.waitTimes < CAN_SEND_TIMEOUT)
    {
        g_sCanFrame.waitTimes++;
    }
    
    if(g_sCanFrame.waitTimes < CAN_SEND_TIMEOUT)
    {
       relust = CANTXOK;
    }
	
	return relust;		
}


void Can_Receive_Msg()
{		   		   
	Message rxMsg;
	// û�н��յ�����,ֱ���˳� 
	if(CAN_MessagePending(CAN_PORT1,CAN_FIFO0))
	{
		CAN_Receive(CAN_PORT1, CAN_FIFO0, &g_sCanFrame.rxMasg);   // ��ȡ����	
		rxMsg.cob_id = g_sCanFrame.rxMasg.StdId;
		rxMsg.rtr = g_sCanFrame.rxMasg.RTR == (CAN_RTR_DATA ? 0 : 1);
		rxMsg.len = g_sCanFrame.rxMasg.DLC;
		memcpy(rxMsg.data, g_sCanFrame.rxMasg.Data, g_sCanFrame.rxMasg.DLC);	
		canDispatch(&AnyId_Canopen_Slave_Data, &rxMsg);					//can֡���մ�����
	}	
}




