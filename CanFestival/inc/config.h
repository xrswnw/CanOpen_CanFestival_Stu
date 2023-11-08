#ifndef _CONFIG_H_
#define _CONFIG_H_

/* Needed defines by Canfestival lib */
#define MAX_CAN_BUS_ID 1
/* New define, if SDO_MAX_LENGTH_TRANSFERT is exceeded allocate data buffer dynamically */
#define SDO_DYNAMIC_BUFFER_ALLOCATION
#define SDO_DYNAMIC_BUFFER_ALLOCATION_SIZE 	(1024)
#define SDO_MAX_LENGTH_TRANSFER 						32
#define SDO_MAX_SIMULTANEOUS_TRANSFERS 		5
#define NMT_MAX_NODE_ID 	2
#define SDO_TIMEOUT_MS 		3000U
#define MAX_NB_TIMER 			8

#define SDO_BLOCK_SIZE  16    //�������

// CANOPEN_BIG_ENDIAN is not defined
#define CANOPEN_LITTLE_ENDIAN 1

#define US_TO_TIMEVAL_FACTOR 8


#define REPEAT_SDO_MAX_SIMULTANEOUS_TRANSFERS_TIMES(repeat)\
repeat
#define REPEAT_NMT_MAX_NODE_ID_TIMES(repeat)\
repeat repeat

#define EMCY_MAX_ERRORS 8
#define REPEAT_EMCY_MAX_ERRORS_TIMES(repeat)\
repeat repeat repeat repeat repeat repeat repeat repeat

#endif
