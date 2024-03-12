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
    printf("ATE0&W\r\n"); //�رջ���
    delay_ms(300); 
    IWDG_Feed();//ι��
    printf("AT+QMTDISC=0\r\n");//�ر�����
    delay_ms(300);
    printf("AT+QMTCLOSE=0\r\n");
    delay_ms(300); 
}
/***
���ڵ��ſ����ԣ����ڵ��Ŷ�IP�����ƣ�TCP����Ҳ��������ƣ�����TCP������ǰ�IPҲ�ǻᷢ��ʧ�ܣ������ƶ����Բ�Ӱ��ʹ�á������ƶ��ͻ�����ʵ��
***/

//������NB�汾
 int main(void)
 {	
    u8 temp,humi;
	 	uint8_t sendata[]="02000800020304";//���������ݣ������ʽ��ƽ̨��ע��Ĳ�������
    delay_init();	    	 //��ʱ������ʼ��	  
    NVIC_Configuration(); 	 //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
		LED_Init();
    uart_init(9600);//����1��ʼ����������PC���д�ӡģ�鷵������
    uart2_init(9600);//��ʼ����BC25�ԽӵĶ˿�
    IWDG_Init(7,625);    //8Sһ��
   //  while(DHT11_Init())
     {}
    OPEN_BC25();//��BC25����
    BC25_Init();//���豸��ʼ��
    BC25_RegIOT();
   while(1)
    {  
     //   DHT11_Read_Data(&temp,&humi);//��ȡ��ʪ������
			  temp=23,humi=46;
	     	BC25_IOTSenddata(strlen(sendata)/2,sendata);
			  LED=!LED;
        delay_ms(500);
			  BC25_RECCTwingData();
        IWDG_Feed();//ι��
    }	 
 }






