#include "AnyID_CanOpen_Tim_HL.h"

static TIMEVAL last_time_set = 0;

void Tim_InitInterface()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	TIM_TimeBaseStructure.TIM_Period = 65536 - 1; 

	TIM_TimeBaseStructure.TIM_Prescaler = 72 - 1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	TIM_ITConfig(TIM1,TIM_IT_Update,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM1_UP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM1, ENABLE);
}

void setTimer(TIMEVAL value)
{
	uint16_t capture = 0;

	capture = TIM1->CNT;					/* ������ֵ */
	TIM1->ARR = capture + value;	/* �Զ����ؼĴ��� */
}

/* ��ȡ��ʱ����ǰ����ֵ */
TIMEVAL getElapsedTime(void)
{
	uint16_t timer = TIM1->CNT;	/* ������ֵ */
	timer = timer - last_time_set;
	return timer;
}

void TIM1_UP_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM1,TIM_IT_Update)!=RESET)
    {
        TIM_ClearITPendingBit(TIM1,TIM_IT_Update);
		last_time_set = TIM1->CNT;
		TimeDispatch();
    }
}
