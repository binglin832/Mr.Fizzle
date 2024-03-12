#include "led.h"
#include "delay.h"
#include "sys.h"
#include "usart.h"	 
#include "math.h"			
#include "stdio.h"
#include "stm32f10x_flash.h"
#include "stdlib.h"
#include "string.h"
#include "wdg.h"
#include "BC25.h"	 
#include "dht11.h"
extern char  RxBuffer[100],RxCounter;
extern unsigned char uart1_getok;
extern char RxCounter1,RxBuffer1[100];
extern unsigned char Timeout,restflag;




void OPEN_BC25(void)
{
   char *strx;
 
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
	IWDG_Feed();//喂狗
   if(strx==NULL)
	{
        PWRKEY=1;//拉低
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);	
        PWRKEY=0;//拉高正常开机
        IWDG_Feed();//喂狗
	}
    printf("AT\r\n"); 
    delay_ms(300);
    IWDG_Feed();//喂狗
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300); 
    IWDG_Feed();//喂狗
    printf("AT+QMTDISC=0\r\n");//关闭连接
    delay_ms(300);
    printf("AT+QMTCLOSE=0\r\n");
    delay_ms(300); 
}
/***
对于电信卡而言，由于电信对IP的限制，TCP发送也会存在限制，所以TCP如果不是绑定IP也是会发送失败，对于移动而言不影响使用。建议移动客户进行实验
***/

//适用于NB版本
 int main(void)
 {	
    u8 temp,humi;
	 	uint8_t sendata[]="02000800020304";//二进制数据，编码格式由平台端注册的参数决定
    delay_init();	    	 //延时函数初始化	  
    NVIC_Configuration(); 	 //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
		LED_Init();
    uart_init(9600);//串口1初始化，可连接PC进行打印模块返回数据
    uart2_init(9600);//初始化和BC25对接的端口
    IWDG_Init(7,625);    //8S一次
   //  while(DHT11_Init())
     {}
    OPEN_BC25();//对BC25开机
    BC25_Init();//对设备初始化
    BC25_RegIOT();
   while(1)
    {  
     //   DHT11_Read_Data(&temp,&humi);//读取温湿度数据
			  temp=23,humi=46;
	     	BC25_IOTSenddata(strlen(sendata)/2,sendata);
			  LED=!LED;
        delay_ms(500);
			  BC25_RECCTwingData();
        IWDG_Feed();//喂狗
    }	 
 }






