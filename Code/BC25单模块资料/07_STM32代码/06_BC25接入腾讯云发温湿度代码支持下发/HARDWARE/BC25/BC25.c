#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
//需要用户改的 
////tengxun的三元素//////////////////////
#define IPdata      "TPW48OE83Y.iotcloud.tencentdevices.com"
#define ClientID     "TPW48OE83Ym001"             //ClientID
#define Username          "TPW48OE83Ym001;12010126;9IOP8;1700640653"      //
#define Password    "17332ccf30b28d986d44bdfce1e0e3e94d40bc4b1d923e72906985ef152b2698;hmacsha256"  //
#define PubTopic         "$thing/up/property/TPW48OE83Y/m001"
#define SubTopic        "$thing/down/property/TPW48OE83Y/m001"
char *strx,*extstrx;
extern char RxBuffer[500];     //接收缓冲,最大USART_REC_LEN个字节.
extern  int  RxCounter; 
BC25 BC25_Status;
void Clear_Buffer(void)//清空缓存
{
		int i;
		Uart1_SendStr((char*)RxBuffer);
		for(i=0;i<500;i++)
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
		 printf("AT\r\n"); 
    delay_ms(300);
    IWDG_Feed();//喂狗
    strx=strstr((const char*)RxBuffer,(const char*)"OK");//返回OK
    printf("ATE0&W\r\n"); //关闭回显
    delay_ms(300); 
    LED=0;
    IWDG_Feed();//喂狗
    printf("AT+QMTDISC=0\r\n");//关闭连接
    delay_ms(300);
    printf("AT+QMTCLOSE=0\r\n");
    delay_ms(300); 
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
						delay_ms(300);//没有信号就复位
                        
					}
				}
             else
             {
                 BC25_Status.netstatus=1;
              }
                
            }
              Clear_Buffer();	
}



void TengxunMQTT_Init(void)//Tengxun服务器配置初始化
{ 
	
	  printf("AT+QMTOPEN=0,\"%s\",1883\r\n",IPdata);//通过TCP方式去连接MQTT服务器 
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//确认返回值正确
    }
    Clear_Buffer(); 
    printf("AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",ClientID,Username,Password);;//去登录MQTT服务器，
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
    printf("AT+QMTSUB=0,1,\"%s\",0\r\n",SubTopic);//订阅个主题
    delay_ms(500);
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//看下返回状态
      while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//看下返回状态
    }
    Clear_Buffer(); 
   

}



//访问tengxunyun需要提交JSON数据
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


void Pub_Msgdata(uint8_t temp,uint8_t humi)//发布数据到tengxunyun显示
{
	  char *strx;
	  u8 datalen,datastr[10],send_jason[200];
		datalen=Tengxunyun_Savedata(send_jason,temp,humi);
   	send_jason[datalen]=0;
	//  Uart1_SendStr(send_jason);
		//sprintf(datastr,"%d",data_len);
		printf("AT+QMTPUB=0,0,0,0,\"%s\"\r\n",PubTopic);//发布消息
		delay_ms(100);
		printf("%s",send_jason);
	   while((USART2->SR&0X40)==0)
		{}//循环发送,直到发送完毕   
    USART2->DR = (u8)0X1A;   
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//发布成功   +QMTPUB: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//发布成功   +QMTPUB: 0,0,0
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
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTRECV:");//返回+QMTRECV:，表明接收到onenet平台
		//+QMTRECV: 0,0,"$creq/a54ff306-8993-52e1-9cbf-5ef099f4c0c8","1"
		if(strx)
		{//+QMTRECV: 0,0,"$thing/down/property/TPW48OE83Y/m001",86,{"method":"control","clientToken":"v2529763559SSYqc::bQTwZvAU3","params":{"LEDCTR":0}}
//				strx=strstr((const char*)RxBuffer,(const char*)"}}");//
//				//  有}"出现代表数据接收完成
//				if(strx)
//				{
//				memcpy(recdata, RxBuffer, RxCounter);//接收到的数据保存 
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

