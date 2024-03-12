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


//���ļ����ã���Ҫ��extern
extern u8	year,month,day,hour,min,sec,week;
extern u8 TIM_1,TIM_2,TIM_3;//��ʱ��������3����ͬƵ�ʵı�־
///////////////////////////////////////////////////////

u8 rst;    //24C02��ʱ�Ӹ�λ��־


void date_init()//24C02��ʱ�Ӹ�λ��ʼ��
 {
	 u8 i=0;
	 if((KEY2==0&&KEY3==0)||rst==1)  //�����ϵ�key_exit��key_enterͬʱ��ס���Խ����ʼ������
 {          
  CleanScreen();
	Show_Str(0,8,128,16," ��ʼ����������",16,0);	
	Show_Str(0,32,128,16,"�˵���    ���ؼ�",16,0);	
	Show_Str(0,48,128,16," ȷ��      ȡ��",16,0);	
	ShowAllGraphRAM();

		while(KEY2 == 0);//�ȴ����ɿ�
		while(1) 
			{
		if(KEY3 ==0) 
			{
			delay_ms(100);
		if(KEY3 ==0)
	{ 
		while(KEY3 == 0);//�ȴ����ɿ�
				
  CleanScreen();
	Show_Str(0,16,112,16,"��ʼ����...",16,0);	
	ShowAllGraphRAM();
		
		year=20;
		month=1;
		day=4;
		hour=12;
		min=12;
		sec=12;
		 
	  SetTime(year,month,day,hour,min,sec,week) ;	
//====================================================================����AT24C02�ĳ�ʼֵ=====================================			
	  AT24CXX_WriteOneByte(0,9);   //��ʾ�ڵ�ַXдY
	  AT24CXX_WriteOneByte(1,9);
	  AT24CXX_WriteOneByte(2,9);
	  AT24CXX_WriteOneByte(3,35);
	  AT24CXX_WriteOneByte(4,9);  //Ĭ�϶Աȶ�ֵ35
	 	AT24CXX_WriteOneByte(5,10);  //Ĭ�ϲ˵�ʱ��20��
		delay_ms(50);
	 
  CleanScreen();
	Show_Str(0,32,96,16,"��ʼ�������",16,0);	
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
	Show_Str(0,32,96,16,"��ʼ����ȡ��",16,0);	
	ShowAllGraphRAM();				
		
	  delay_ms(500);
		while(KEY2== 0);//�ȴ����ɿ�
		CleanScreen();
		break;
		 }
	  }
   }
	}
	
	i =AT24CXX_ReadOneByte(4);  //��ȡ�Աȶȵ�ֵ 
	St7565_WriteCmd(0x81);      //��18���� �Աȶ� 6λ��ѹֵ���ݣ�0��63 �� 64 ��		
	St7565_WriteCmd(i);         //��Χ0x00-0x3f ֵԽ��ԽŨ����һ��ָ��ͬʱ��0x81����		
		
 }
void err_set()  //���24C02�Ƿ����ò�������
{
	  printf("��ȡ24C02��������\r\n");  //��ӡ����
		printf("byte0=%d \r\n",AT24CXX_ReadOneByte(0));	
		printf("byte1=%d \r\n",AT24CXX_ReadOneByte(1));		
		printf("byte2=%d \r\n",AT24CXX_ReadOneByte(2));	
		printf("byte3=%d \r\n",AT24CXX_ReadOneByte(3));	
		printf("byte4=%d \r\n",AT24CXX_ReadOneByte(4));		
		printf("byte5=%d \r\n",AT24CXX_ReadOneByte(5));		
			
		if(AT24CXX_ReadOneByte(0)>15||AT24CXX_ReadOneByte(1)>15||AT24CXX_ReadOneByte(2)>15||AT24CXX_ReadOneByte(3)>15)        //ͨ����ȡ24C02�ڲ������жϲ����Ƿ�����
	{

	Show_Str(0,0,112,16,"���������д���",16,0);	
	Show_Str(0,16,128,16,"ϵͳ������������",16,0);	
	Show_Str(0,32,128,16,"�����˵���������",16,0);	
	Show_Str(0,48,80,16,"��ʼ������",16,0);	

	ShowAllGraphRAM();				

		while(1)
		{
		if(KEY3 == 0) 
			{
			delay_ms(100);
		if(KEY3 == 0)
	  { 
		  rst=1;
			while(KEY3 == 0);//�ȴ����ɿ�
			date_init();
			break;
	      }
      }
		}
	}
}



int main(void)
{	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//�����ж����ȼ�����Ϊ��2��2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	SysTick_Initaize();   //ϵͳ�δ�ʱ����ʼ������
	delay_init();	    	         //��ʱ������ʼ��	
	uart_init(9600);		         //���ڴ�ӡ��Ҫ�����ʼ������ 
	printf("ϵͳ������...\r\n");
	printf(".............\r\n");

	TIM3_Int_Init(1000,7199);    //10Khz�ļ���Ƶ�ʣ�������5000Ϊ500ms  
			
	font_init();                 //SPI1 ��ʼ��    ����ֿ�

	AT24CXX_Init();              //��ʼ��I2C      24CXX��ʼ���ͼ��ͨѶ�Ƿ�����
	KEY_Init();	                 //�����˿ڳ�ʼ��	
	Init_St7565();               //LCD12864��ʼ��
	LED_Init();                  //LED�Ƴ�ʼ��	
	INA226_Init();               //��ѹ��������ʼ��
	CW2015_Init();               //CW2015���ؼƳ�ʼ��		
	DHT11_Init();	               //��ʪ�ȴ�������ʼ��
	EXTIX_Init();		             //�����жϳ�ʼ��		

	date_init();                 //�������ݳ�ʼ������ȡ��ʹ�ó�ʼ����Ĳ���
	err_set();                   //���ô�����
					
   lcd_bk_high();
				
while(1)
	{			
	
		Menu_key_set();
//        delay_ms(5000);		
//		printf("��ѹ=%d\r\n",INA226_GetVoltage(INA226_ADDR1));
//		printf("����=%d\r\n",INA226_GetCurrent(INA226_ADDR1));		
			
	}   
} 







