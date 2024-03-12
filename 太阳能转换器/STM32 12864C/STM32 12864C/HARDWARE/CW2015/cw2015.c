#include "cw2015.h" 
#include "delay.h"
#include "myiic.h"   

void CW2015_Init(void)
{
	IIC_Init();                     //��ʼ��IIC��
	delay_ms(50);

	CW2015_WriteOneByte(0x0A,0x00);	  //�ϵ�Ĭ�����ߣ������ڴ˻���
//		delay_ms(500);                   //�ʵ���ʱ
//	QSTART(1);                        //����QSTART����ʼ��������
//		delay_ms(500);                   //�ʵ���ʱ
//	QSTART(0);                        //�ָ�QSTART
	delay_ms(50);                    //�ʵ���ʱ
	
	CW2015_WriteOneByte(CONFIG,0x50);//���õ���������ֵ
	CW2015_WriteOneByte(MOOD,0x00);
}

u8 CW2015_ReadOneByte(u8 ReadAddr)//����ֵ  :����������
{				  
	  u8 temp=0;		  	    																 
    IIC_Start();  
	
		IIC_Send_Byte(CW_WriteAddr);//����д����
		IIC_Wait_Ack();
	
		IIC_Send_Byte(ReadAddr);//���ͼĴ�����ַ
		IIC_Wait_Ack();	 

	  IIC_Start(); 
		IIC_Send_Byte(CW_ReadAddr);           //���Ͷ�����	
	  IIC_Wait_Ack();	
    temp=IIC_Read_Byte(0);		   
    IIC_Stop();//����һ��ֹͣ����	 
   
	return temp;
}

u8 CW2015_WriteOneByte(u8 WriteAddr,u8 Data)
{
    IIC_Start();
		IIC_Send_Byte(CW_WriteAddr );	    //����д����
		if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	   {
	  	IIC_Stop();		 
		  return 1;		
	   }
	  IIC_Send_Byte(WriteAddr);//���ͼĴ�����ַ
	  IIC_Wait_Ack(); 	 										  		   
	  IIC_Send_Byte(Data);     //�����ֽ�							   
	  if(IIC_Wait_Ack())	//�ȴ�Ӧ��
	   {
	  	IIC_Stop();		 
		  return 1;		
	   }  		    	   
    IIC_Stop();//����һ��ֹͣ���� 
	return 0;	 
}
u8 CW2015_ID(void)      //��CW2015��ID
{
  u8 temp=0;
  temp=CW2015_ReadOneByte(0x00);
	return temp;
}

u16 CW2015_Vcell(void) //����ص�ѹ
{
	u16 temp_H=0;
    u16 temp_L=0;
    temp_H=CW2015_ReadOneByte(VCELL_H);//����8λ
	temp_L=CW2015_ReadOneByte(VCELL_L);//����8λ
	
	return ((temp_H<<8)+temp_L)*0.305;  //��ȡ������14λADCֵ������305uV�������õ�ѹ     ��λmV
}

//����ص���
//mode=0  �ٷֱ�  mode=1  1/256
u8 CW2015_Soc(u8 mode)
{
 u8 temp=0;
 if(mode==0)
  {
	  temp=CW2015_ReadOneByte(SOC_B);//���ٷֱȵĵ���
	}
 if(mode==1)
  {
	  temp=CW2015_ReadOneByte(SOC);//��1/256�ĵ���
  }
	return temp;
}	
u16 CW2015_Time(void)//��ȡʣ��ʹ��ʱ�� ����
{
	u16 temp_H=0;
    u16 temp_L=0;
    temp_H=CW2015_ReadOneByte(RRT_H);//����8λ
//	temp_H=temp_H - 0x80;
	temp_L=CW2015_ReadOneByte(RRT_L);//����8λ
	
	return (temp_H<<8)+temp_L;

	
}

u8 CW2015_ALRT(void)//SOC�Ĵ�����ֵ������־λ 
{ 
 u8 temp=0;
 temp=CW2015_ReadOneByte(RRT_H);//����8λ
 temp=temp>>7;
 return temp;
}





