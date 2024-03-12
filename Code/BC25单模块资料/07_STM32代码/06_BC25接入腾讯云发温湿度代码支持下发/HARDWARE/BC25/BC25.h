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
void  MQTT_Init(void);
void aliyunMQTT_PUBdata(u8 temp,u8 humi);
u8 MqttOnenet_Savedata(u8 *t_payload,u8 temp,u8 humi);
uint8_t MqttOnenet_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi);
void Pub_Msgdata(uint8_t temp,uint8_t humi);
void  Sub_Msgdata(void);
void TengxunMQTT_Init(void);
uint8_t Tengxunyun_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi);
void Pub_Msgdata(uint8_t temp,uint8_t humi);
void  Sub_Msgdata(void);

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

