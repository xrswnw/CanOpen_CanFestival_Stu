
/* File generated by gen_cfile.py. Should not be modified. */

#ifndef ANYID_CANOPEN_CLIENT_H
#define ANYID_CANOPEN_CLIENT_H

#include "data.h"

/* Prototypes of function provided by object dictionnary */
UNS32 AnyId_Canopen_Client_valueRangeTest (UNS8 typeValue, void * value);
const indextable * AnyId_Canopen_Client_scanIndexOD (UNS16 wIndex, UNS32 * errorCode, ODCallback_t **callbacks);

/* Master node data struct */
extern CO_Data AnyId_Canopen_Client_Data;
extern UNS64 Uid;		/* Mapped at index 0x2000, subindex 0x00*/
extern UNS8 KeyValue;		/* Mapped at index 0x2001, subindex 0x00*/
extern UNS8 AddValue;		/* Mapped at index 0x2002, subindex 0x00*/

#endif // ANYID_CANOPEN_CLIENT_H