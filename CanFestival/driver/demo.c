#include "demo.h"

//////////////////////////////////////////////////SDO����(������)///////////////////////////////////////////////////////////////////
/* ���������� */
UNS8 ObjDict_highestSubIndex_obj1200 = 3;
/* �ͻ���->�������õ�COB-ID */
UNS32 ObjDict_obj1200_COB_ID_Client_to_Server_Transmit_SDO = 0x600 + 1;	/* 1�Žڵ� */
/* ������->�ͻ����õ�COB-ID */
UNS32 ObjDict_obj1200_COB_ID_Server_to_Client_Receive_SDO = 0x580 + 1;	/* 1�Žڵ� */
/* ��Ӧ�������Ľڵ�� */
UNS8 ObjDict_obj1200_Node_ID_of_the_SDO_Server = 0x1;	/* 1�Žڵ� */
subindex ObjDict_Index1200[] = 
{
	{RO, uint8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj1200},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1200_COB_ID_Client_to_Server_Transmit_SDO},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1200_COB_ID_Server_to_Client_Receive_SDO},
	{RW, uint8, sizeof(UNS8), (void *)&ObjDict_obj1200_Node_ID_of_the_SDO_Server}
};
////////////////////////////////////////////����PDOͨѶ��������(�ͻ���)///////////////////////////////////////////////////////////////
/* �������� */
UNS8 ObjDict_highestSubIndex_obj1800 = 5;
/* ��վ�ķ���PDO��Ӧ����վ�Ľ���PDO���������ߵ�COB-ID��Ҫ����һ�� */
UNS32 ObjDict_obj1800_COB_ID_used_by_PDO = 0x181;	/* PDO1 1�Žڵ� */
/* PDO���ͣ�0��ʾͬ�������ڣ�1-240��ʾͬ�����ڣ�252��ʾͬ��RTR��253��ʾ�첽RTR��254��ʾ�첽�������ض��¼���255��ʾ�첽�豸��Э���ض��¼�����������Ϊͬ��һ�� */
UNS8 ObjDict_obj1800_Transmission_Type = 0x01;
/* ��Ϊ������Ϊ256kB/S����׼֡�108λ */
UNS16 ObjDict_obj1800_Inhibit_Time = 1000; /* 0 0.1ms */
/* ������ʹ�� */
UNS8 ObjDict_obj1800_Compatibility_Entry = 0x0;
/* ��վ��Ϊ�¼���ʱ���ڣ���վ��Ϊ�¼���ʱʱ�䣬����ʵ����ͬ���������Բ��������� */
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
////////////////////////////////////////////����PDOӳ���������(�ͻ���)///////////////////////////////////////////////////////////////
/* �������� */
UNS8 ObjDict_highestSubIndex_obj1A00 = 2;
/* bit0-7��ʾλ����bit8-15��ʾ��������bit16-32��ʾ���� */
UNS32 ObjDict_obj1A00[] = 
{
	0x20000108,	/* ����Ϊ2000��������Ϊ1��λ��Ϊ8λ */
	0x20000210,	/* ����Ϊ2000��������Ϊ2��λ��Ϊ16λ */
};
subindex ObjDict_Index1A00[] = 
{
	{RW, uint8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj1A00},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1A00[0]},
	{RW, uint32, sizeof(UNS32), (void *)&ObjDict_obj1A00[1]},
};
//////////////////////////////////////////������PDOӳ�����������(�ͻ���)///////////////////////////////////////////////////////////////
/* �������� */
UNS8 ObjDict_highestSubIndex_obj2000 = 2;
/* ���ڴ洢��Ҫ���͵����� */
UNS8 ControlWordAxis1 = 100;
/* ���ڴ洢��Ҫ���͵����� */
UNS16 ControlWordAxis2 = 1000;
subindex ObjDict_Index2000[] =                                                    
{
	{RW, int8, sizeof(UNS8), (void *)&ObjDict_highestSubIndex_obj2000}, 
	{RW, uint8, sizeof(UNS8), (void *)&ControlWordAxis1},
	{RW, uint16, sizeof(UNS16), (void *)&ControlWordAxis2},
};
///////////////////////////////////////////////�ֵ�����/////////////////////////////////////////////////////////////////////////////
/* �ֵ������ */
const indextable ObjDict_objdict[] = 
{
	{NULL, 0, 0x0},
	{(subindex *)ObjDict_Index1200, sizeof(ObjDict_Index1200) / sizeof(ObjDict_Index1200[0]), 0x1200},
	{(subindex *)ObjDict_Index1800, sizeof(ObjDict_Index1800) / sizeof(ObjDict_Index1800[0]), 0x1800},
	{(subindex *)ObjDict_Index1A00, sizeof(ObjDict_Index1A00) / sizeof(ObjDict_Index1A00[0]), 0x1600},
	{(subindex *)ObjDict_Index2000, sizeof(ObjDict_Index2000) / sizeof(ObjDict_Index2000[0]), 0x2000},
};

/* ͨ�����������ֵ��е��±� */
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

/* ���ֵ��е��±� */
const quick_index ObjDict_firstIndex = {
	1,	/* ��һ��SDO��������ÿ����վ����һ�������� */
	0,	/* ��һ��SDO�ͻ��ˣ���վ����Ҫ���ÿͻ��� */
	0,	/* ��һ��PDO���ն�ͨѶ��������վ����Ҫ���ý��ն� */
	0,	/* ��һ��PDO���ն�ӳ���������վ����Ҫ���ý��ն� */
	2,	/* ��һ��PDO���Ͷ�ͨѶ��������������1�����Ͷ�(���4��) */
	3		/* ��һ��PDO���Ͷ�ӳ���������������1�����Ͷ�(���4��) */
};

/* ���ֵ��е��±� */
const quick_index ObjDict_lastIndex = {
	1,	/* ���һ��SDO��������ÿ����վ����һ�������� */
	0,	/* ���һ��SDO�ͻ��ˣ���վ����Ҫ���ÿͻ��� */
	0,	/* ���һ��PDO���ն�ͨѶ��������վ����Ҫ���ý��ն� */
	0,	/* ���һ��PDO���ն�ӳ���������վ����Ҫ���ý��ն� */
	2,	/* ���һ��PDO���Ͷ�ͨѶ��������������1�����Ͷ�(���4��) */
	3		/* ���һ��PDO���Ͷ�ӳ���������������1�����Ͷ�(���4��) */
};

/* �ֵ�Ĵ�С */
const UNS16 ObjDict_ObjdictSize = sizeof(ObjDict_objdict)/sizeof(ObjDict_objdict[0]);

/* �Ը��������͵ķ�Χ���м��Ϸ��Լ�� */
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
/////////////////////////////////////////////�ڵ���������////////////////////////////////////////////////////////////////////////////
/* ObjDict_iam_a_slave = 1�����ýڵ�Ϊ��վ */
const UNS8 ObjDict_iam_a_slave = 1;
/* ���øô�վ�ڵ��Ϊ1 */
UNS8 ObjDict_bDeviceNodeId = 0x01;
/////////////////////////////////////////////����/�ڵ㱣����������////////////////////////////////////////////////////////////////////////////
/* �������Ĵ�վ�ڵ�������վ����Ҫ���ã��������ﲻʹ���������� */
UNS8 ObjDict_highestSubIndex_obj1016 = 0;
/* ����������ڣ����д���˴�վnod-id�͵���ʱʱ�䣬��վ����Ҫ���ã��������ﲻʹ���������� */
UNS32 ObjDict_obj1016[1];
/* �������Ķ�ʱ�¼�״̬����վ����Ҫ���ã�  */
TIMER_HANDLE ObjDict_heartBeatTimers[1];
/* ��վ�ϱ����������ʱ�䣬��վ����Ҫ���� */
UNS16 ObjDict_obj1017 = 2000;
/* �ڵ㱣�����ģ�ÿ60��ѯ��һ�����нڵ�������� */
UNS16 ObjDict_obj100C = 60;
/* �ڵ㱣�����ģ�3�β��ظ�����Ϊ�ڵ���� */	
UNS8 ObjDict_obj100D = 3;

/////////////////////////////////////////////////ͬ����������/////////////////////////////////////////////////////////////////////////////////
/* bit31��ʾ�Ƿ���ͬ�����ģ���16λ��ʾcob-id����վ����Ҫ���� */
UNS32 ObjDict_obj1005 = 0;
/* ͬ�����ķ����¼����ʱ�䣬��վ����Ҫ���� */
UNS32 ObjDict_obj1006 = 0;

/////////////////////////////////////////////////������������/////////////////////////////////////////////////////////////////////////////////
/* �������ģ����ڴ�Ŵ�����Ϣ */
UNS32 ObjDict_obj1003[EMCY_MAX_ERRORS];
/* ������� */
UNS8 ObjDict_highestSubIndex_obj1003 = 0;
/* ���ڴ�Ŵ����־λ����վ����Ҫ���� */
UNS8 ObjDict_obj1001 = 0x0;
/* ��������cob-id����վ����Ҫ���ã���վ��setNodeId������Ҳ���ʼ��������Ҳ�������� */
UNS32 ObjDict_obj1014 = 0x80;

//////////////////////////////////////////////////PDO��������/////////////////////////////////////////////////////////////////////////////////
/* ��ʼ��PDO״̬ */
s_PDO_status ObjDict_PDO_status[1] = {s_PDO_status_Initializer};

///////////////////////////////////////////////////�ֵ䶨��///////////////////////////////////////////////////////////////////////////////////
/* CANOPEN�ֵ� */
CO_Data ObjDict_Data = CANOPEN_NODE_DATA_INITIALIZER(ObjDict);
