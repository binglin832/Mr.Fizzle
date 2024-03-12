#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC25 BC25_Status;
unsigned char socketnum=0;//当前的socket号码
void Clear_Buffer(void)//清空缓存
{
		u8 i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<100;i++)
		RxBuffer[i]=0;//缓存
		RxCounter=0;
		IWDG_Feed();//喂狗
	
}
void BC25_Init(void)
{
    printf("AT\r\n"); 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT\r\n"); 
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    }
    printf("AT+CFUN=1\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"460");//返460，表明识别到卡了
    Clear_Buffer();	
    while(strx==NULL)
    {
        Clear_Buffer();	
        printf("AT+CIMI\r\n");//获取卡号，类似是否存在卡的意思，比较重要。
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"460");//返回OK,说明卡是存在的
    }
        printf("AT+CGATT=1\r\n");//激活网络，PDP
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"OK");//返OK
        Clear_Buffer();	
        printf("AT+CGATT?\r\n");//查询激活状态
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返1
        Clear_Buffer();	
		while(strx==NULL)
		{
            Clear_Buffer();	
            printf("AT+CGATT?\r\n");//获取激活状态
            delay_ms(300);
            strx=strstr((const char*)RxBuffer,(const char*)"+CGATT: 1");//返回1,表明注网成功
		}
		printf("AT+CESQ\r\n");//查看获取CSQ值
        delay_ms(300);
        strx=strstr((const char*)RxBuffer,(const char*)"+CESQ");//返回CSQ
		if(strx)
			{
				BC25_Status.CSQ=(strx[7]-0x30)*10+(strx[8]-0x30);//获取CSQ
				if((BC25_Status.CSQ==99)||((strx[7]-0x30)==0))//说明扫网失败
				{
					while(1)
					{
                        BC25_Status.netstatus=0;
						Uart1_SendStr("信号搜索失败，请查看原因!\r\n");
                       
                        
					}
				}
             else
             {
                 BC25_Status.netstatus=1;
              }
                
            }
              Clear_Buffer();	
}




void BC25_RegIOT(void)//平台注册
{
	 printf("AT+CGSN=1\r\n");//imei data
	 delay_ms(300);
	 Clear_Buffer();	 
   printf("AT+NCDPCLOSE\r\n");
	 delay_ms(300);
	 printf("AT+CSCON=1\r\n");//设置上报数据状态响应
	 delay_ms(300);
	 printf("AT+NNMI=1\r\n");//设置接收数据直吐模式
	 delay_ms(300);
	 Clear_Buffer();	
	 printf("AT+NCFG=0,86400\r\n");//配置生命周期
	 delay_ms(300);
	 Clear_Buffer();	 
	 printf("AT+NCDPOPEN=\"221.229.214.202\",5683\r\n");//配置AEP连接服务器
	 delay_ms(300);
	 strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 3");//连接上服务器并可以发数据
	 delay_ms(300);
	 while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 3");//
			delay_ms(100);
		}  
	 Clear_Buffer();	
		//准备进行数据发送
}

void BC25_IOTSenddata(u8 len,u8 *data)//上发数据，上发的数据跟对应的插件有关系，用户需要注意插件然后对应数据即可
{ 
   printf("AT+NMGS=%d,%s,100\r\n",len,data); //发送数据到平台
	delay_ms(300);
	strx=strstr((const char*)RxBuffer,(const char*)"+QLWEVTIND: 4");//发送数据成功
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




