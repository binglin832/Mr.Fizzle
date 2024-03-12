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
#include "timer.h"
#include "stm32f10x_tim.h"
#include "BC25.h"	 
#include "dht11.h"
extern char  RxBuffer[100],RxCounter;

void OPEN_BC25(void)
{
   char *strx;
 
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
	IWDG_Feed();//ι��
   if(strx==NULL)
	{
        PWRKEY=1;//����
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);
        delay_ms(300);	
        PWRKEY=0;//������������
        IWDG_Feed();//ι��
	}
    printf("AT\r\n"); 
    delay_ms(300);
    IWDG_Feed();//ι��
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300); 
    LED=0;
    IWDG_Feed();//ι��
    printf("AT+QMTDISC=0\r\n");//�ر�����
    delay_ms(300);
    printf("AT+QMTCLOSE=0\r\n");
    delay_ms(300); 
}
/***
�˹���ʵ���������Ʒ����ݣ��û���Ҫ������Ԫ������������һ�¡����ֺ��Լ����豸һ��
***/

//������NB�汾
 int main(void)
 {	
    u8 temp,humi;
    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
    LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
    uart_init(9600);//����1��ʼ����������PC���д�ӡģ�鷵������
    uart2_init(9600);//�Խ�BC25���ڳ�ʼ��
  ///  IWDG_Init(7,625);    //8Sһ��
   //  while(DHT11_Init())
     {}
    OPEN_BC25();//��BC25����  
    BC25_Init();//���豸��ʼ��
    MQTT_Init();
   while(1)
    {  
       // DHT11_Read_Data(&temp,&humi);//��ȡ��ʪ������
        temp=32;  humi=67;//�ǵ������д�����
	    	aliyunMQTT_PUBdata(temp,humi);
        delay_ms(500);
			  BC25_RECData();
		 //  	CParsejson();
				LED=!LED;
        IWDG_Feed();//ι��
    }	 
 }






