#ifndef __KEY_H
#define __KEY_H	 
#include "sys.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//������������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   	 


#define KEY0  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_3)  //��ȡ������
#define KEY1  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_4)  //��ȡ������
#define KEY2  GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_5)  //��ȡ�����˳� 
#define KEY3  GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_15) //��ȡ��������

 
void KEY_Init(void);//IO��ʼ��		
#endif
