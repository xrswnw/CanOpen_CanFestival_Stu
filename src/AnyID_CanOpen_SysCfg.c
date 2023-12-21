#include "AnyID_CanOpen_SysCfg.h"

u32 g_nSysState = SYS_STAT_IDLE;
u32 g_nLedDelayTime = 0;
u32 g_nTestIndex = 0;


#if SYS_ENABLE_WDT
    #define SYS_ENABLE_TEST         0
#else
    #define SYS_ENABLE_TEST         1
#endif

void Sys_Delayms(u32 n)
{
    //72MHZ
    n *= 0x6000;
    n++;
    while(n--);
}


void Sys_CfgClock(void)
{
    ErrorStatus HSEStartUpStatus;

    RCC_DeInit();
    //Enable HSE
    RCC_HSEConfig(RCC_HSE_ON);

    //Wait till HSE is ready
    HSEStartUpStatus = RCC_WaitForHSEStartUp();

    if(HSEStartUpStatus == SUCCESS)
    {

        //HCLK = SYSCLK = 72.0M
        RCC_HCLKConfig(RCC_SYSCLK_Div1);

        //PCLK2 = HCLK = 72.0M
        RCC_PCLK2Config(RCC_HCLK_Div1);

        //PCLK1 = HCLK/2 = 33.9M
        RCC_PCLK1Config(RCC_HCLK_Div2);

        //ADCCLK = PCLK2/2
        RCC_ADCCLKConfig(RCC_PCLK2_Div8);

        // Select USBCLK source 72 / 1.5 = 48M
        RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);

        //Flash 2 wait state
        FLASH_SetLatency(FLASH_Latency_2);

        //Enable Prefetch Buffer
        FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

		
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_15);    //PLL在最后设置

        //Enable PLL
        RCC_PLLCmd(ENABLE);

        //Wait till PLL is ready
        while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
        {
        }

        //Select PLL as system clock source
        RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

        //Wait till PLL is used as system clock source
        while(RCC_GetSYSCLKSource() != 0x08)
        {
        }
    }
}

void Sys_CfgPeriphClk(FunctionalState state)
{

    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA |
                           RCC_APB2Periph_GPIOB |
                           RCC_APB2Periph_GPIOC |
                           RCC_APB2Periph_AFIO  |
							RCC_APB2Periph_TIM1|
                           RCC_APB2Periph_GPIOD, state);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1 | RCC_AHBPeriph_DMA2, state);
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_CAN1 |
						   RCC_APB1Periph_USART3 |
                           RCC_APB1Periph_PWR    |
                           RCC_APB1Periph_BKP , state);
}

void Sys_CfgNVIC(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
    //Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  //VECT_TAB_FLASH
    //Set the Vector Table base location at 0x08000000
#ifdef _ANYID_BOOT_GD32_
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
#endif

    //Configure the Priority Group to 2 bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}


const PORT_INF SYS_RUN_LED  = {GPIOB, GPIO_Pin_1};


void Sys_CtrlIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure = {0};
     
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = SYS_RUN_LED.Pin;
    GPIO_Init(SYS_RUN_LED.Port, &GPIO_InitStructure);

}


void Sys_Init(void)
{
    Sys_CfgClock();
    Sys_CfgNVIC();
    Sys_CfgPeriphClk(ENABLE);
    Sys_CtrlIOInit();


#if SYS_ENABLE_WDT
    WDG_InitIWDG();
#endif
	FRam_InitInterface();
	Device_ReadDeviceParamenter();
	//RCC_ClocksTypeDef g_sSysclockFrep; RCC_GetClocksFreq(&g_sSysclockFrep);    查看时钟频率
 	if(Can_InitInterface())//can初始化失败
	{			
		while(1)
		{
		
		}
	}
	Periph_InitInterface();
	RS485_Init(UART_BAUDRARE);
    Tim_InitInterface();
	STick_InitSysTick();
	
	//
	Sys_StateInit();	//设备上线，bootup
    Sys_EnableInt();
}

void Sys_LedTask(void)
{
	static u32 g_nTempTick = 0, j = 0;
    if(a_CheckStateBit(g_nSysState, SYS_STAT_RUNLED))
    {
		g_nLedDelayTime++;
		a_ClearStateBit(g_nSysState, SYS_STAT_RUNLED);
		if(g_nLedDelayTime & 0x01)
		{
			PeriphLed1_Open();
			PeriphLed2_Close();
			PeriphLed3_Open();
		}
		else
		{
			PeriphLed1_Close();
			PeriphLed2_Open();
			PeriphLed3_Close();
		}
		
		if(g_nTempTick >= 10)
		{
			uid += 2;
			g_nTempTick = 0;
			testbuf[j++] += (testbuf[j] % 5 + j + 1);
			//u8 wnull[3] = {0x11, 0x22, 0x33};
			//Uart_WriteBuffer(wnull, 3);	
			if(j > 2)
			{
				j = 0;
			}
		}
		else
		{
			g_nTempTick++;
		}
	#if SYS_ENABLE_WDT
	WDG_FeedIWDog();
	#endif

    }
  
}

void Sys_PdoTask()
{
	keyValue = Periph_GetKeyValue();
	addrValue = Periph_GetAddr();
}




void Sys_UartTask(void)
{
/*
    if(USART_GetFlagStatus(UART_PORT, USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE))
    {
        USART_ClearFlag(UART_PORT, USART_FLAG_ORE | USART_FLAG_NE | USART_FLAG_FE | USART_FLAG_PE);
        Uart_InitInterface(UART_BAUDRARE);
        Uart_ConfigInt();
        Uart_EnableInt(ENABLE, DISABLE);
    }


    if(Uart_IsRcvFrame(g_sRS485RcvFrame))
    {
        memcpy(&g_sUartRcvTempFrame, &g_sRS485RcvFrame, sizeof(g_sRS485RcvFrame));
        Uart_ResetFrame(&g_sRS485RcvFrame);
        
        if(g_sUartRcvTempFrame.length >= UART_FRAME_MIN_LEN)
        {
            u16 crc1 = 0, crc2 = 0;
            
            crc1 = Uart_GetFrameCrc(g_sUartRcvTempFrame.buffer, g_sUartRcvTempFrame.length);
            crc2 = a_GetCrc(g_sUartRcvTempFrame.buffer + UART_FRAME_POS_LEN, g_sUartRcvTempFrame.length - 4);

            if(crc1 == crc2)
            {
                u16 txLen = 1;//Reader_ProcessUartFrame(g_sUartRcvTempFrame.buffer, g_sUartRcvTempFrame.length);
                if(txLen > 0)
                {
                    a_SetStateBit(g_nSysState, SYS_STAT_UARTTX);
                }
            }
        }
    }
    
    if(a_CheckStateBit(g_nSysState, SYS_STAT_UARTTX))
    {
        a_ClearStateBit(g_nSysState, SYS_STAT_UARTTX);
        Uart_WriteBuffer(g_sReaderRspFrame.buffer, g_sReaderRspFrame.len);
        if(g_sReaderRspFrame.cmd == READER_CMD_RESET)
        {
            Sys_Delayms(5);
            Sys_SoftReset();
        }
    }
*/
}


void Sys_StateInit()
{
	setNodeId(&AnyId_Canopen_Slave_Data, Periph_GetAddr());
	setState(&AnyId_Canopen_Slave_Data, Initialisation);			//设备上线
	
}