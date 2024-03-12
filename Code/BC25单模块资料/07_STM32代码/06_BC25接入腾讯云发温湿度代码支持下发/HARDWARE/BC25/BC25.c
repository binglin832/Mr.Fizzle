#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
//��Ҫ�û��ĵ� 
////tengxun����Ԫ��//////////////////////
#define IPdata      "TPW48OE83Y.iotcloud.tencentdevices.com"
#define ClientID     "TPW48OE83Ym001"             //ClientID
#define Username          "TPW48OE83Ym001;12010126;9IOP8;1700640653"      //
#define Password    "17332ccf30b28d986d44bdfce1e0e3e94d40bc4b1d923e72906985ef152b2698;hmacsha256"  //
#define PubTopic         "$thing/up/property/TPW48OE83Y/m001"
#define SubTopic        "$thing/down/property/TPW48OE83Y/m001"
char *strx,*extstrx;
extern char RxBuffer[500];     //���ջ���,���USART_REC_LEN���ֽ�.
extern  int  RxCounter; 
BC25 BC25_Status;
void Clear_Buffer(void)//��ջ���
{
		int i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<500;i++)
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



void TengxunMQTT_Init(void)//Tengxun���������ó�ʼ��
{ 
	
	  printf("AT+QMTOPEN=0,\"%s\",1883\r\n",IPdata);//ͨ��TCP��ʽȥ����MQTT������ 
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//ȷ�Ϸ���ֵ��ȷ
    }
    Clear_Buffer(); 
    printf("AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",ClientID,Username,Password);;//ȥ��¼MQTT��������
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//���·���״̬
    }
    Clear_Buffer(); 
    printf("AT+QMTSUB=0,1,\"%s\",0\r\n",SubTopic);//���ĸ�����
    delay_ms(500);
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//���·���״̬
      while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//���·���״̬
    }
    Clear_Buffer(); 
   

}



//����tengxunyun��Ҫ�ύJSON����
uint8_t Tengxunyun_Savedata(uint8_t *t_payload,uint8_t temp,uint8_t humi)
{
	char json[]="{\"method\":\"report\",\"clientToken\":\"123\",\"params\":{\"temp\":%d,\"humi\":%d}}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
		uint8_t value;
    sprintf(t_json, json, temp, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}


void Pub_Msgdata(uint8_t temp,uint8_t humi)//�������ݵ�tengxunyun��ʾ
{
	  char *strx;
	  u8 datalen,datastr[10],send_jason[200];
		datalen=Tengxunyun_Savedata(send_jason,temp,humi);
   	send_jason[datalen]=0;
	//  Uart1_SendStr(send_jason);
		//sprintf(datastr,"%d",data_len);
		printf("AT+QMTPUB=0,0,0,0,\"%s\"\r\n",PubTopic);//������Ϣ
		delay_ms(100);
		printf("%s",send_jason);
	   while((USART2->SR&0X40)==0)
		{}//ѭ������,ֱ���������   
    USART2->DR = (u8)0X1A;   
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//�����ɹ�   +QMTPUB: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//�����ɹ�   +QMTPUB: 0,0,0
		}
		delay_ms(500);
		Sub_Msgdata();
		//	//	Clear_Buffer(); 
		Uart1_SendStr(RxBuffer);
		RxCounter=0;
}



void  Sub_Msgdata(void)//sub  data
{
	  uint8_t recdata[250];
		Uart1_SendStr(RxBuffer);
	if(uart2_getok)
	{
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTRECV:");//����+QMTRECV:���������յ�onenetƽ̨
		//+QMTRECV: 0,0,"$creq/a54ff306-8993-52e1-9cbf-5ef099f4c0c8","1"
		if(strx)
		{//+QMTRECV: 0,0,"$thing/down/property/TPW48OE83Y/m001",86,{"method":"control","clientToken":"v2529763559SSYqc::bQTwZvAU3","params":{"LEDCTR":0}}
//				strx=strstr((const char*)RxBuffer,(const char*)"}}");//
//				//  ��}"���ִ������ݽ������
//				if(strx)
//				{
//				memcpy(recdata, RxBuffer, RxCounter);//���յ������ݱ��� 
//				CParsejson(recdata); 

//		}
//		memset(recdata, 0, 250);
			strx=strstr((const char*)RxBuffer,(const char*)"\"LEDCTR\":0");//
	    if(strx)
			{
					SWITCH=0;
				  Clear_Buffer(); 
			}
			strx=strstr((const char*)RxBuffer,(const char*)"\"LEDCTR\":1");//
	    if(strx)
			{
			    SWITCH=1;
					Clear_Buffer(); 
				
			}
		} 
		RxCounter=0;
		uart2_getok=0;
	}

}

