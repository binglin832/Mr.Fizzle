#ifndef __BC25_H
#define __BC25_H	
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"
void Clear_Buffer(void);//清空缓存	
void BC25_Init(void);
void BC25_PDPACT(void);
void BC25_ConTCP(void);
void BC25_CreateTCPSokcet(void);
void BC25_RECData(void);
void BC25_Senddata(uint8_t *len,uint8_t *data);
void BC25_Senddatahex(uint8_t *len,uint8_t *data);
void BC25_CreateSokcet(void);
void Clear_Buffer(void);
void BC25_ChecekConStatus(void);
void  BC25_RegIOT(void);
void BC25_IOTSenddata(u8 len,u8 *data);
void BC25_RECCTwingData(void);
typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //编号
    uint8_t reclen;   //获取到数据的长度
    uint8_t res;      
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus;//网络指示灯
} BC25;

#endif




