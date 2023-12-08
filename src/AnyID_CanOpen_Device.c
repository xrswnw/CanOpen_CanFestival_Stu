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
    if((b == FALSE && bBackup == FALSE))
    {
        memset(&g_sDeviceParams, 0, sizeof(DEVICE_PARAMS));
        
		//v 典型值：1000K、800k、500K、250k、125k、50k、20k、10k
		//v = PLCK1 / (psc * (tbs1 + 1 + tbs2 + 1 + 1)) = 60/(10 *(6 + 1 + 6 + 1+ 1)) 
		//v = 4000 / psc
		//psc : 1 to 5000
		g_sDeviceParams.canPara.mode = CAN_Mode_Normal;			
		g_sDeviceParams.canPara.psc = CAN_BUD_1000K_PSCVALUE;	//可变参数，调整can波特率
		g_sDeviceParams.canPara.tbs1 = CAN_BS1_7tq;				//暂时固定
		g_sDeviceParams.canPara.tbs2 = CAN_BS2_7tq;				//暂时固定
		g_sDeviceParams.canPara.tsjw = CAN_SJW_2tq;				//暂时固定
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
	setNodeId(&AnyId_Canopen_Slave_Data, /*1*/Periph_GetAddr());
	setState(&AnyId_Canopen_Slave_Data, Initialisation);			//设备上线
	//	setState(&AnyId_Canopen_Client_Data, Operational);			//设备动作
}