#ifndef _ANYID_SM5001_DEVICE_H
#define _ANYID_SM5001_DEVICE_H

#include "AnyID_CanOpen_Config.h"
#include "AnyID_CanOpen_Can_HL.h"
#include "AnyID_CanOpen_FRam.h"
#include "AnyID_CanOpen_Tim_HL.h"



#define DEVICE_VERSION_SIZE 50















typedef struct deviceParamenter{








	CAN_PARAMENTERINFO canPara;
	u32 crc;
}DEVICE_PARAMS;

extern DEVICE_PARAMS g_sDeviceParams;



BOOL Device_WriteDeviceParamenter(void);
void Device_ReadDeviceParamenter(void);
void Device_CanPeriphInit();
#endif