#ifndef __dht11_H
#define __dht11_H 			   
#include "sys.h"  

//IO��������
#define DHT11_IO_IN()   {GPIOB->CRH&=0xFF0FFFFF;GPIOB->CRH|=0x00800000;}   //�޸�IO��    ǰ��������Ҫ�޸�  Ҫ��Ӧ��
#define DHT11_IO_OUT()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=0x00300000;}   //

////IO��������											   
#define	DHT11_DQ_OUT PBout(13) //���ݶ˿�	
#define	DHT11_DQ_IN  PBin(13)  //���ݶ˿�	


u8 DHT11_Read_Data(u8 *temp,u8 *humi);//��ȡ��ʪ��
u8 DHT11_Read_Byte(void);//����һ���ֽ�
u8 DHT11_Read_Bit(void);//����һ��λ
u8 DHT11_Check(void);//����Ƿ����DHT11
void DHT11_Rst(void);//��λDHT11    
u8 DHT11_Init(void);


#endif
