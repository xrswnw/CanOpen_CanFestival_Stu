#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Needed defines by Canfestival lib */
#define MAX_CAN_BUS_ID 1
/* New define, if SDO_MAX_LENGTH_TRANSFERT is exceeded allocate data buffer dynamically */
#define SDO_DYNAMIC_BUFFER_ALLOCATION							//动态内存开关
#define SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE 	(2048)
#define SDO_MAX_LENGTH_TRANSFER				32
#define SDO_MAX_SIMULTANEOUS_TRANSFERS 		5					//最大同时传输的SDO数目
#define NMT_MAX_NODE_ID						128					//支持保存的节点状态数据，最大128
#define SDO_TIMEOUT_MS						8000U
#define MAX_NB_TIMER						8

#define SDO_BLOCK_SIZE						16    //测试添加

// CANOPEN_BIG_ENDIAN is not defined
#define CANOPEN_LITTLE_ENDIAN				1

#define US_TO_TIMEVAL_FACTOR				8

#define CANOPEN_INDEX_TPDO1				0x1800
#define CANOPEN_INDEX_TPDO2				0x1801
#define CANOPEN_INDEX_TPDO3				0x1802
#define CANOPEN_INDEX_TPDO4				0x1803

#define CANOPEN_INDEX_RPDO1				0x1400
#define CANOPEN_INDEX_RPDO2				0x1401
#define CANOPEN_INDEX_RPDO3				0x1402
#define CANOPEN_INDEX_RPDO4				0x1403


#define REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERS_TIMES(repeat)\
repeat
#define REPEAT_NMT_MAX_NODE_ID_TIMES(repeat)\
repeat repeat

#define EMCY_MAX_ERRORS 8
#define REPEAT_EMCY_MAX_ERRORS_TIMES(repeat)\
repeat repeat repeat repeat repeat repeat repeat repeat

#endif
