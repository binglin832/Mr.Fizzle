#include <clock.h>
#include "nongliSTM.h"
#include <ds3231.h>
#include "st7565.h"
#include "zimu.h"
#include "usart.h"
#include "key.h"
#include "delay.h"
#include "24c02.h" 
#include "text.h"	
#include "dht11.h"
#include "cw2015.h" 
#include "ina226.h"
#include "led.h"

extern u8 keyflag;//�����жϷ���ֵ


extern u16 TIM_1,TIM_2,TIM_3;//��ʱ��������3����ͬƵ�ʵı�־
extern u8 year,month,day,hour,min,sec,week;
extern u8 year_moon,month_moon,day_moon;//�ⲿ����ũ��


extern u8 rst;    //24C02��ʱ�Ӹ�λ��־


u8 s_main,d_main,M_time;
u8 temperature,humidity;  	  

/*****************************************************************************/
void main_ss()  //�Զ������������ʱ  �ڵ���ʱ��Ҫ��ʼ��
{
	u8 i,j; 
	j =AT24CXX_ReadOneByte(5);   
	i = read_random(0x00);	
	s_main=(i/16*10+i%16)+j;	
	if(s_main>=60){s_main=s_main%60;}
}
void main_return()    //��ȡ��ǰ�����Ӿ���ֵ
{
	u8	i;
	i = read_random(0x00);
	d_main=(i/16*10+i%16);
}
void menu_time_lcm() //��ʾ�˵�ʱ��
{	
  M_time = s_main;
  if(s_main<d_main) M_time =s_main+60;
  M_time = M_time-d_main;
  LCD_ShowNum(0,52,M_time,2,12,1);//X,Y�������,��ʾ��ֵ,��ֵλ���������С 
	ShowAllGraphRAM();	
}

void getdata(void)                           //��ȡ��Ҫ��ʾ��ʹ�õ���Ϣ
{
	if(TIM_1%20==1)    //����X����������Y
  { 
	DHT11_Read_Data(&temperature,&humidity);	//��ȡ��ʪ��ֵ	
//	printf("�¶�ֵ=%d\r\n",temperature);	
//	printf("ʪ��=%d\r\n",humidity);	 
    LED0=!LED0;	
    LCDBK=AT24CXX_ReadOneByte(9); 	  
  } 

}

void dht11disp(void)
{

	Show_Str(112,28,12,12,"��",12,0);
	Show_Str(112,40,12,12,"%",12,0);	
	LCD_ShowNum(100,28,temperature,2,12,1);
	LCD_ShowNum(100,40,humidity,2,12,1);
  ShowAllGraphRAM();	

}	



void dispmaohao (void)  //��ʾð��  ������������ֿ��Ե�����˸Ƶ��
{
	
if(TIM_1%10>5)	{LCD_zimu(32,24,32,1,HZ16X32,11);	}	
if(TIM_1%10<5)	{LCD_zimu(32,24,32,1,HZ16X32,10);	}
}
	

void disptime1 (void)  //��ʾʱ���������1
{
    readdata();//ˢ������������
	dispmaohao();//��ʾð�� 
	Conversion(0,year,month,day);              //����ũ�����빫����ȥ�����ũ�� 
	
	LCD_zimu(0,21,32,1,HZ16X32,hour/10);
	LCD_zimu(16,21,32,1,HZ16X32,hour%10);
	
	LCD_zimu(48,21,32,1,HZ16X32,min/10);
	LCD_zimu(64,21,32,1,HZ16X32,min%10);
		

if(TIM_1%200<66)
	{
	  DrawRectangle(0,8,80,22,3); 		
	Show_Str(0,8,64,16,"20  -  -",16,1); 
	LCD_ShowNum(16,8,year,2,16,1);
	LCD_ShowNum(40,8,month,2,16,1);	
	LCD_ShowNum(64,8,day,2,16,1);	
		
	}
if(TIM_1%200>66&&TIM_1%200<132)
	{
	  DrawRectangle(0,8,80,22,3); 		
    Show_Str(0,10,24,12,"����",12,1);		
	   if(week==7)	{Show_Str(24,10,42,12,"�� ��Ϣ",12,1);}  //���� ��
  else if(week==6)	{Show_Str(24,10,42,12,"�� ��Ϣ",12,1);}
  else if(week==5)	{Show_Str(24,10,42,12,"�� �ϰ�",12,1);}
  else if(week==4)	{Show_Str(24,10,42,12,"�� �ϰ�",12,1);}
  else if(week==3)	{Show_Str(24,10,42,12,"�� �ϰ�",12,1);}
  else if(week==2)	{Show_Str(24,10,42,12,"�� �ϰ�",12,1);}
  else if(week==1)	{Show_Str(24,10,42,12,"һ �ϰ�",12,1);}			
	}	
if(TIM_1%200>132)
	{
	  DrawRectangle(0,8,80,22,3);              //�����ʾ����
    Show_Str(0,10,60,12,"ũ    ��",12,1);
    	if(month_moon==1)	  Show_Str(12,10,24,12,"  һ",12,1); 
else	if(month_moon==2)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==3)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==4)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==5)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==6)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==7)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==8)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==9)	  Show_Str(12,10,24,12,"  ��",12,1); 
else	if(month_moon==10)	  Show_Str(12,10,24,12,"  ʮ",12,1); 
else	if(month_moon==11)	  Show_Str(12,10,24,12,"ʮһ",12,1); 
else	if(month_moon==12)	  Show_Str(12,10,24,12,"ʮ��",12,1); 		

    	if(day_moon==1)	    Show_Str(48,10,24,12,"��һ",12,1); 
else	if(day_moon==2)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==3)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==4)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==5)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==6)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==7)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==8)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==9)	    Show_Str(48,10,24,12,"����",12,1); 
else	if(day_moon==10)	Show_Str(48,10,24,12,"��ʮ",12,1); 
else	if(day_moon==11)	Show_Str(48,10,24,12,"ʮһ",12,1); 
else	if(day_moon==12)	Show_Str(48,10,24,12,"ʮ��",12,1); 		
else	if(day_moon==13)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==14)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==15)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==16)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==17)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==18)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==19)	Show_Str(48,10,24,12,"ʮ��",12,1); 
else	if(day_moon==20)	Show_Str(48,10,24,12,"��ʮ",12,1); 
else	if(day_moon==21)	Show_Str(48,10,24,12,"إһ",12,1); 
else	if(day_moon==22)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==23)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==24)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==25)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==26)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==27)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==28)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==29)	Show_Str(48,10,24,12,"إ��",12,1); 
else	if(day_moon==30)	Show_Str(48,10,24,12,"��ʮ",12,1); 	
	}
	
   LCD_zimu(0,0,14,1,HZ16X8,CW2015_Soc(0)/7);	//��ʾ��ص���ͼ��   
   dht11disp();
		
  ShowAllGraphRAM();	
	
}

//=================================================================1.ʱ�����=============================================================//
void shijianset (void)
{ 
  u8 i=0;  //���������Ŀ��λ�ã���ǵ����ڼ���
  u8 temp = 0;
  u8 motemp,yytemp,daytemp;	
  main_ss();	//��ʼ���˵���ʾʱ��
	
	while(1)
{
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		
	
	  readdata();//ˢ������������
//===================================================================================���㲻ͬ�·ݵ�����=================================
		motemp=month/16*10+month%16;   //ת����10�����·�����
		yytemp=year/16*10+year%16;   //ת����10�����������
    if(motemp==2&&yytemp%4!=0){daytemp=28;}//ƽ��2��28��
		if(motemp==2&&yytemp%4==0){daytemp=29;}//����2��29��
		if(motemp==1||motemp==3||motemp==5||motemp==7||motemp==8||motemp==10||motemp==12){daytemp=31;}//31����·�
		if(motemp==4||motemp==6||motemp==9||motemp==11){daytemp=30;}//30����·�
	
		
		switch(keyflag)
		{	
   	case 4: 
    keyflag=0;		
   	main_ss();
  	CleanScreen();		
		i++;if(i==6)i=0; //============================================================���õ����Ĳ���������һ��6������������ѭ��
	  break;
	
    case 1:    //====================================================================�ӵ���========================================
    keyflag=0;
    main_ss();
  	CleanScreen();		
	     if(i==0)		{year++;		if(year>99)year=0;		    temp = HEX2BCD(year);     write_byte(0x06,temp);}    //�޸���
	else if(i==1)		{month++;		if(month>12)month=1;	    temp = HEX2BCD(month);    write_byte(0x05,temp);}    //�޸���			
	else if(i==2)		{day++;		  if(day>daytemp)day=1;	    temp = HEX2BCD(day);      write_byte(0x04,temp);}    //�޸���
	else if(i==3)		{hour++;		if(hour>23)hour=0;	      temp = HEX2BCD(hour);     write_byte(0x02,temp);}    //�޸�Сʱ		
	else if(i==4)		{min++;	  	if(min>59)min=0;	        temp = HEX2BCD(min);      write_byte(0x01,temp);}    //�޸ķ���	
	else if(i==5)		{sec++;	  	if(sec>59)sec=0;	        temp = HEX2BCD(sec);      write_byte(0x00,temp);}    //�޸�����			
    break;
		
	  case 2: //=====================================================================������=========================================
		//��Ϊ�޷������κ�0�Ƚ������壬year<0����д��year==255��  ��Χ��0~255�������ԭ��0000 0000�ټ�1��Ϊ1111 1111	
    keyflag=0;
    main_ss();
    CleanScreen();		
	     if(i==0)	  {year--;		if(year==255)year=99;     temp = HEX2BCD(year);     write_byte(0x06,temp);}     //�޸���				
	else if(i==1)	  {month--;	  if(month<1)month=12;		  temp = HEX2BCD(month);    write_byte(0x05,temp);}     //�޸���		
	else if(i==2)     {day--;     if(day<1)day=daytemp;		  temp = HEX2BCD(day);      write_byte(0x04,temp);}     //�޸���
	else if(i==3)   	{hour--;    if(hour==255)hour=23;	    temp = HEX2BCD(hour);     write_byte(0x02,temp);}     //�޸�Сʱ
	else if(i==4)		{min--;	  	if(min==255)min=59;	      temp = HEX2BCD(min);      write_byte(0x01,temp);}     //�޸ķ���	
	else if(i==5)		{sec--;	  	if(sec==255)sec=59;	      temp = HEX2BCD(sec);      write_byte(0x00,temp);}     //�޸�����				
    break;
		
		
	case 3:	
	keyflag=0;				
	return;		
	
  	default:
	  delay_ms(1);
	if(s_main==d_main) 	 	{	return;	}	
	
			}
	   CleanScreen();
	  Show_Str(0,0,128,16,"20  -  -  ����",16,0);
		Show_Str(0,32,128,16,"  :  :",16,0);  			
	  Show_Str(16,48,32,16,"����",16,0);				
    if(i==0)	 {LCD_ShowNum(16,0,year, 2,16,2); Show_Str(64,48,32,16,"���",16,0);}
    else       {LCD_ShowNum(16,0,year, 2,16,0);     }
    if(i==1)   {LCD_ShowNum(40,0,month,2,16,2);	Show_Str(64,48,32,16,"�·�",16,0);}
    else       {LCD_ShowNum(40,0,month,2,16,0);			}	    
    if(i==2)   {LCD_ShowNum(64,0,day,  2,16,2);	Show_Str(64,48,32,16,"����",16,0);}
    else       {LCD_ShowNum(64,0,day,  2,16,0);		  }   
		if(i==3)	 {LCD_ShowNum(0,32,hour, 2,16,2);	Show_Str(64,48,32,16,"Сʱ",16,0);}
    else       {LCD_ShowNum(0,32,hour, 2,16,0);			}
    if(i==4)	 {LCD_ShowNum(24,32,min, 2,16,2);	Show_Str(64,48,32,16,"����",16,0);}
		else       {LCD_ShowNum(24,32,min, 2,16,0);			}    
		if(i==5)   {LCD_ShowNum(48,32,sec, 2,16,2);	Show_Str(64,48,32,16,"����",16,0);} 
		else       {LCD_ShowNum(48,32,sec, 2,16,0);			}				   
       if(week==7)	{Show_Font(112,0,"��",16,1);}  //���� ��
  else if(week==6)	{Show_Font(112,0,"��",16,1);}
  else if(week==5)	{Show_Font(112,0,"��",16,1);}
  else if(week==4)	{Show_Font(112,0,"��",16,1);}
  else if(week==3)	{Show_Font(112,0,"��",16,1);}
  else if(week==2)	{Show_Font(112,0,"��",16,1);}
  else if(week==1)	{Show_Font(112,0,"һ",16,1);}	
  }
}

void blltset (void)  //������������
{			
	s8 i=0;     //���������Ŀ��λ�ã���ǵ����ڼ��� S ����С��0
  main_ss();	//��ʼ���˵���ʾʱ��
  i =AT24CXX_ReadOneByte(9);   	
	while(1)
{
  	main_return();
  	menu_time_lcm();
  	if(s_main==d_main) 	 	{	break;		}		
	
	  Show_Str(16,0,96,16,"���⿪�ؿ���",16,0);	
	  Show_Str(16,24,48,16,"��ǰֵ",16,0);	
	  Show_Str(80,24,12,12,"��",12,1); 
	  LCD_ShowNum(100,24,i,2,16,1);//X,Y�������,��ʾ��ֵ,��ֵλ���������С 
	  ShowAllGraphRAM();	
			
		switch(keyflag)
		{	
	case 4: 
	keyflag=0;
   	main_ss();		
	AT24CXX_WriteOneByte(9,i);
	ShowMsgSimple("�����ѱ���",600);
    CleanScreen();		
	  break;
	
  case 1:    //====================================================================�ӵ���========================================
	keyflag=0;
  main_ss();	
		i++;
   if(i>1)i=0;
    break;
		
	case 2:   //=====================================================================������=========================================
	keyflag=0;
   	main_ss();	
		i--;
	if(i<0)i=1;
    break;

	case 3:	
	keyflag=0;				
	return;		
  
   	default:
	  delay_ms(1);
			}					 
  }
}
void bltimeset (void)  //����ʱ������0һֱ��
{			
	u8 i=0;  //��������ֵ	
  main_ss();	//��ʼ���˵���ʾʱ��
	i =AT24CXX_ReadOneByte(2);                  
	
	while(1)
{
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		

	  Show_Str(16,0,96,16,"����ʱ�����",16,0);//	X,Y�������,����X,Y,�����С,����,ģʽ
	  Show_Str(16,24,48,16,"��ǰֵ",16,0);	  	
  	  LCD_ShowNum(80,24,i,2,16,1);//X,Y�������,��ʾ��ֵ,��ֵλ���������С  
	  ShowAllGraphRAM();
	
		switch(keyflag)
		{	
	case 4: 
	keyflag=0; 
  	main_ss();
	  AT24CXX_WriteOneByte(2,i);
	  ShowMsgSimple("�����ѱ���",600);
    CleanScreen();	
	  break;
	
	case 1:         //=====================================================================�ӵ���=========================================
	keyflag=0;
   	main_ss();	
		i++;
	if(i>60)i=0;    //============================================================	
    break;
	
  case 2:         //====================================================================������========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==255)i=60; //============================================================С��0����д��==255��ñ���
    break;
		
	case 3:	
	keyflag=0;				
	return;		

  	default:
	  delay_ms(1);
			}					 
  }
}
void crset (void)  //�Աȶ�����
{			
  u8 i;
  main_ss();	//��ʼ���˵���ʾʱ��
	
	while(1)
{

	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}			
	
		i =AT24CXX_ReadOneByte(4);     
	  Show_Str(24,0,96,16,"�Աȶȵ���",16,0);//	X,Y�������,����X,Y,�����С,����,ģʽ
		Show_Str(16,24,48,16,"��ǰֵ",16,0);	
  	
  	LCD_ShowNum(80,24,i,2,16,1);//X,Y�������,��ʾ��ֵ,��ֵλ���������С 
 
	  ShowAllGraphRAM();
	
	  St7565_WriteCmd(0x81);      //��18���� �Աȶ� 6λ��ѹֵ���ݣ�0��63 �� 64 ��		
	  St7565_WriteCmd(i);         //��Χ0x00-0x3f ֵԽ��ԽŨ����һ��ָ��ͬʱ��0x81����		
	

		switch(keyflag)
		{	
	case 4: 
	keyflag=0;
   	main_ss();		
	  ShowMsgSimple("�����Զ�����",600);
	  CleanScreen();	
	  break;
	
	case 1: //=====================================================================�ӵ���=========================================
	keyflag=0;
   	main_ss();	
		i++;
	if(i==61)i=25; //============================================================
	 AT24CXX_WriteOneByte(4,i);	
    break;
	
  case 2:    //====================================================================������========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==24)i =60; //============================================================
		AT24CXX_WriteOneByte(4,i);
    break;
		
	case 3:	
	keyflag=0;				
	return;		

	
  	default:
	  delay_ms(1);
			}					 
  }
}
void menutimeset (void)  //�˵�ʱ������
{			
	u8 i=0;  //��������ֵ	
  main_ss();	//��ʼ���˵���ʾʱ��
	i =AT24CXX_ReadOneByte(5); 	
	while(1)
  {
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		

	Show_Str(16,0,96,16,"�˵�ʱ�����",16,0);//	X,Y�������,����X,Y,�����С,����,ģʽ
	Show_Str(16,24,48,16,"��ǰֵ",16,0);	 	
  	LCD_ShowNum(80,24,i,2,16,0);//X,Y�������,��ʾ��ֵ,��ֵλ���������С  
	ShowAllGraphRAM();
	
		switch(keyflag)
		{	
		case 4: 
		keyflag=0;
		main_ss();
		AT24CXX_WriteOneByte(5,i);
		ShowMsgSimple("�����ѱ���",600);
		CleanScreen();	
		break;
		
		case 1:    //====================================================================�ӵ���========================================
		keyflag=0;
		main_ss();	
		i--;
		if(i==4)i=59; //============================================================
		break;
			
		case 2: //=====================================================================������=========================================
		keyflag=0;
		main_ss();	
		i++;
		if(i==60)i=5; //============================================================	
		break;
		
		case 3:	
		keyflag=0;				
		return;		

		default:
		delay_ms(1);
			}					 
  }
}
void dingshiset1 (void)  //������һ�������ö�ʱ����
{			
	u8 i=0;  //��������ֵ	
  main_ss();	//��ʼ���˵���ʾʱ��
	i =AT24CXX_ReadOneByte(8); 	
	while(1)
  {
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		

	Show_Str(16,0,96,16,"����ʱ�����",16,0);//	X,Y�������,����X,Y,�����С,����,ģʽ

     if(i==0){Show_Str(16,24,72,12,"����һֱ����",12,1);} 
else if(i==1){Show_Str(16,24,72,12,"ÿ60���ӱ任",12,1);}  
else if(i==2){Show_Str(16,24,72,12,"ÿ30���ӱ任",12,1);}  
else if(i==3){Show_Str(16,24,72,12,"ÿ10���ӱ任",12,1);}  
else if(i==4){Show_Str(16,24,72,12,"ÿ02���ӱ任",12,1);}  
else if(i==5){Show_Str(16,24,72,12,"ÿ01���ӱ任",12,1);} 
else if(i==6){Show_Str(16,24,72,12,"ÿ��6-18����",12,1);} 
	ShowAllGraphRAM();
	
		switch(keyflag)
		{	
		case 4: 
		keyflag=0;
		main_ss();
		AT24CXX_WriteOneByte(8,i);
		ShowMsgSimple("�����ѱ���",600);
		CleanScreen();	
		break;
		
		case 1:    //====================================================================�ӵ���========================================
		keyflag=0;
		main_ss();	
		i--;
		if(i==255)i=6; //============================================================
		break;
			
		case 2: //=====================================================================������=========================================
		keyflag=0;
		main_ss();	
		i++;
		if(i>6)i=0; //============================================================	
		break;
		
		case 3:	
		keyflag=0;				
		return;		

		default:
		delay_ms(1);
			}					 
  }
}
void ina226disp(void)
{

	u16 i=0,a,b;  //��������ֵ	
  main_ss();	//��ʼ���˵���ʾʱ��
	
		printf("��ѹ=%d\r\n",INA226_GetVoltage(INA226_ADDR1));
		printf("����=%d\r\n",INA226_GetCurrent(INA226_ADDR1));	
	
	while(1)
{
	main_return();
	menu_time_lcm();
  if(s_main==d_main) 	 	{	break;		}		

	  Show_Str(16,0,96,16,"INA226",16,0);//	X,Y�������,����X,Y,�����С,����,ģʽ
	  Show_Str(16,24,48,12,"��ص�ѹ",12,1);	  	


	Show_Str(112,28,24,12,"mV",12,1);
	LCD_ShowNum(80,28,INA226_GetVoltage(INA226_ADDR1),4,12,1);
		
	Show_Str(112,40,24,12,"mA",12,1);		
  if(INA226_GetCurrent(INA226_ADDR1)<0x8000)		
    {
    a=(INA226_GetCurrent(INA226_ADDR1))/2/10;		
	LCD_ShowNum(80,40,a,4,12,1);		
    Show_Str(16,38,48,12,"�ŵ����",12,1);				
    }
else{ 
    b=(65536-INA226_GetCurrent(INA226_ADDR1)-5)/2/10;	
	LCD_ShowNum(80,40,b,4,12,1);
    Show_Str(16,38,48,12,"������",12,1);	
    }
	ShowAllGraphRAM();
	
		switch(keyflag)
		{	
	case 4: 
	keyflag=0; 
  	main_ss();

    CleanScreen();	
	  break;
	
  case 1:         //====================================================================�ӵ���========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==255)i=60; //============================================================С��0����д��==255��ñ���
    break;
		
	case 2:         //=====================================================================������=========================================
	keyflag=0;
   	main_ss();	
		i++;
	if(i>60)i=0;    //============================================================	
    break;

	case 3:	
	keyflag=0;				
	return;		

  	default:
	  delay_ms(1);
		if(s_main==d_main) 	 	{	return;	}			
			}					 
  }		
}
	 
void cw2015disp(void)
{
	u8 i=0;  //
  main_ss();	//��ʼ���˵���ʾʱ��
	
		printf("CW2015 ID:%d\r\n",CW2015_ID());
		printf("�����ٷ�֮%d\r\n",CW2015_Soc(0));	
		printf("��ص�ѹ:%d\r\n\r\n",CW2015_Vcell());
		printf("ʹ��ʱ��:%d\r\n\r\n", CW2015_Time());	
	
		
	while(1)
{
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}			

	  Show_Str(16,0,96,16,"CW2015",16,0);//	X,Y�������,����X,Y,�����С,����,ģʽ	
//  	  LCD_ShowNum(80,24,i,2,16,0);//X,Y�������,��ʾ��ֵ,��ֵλ���������С  

	Show_Str(0,16,36,12,"ID��",12,0);
	Show_Str(0,28,48,12,"SOC:",12,0);
	Show_Str(0,40,48,12,"Vce:",12,0);
	Show_Str(0,52,48,12,"Tim:",12,0);
	
	LCD_ShowNum(40,16,CW2015_ID(),4,12,1);
	LCD_ShowNum(40,28,CW2015_Soc(0),4,12,1);		
	LCD_ShowNum(40,40,CW2015_Vcell(),4,12,1);	
	LCD_ShowNum(40,53,CW2015_Time(),4,12,1);		
	
	  ShowAllGraphRAM();
	
		switch(keyflag)
		{	
	case 4: 
	keyflag=0; 
  	main_ss();

    CleanScreen();	
	  break;
	
  case 1:         //====================================================================�ӵ���========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==255)i=60; //============================================================С��0����д��==255��ñ���
    break;
		
	case 2:         //=====================================================================������=========================================
	keyflag=0;
   	main_ss();	
		i++;
	if(i>60)i=0;    //============================================================	
    break;

	case 3:	
	keyflag=0;				
	return;		

  	default:
	  delay_ms(1);
		if(s_main==d_main) 	 	{	return;	}	
			}					 
  }	

  }	


