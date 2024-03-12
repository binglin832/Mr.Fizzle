#ifndef __BC25_H
#define __BC25_H	
#include "usart.h"
#include <stm32f10x.h>
#include "delay.h"
void Clear_Buffer(void);//��ջ���	
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
void  MQTT_Init(void);
void aliyunMQTT_PUBdata(u8 temp,u8 humi);
u8 Mqttaliyun_Savedata(u8 *t_payload,u8 temp,u8 humi);
void BC25_RECData(void);
void CParsejson(char *data);
typedef struct
{
    uint8_t CSQ;    
    uint8_t Socketnum;   //���
    uint8_t reclen;   //��ȡ�����ݵĳ���
    uint8_t res;      
    uint8_t recdatalen[10];
    uint8_t recdata[100];
    uint8_t netstatus;//����ָʾ��
} BC25;

#endif


