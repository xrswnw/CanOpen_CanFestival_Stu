#include "demo.h"

//////////////////////////////////////////////////SDO配置(服务器)///////////////////////////////////////////////////////////////////
/* 子索引个数 */
UNS8 ObjDict_highestSubIndex_obj1200 = 3;
/* 客户端->服务器用的COB-ID */
UNS32 ObjDict_obj1200_COB_ID_Client_to_Server_Transmit_SDO = 0x600 + 1;	/* 1号节点 */
/* 服务器->客户端用的COB-ID */
UNS32 ObjDict_obj1200_COB_ID_Server_to_Client_Receive_SDO = 0x580 + 1;	/* 1号节点 */
/* 对应服务器的节点号 */
UNS8 ObjDict_obj1200_Node_ID_of_the_SDO_Server = 0x1;	/* 1号节点 */
subindex ObjDict_Index1200[] = 
{
	{RO, uint8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj1200},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1200_COB_ID_Client_to_Server_Transmit_SDO},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1200_COB_ID_Server_to_Client_Receive_SDO},
	{RW, uint8, sizeof(UNS8), (void *)&ObjDict_obj1200_Node_ID_of_the_SDO_Server}
};
////////////////////////////////////////////发送PDO通讯参数配置(客户端)///////////////////////////////////////////////////////////////
/* 子索引数 */
UNS8 ObjDict_highestSubIndex_obj1800 = 5;
/* 从站的发送PDO对应着主站的接收PDO，所以两边的COB-ID需要保持一致 */
UNS32 ObjDict_obj1800_COB_ID_used_by_PDO = 0x181;	/* PDO1 1号节点 */
/* PDO类型，0表示同步非周期，1-240表示同步周期，252表示同步RTR，253表示异步RTR，254表示异步制造商特定事件、255表示异步设备子协议特定事件，这里配置为同步一次 */
UNS8 ObjDict_obj1800_Transmission_Type = 0x01;
/* 因为波特率为256kB/S，标准帧最长108位 */
UNS16 ObjDict_obj1800_Inhibit_Time = 1000; /* 0 0.1ms */
/* 保留不使用 */
UNS8 ObjDict_obj1800_Compatibility_Entry = 0x0;
/* 从站作为事件定时周期，主站作为事件超时时间，这里实验用同步周期所以不进行配置 */
UNS16 ObjDict_obj1800_Event_Timer = 0;
subindex ObjDict_Index1800[] = 
{
	{RO, uint8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj1800},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1800_COB_ID_used_by_PDO},
	{RW, uint8, sizeof(UNS8), (void *)&ObjDict_obj1800_Transmission_Type},
	{RW, uint16, sizeof(UNS16), (void *)&ObjDict_obj1800_Inhibit_Time},
	{RW, uint8, sizeof(UNS8), (void *)&ObjDict_obj1800_Compatibility_Entry},
	{RW, uint16, sizeof(UNS16), (void *)&ObjDict_obj1800_Event_Timer}
};
ODCallback_t ObjDict_Index1800_callbacks[] = 
{
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
	NULL,
};
////////////////////////////////////////////发送PDO映射参数配置(客户端)///////////////////////////////////////////////////////////////
/* 子索引数 */
UNS8 ObjDict_highestSubIndex_obj1A00 = 2;
/* bit0-7表示位数，bit8-15表示子索引，bit16-32表示索引 */
UNS32 ObjDict_obj1A00[] = 
{
	0x20000108,	/* 索引为2000，子索引为1，位数为8位 */
	0x20000210,	/* 索引为2000，子索引为2，位数为16位 */
};
subindex ObjDict_Index1A00[] = 
{
	{RW, uint8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj1A00},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1A00[0]},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1A00[1]},
};
//////////////////////////////////////////被接收PDO映射的索引配置(客户端)///////////////////////////////////////////////////////////////
/* 子索引数 */
UNS8 ObjDict_highestSubIndex_obj2000 = 2;
/* 用于存储将要发送的数据 */
UNS8 ControlWordAxis1 = 100;
/* 用于存储将要发送的数据 */
UNS16 ControlWordAxis2 = 1000;
subindex ObjDict_Index2000[] =                                                    
{
	{RW, int8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj2000}, 
	{RW, uint8, sizeof(UNS8), (void *)&ControlWordAxis1},
	{RW, uint16, sizeof(UNS16), (void *)&ControlWordAxis2},
};
///////////////////////////////////////////////字典配置/////////////////////////////////////////////////////////////////////////////
/* 字典的声明 */
const indextable ObjDict_objdict[] = 
{
	{NULL, 0, 0x0},
	{(subindex *)ObjDict_Index1200, sizeof(ObjDict_Index1200) / sizeof(ObjDict_Index1200[0]), 0x1200},
	{(subindex *)ObjDict_Index1800, sizeof(ObjDict_Index1800) / sizeof(ObjDict_Index1800[0]), 0x1800},
	{(subindex *)ObjDict_Index1A00, sizeof(ObjDict_Index1A00) / sizeof(ObjDict_Index1A00[0]), 0x1600},
	{(subindex *)ObjDict_Index2000, sizeof(ObjDict_Index2000) / sizeof(ObjDict_Index2000[0]), 0x2000},
};

/* 通过索引查在字典中的下标 */
const indextable *ObjDict_scanIndexOD(UNS16 wIndex, UNS32 *errorCode, ODCallback_t **callbacks)
{
	int i;
	*callbacks = NULL;
	
	switch(wIndex)
	{
		case 0x1200: i = 1; break;
		case 0x1800: i = 2;	*callbacks = ObjDict_Index1800_callbacks; break;
		case 0x1A00: i = 3;	break;
		case 0x2000: i = 4; break;
		default:
			*errorCode = OD_NO_SUCH_OBJECT;
			return NULL;
	}
	*errorCode = OD_SUCCESSFUL;
	return &ObjDict_objdict[i];
}

/* 在字典中的下标 */
const quick_index ObjDict_firstIndex = {
	1,	/* 第一个SDO服务器，每个从站配置一个服务器 */
	0,	/* 第一个SDO客户端，从站不需要配置客户端 */
	0,	/* 第一个PDO接收端通讯参数，从站不需要配置接收端 */
	0,	/* 第一个PDO接收端映射参数，从站不需要配置接收端 */
	2,	/* 第一个PDO发送端通讯参数，这里配置1个发送端(最多4个) */
	3		/* 第一个PDO发送端映射参数，这里配置1个发送端(最多4个) */
};

/* 在字典中的下标 */
const quick_index ObjDict_lastIndex = {
	1,	/* 最后一个SDO服务器，每个从站配置一个服务器 */
	0,	/* 最后一个SDO客户端，从站不需要配置客户端 */
	0,	/* 最后一个PDO接收端通讯参数，从站不需要配置接收端 */
	0,	/* 最后一个PDO接收端映射参数，从站不需要配置接收端 */
	2,	/* 最后一个PDO发送端通讯参数，这里配置1个发送端(最多4个) */
	3		/* 最后一个PDO发送端映射参数，这里配置1个发送端(最多4个) */
};

/* 字典的大小 */
const UNS16 ObjDict_ObjdictSize = sizeof(ObjDict_objdict)/sizeof(ObjDict_objdict[0]);

/* 对各数据类型的范围进行检查合法性检查 */
UNS32 ObjDict_valueRangeTest(UNS8 typeValue, void *value)
{
  switch(typeValue) 
	{
//    case int8:
//      if(*(INTEGER8 *)value != (INTEGER8)0)	//
//				return OD_VALUE_RANGE_EXCEEDED;
//      break;
			
		default:
			break;
  }	
	return 0;
}
/////////////////////////////////////////////节点属性配置////////////////////////////////////////////////////////////////////////////
/* ObjDict_iam_a_slave = 1表明该节点为从站 */
const UNS8 ObjDict_iam_a_slave = 1;
/* 设置该从站节点号为1 */
UNS8 ObjDict_bDeviceNodeId = 0x01;
/////////////////////////////////////////////心跳/节点保护报文配置////////////////////////////////////////////////////////////////////////////
/* 心跳报文从站节点数，主站才需要配置，但是这里不使用心跳报文 */
UNS8 ObjDict_highestSubIndex_obj1016 = 0;
/* 心跳报文入口，其中存放了从站nod-id和倒计时时间，主站才需要配置，但是这里不使用心跳报文 */
UNS32 ObjDict_obj1016[1];
/* 心跳报文定时事件状态，主站才需要配置，  */
TIMER_HANDLE ObjDict_heartBeatTimers[1];
/* 从站上报心跳包间隔时间，从站才需要配置 */
UNS16 ObjDict_obj1017 = 2000;
/* 节点保护报文，每60秒询问一次所有节点在线情况 */
UNS16 ObjDict_obj100C = 60;
/* 节点保护报文，3次不回复则认为节点掉线 */	
UNS8 ObjDict_obj100D = 3;

/////////////////////////////////////////////////同步报文配置/////////////////////////////////////////////////////////////////////////////////
/* bit31表示是否开启同步报文，低16位表示cob-id，主站才需要配置 */
UNS32 ObjDict_obj1005 = 0;
/* 同步报文发送事件间隔时间，主站才需要配置 */
UNS32 ObjDict_obj1006 = 0;

/////////////////////////////////////////////////紧急报文配置/////////////////////////////////////////////////////////////////////////////////
/* 紧急报文，用于存放错误信息 */
UNS32 ObjDict_obj1003[EMCY_MAX_ERRORS];
/* 错误个数 */
UNS8 ObjDict_highestSubIndex_obj1003 = 0;
/* 用于存放错误标志位，主站不需要配置 */
UNS8 ObjDict_obj1001 = 0x0;
/* 紧急报文cob-id，主站不需要配置，从站在setNodeId函数中也会初始化，所以也不用配置 */
UNS32 ObjDict_obj1014 = 0x80;

//////////////////////////////////////////////////PDO报文配置/////////////////////////////////////////////////////////////////////////////////
/* 初始化PDO状态 */
s_PDO_status ObjDict_PDO_status[1] = {s_PDO_status_Initializer};

///////////////////////////////////////////////////字典定义///////////////////////////////////////////////////////////////////////////////////
/* CANOPEN字典 */
CO_Data ObjDict_Data = CANOPEN_NODE_DATA_INITIALIZER(ObjDict);
