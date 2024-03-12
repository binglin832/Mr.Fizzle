#include "led.h"


//初始化PA0和PB0为输出口.并使能这两个口的时钟		    
//LED IO初始化
void LED_Init(void)
{
 
		GPIO_InitTypeDef  GPIO_InitStructure;

		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB, ENABLE);	 //使能PB,PE端口时钟

		GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //LED0-->PB0 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
		GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOB, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA0


	  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;				 //PWRKEY-->PA0 端口配置
		GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 		 //推挽输出
	  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;		 //IO口速度为50MHz
		GPIO_Init(GPIOA, &GPIO_InitStructure);					 //根据设定参数初始化GPIOA0

		PWRKEY=1;
				
}
 
