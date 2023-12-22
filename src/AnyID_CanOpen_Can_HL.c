#include "AnyID_CanOpen_Can_HL.h"

const PORT_INF CAN_RX_PORT = {GPIOA, GPIO_Pin_11};
const PORT_INF CAN_TX_PORT = {GPIOA, GPIO_Pin_12};

CAN_FRAME g_sCanFrame = {0};

BOOL Can_InitInterface()
{
	BOOL bOk = FALSE;
	GPIO_InitTypeDef GPIO_InitStructure = {0};
	CAN_InitTypeDef  CAN_InitStructure = {0};
	CAN_FilterInitTypeDef CAN_FilterInitStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	DEVICE_PARAMS *pCanOpenPara = NULL;
	pCanOpenPara = &g_sDeviceParams;
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_RX_PORT.Pin;
	
	GPIO_Init(CAN_RX_PORT.Port, &GPIO_InitStructure);	

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = CAN_TX_PORT.Pin;
	
	GPIO_Init(CAN_TX_PORT.Port, &GPIO_InitStructure);

	CAN_InitStructure.CAN_ABOM = pCanOpenPara->canPara.CAN_ABOM;			
	CAN_InitStructure.CAN_AWUM = pCanOpenPara->canPara.CAN_AWUM;   			
	CAN_InitStructure.CAN_NART = pCanOpenPara->canPara.CAN_NART;			
	CAN_InitStructure.CAN_RFLM = pCanOpenPara->canPara.CAN_RFLM;   			
	CAN_InitStructure.CAN_TTCM = pCanOpenPara->canPara.CAN_TTCM;			
	CAN_InitStructure.CAN_TXFP = pCanOpenPara->canPara.CAN_TXFP;   			
	CAN_InitStructure.CAN_BS1 = pCanOpenPara->canPara.CAN_BS1;
	CAN_InitStructure.CAN_BS2 = pCanOpenPara->canPara.CAN_BS2;	
	CAN_InitStructure.CAN_SJW =  pCanOpenPara->canPara.CAN_SJW;	
	CAN_InitStructure.CAN_Mode = pCanOpenPara->canPara.CAN_Mode;
	CAN_InitStructure.CAN_Prescaler = pCanOpenPara->canPara.CAN_Prescaler;
	
	if(CAN_Init(CAN_PORT1, &CAN_InitStructure) == CANINITOK)		
	{
		CAN_FilterInitStructure.CAN_FilterNumber = pCanOpenPara->canFilterPara.CAN_FilterMode;  					
		CAN_FilterInitStructure.CAN_FilterMode = pCanOpenPara->canFilterPara.CAN_FilterMode;   
		CAN_FilterInitStructure.CAN_FilterScale = pCanOpenPara->canFilterPara.CAN_FilterScale;   
		CAN_FilterInitStructure.CAN_FilterIdHigh = pCanOpenPara->canFilterPara.CAN_FilterIdHigh;   			
		CAN_FilterInitStructure.CAN_FilterIdLow = pCanOpenPara->canFilterPara.CAN_FilterIdLow;
		CAN_FilterInitStructure.CAN_FilterMaskIdHigh = pCanOpenPara->canFilterPara.CAN_FilterMaskIdHigh;   			
		CAN_FilterInitStructure.CAN_FilterMaskIdLow = pCanOpenPara->canFilterPara.CAN_FilterMaskIdLow;
		CAN_FilterInitStructure.CAN_FilterFIFOAssignment = pCanOpenPara->canFilterPara.CAN_FilterFIFOAssignment;  
		CAN_FilterInitStructure.CAN_FilterActivation = pCanOpenPara->canFilterPara.CAN_FilterActivation;  	
		CAN_FilterInit(&CAN_FilterInitStructure);  	
		
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
	g_sCanFrame.txMasg.StdId = TxMessage->cob_id;   			// 标准标识符
	g_sCanFrame.txMasg.IDE = CAN_ID_STD;   						// 使用标准标识符
	g_sCanFrame.txMasg.RTR = (TxMessage->rtr == CAN_RTR_DATA ? 0 : 2);	
	g_sCanFrame.txMasg.DLC = TxMessage->len;
	memcpy(g_sCanFrame.txMasg.Data, TxMessage->data, TxMessage->len);
	mailBox = CAN_Transmit(CAN_PORT1, &g_sCanFrame.txMasg);   // 发送报文   
	// 等待发送结束
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
	// 没有接收到数据,直接退出 
	if(CAN_MessagePending(CAN_PORT1,CAN_FIFO0))
	{
		CAN_Receive(CAN_PORT1, CAN_FIFO0, &g_sCanFrame.rxMasg);   // 读取数据	
		rxMsg.cob_id = g_sCanFrame.rxMasg.StdId;
		rxMsg.rtr = g_sCanFrame.rxMasg.RTR == (CAN_RTR_DATA ? 0 : 1);
		rxMsg.len = g_sCanFrame.rxMasg.DLC;
		memcpy(rxMsg.data, g_sCanFrame.rxMasg.Data, g_sCanFrame.rxMasg.DLC);	
		canDispatch(&AnyId_Canopen_Slave_Data, &rxMsg);					//can帧接收处理函数,此处可做处理，选择是否接入Can Open
	}	
}




