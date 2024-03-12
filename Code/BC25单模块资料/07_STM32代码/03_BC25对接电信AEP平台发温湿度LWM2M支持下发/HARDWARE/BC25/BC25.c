#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC25 BC25_Status;
unsigned char socketnum=0;//��ǰ��socket����
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
                       
                        
					}
				}
             else
             {
                 BC25_Status.netstatus=1;
              }
                
            }
              Clear_Buffer();	
}




void BC25_RegIOT(void)//ƽ̨ע��
{
	 printf("AT+CGSN=1\r\n");//imei data
	 delay_ms(300);
	 Clear_Buffer();	 
   printf("AT+NCDPCLOSE\r\n");
	 delay_ms(300);
	 printf("AT+CSCON=1\r\n");//�����ϱ�����״̬��Ӧ
	 delay_ms(300);
	 printf("AT+NNMI=1\r\n");//���ý�������ֱ��ģʽ
	 delay_ms(300);
	 Clear_Buffer();	
	 printf("AT+NCFG=0,86400\r\n");//������������
	 delay_ms(300);
	 Clear_Buffer();	 
	 printf("AT+NCDPOPEN=\"221.229.214.202\",5683\r\n");//����AEP���ӷ�����
	 delay_ms(300);
	 strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 3");//�����Ϸ����������Է�����
	 delay_ms(300);
	 while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 3");//
			delay_ms(100);
		}  
	 Clear_Buffer();	
		//׼���������ݷ���
}

void BC25_IOTSenddata(u8 len,u8 *data)//�Ϸ����ݣ��Ϸ������ݸ���Ӧ�Ĳ���й�ϵ���û���Ҫע����Ȼ���Ӧ���ݼ���
{ 
   printf("AT+NMGS=%d,%s,100\r\n",len,data); //�������ݵ�ƽ̨
	delay_ms(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 4");//�������ݳɹ�
	delay_ms(300);
	while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 4");//
			delay_ms(100);
		}  
		// Clear_Buffer();
		 Uart1_SendStr((char*)RxBuffer);
    RxCounter=0;
}


void BC25_RECCTwingData(void)
{
	strx=strstr((const char*)RxBuffer,(const char*)"+NNMI:");//key word
	if(strx)
	{
		if(strx[24]=='0')
			SWTICH=0;
		else if(strx[24]=='1')
			SWTICH=1;

	}
    RxCounter=0;
    
}




