#ifndef _ANYID_SM5001_SYSCFG_H
#define _ANYID_SM5001_SYSCFG_H

#include "AnyID_CanOpen_SysTick_HL.h"
#include "AnyID_CanOpen_RTC.h"
#include "AnyID_CanOpen_WDG_HL.h"
#include "AnyID_CanOpen_Tim_HL.h"
#include "AnyID_CanOpen_Can_HL.h"
#include "AnyID_CanOpen_RS485.h"
#include "AnyID_CanOpen_Periph_HL.h"


extern u32 g_nSysState;
#define SYS_NULL_TICK                   0

#define SYS_STAT_IDLE                   0x00000001	        //
#define SYS_STAT_RUNLED                 0x00000002
#define SYS_STAT_UARTTX                 0x00000004


//²âÊÔ
#define SYS_STAT_FIRE_TEST               0x80000000
#define SYS_STAT_SOUND_TEST              0x40000000
#define SYS_STAT_LTE_TEST                0x20000000
#define SYS_STAT_GATE_TEST               0x10000000


#define SYS_LED_DELAY                   20
#define SYS_CHECK_CFGMODE_TIM           200

extern const PORT_INF SYS_RUN_LED;
#define Sys_LedOff()                  SYS_RUN_LED.Port->BSRR = SYS_RUN_LED.Pin
#define Sys_LedOn()                   SYS_RUN_LED.Port->BRR = SYS_RUN_LED.Pin


void Sys_Delayms(u32 n);
void Sys_CfgClock(void);
void Sys_CfgPeriphClk(FunctionalState state);
void Sys_CfgNVIC(void);

void Sys_CtrlIOInit(void);

void Sys_Init(void);
void Sys_LedTask(void);
void Sys_PdoTask();
void Sys_UartTask(void);
void Sys_StateInit();
void Sys_DefaultInfoInit();
#endif
