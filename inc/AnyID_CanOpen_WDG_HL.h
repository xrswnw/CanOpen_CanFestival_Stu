#ifndef _ANYID_CANOPEN_WDG_HL_
#define _ANYID_CANOPEN_WDG_HL_

#include "AnyID_CanOpen_Config.h"


void WDG_InitIWDG(void);


#if SYS_ENABLE_WDT
    #define WDG_FeedIWDog()	IWDG_ReloadCounter()
#else
    #define WDG_FeedIWDog()	
#endif


#endif

