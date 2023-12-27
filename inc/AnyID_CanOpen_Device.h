#ifndef ANYID_CANOPEN_DEVICE_H_
#define ANYID_CANOPEN_DEVICE_H_

#include "AnyID_CanOpen_FRam.h"
#include "AnyID_CanOpen_RS485.h"

#define DEVICE_VERSION_SIZE 50


//典型通信参数
#define DEVICE_CAN_BUD_1000K_PSCVALUE		4
#define DEVICE_CAN_BUD_800K_PSCVALUE		5
#define DEVICE_CAN_BUD_500K_PSCVALUE		8
#define DEVICE_CAN_BUD_250K_PSCVALUE		16
#define DEVICE_CAN_BUD_125K_PSCVALUE		32
#define DEVICE_CAN_BUD_50K_PSCVALUE			80
#define DEVICE_CAN_BUD_20K_PSCVALUE			200
#define DEVICE_CAN_BUD_10K_PSCVALUE			400



typedef struct deviceParamenter{
	u8 mode;
	u16 addr;
	u16 flag;
	u16 rfu1;
	u16 rfu2;
	u16 rfu3;
	u16 rfu4;
	CAN_InitTypeDef canPara;
	CAN_FilterInitTypeDef canFilterPara;
	u32 crc;
}DEVICE_PARAMS;
extern DEVICE_PARAMS g_sDeviceParams;

#define DEVICE_FLAG_RESET					0x01
typedef struct deviceInfo{
	u8 state;
	u8 flag;



}DEVICE_INFO;	
extern DEVICE_INFO g_sDeviceInfo;

#define DEVICE_VERSION_DEVICE_NAME_LEN			5
#define DEVICE_VERSION_DEVICE_SOFTWARE_LEN		8
#define DEVICE_VERSION_DEVICE_HARDWARE_LEN		7
extern const u8 DEVICE_VERSION[DEVICE_VERSION_SIZE];






void Device_ReadDeviceParamenter();
BOOL Device_WriteDeviceParamenter(void);
#endif