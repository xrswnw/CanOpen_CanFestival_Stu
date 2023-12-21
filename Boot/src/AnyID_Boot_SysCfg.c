#include "AnyID_Boot_SysCfg.h"

u32 g_nSysState = 0;
u32 g_nDeviceNxtEraseAddr = 0;
u32 g_nDeviceNxtDownloadAddr = 0;
u32 g_nBootLimitTime = 0;

#define SYS_BOOT_VER_SIZE               50
const u8 SYS_BOOT_VERSION[SYS_BOOT_VER_SIZE] = "Boot V3.1_22032600 GD32F3xx";

void Sys_Delayms(u32 n)             //系统延时n毫秒
{
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

        //PLLCLK = 12.00MHz * 10 = 120 MHz
        RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_10);    //PLL在最后设置

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
                          RCC_APB2Periph_GPIOD |
                          RCC_APB2Periph_AFIO, state);
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2 | 
                           RCC_APB1Periph_USART3 |RCC_APB1Periph_UART4|
                           RCC_APB1Periph_UART5  , state);

}

void Sys_CfgNVIC(void)
{
    //NVIC_InitTypeDef NVIC_InitStructure;
#ifdef  VECT_TAB_RAM
    //Set the Vector Table base location at 0x20000000
    NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  //VECT_TAB_FLASH
    //Set the Vector Table base location at 0x08000000
#ifdef _ANYID_BOOT_STM32_
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x4000);
#else
    NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0000);
#endif
#endif

    //Configure the Priority Group to 2 bits
    NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
}

const PORT_INF SYS_RUNLED_COM = {GPIOB, GPIO_Pin_1};
const PORT_INF SYS_DATALED_COM = {GPIOB, GPIO_Pin_3};
void Sys_CtrlIOInit(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    
    GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);

    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    GPIO_InitStructure.GPIO_Pin = SYS_RUNLED_COM.Pin;
    GPIO_Init(SYS_RUNLED_COM.Port, &GPIO_InitStructure);
    GPIO_InitStructure.GPIO_Pin = SYS_DATALED_COM.Pin;
    GPIO_Init(SYS_DATALED_COM.Port, &GPIO_InitStructure);

}



void Sys_Init(void)
{
#if SYS_ENABLE_WDT
    WDG_InitIWDG();
#endif
    //
    Sys_CfgClock();

    Sys_CfgNVIC();
    Sys_CfgPeriphClk(ENABLE);

    //禁止中断
    Sys_DisableInt();
    Sys_CtrlIOInit();
    Sys_RunLedOn();

    FRam_InitInterface();
    Fram_ReadBootParamenter();
    
    Flash_InitInterface();
    Flash_Init();
    //Flash_ReadId();
    //Flash_Demo();               //--------------------测试

    Gate_InitInterface(GATE_BAUDRARE);
    Gate_ConfigInt(ENABLE);
    Gate_EnableInt(ENABLE, DISABLE);
	
	//g_sFramBootParamenter.appState = FRAM_BOOT_APP_DATA_DOWD;
    
    if(g_sFramBootParamenter.appState != FRAM_BOOT_APP_OK)
    {
      //  EC20_Init();
       // EC20_ConnectInit(&g_sEC20Connect, EC20_CNT_CMD_PWRON, &g_sEC20Params);
        //a_SetState(g_sEC20Connect.state, EC20_CNT_OP_STAT_TX);
    }

    //SysTick 初始化 5ms
    STick_InitSysTick();
    Sys_RunLedOff();

    //使能中断
    Sys_EnableInt();
    

    
}

void Sys_LedTask(void)
{ 
    static u32 ledTimes = 0;
    if(a_CheckStateBit(g_nSysState, SYS_STAT_RUNLED))
    {
        a_ClearStateBit(g_nSysState, SYS_STAT_RUNLED);
        ledTimes ++;
        if(ledTimes & 0x01)
        {
            Sys_RunLedOff();
            Sys_DataLedOff();
        }
        else
        {
            Sys_RunLedOn();
            Sys_DataLedOn();
        }
    #if SYS_ENABLE_WDT
        WDG_FeedIWDog();
    #endif
    }
}

typedef  void (*pFunction)(void);
pFunction Jump_To_Application;
uint32_t JumpAddress;
void Sys_Jump(u32 address)
{
    u32 stackAddr = 0;
    Sys_DisableInt();
    stackAddr = *((vu32 *)address);
    //查看栈地址是否在RAM区或者CCRAM区
    if((stackAddr & 0x2FFE0000) == 0x20000000)
    {
        JumpAddress = *(vu32 *)(address + 4);
        Jump_To_Application = (pFunction) JumpAddress;

        __set_MSP(*(vu32 *)address);
        Jump_To_Application();
    }
    else
    {
        a_SetState(g_nSysState, SYS_STAT_IDLE);
        g_nSysTick = 0;
    }
    //while(1)
    {
    #if SYS_ENABLE_WDT
        WDG_FeedIWDog();
    #endif
    }
    Sys_EnableInt();
}

void Sys_GateTask(void)
{

}

void Sys_BootTask(void)
{
}

BOOL Sys_CheckVersion(void)
{
    BOOL b = FALSE;
    u8 *p = (u8 *)SYS_BOOT_VER_ADDR;
    u8 i = 0, c = 0;
        
    if(memcmp(p, SYS_VER_HEAD, SYS_VER_HEAD_SIZE) == 0) //设备型号正确
    {
        for(i = SYS_VER_HEAD_SIZE; i < SYS_VERSION_SIZE; i++)
        {
            c = *p++;
            if((c < ' ' || c > 127) && (c != 0x00))
            {
                break;
            }
        }
        if(i == SYS_VERSION_SIZE)
        {
            b = TRUE;
        }
    }

    if(b == FALSE)
    {
        if(memcmp((u8 *)SYS_BOOT_HARDTYPE_ADDR, SYS_VER_HARD_TYPE, sizeof(SYS_VER_HARD_TYPE) - 1) == 0) //设备硬件型号正确也可以
        {
            b = TRUE;
        }
    }
    
    return b;
}

void Sys_EC20Task(void)
{
  
}

void Sys_ServerTask(void)
{
}

void Sys_DownDataTask()
{   
   

}

