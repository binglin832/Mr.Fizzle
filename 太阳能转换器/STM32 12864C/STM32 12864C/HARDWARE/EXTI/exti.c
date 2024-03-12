#include "exti.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//中断使用说明
//中断一共有EXTI_Line0~EXTI_Line15     一共16条中断线
//PA0PB0PC0PD0......共用EXTI_Line0..............PA5PB5PC5PD5......共用EXTI_Line5..............PA15PB15PC15PD15......共用EXTI_Line15
//EXTI_Line0~~~~~~~~~~~~~~~~~~~~~~void EXTI0_IRQHandler(void)				   单独中断函数		
//EXTI_Line1~~~~~~~~~~~~~~~~~~~~~~void EXTI1_IRQHandler(void)				   单独中断函数		
//EXTI_Line2~~~~~~~~~~~~~~~~~~~~~~void EXTI2_IRQHandler(void)				   单独中断函数		
//EXTI_Line3~~~~~~~~~~~~~~~~~~~~~~void EXTI3_IRQHandler(void)				   单独中断函数
//EXTI_Line4~~~~~~~~~~~~~~~~~~~~~~void EXTI4_IRQHandler(void)				   单独中断函数	 
//EXTI_Line5~9~~~~~~~~~~~~~~~~~~~~void EXTI9_5_IRQHandler(void)				 共用中断函数	     函数中判断是哪一个中断
//EXTI_Line15~10~~~~~~~~~~~~~~~~~~void EXTI15_10_IRQHandler(void)			 共用中断函数	     函数中判断是哪一个中断
//////////////////////////////////////////////////////////////////////////////////   
u8 keyflag;
//外部中断0服务程序
void EXTIX_Init(void)
{ 
   	EXTI_InitTypeDef EXTI_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	按键端口初始化

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

	//GPIOB.3 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

	//GPIOB.4 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOB.5 中断线以及中断初始化配置
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	  	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

    //GPIOA.15 中断线以及中断初始化配置
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//下降沿触发
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//使能按键所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);  	  //根据NVIC_InitStruct中指定的参数初始化外设NVIC寄存器
				

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			      //使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure);
		
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //使能按键所在的外部中断通道    DHT11需要更高的优先级  所以更改
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;   	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					  //子优先级2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
  
}
//外部中断服务程序 
void EXTI3_IRQHandler(void)
{
	if(KEY0==0)
	{	  
keyflag=1;
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //清除EXTI0线路挂起位
}
void EXTI4_IRQHandler(void)
{
	if(KEY1==0)
	{	  
keyflag=2;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //清除EXTI0线路挂起位
}
void EXTI9_5_IRQHandler(void)
{	
	if(KEY2==0)	 //按键KEY0
	{
keyflag=3;
	}	
  EXTI_ClearITPendingBit(EXTI_Line5); 
}
void EXTI15_10_IRQHandler(void)
{		 
  if(KEY3==0)
	{
keyflag=4;
	}
  EXTI_ClearITPendingBit(EXTI_Line15); 
}	


