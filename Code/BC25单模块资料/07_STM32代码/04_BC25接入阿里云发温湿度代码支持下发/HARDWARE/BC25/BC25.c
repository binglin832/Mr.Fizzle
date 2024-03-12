#include "BC25.h"
#include "string.h"
#include "usart.h"
#include "wdg.h"
#include "led.h"
#include "cJSON.h"
char *strx,*extstrx;
extern unsigned char  RxBuffer[255],RxCounter;
BC25 BC25_Status;
////阿里云的三元素,在设备里面都可以查询到//////////////////////
#define ProductKey     "a1ZMkwhevG9"             //产品KEY
#define DeviceName          "mzh001"      //
#define DeviceSecret    "cc96840980ac49570acc47e2c0195b5c"  //
#define PubTopic         "/sys/a1ZMkwhevG9/mzh001/thing/event/property/post"//发布主题
#define SubTopic        "/sys/a1NXoxeFw4c/mzh002/thing/service/property/set"//订阅主题
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




void BC25_ConTCP(void)
{
	printf("AT+QICLOSE=0\r\n");//关闭socekt连接
	delay_ms(300);
    Clear_Buffer();
    IWDG_Feed();//喂狗
}
void BC25_CreateTCPSokcet(void)//创建sokcet
{

    printf("AT+QIOPEN=1,0,\"TCP\",\"47.99.80.89\",14269,1234,1\r\n");//创建连接TCP,输入IP以及服务器端口号码 ,采用直接吐出的方式
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//检查是否登陆成功
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"+QIOPEN: 0,0");//检查是否登陆成功
            delay_ms(100);
		}  
     Clear_Buffer();	
    
}
void BC25_Senddata(uint8_t *len,uint8_t *data)//字符串形式
{
    printf("AT+QSOSEND=0,%s,%s\r\n",len,data);
}

void BC25_Senddatahex(uint8_t *len,uint8_t *data)//发送十六进制数据
{
    printf("AT+QISENDEX=0,%s,%s\r\n",len,data);
        delay_ms(300);
 	while(strx==NULL)
		{
            strx=strstr((const char*)RxBuffer,(const char*)"SEND OK");//检查是否发送成功
            delay_ms(100);
		}  
     Clear_Buffer();	
}



void  MQTT_Init(void)
{
  
    printf("AT+QMTCFG=\"aliauth\",0,\"%s\",\"%s\",\"%s\"\r\n",ProductKey,DeviceName,DeviceSecret);
    delay_ms(300);
    printf("AT+QMTOPEN=0,\"139.196.135.135\",1883\r\n");//通过TCP方式去连接MQTT阿里云服务器 
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//看下返回状态
  while(strx==NULL)
    {
      strx=strstr((const char*)RxBuffer,(const char*)"+QMTOPEN: 0,0");//确认返回值正确
    }
    Clear_Buffer(); 
   printf("AT+QMTCONN=0,\"clientExample_1987\"\r\n");//去登录MQTT服务器
    delay_ms(300);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTCONN: 0,0,0");//看下返回状态
    }
    Clear_Buffer(); 
    /*
    printf("AT+QMTSUB=0,1,\"mzh_m26\",0\r\n");//订阅个主题
        delay_ms(500);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//订阅成功
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTSUB: 0,1,0,0");//订阅成功
    }
    Clear_Buffer(); 
    */
}

void aliyunMQTT_PUBdata(u8 temp,u8 humi)
{
     u8 t_payload[200],len;
     printf("AT+QMTPUB=0,0,0,0,\"%s\"\r\n",PubTopic);//发布主题
     delay_ms(300);
     len=Mqttaliyun_Savedata(t_payload,temp,humi);
     t_payload[len]=0;
     printf("%s",t_payload);
  while((USART2->SR&0X40)==0){}//循环发送,直到发送完毕   
     USART2->DR = (u8) 0x1A;    
     delay_ms(100);
     strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
  while(strx==NULL)
    {
        strx=strstr((const char*)RxBuffer,(const char*)"+QMTPUB: 0,0,0");//看下返回状态
    }
		Uart1_SendStr((char*)RxBuffer);
		RxCounter=0;
   // Clear_Buffer(); 
}

//访问阿里云需要提交JSON数据
u8 Mqttaliyun_Savedata(u8 *t_payload,u8 temp,u8 humi)
{

    char json[]="{\"id\":\"26\",\"version\":\"1.0\",\"sys\":{\"ack\":0},\"params\":{\"CurrentTemperature\":{\"value\":%d},\"CurrentHumidity\":{\"value\":%d}},\"method\":\"thing.event.property.post\"}";	 //     char json[]="{\"datastreams\":[{\"id\":\"location\",\"datapoints\":[{\"value\":{\"lon\":%2.6f,\"lat\":%2.6f}}]}]}";
    char t_json[200];
    unsigned short json_len;
    sprintf(t_json, json, temp, humi);
    json_len = strlen(t_json)/sizeof(char);
  	memcpy(t_payload, t_json, json_len);
    return json_len;
}



void BC25_RECData(void)
{
	  uint8_t recdata[250];
		Uart1_SendStr(RxBuffer);
    strx=strstr((const char*)RxBuffer,(const char*)"+QMTRECV:");//返回+QMTRECV:，表明接收到onenet平台
	      //+QMTRECV: 0,0,"$creq/a54ff306-8993-52e1-9cbf-5ef099f4c0c8","1"
	if(strx)
	{
		//	strx=strstr((const char*)RxBuffer,(const char*)"\"}");
			//  有}出现代表数据接收完成

				memcpy(recdata, RxBuffer, RxCounter);//接收到的数据保存 
				CParsejson(recdata); 
				Clear_Buffer();  
				memset(recdata, 0, 250);
	 
	} 
	if(uart2_getok)//空闲的时候，将数组归0
	{
		RxCounter=0;
	}
    
    
}


void CParsejson(char *data)
{
    uint8_t i=0,*jsonstrx;
		uint8_t jsondata[250];
		uint8_t status;
  // char* data = "{\"method\":\"thing.service.property.set\",\"id\":\"1686637400\",\"params\":{\"WorkSwitch\":1},\"version\":\"1.0.0\"}";
     cJSON * root, *itemName,*itemAge,*order;
     //从缓冲区中解析json结构
    jsonstrx=strstr((const char*)data,(const char*)"{"); //判断到有"{出现,代表json产生 
    if(jsonstrx)
    {
         while(1)
        {
            if(jsonstrx[i]==0x0D)//找到结束符，就退出  
            {   
                jsondata[i]=0;
                i=0;
                break;
            }
            jsondata[i]=jsonstrx[i];
            i++;
        }
        
    }

    Uart1_SendStr(jsondata);
    root = cJSON_Parse(jsondata);   
    if (jsondata == NULL)
    {
        return;
    }

    order = cJSON_GetObjectItem(root, "params");
	if (order == NULL) {
		return ;
	}
    itemName = cJSON_GetObjectItem(order, "NightLightSwitch");

    if (itemName)
    {
      status= itemName->valueint;//
        if(status)//开继电器
				SWITCH=1;
       else//关继电器
				SWITCH=0;
      
    }
		
		 itemName = cJSON_GetObjectItem(order, "Buzzer");
    if (itemName)
    {
      status= itemName->valueint;//
//        if(status)//开BEEP
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_RESET);
//       else//关BEEP
//        HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, GPIO_PIN_SET);
    }
    //将json结构占用的数据空间释放
    memset(jsondata, 0, 250);
    cJSON_Delete(root);
    
}


