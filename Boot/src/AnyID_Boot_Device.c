#include "AnyID_Boot_Device.h"

DEVICE_SENVER_TXBUFFER g_sDeviceServerTxBuf = {0};
DEVICE_UPDATA_INFO g_sDeviceUpDataInfo;




void Device_Delayms(u32 n)           
{
    n *= 0x6000;
    n++;
    while(n--);
}


void Device_CommunTxCmd(DEVICE_SENVER_TXBUFFER *pCntOp, u32 sysTick)
{
   
   
}



BOOL Device_CommunCheckRsp(DEVICE_SENVER_TXBUFFER *pCntOp, u8 *pRxBuf)
{
 
}


void Device_CommunStep(DEVICE_SENVER_TXBUFFER *pCntOp)
{
}


BOOL Device_Chk_VersionFrame(u8 *pBuffer, DEVICE_UPDATA_INFO *pDataInfo)
{
   
}

BOOL Device_WeiteData(DEVICE_UPDATA_INFO *pDataInfo)
{
}


void Device_Analys_Data(DEVICE_UPDATA_INFO *pDataInfo)
{
}

char *Device_Analys_Data_Len(char *lenBuffer, u8 step)
{

}


u32 Device_Search_Data(u8 *pBuffer)
{

}


BOOL Device_Chk_Version()
{

}








BOOL Device_Erase_McuFlash(u32 addr)
{
    BOOL tf = FALSE;




    return tf;
}