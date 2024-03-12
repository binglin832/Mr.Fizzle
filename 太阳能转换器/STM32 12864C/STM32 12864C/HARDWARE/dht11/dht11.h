#ifndef __dht11_H
#define __dht11_H 			   
#include "sys.h"  

//IO方向设置
#define DHT11_IO_IN()   {GPIOB->CRH&=0xFF0FFFFF;GPIOB->CRH|=0x00800000;}   //修改IO口    前面两处都要修改  要对应上
#define DHT11_IO_OUT()  {GPIOB->CRH&=0XFF0FFFFF;GPIOB->CRH|=0x00300000;}   //

////IO操作函数											   
#define	DHT11_DQ_OUT PBout(13) //数据端口	
#define	DHT11_DQ_IN  PBin(13)  //数据端口	


u8 DHT11_Read_Data(u8 *temp,u8 *humi);//读取温湿度
u8 DHT11_Read_Byte(void);//读出一个字节
u8 DHT11_Read_Bit(void);//读出一个位
u8 DHT11_Check(void);//检测是否存在DHT11
void DHT11_Rst(void);//复位DHT11    
u8 DHT11_Init(void);


#endif
