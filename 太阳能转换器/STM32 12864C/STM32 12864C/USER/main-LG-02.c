#include "menu.h"
#include "sys.h"
#include "delay.h"
#include "usart.h"
#include <ds3231.h> 
#include "myiic.h"
#include "w25qxx.h" 
#include "st7565.h"
#include "zimu.h"
#include "key.h"
#include "clock.h"
#include "24c02.h"
#include "timer.h"
#include "led.h"
#include "text.h"	
#include "dht11.h"
#include "exti.h"
#include "ina226.h"
#include "cw2015.h" 


//跨文件调用，需要加extern
extern u8	year,month,day,hour,min,sec,week;
extern u8 TIM_1,TIM_2,TIM_3;//定时器产生的3个不同频率的标志
///////////////////////////////////////////////////////

u8 rst;    //24C02和时钟复位标志


void date_init()//24C02和时钟复位初始化
 {
	 u8 i=0;
	 if((KEY2==0&&KEY3==0)||rst==1)  //开机上电key_exit和key_enter同时按住可以进入初始化操作
 {          
  CleanScreen();
	Show_Str(0,8,128,16," 初始化设置数据",16,0);	
	Show_Str(0,32,128,16,"菜单键    返回键",16,0);	
	Show_Str(0,48,128,16," 确认      取消",16,0);	
	ShowAllGraphRAM();

		while(KEY2 == 0);//等待键松开
		while(1) 
			{
		if(KEY3 ==0) 
			{
			delay_ms(100);
		if(KEY3 ==0)
	{ 
		while(KEY3 == 0);//等待键松开
				
  CleanScreen();
	Show_Str(0,16,112,16,"初始化中...",16,0);	
	ShowAllGraphRAM();
		
		year=20;
		month=1;
		day=4;
		hour=12;
		min=12;
		sec=12;
		 
	  SetTime(year,month,day,hour,min,sec,week) ;	
//====================================================================设置AT24C02的初始值=====================================			
	  AT24CXX_WriteOneByte(0,9);   //表示在地址X写Y
	  AT24CXX_WriteOneByte(1,9);
	  AT24CXX_WriteOneByte(2,9);
	  AT24CXX_WriteOneByte(3,35);
	  AT24CXX_WriteOneByte(4,9);  //默认对比度值35
	 	AT24CXX_WriteOneByte(5,10);  //默认菜单时间20秒
		delay_ms(50);
	 
  CleanScreen();
	Show_Str(0,32,96,16,"初始化已完成",16,0);	
	ShowAllGraphRAM();
	
	  delay_ms(1000);

	 i=1; rst=0;
	 CleanScreen();
	 break;
    }
	 }
	  if(KEY2 == 0&&i==0) 
	{
		delay_ms(100);
  	if(KEY2 == 0&&i==0)
	{ 
  CleanScreen();
	Show_Str(0,32,96,16,"初始化已取消",16,0);	
	ShowAllGraphRAM();				
		
	  delay_ms(500);
		while(KEY2== 0);//等待键松开
		CleanScreen();
		break;
		 }
	  }
   }
	}
	
	i =AT24CXX_ReadOneByte(4);  //读取对比度的值 
	St7565_WriteCmd(0x81);      //第18命令 对比度 6位电压值数据，0～63 共 64 级		
	St7565_WriteCmd(i);         //范围0x00-0x3f 值越大越浓和上一条指令同时用0x81不动		
		
 }
void err_set()  //检查24C02是否设置参数错误
{
	  printf("读取24C02部分数据\r\n");  //打印数据
		printf("byte0=%d \r\n",AT24CXX_ReadOneByte(0));	
		printf("byte1=%d \r\n",AT24CXX_ReadOneByte(1));		
		printf("byte2=%d \r\n",AT24CXX_ReadOneByte(2));	
		printf("byte3=%d \r\n",AT24CXX_ReadOneByte(3));	
		printf("byte4=%d \r\n",AT24CXX_ReadOneByte(4));		
		printf("byte5=%d \r\n",AT24CXX_ReadOneByte(5));		
			
		if(AT24CXX_ReadOneByte(0)>15||AT24CXX_ReadOneByte(1)>15||AT24CXX_ReadOneByte(2)>15||AT24CXX_ReadOneByte(3)>15)        //通过读取24C02内部数据判断参数是否正常
	{

	Show_Str(0,0,112,16,"设置数据有错误",16,0);	
	Show_Str(0,16,128,16,"系统不能正常启动",16,0);	
	Show_Str(0,32,128,16,"按“菜单键”启动",16,0);	
	Show_Str(0,48,80,16,"初始化程序",16,0);	

	ShowAllGraphRAM();				

		while(1)
		{
		if(KEY3 == 0) 
			{
			delay_ms(100);
		if(KEY3 == 0)
	  { 
		  rst=1;
			while(KEY3 == 0);//等待键松开
			date_init();
			break;
	      }
      }
		}
	}
}



int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//设置中断优先级分组为组2：2位抢占优先级，2位响应优先级
	SysTick_Initaize();   //系统滴答定时器初始化函数
	delay_init();	    	         //延时函数初始化	
	uart_init(9600);		         //串口打印需要必须初始化串口 
	printf("系统启动中...\r\n");
	printf(".............\r\n");

	TIM3_Int_Init(1000,7199);    //10Khz的计数频率，计数到5000为500ms  
			
	font_init();                 //SPI1 初始化    检查字库

	AT24CXX_Init();              //初始化I2C      24CXX初始化和检测通讯是否正常
	KEY_Init();	                 //按键端口初始化	
	Init_St7565();               //LCD12864初始化
	LED_Init();                  //LED灯初始化	
	INA226_Init();               //电压电流检测初始化
	CW2015_Init();               //CW2015库仑计初始化		
	DHT11_Init();	               //温湿度传感器初始化
	EXTIX_Init();		             //按键中断初始化		

	date_init();                 //设置数据初始化，读取并使用初始化后的参数
	err_set();                   //设置错误检测
					
   lcd_bk_high();
				
while(1)
	{			
	
		Menu_key_set();
//        delay_ms(5000);		
//		printf("电压=%d\r\n",INA226_GetVoltage(INA226_ADDR1));
//		printf("电流=%d\r\n",INA226_GetCurrent(INA226_ADDR1));		
			
	}   
} 







