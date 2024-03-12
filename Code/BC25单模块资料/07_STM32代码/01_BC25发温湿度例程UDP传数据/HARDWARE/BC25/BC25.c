#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern char  RxBuffer[100],RxCounter;
BC25 BC25_Status;
unsigned char socketnum=0;//��ǰ��socket����
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr(RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//����
		RxCounter=0;
		IWDG_Feed();//ι��
	
}
void BC25_Init(void)
{
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//����OK
    }
		printf("AT+QICLSOE=0\r\n");//
    delay_ms(300);
    printf("AT+CFUN=1\r\n");//������Ƶ
    delay_ms(300);
    printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//��460������ʶ�𵽿���
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//����OK,˵�����Ǵ��ڵ�
    }
        printf("AT+CGATT=1\r\n");//�������磬PDP
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//��OK
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//��ѯ����״̬
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//��1
        Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            printf("AT+CGATT?\r\n");//��ȡ����״̬
            delay_ms(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//����1,����ע���ɹ�
		}
		printf("AT+CSQ\r\n");//�鿴��ȡCSQֵ
        delay_ms(300);
     Clear_Buffer();
}




void BC25_CreateUDPSokcet(void)//����sokcet
{

    printf("AT+QIOPEN=1,0,\"UDP\",\"47.92.146.210\",9999,0,0\r\n");//��������UDP,����IP�Լ��������˿ں��� ,����ֱ���³��ķ�ʽ
    delay_ms(300);//UDP������ģʽ������ȥ����Ƿ��¼�������ɹ�
    Clear_Buffer();	
    
}
void BC25_Senddata(uint8_t *len,uint8_t *data)//�ַ�����ʽ
{
    printf("AT+QISEND=0,%s,\"%s\"\r\n",len,data);
	  delay_ms(300);
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//����Ƿ��ͳɹ�
            delay_ms(100);
		}  
	  Uart1_SendStr(RxBuffer);
		RxCounter=0;
}

void BC25_RECData()
{
    char i;
    strx=strstr((const char*)RxBuffer,(const char*)"++QIURC:");//����+QIURC:���������յ�UDP���������ص�����
    if(strx)
    {
       

      
            Clear_Buffer();
                 
    }
}

