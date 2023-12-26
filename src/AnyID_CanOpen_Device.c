#include "AnyID_CanOpen_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "M3331 23121201 G231212";

DEVICE_PARAMS g_sDeviceParams = {0};


void Device_Init()
{






}



void Device_ReadDeviceParamenter(void)
{
     BOOL b = FALSE, bBackup = FALSE;
     
    b = FRam_ReadBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    if(b)
    {
        u32 crc1 = 0, crc2 = 0;

        crc1 = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);
        crc2 = g_sDeviceParams.crc;

        //检测参数是否正确，如果不正确，使用默认参数操作
        if(crc1 != crc2)
        {
            b = FALSE;
        }
    }  
    bBackup = FRam_ReadBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    if(bBackup)
    {
        u32 crc1 = 0, crc2 = 0;

        crc1 = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);
        crc2 = g_sDeviceParams.crc;

        //检测参数是否正确，如果不正确，使用默认参数操作                        .
        if(crc1 != crc2)
        {
            bBackup = FALSE;
        }
    }
    if((b == FALSE && bBackup == FALSE))
    {
        memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
        //can硬件参数
		//v 典型值：1000K、800k、500K、250k、125k、50k、20k、10k
		//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 60/(10 *(6 + 1 + 6 + 1+ 1)) 
		//v = 4000 / psc，默认1000K

		g_sDeviceParams.canPara.CAN_BS1 = CAN_BS1_7tq;
		g_sDeviceParams.canPara.CAN_BS2 = CAN_BS2_7tq;
		g_sDeviceParams.canPara.CAN_SJW = CAN_SJW_2tq;
		g_sDeviceParams.canPara.CAN_Prescaler = DEVICE_CAN_BUD_1000K_PSCVALUE;
		g_sDeviceParams.canPara.CAN_Mode = CAN_Mode_Normal;
		g_sDeviceParams.canPara.CAN_ABOM = DISABLE;   								// 软件自动离线管理
		g_sDeviceParams.canPara.CAN_AWUM = DISABLE;   								// 睡眠模式通过软件唤醒(清除CAN->MCR的SLEEP位)
		g_sDeviceParams.canPara.CAN_NART = ENABLE;   								// 使用报文自动传送 
		g_sDeviceParams.canPara.CAN_RFLM = DISABLE;   								// 报文不锁定,新的覆盖旧的 
		g_sDeviceParams.canPara.CAN_TTCM = DISABLE;									// 非时间触发通信模式  
		g_sDeviceParams.canPara.CAN_TXFP = DISABLE;   								// 优先级由报文标识符决定 
		//can过滤器
		g_sDeviceParams.canFilterPara.CAN_FilterNumber = 0;  					 	// 过滤器0	
		g_sDeviceParams.canFilterPara.CAN_FilterIdHigh = 0x0000;   					// 32位ID
		g_sDeviceParams.canFilterPara.CAN_FilterIdLow = 0x0000;
		g_sDeviceParams.canFilterPara.CAN_FilterMaskIdHigh = 0x0000;   				// 32位MASK
		g_sDeviceParams.canFilterPara.CAN_FilterMaskIdLow = 0x0000;
		g_sDeviceParams.canFilterPara.CAN_FilterMode = CAN_FilterMode_IdMask;   	// 掩码模式
		g_sDeviceParams.canFilterPara.CAN_FilterScale = CAN_FilterScale_32bit;   	// 32位 
		g_sDeviceParams.canFilterPara.CAN_FilterFIFOAssignment = CAN_FilterFIFO0;   // 过滤器0关联到FIFO0
		g_sDeviceParams.canFilterPara.CAN_FilterActivation = ENABLE;				// 激活过滤器0

        Device_WriteDeviceParamenter();
    }
    else if(b == TRUE && bBackup == FALSE)
    {
        FRam_ReadBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
        FRam_WriteBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    }
    else if(b == FALSE && bBackup == TRUE)
    {
        FRam_ReadBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
        FRam_WriteBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    }
    
    Fram_ReadBootParamenter();
	if((g_sFramBootParamenter.appState != FRAM_BOOT_APP_OK) )
    {
        g_sFramBootParamenter.appState = FRAM_BOOT_APP_OK;
        Fram_WriteBootParamenter();
    }

}


BOOL Device_WriteDeviceParamenter(void)
{
    BOOL b = FALSE;

    g_sDeviceParams.crc = 0;
    g_sDeviceParams.crc = a_GetCrc((u8 *)(&g_sDeviceParams), (sizeof(DEVICE_PARAMS)) - 4);

    b = FRam_WriteBuffer(FRAME_INFO_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    b = FRam_WriteBuffer(FRAME_INFO_BACKUP_ADDR, sizeof(DEVICE_PARAMS), (u8 *)(&g_sDeviceParams));
    
    return b;
}
