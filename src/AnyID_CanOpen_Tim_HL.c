#include "AnyID_CanOpen_Tim_HL.h"
void Tim_InitInterface()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	//CK_CNT_OV = CK_PSC / (PSC + 1) / (ARR + 1),			��ƵΪ1Mhz,ÿ1us����һ���жϣ�canfestival��С�r�g��λ��1us,����ʱ�������ģ��赽timescfg���Ķ�Ӧ�궨��
	TIM_TimeBaseStructure.TIM_Period = 0xFFFF - 1; 
	TIM_TimeBaseStructure.TIM_Prescaler = TIM_PSE_SYS_FREQ;			
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM_PORT, &TIM_TimeBaseStructure);
	
	TIM_ITConfig(TIM_PORT,TIM_IT_Update,ENABLE);
	TIM_ClearITPendingBit(TIM_PORT, TIM_IT_Update);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM_PORT, ENABLE);
}

/*���µ�ǰTick*/
void setTimer(TIMEVAL value)
{
	TIM_PORT->ARR =  value;
}

/*��ȡ��ǰTick*/
TIMEVAL getElapsedTime(void)
{
	return TIM_PORT->CNT;
}

/*CanFestival����*/
void TIM1_UP_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM_PORT, TIM_IT_Update)!= RESET)
    {
        TIM_ClearITPendingBit(TIM_PORT, TIM_IT_Update);
		TimeDispatch();					
    }
}
