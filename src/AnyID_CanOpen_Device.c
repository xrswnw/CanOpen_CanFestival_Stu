#include "AnyID_CanOpen_Device.h"

const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE]@0x08005000 = "SM5001 23102100 GD322302";


DEVICE_PARAMS g_sDeviceParams = {0};






void Device_ReadDeviceParamenter(void)                             		     //OK
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
    if(1)//((b == FALSE && bBackup == FALSE))
    {
        memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
        
		//bud = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 60/(10 *(4 + 1 + 5+ 1+ 1))  = 500K，默认
		g_sDeviceParams.canPara.mode = CAN_Mode_Normal;			
		g_sDeviceParams.canPara.psc = 20;
		g_sDeviceParams.canPara.tbs1 = CAN_BS1_5tq;
		g_sDeviceParams.canPara.tbs2 = CAN_BS2_6tq;
		g_sDeviceParams.canPara.tsjw = CAN_SJW_2tq;
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


void Device_CanPeriphInit()
{
	setNodeId(&ObjDict_Data, 10);
	setState(&ObjDict_Data, Initialisation);
}