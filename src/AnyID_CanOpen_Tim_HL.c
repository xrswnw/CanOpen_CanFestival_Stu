#include "AnyID_CanOpen_Tim_HL.h"
void Tim_InitInterface()
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure = {0};
	NVIC_InitTypeDef NVIC_InitStructure = {0};
	
	//CK_CNT_OV = CK_PSC / (PSC + 1) / (ARR + 1),			分频为1Mhz,每1us产生一个中断，canfestival最小時間單位是1us,若计时基数更改，需到timescfg更改对应宏定义
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

/*更新当前Tick*/
void setTimer(TIMEVAL value)
{
	TIM_PORT->ARR =  value;
}

/*获取当前Tick*/
TIMEVAL getElapsedTime(void)
{
	return TIM_PORT->CNT;
}

/*CanFestival调度*/
void TIM1_UP_IRQHandler(void) 
{
    if(TIM_GetITStatus(TIM_PORT, TIM_IT_Update)!= RESET)
    {
        TIM_ClearITPendingBit(TIM_PORT, TIM_IT_Update);
		TimeDispatch();					
    }
}
