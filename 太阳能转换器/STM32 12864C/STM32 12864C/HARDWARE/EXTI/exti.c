#include "exti.h"
#include "key.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//�ж�ʹ��˵��
//�ж�һ����EXTI_Line0~EXTI_Line15     һ��16���ж���
//PA0PB0PC0PD0......����EXTI_Line0..............PA5PB5PC5PD5......����EXTI_Line5..............PA15PB15PC15PD15......����EXTI_Line15
//EXTI_Line0~~~~~~~~~~~~~~~~~~~~~~void EXTI0_IRQHandler(void)				   �����жϺ���		
//EXTI_Line1~~~~~~~~~~~~~~~~~~~~~~void EXTI1_IRQHandler(void)				   �����жϺ���		
//EXTI_Line2~~~~~~~~~~~~~~~~~~~~~~void EXTI2_IRQHandler(void)				   �����жϺ���		
//EXTI_Line3~~~~~~~~~~~~~~~~~~~~~~void EXTI3_IRQHandler(void)				   �����жϺ���
//EXTI_Line4~~~~~~~~~~~~~~~~~~~~~~void EXTI4_IRQHandler(void)				   �����жϺ���	 
//EXTI_Line5~9~~~~~~~~~~~~~~~~~~~~void EXTI9_5_IRQHandler(void)				 �����жϺ���	     �������ж�����һ���ж�
//EXTI_Line15~10~~~~~~~~~~~~~~~~~~void EXTI15_10_IRQHandler(void)			 �����жϺ���	     �������ж�����һ���ж�
//////////////////////////////////////////////////////////////////////////////////   
u8 keyflag;
//�ⲿ�ж�0�������
void EXTIX_Init(void)
{ 
   	EXTI_InitTypeDef EXTI_InitStructure;
  	NVIC_InitTypeDef NVIC_InitStructure;

    KEY_Init();	 //	�����˿ڳ�ʼ��

  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

	//GPIOB.3 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource3);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line3;	
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

	//GPIOB.4 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource4);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line4;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOB.5 �ж����Լ��жϳ�ʼ������
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB,GPIO_PinSource5);
  	EXTI_InitStructure.EXTI_Line=EXTI_Line5;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);	  	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

    //GPIOA.15 �ж����Լ��жϳ�ʼ������
 	  GPIO_EXTILineConfig(GPIO_PortSourceGPIOA,GPIO_PinSource15); 
  	EXTI_InitStructure.EXTI_Line=EXTI_Line15;
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;//�½��ش���
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
    EXTI_Init(&EXTI_InitStructure);		//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���


  	NVIC_InitStructure.NVIC_IRQChannel = EXTI3_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���

    NVIC_InitStructure.NVIC_IRQChannel = EXTI4_IRQn;			//ʹ�ܰ������ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);  	  //����NVIC_InitStruct��ָ���Ĳ�����ʼ������NVIC�Ĵ���
				

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;			      //ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					//�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure);
		
   	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			    //ʹ�ܰ������ڵ��ⲿ�ж�ͨ��    DHT11��Ҫ���ߵ����ȼ�  ���Ը���
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x02;   	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x02;					  //�����ȼ�2
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
  
}
//�ⲿ�жϷ������ 
void EXTI3_IRQHandler(void)
{
	if(KEY0==0)
	{	  
keyflag=1;
	}
	EXTI_ClearITPendingBit(EXTI_Line3);  //���EXTI0��·����λ
}
void EXTI4_IRQHandler(void)
{
	if(KEY1==0)
	{	  
keyflag=2;
	}
	EXTI_ClearITPendingBit(EXTI_Line4);  //���EXTI0��·����λ
}
void EXTI9_5_IRQHandler(void)
{	
	if(KEY2==0)	 //����KEY0
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


