#include "AnyID_CanOpen_SysCfg.h"

int main(void)
{
    Sys_Init();
    while(1)
    {
        Sys_LedTask();
    }
}






