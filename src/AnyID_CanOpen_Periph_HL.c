#include "AnyID_CanOpen_Periph_HL.h"

const PORT_INF PERIPH_RUN_LED_1  = {GPIOA, GPIO_Pin_1};
const PORT_INF PERIPH_RUN_LED_2  = {GPIOA, GPIO_Pin_1};
const PORT_INF PERIPH_RUN_LED_3  = {GPIOA, GPIO_Pin_2};

const PORT_INF PERIPH_RUN_KEY_1  = {GPIOC, GPIO_Pin_8};
const PORT_INF PERIPH_RUN_KEY_2  = {GPIOC, GPIO_Pin_6};
const PORT_INF PERIPH_RUN_KEY_3  = {GPIOB, GPIO_Pin_14};

const PORT_INF PERIPH_RUN_ADDR_1  = {GPIOA, GPIO_Pin_3};
const PORT_INF PERIPH_RUN_ADDR_2  = {GPIOA, GPIO_Pin_4};
const PORT_INF PERIPH_RUN_ADDR_3  = {GPIOA, GPIO_Pin_5};

void Periph_InitInterface()
{
	GPIO_InitTypeDef GPIO_InitStructure = {0};


	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = PERIPH_RUN_LED_1.Pin | PERIPH_RUN_LED_2.Pin | PERIPH_RUN_LED_3.Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(PERIPH_RUN_LED_1.Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_InitStructure.GPIO_Pin = PERIPH_RUN_KEY_1.Pin | PERIPH_RUN_KEY_2.Pin;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(PERIPH_RUN_KEY_1.Port, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pin = PERIPH_RUN_KEY_3.Pin | PERIPH_RUN_KEY_3.Pin;
	GPIO_Init(PERIPH_RUN_KEY_3.Port, &GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin = PERIPH_RUN_ADDR_1.Pin | PERIPH_RUN_ADDR_1.Pin | PERIPH_RUN_ADDR_1.Pin;
	
	GPIO_Init(PERIPH_RUN_ADDR_1.Port, &GPIO_InitStructure);
	
}

u8 Periph_GetAddr()
{
    u8 count = 0, addr = 0, t = 0;
    while(count < 20)
    {
        if((PERIPH_RUN_ADDR_1.Port->IDR & PERIPH_RUN_ADDR_1.Pin) == 0x0000)
        {
            t |= 0x04;
        }
        if((PERIPH_RUN_ADDR_2.Port->IDR & PERIPH_RUN_ADDR_2.Pin) == 0x0000)
        {
            t |= 0x02;
        }
        if((PERIPH_RUN_ADDR_3.Port->IDR & PERIPH_RUN_KEY_3.Pin) == 0x0000)
        {
            t |= 0x01;
        }
        if(t == addr)
        {
            count++;
        }
        else
        {
            count = 0;
            addr = t;
        }
    }
	return addr;
}

u8 Periph_GetKeyValue()
{
	u8 count = 0, value = 0, t = 0;
    while(count < 20)
    {
        if((PERIPH_RUN_KEY_1.Port->IDR & PERIPH_RUN_KEY_1.Pin) == 0x0000)
        {
            t |= 0x04;
        }
        if((PERIPH_RUN_KEY_2.Port->IDR & PERIPH_RUN_KEY_2.Pin) == 0x0000)
        {
            t |= 0x02;
        }
        if((PERIPH_RUN_KEY_3.Port->IDR & PERIPH_RUN_KEY_3.Pin) == 0x0000)
        {
            t |= 0x01;
        }
        if(t == value)
        {
            count++;
        }
        else
        {
            count = 0;
            value = t;
        }
    }
	return value;
}
