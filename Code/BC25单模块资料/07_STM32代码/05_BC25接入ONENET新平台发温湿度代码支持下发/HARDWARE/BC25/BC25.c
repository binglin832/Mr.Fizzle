#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
//需要用户改的 
#define Pubtopic  "$sys/hMx07yMJon/m001/dp/post/json"  //发布主题
#define Subtopic  "$sys/hMx07yMJon/m001/cmd/request/+"  //订阅主题
//需要用户改的 
#define ProductID  "hMx07yMJon"  //产品名称
#define DeviceID  "m001"  //设备名称
#define Token    "version=2018-10-31&res=products%2FhMx07yMJon&et=1893430861&method=md5&sign=RBalJsFO9yg8BW05Bwz2EA%3D%3D"  //token值
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC25 BC25_Status;
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






void  MQTT_Init(void)
{
  
     printf("AT+QMTCFG=\"version\",0,1\r\n");//设备版本
    delay_ms(500);
    printf("AT+QMTOPEN=0,\"183.230.40.96\",1883\r\n");//通过TCP方式去连接MQTT服务器 
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//看下返回状态
    while(strx==NULL)
    {
      strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//确认返回值正确
    }
    Clear_Buffer(); 
    printf("AT+QMTCONN=0,\"%s\",\"%s\",\"%s\"\r\n",DeviceID,ProductID,Token);//去登录MQTT服务器，设备name，产品ID,TOKEN息。用户根据实际需要进行更改
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
		 printf("AT+QMTSUB=0,1,\"%s\",0\r\n",Subtopic);//订阅个主题
    delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//订阅成功
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//订阅成功
    }
    Clear_Buffer(); 
}


//访问ONENET需要提交JSON数据
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



void Pub_Msgdata(uint8_t temp,uint8_t humi)//发布数据到ONENET显示
{
	  char *strx;
	  u8 datalen,datastr[10],send_jason[200];
		datalen=MqttOnenet_Savedata(send_jason,temp,humi);
   	send_jason[datalen]=0;
	 // Uart1_SendStr(send_jason);
		//sprintf(datastr,"%d",data_len);
		printf("AT+QMTPUB=0,0,0,0,\"%s\"\r\n",Pubtopic);//发布消息
		delay_ms(100);
		printf("%s",send_jason);
	   while((USART2->SR&0X40)==0)
		{}//循环发送,直到发送完毕   
    USART2->DR = (u8) 0X1A;   
		strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//发布成功   +QMTPUB: 0,0,0
		while(strx==NULL)
		{
			strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//发布成功   +QMTPUB: 0,0,0
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