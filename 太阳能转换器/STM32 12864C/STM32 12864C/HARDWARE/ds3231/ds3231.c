#include <ds3231.h>
#include "delay.h"
#include "myiic.h"


u8 year,month,day,hour,min,sec,week;  //���߶�����з���



u8 BCD2HEX(u8 val)    //BCDת��ΪByte
{
	u8 i;
	i = val & 0x0f;
	val >>= 4;
	val &= 0x0f;
	val *= 10;
	i += val;
	return i;
}
u8 HEX2BCD(u8 val)    //B��ת��ΪBCD��
{
  	u8 i,j,k;
	i = val / 10;
	j = val % 10;
	k = j + (i << 4);
	return k;
}



u8 write_byte(u8 addr, u8 write_data)
{
    IIC_Start();
    IIC_Send_Byte(DS3231_WriteAddress);
		IIC_Wait_Ack();
	
    IIC_Send_Byte(addr);   
		IIC_Wait_Ack();
	
    IIC_Send_Byte(write_data);
		IIC_Wait_Ack();
   
    IIC_Stop();
    delay_us(10); //10     
		return 1;
}					  

u8 read_current()
{
    u8 read_data;
    IIC_Start();
    IIC_Send_Byte(DS3231_ReadAddress);
    IIC_Wait_Ack();
    read_data = IIC_Read_Byte(1);
    IIC_Stop();
    return read_data;
}						 

u8 read_random(u8 random_addr)
{
    u8 Tmp;
	IIC_Start();
  IIC_Send_Byte(DS3231_WriteAddress);
	IIC_Wait_Ack(); 
  IIC_Send_Byte(random_addr);
	IIC_Wait_Ack(); 
	Tmp=read_current();
	if(random_addr==DS3231_HOUR) 
	Tmp&=0x3f;
					    
  return(Tmp);//��ת10�������
}	
//=====================================���ڼ���=================================//
u8 PGday(u16 year,u8 month,u8 day)
{
	  u16 j,count=0;
	  u16 MonthAdd[12] = {0,31,59,90,120,151,181,212,243,273,304,334};
	  count = MonthAdd[month-1];
	  count = count + day;
	  if((year%4 == 0 & year %100 != 0||year % 400 == 0)&& month >= 3)
		count += 1;
	  count = count + (year - 1901) * 365;
	  for(j = 1901;j < year;j++)
	  {
		 if(j % 4 == 0 & j % 100 != 0 || j % 400 == 0)
			 count++;
	  }
//	 return ((count+1) % 7);//����������0
	  week=(count+1) % 7;
	   if (week==0) week=7;  //������԰������գ�0����Ϊ7
 return (week);
}

u8 readdata()
{
	u8 temppp=0;//���BCD��תΪʮ���ƣ����㴦��
  temppp=read_random(0x00);//��
	sec=BCD2HEX(temppp);
  temppp=read_random(0x01);//��
	min=BCD2HEX(temppp);
  temppp=read_random(0x02);  //ʱ
	hour=BCD2HEX(temppp);
	
week= PGday(2000+year,month,day);//���ڼ��㣬	
//  temppp=read_random(0x03);  //����
//	week=BCD2HEX(temppp);
  temppp=read_random(0x04);  //��
	day=BCD2HEX(temppp);
  temppp=read_random(0x05); //��
	month=BCD2HEX(temppp);
  temppp=read_random(0x06);  //��
	year=BCD2HEX(temppp);
	//temp=I2CReadAdd(0x11);//�¶�

  return temppp;

}

void SetTime(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec,u8 week)  //����ʱ���Ӻ���
{
    unsigned char temp = 0;
   
    temp = HEX2BCD(year);
    write_byte(0x06,temp);   //�޸���
   
    temp = HEX2BCD(month);
    write_byte(0x05,temp);  //�޸���
   
    temp = HEX2BCD(day);
    write_byte(0x04,temp);   //�޸���
	
//		temp = week;
//	  write_byte(0x03,temp); 	//�޸�����	
   
    temp = HEX2BCD(hour);
    write_byte(0x02,temp);   //�޸�ʱ
   
    temp = HEX2BCD(min);
    write_byte(0x01,temp); 	//�޸ķ�
   
    temp = HEX2BCD(sec);
    write_byte(0x00,temp); 	//�޸���

} 


