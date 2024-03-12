#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
//��Ҫ�û��ĵ� 
#define Pubtopic  "$sys/hMx07yMJon/m001/dp/post/json"  //��������
#define Subtopic  "$sys/hMx07yMJon/m001/cmd/request/+"  //��������
//��Ҫ�û��ĵ� 
#define ProductID  "hMx07yMJon"  //��Ʒ����
#define DeviceID  "m001"  //�豸����
#define Token    "version=2018-10-31&res=products%2FhMx07yMJon&et=1893430861&method=md5&sign=RBalJsFO9yg8BW05Bwz2EA%3D%3D"  //tokenֵ
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC25 BC25_Status;
void Clear_Buffer(void)//��ջ���
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
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
    printf("AT+CFUN=1\r\n");//��ȡ���ţ������Ƿ���ڿ�����˼���Ƚ���Ҫ��
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
		printf("AT+CESQ\r\n");//�鿴��ȡCSQֵ
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//����CSQ
		if(strx)
			{
				BC25_Status.CSQ=(strx[7]-0x30)*10+(strx[8]-0x30);//��ȡCSQ
				if((BC25_Status.CSQ==99)||((strx[7]-0x30)==0))//˵��ɨ��ʧ��
				{
					while(1)
					{
            BC25_Status.netstatus=0;
						Uart1_SendStr("�ź�����ʧ�ܣ���鿴ԭ��!\r\n");
						delay_ms(300);//û���źž͸�λ
                        
					}
				}
             else
             {
                 BC25_Status.netstatus=1;
              }
                
            }
              Clear_Buffer();	
}






void  MQTT_Init(void)
{
  
     printf("AT+QMTCFG=\"version\",0,1\r\n");//�豸�汾
    delay_ms(500);
    printf("AT+QMTOPEN=0,\"183.230.40.96\",1883\r\n");//ͨ��TCP��ʽȥ����MQTT������ 
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//���·���״̬
    while(strx==NULL)
    {
      strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//ȷ�Ϸ���ֵ��ȷ
    }
    Clear_Buffer(); 
    printf("AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",DeviceID,ProductID,Token);//ȥ��¼MQTT���������豸name����ƷID,TOKENϢ���û�����ʵ����Ҫ���и���
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
		 printf("AT+QMTSUB=0,1,\"%s\",0\r\n",Subtopic);//���ĸ�����
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//���ĳɹ�
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//���ĳɹ�
    }
    Clear_Buffer(); 
}


//����ONENET��Ҫ�ύJSON����
uint8_t MqttOnenet_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi)
{
    char json[]="{\"id\":123,\"dp\":{\"temperatrue\":[{\"v\":%d}],\"humi\":[{\"v\":%d}]}}";
    char t_json[200];
    unsigned short json_len;
  	sprintf(t_json, json, temp,humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}



void Pub_Msgdata(uint8_t temp,uint8_t humi)//�������ݵ�ONENET��ʾ
{
	  char *strx;
	  u8 datalen,datastr[10],send_jason[200];
		datalen=MqttOnenet_Savedata(send_jason,temp,humi);
   	send_jason[datalen]=0;
	 // Uart1_SendStr(send_jason);
		//sprintf(datastr,"%d",data_len);
		printf("AT+QMTPUB=0,0,0,0,\"%s\"\r\n",Pubtopic);//������Ϣ
		delay_ms(100);
		printf("%s",send_jason);
	   while((USART2->SR&0X40)==0)
		{}//ѭ������,ֱ���������   
    USART2->DR = (u8) 0X1A;   
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//�����ɹ�   +QMTPUB: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//�����ɹ�   +QMTPUB: 0,0,0
		}
		delay_ms(500);
		Uart1_SendStr(RxBuffer);
		RxCounter=0;
}

void  Sub_Msgdata(void)//sub  data
{
	strx=strstr((const char*)RxBuffer,(const char*)"+QMTRECV:");
	if(strx)
	{
			strx=strstr((const char*)RxBuffer,(const char*)"S1");
		  if(strx)
			{
			//	LED=0;
				SWITCH=1;
			}
				strx=strstr((const char*)RxBuffer,(const char*)"S0");
			if(strx)
			{
				SWITCH=0;
			}
	Clear_Buffer(); 
	}
	

}