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

extern u8 keyflag;//按键中断返回值


extern u16 TIM_1,TIM_2,TIM_3;//定时器产生的3个不同频率的标志
extern u8 year,month,day,hour,min,sec,week;
extern u8 year_moon,month_moon,day_moon;//外部引用农历


extern u8 rst;    //24C02和时钟复位标志


u8 s_main,d_main,M_time;
u8 temperature,humidity;  	  

/*****************************************************************************/
void main_ss()  //自动返回主界面计时  在调用时需要初始化
{
	u8 i,j; 
	j =AT24CXX_ReadOneByte(5);   
	i = read_random(0x00);	
	s_main=(i/16*10+i%16)+j;	
	if(s_main>=60){s_main=s_main%60;}
}
void main_return()    //读取当前的秒钟具体值
{
	u8	i;
	i = read_random(0x00);
	d_main=(i/16*10+i%16);
}
void menu_time_lcm() //显示菜单时间
{	
  M_time = s_main;
  if(s_main<d_main) M_time =s_main+60;
  M_time = M_time-d_main;
  LCD_ShowNum(0,52,M_time,2,12,1);//X,Y起点坐标,显示数值,数值位数，字体大小 
	ShowAllGraphRAM();	
}

void getdata(void)                           //获取需要显示和使用的信息
{
	if(TIM_1%20==1)    //除以X的余数等于Y
  { 
	DHT11_Read_Data(&temperature,&humidity);	//读取温湿度值	
//	printf("温度值=%d\r\n",temperature);	
//	printf("湿度=%d\r\n",humidity);	 
    LED0=!LED0;	
    LCDBK=AT24CXX_ReadOneByte(9); 	  
  } 

}

void dht11disp(void)
{

	Show_Str(112,28,12,12,"℃",12,0);
	Show_Str(112,40,12,12,"%",12,0);	
	LCD_ShowNum(100,28,temperature,2,12,1);
	LCD_ShowNum(100,40,humidity,2,12,1);
  ShowAllGraphRAM();	

}	



void dispmaohao (void)  //显示冒号  调整后面的数字可以调节闪烁频率
{
	
if(TIM_1%10>5)	{LCD_zimu(32,24,32,1,HZ16X32,11);	}	
if(TIM_1%10<5)	{LCD_zimu(32,24,32,1,HZ16X32,10);	}
}
	

void disptime1 (void)  //显示时间的主界面1
{
    readdata();//刷新年月日星期
	dispmaohao();//显示冒号 
	Conversion(0,year,month,day);              //更新农历输入公历进去换算出农历 
	
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
    Show_Str(0,10,24,12,"星期",12,1);		
	   if(week==7)	{Show_Str(24,10,42,12,"日 休息",12,1);}  //汉字 六
  else if(week==6)	{Show_Str(24,10,42,12,"六 休息",12,1);}
  else if(week==5)	{Show_Str(24,10,42,12,"五 上班",12,1);}
  else if(week==4)	{Show_Str(24,10,42,12,"四 上班",12,1);}
  else if(week==3)	{Show_Str(24,10,42,12,"三 上班",12,1);}
  else if(week==2)	{Show_Str(24,10,42,12,"二 上班",12,1);}
  else if(week==1)	{Show_Str(24,10,42,12,"一 上班",12,1);}			
	}	
if(TIM_1%200>132)
	{
	  DrawRectangle(0,8,80,22,3);              //清空显示区域
    Show_Str(0,10,60,12,"农    月",12,1);
    	if(month_moon==1)	  Show_Str(12,10,24,12,"  一",12,1); 
else	if(month_moon==2)	  Show_Str(12,10,24,12,"  二",12,1); 
else	if(month_moon==3)	  Show_Str(12,10,24,12,"  三",12,1); 
else	if(month_moon==4)	  Show_Str(12,10,24,12,"  四",12,1); 
else	if(month_moon==5)	  Show_Str(12,10,24,12,"  五",12,1); 
else	if(month_moon==6)	  Show_Str(12,10,24,12,"  六",12,1); 
else	if(month_moon==7)	  Show_Str(12,10,24,12,"  七",12,1); 
else	if(month_moon==8)	  Show_Str(12,10,24,12,"  八",12,1); 
else	if(month_moon==9)	  Show_Str(12,10,24,12,"  九",12,1); 
else	if(month_moon==10)	  Show_Str(12,10,24,12,"  十",12,1); 
else	if(month_moon==11)	  Show_Str(12,10,24,12,"十一",12,1); 
else	if(month_moon==12)	  Show_Str(12,10,24,12,"十二",12,1); 		

    	if(day_moon==1)	    Show_Str(48,10,24,12,"初一",12,1); 
else	if(day_moon==2)	    Show_Str(48,10,24,12,"初二",12,1); 
else	if(day_moon==3)	    Show_Str(48,10,24,12,"初三",12,1); 
else	if(day_moon==4)	    Show_Str(48,10,24,12,"初四",12,1); 
else	if(day_moon==5)	    Show_Str(48,10,24,12,"初五",12,1); 
else	if(day_moon==6)	    Show_Str(48,10,24,12,"初六",12,1); 
else	if(day_moon==7)	    Show_Str(48,10,24,12,"初七",12,1); 
else	if(day_moon==8)	    Show_Str(48,10,24,12,"初八",12,1); 
else	if(day_moon==9)	    Show_Str(48,10,24,12,"初九",12,1); 
else	if(day_moon==10)	Show_Str(48,10,24,12,"初十",12,1); 
else	if(day_moon==11)	Show_Str(48,10,24,12,"十一",12,1); 
else	if(day_moon==12)	Show_Str(48,10,24,12,"十二",12,1); 		
else	if(day_moon==13)	Show_Str(48,10,24,12,"十三",12,1); 
else	if(day_moon==14)	Show_Str(48,10,24,12,"十四",12,1); 
else	if(day_moon==15)	Show_Str(48,10,24,12,"十五",12,1); 
else	if(day_moon==16)	Show_Str(48,10,24,12,"十六",12,1); 
else	if(day_moon==17)	Show_Str(48,10,24,12,"十七",12,1); 
else	if(day_moon==18)	Show_Str(48,10,24,12,"十八",12,1); 
else	if(day_moon==19)	Show_Str(48,10,24,12,"十九",12,1); 
else	if(day_moon==20)	Show_Str(48,10,24,12,"二十",12,1); 
else	if(day_moon==21)	Show_Str(48,10,24,12,"廿一",12,1); 
else	if(day_moon==22)	Show_Str(48,10,24,12,"廿二",12,1); 
else	if(day_moon==23)	Show_Str(48,10,24,12,"廿三",12,1); 
else	if(day_moon==24)	Show_Str(48,10,24,12,"廿四",12,1); 
else	if(day_moon==25)	Show_Str(48,10,24,12,"廿五",12,1); 
else	if(day_moon==26)	Show_Str(48,10,24,12,"廿六",12,1); 
else	if(day_moon==27)	Show_Str(48,10,24,12,"廿七",12,1); 
else	if(day_moon==28)	Show_Str(48,10,24,12,"廿八",12,1); 
else	if(day_moon==29)	Show_Str(48,10,24,12,"廿九",12,1); 
else	if(day_moon==30)	Show_Str(48,10,24,12,"三十",12,1); 	
	}
	
   LCD_zimu(0,0,14,1,HZ16X8,CW2015_Soc(0)/7);	//显示电池电量图标   
   dht11disp();
		
  ShowAllGraphRAM();	
	
}

//=================================================================1.时间调节=============================================================//
void shijianset (void)
{ 
  u8 i=0;  //标记设置项目的位置，标记调整第几个
  u8 temp = 0;
  u8 motemp,yytemp,daytemp;	
  main_ss();	//初始化菜单显示时间
	
	while(1)
{
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		
	
	  readdata();//刷新年月日星期
//===================================================================================计算不同月份的天数=================================
		motemp=month/16*10+month%16;   //转换成10进制月份数据
		yytemp=year/16*10+year%16;   //转换成10进制年份数据
    if(motemp==2&&yytemp%4!=0){daytemp=28;}//平年2月28天
		if(motemp==2&&yytemp%4==0){daytemp=29;}//闰年2月29天
		if(motemp==1||motemp==3||motemp==5||motemp==7||motemp==8||motemp==10||motemp==12){daytemp=31;}//31天的月份
		if(motemp==4||motemp==6||motemp==9||motemp==11){daytemp=30;}//30天的月份
	
		
		switch(keyflag)
		{	
   	case 4: 
    keyflag=0;		
   	main_ss();
  	CleanScreen();		
		i++;if(i==6)i=0; //============================================================设置调整的参数个数，一共6个参数，进行循环
	  break;
	
    case 1:    //====================================================================加调整========================================
    keyflag=0;
    main_ss();
  	CleanScreen();		
	     if(i==0)		{year++;		if(year>99)year=0;		    temp = HEX2BCD(year);     write_byte(0x06,temp);}    //修改年
	else if(i==1)		{month++;		if(month>12)month=1;	    temp = HEX2BCD(month);    write_byte(0x05,temp);}    //修改月			
	else if(i==2)		{day++;		  if(day>daytemp)day=1;	    temp = HEX2BCD(day);      write_byte(0x04,temp);}    //修改日
	else if(i==3)		{hour++;		if(hour>23)hour=0;	      temp = HEX2BCD(hour);     write_byte(0x02,temp);}    //修改小时		
	else if(i==4)		{min++;	  	if(min>59)min=0;	        temp = HEX2BCD(min);      write_byte(0x01,temp);}    //修改分钟	
	else if(i==5)		{sec++;	  	if(sec>59)sec=0;	        temp = HEX2BCD(sec);      write_byte(0x00,temp);}    //修改秒钟			
    break;
		
	  case 2: //=====================================================================减调整=========================================
		//因为无符号整形和0比较无意义，year<0可以写成year==255，  范围是0~255，补码的原因0000 0000再减1变为1111 1111	
    keyflag=0;
    main_ss();
    CleanScreen();		
	     if(i==0)	  {year--;		if(year==255)year=99;     temp = HEX2BCD(year);     write_byte(0x06,temp);}     //修改年				
	else if(i==1)	  {month--;	  if(month<1)month=12;		  temp = HEX2BCD(month);    write_byte(0x05,temp);}     //修改月		
	else if(i==2)     {day--;     if(day<1)day=daytemp;		  temp = HEX2BCD(day);      write_byte(0x04,temp);}     //修改日
	else if(i==3)   	{hour--;    if(hour==255)hour=23;	    temp = HEX2BCD(hour);     write_byte(0x02,temp);}     //修改小时
	else if(i==4)		{min--;	  	if(min==255)min=59;	      temp = HEX2BCD(min);      write_byte(0x01,temp);}     //修改分钟	
	else if(i==5)		{sec--;	  	if(sec==255)sec=59;	      temp = HEX2BCD(sec);      write_byte(0x00,temp);}     //修改秒钟				
    break;
		
		
	case 3:	
	keyflag=0;				
	return;		
	
  	default:
	  delay_ms(1);
	if(s_main==d_main) 	 	{	return;	}	
	
			}
	   CleanScreen();
	  Show_Str(0,0,128,16,"20  -  -  星期",16,0);
		Show_Str(0,32,128,16,"  :  :",16,0);  			
	  Show_Str(16,48,32,16,"调整",16,0);				
    if(i==0)	 {LCD_ShowNum(16,0,year, 2,16,2); Show_Str(64,48,32,16,"年份",16,0);}
    else       {LCD_ShowNum(16,0,year, 2,16,0);     }
    if(i==1)   {LCD_ShowNum(40,0,month,2,16,2);	Show_Str(64,48,32,16,"月份",16,0);}
    else       {LCD_ShowNum(40,0,month,2,16,0);			}	    
    if(i==2)   {LCD_ShowNum(64,0,day,  2,16,2);	Show_Str(64,48,32,16,"日期",16,0);}
    else       {LCD_ShowNum(64,0,day,  2,16,0);		  }   
		if(i==3)	 {LCD_ShowNum(0,32,hour, 2,16,2);	Show_Str(64,48,32,16,"小时",16,0);}
    else       {LCD_ShowNum(0,32,hour, 2,16,0);			}
    if(i==4)	 {LCD_ShowNum(24,32,min, 2,16,2);	Show_Str(64,48,32,16,"分钟",16,0);}
		else       {LCD_ShowNum(24,32,min, 2,16,0);			}    
		if(i==5)   {LCD_ShowNum(48,32,sec, 2,16,2);	Show_Str(64,48,32,16,"秒钟",16,0);} 
		else       {LCD_ShowNum(48,32,sec, 2,16,0);			}				   
       if(week==7)	{Show_Font(112,0,"日",16,1);}  //汉字 日
  else if(week==6)	{Show_Font(112,0,"六",16,1);}
  else if(week==5)	{Show_Font(112,0,"五",16,1);}
  else if(week==4)	{Show_Font(112,0,"四",16,1);}
  else if(week==3)	{Show_Font(112,0,"三",16,1);}
  else if(week==2)	{Show_Font(112,0,"二",16,1);}
  else if(week==1)	{Show_Font(112,0,"一",16,1);}	
  }
}

void blltset (void)  //背光亮度设置
{			
	s8 i=0;     //标记设置项目的位置，标记调整第几个 S 可以小于0
  main_ss();	//初始化菜单显示时间
  i =AT24CXX_ReadOneByte(9);   	
	while(1)
{
  	main_return();
  	menu_time_lcm();
  	if(s_main==d_main) 	 	{	break;		}		
	
	  Show_Str(16,0,96,16,"背光开关控制",16,0);	
	  Show_Str(16,24,48,16,"当前值",16,0);	
	  Show_Str(80,24,12,12,"开",12,1); 
	  LCD_ShowNum(100,24,i,2,16,1);//X,Y起点坐标,显示数值,数值位数，字体大小 
	  ShowAllGraphRAM();	
			
		switch(keyflag)
		{	
	case 4: 
	keyflag=0;
   	main_ss();		
	AT24CXX_WriteOneByte(9,i);
	ShowMsgSimple("设置已保存",600);
    CleanScreen();		
	  break;
	
  case 1:    //====================================================================加调整========================================
	keyflag=0;
  main_ss();	
		i++;
   if(i>1)i=0;
    break;
		
	case 2:   //=====================================================================减调整=========================================
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
void bltimeset (void)  //背光时间设置0一直亮
{			
	u8 i=0;  //按键返回值	
  main_ss();	//初始化菜单显示时间
	i =AT24CXX_ReadOneByte(2);                  
	
	while(1)
{
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		

	  Show_Str(16,0,96,16,"背光时间调节",16,0);//	X,Y起点坐标,区域X,Y,字体大小,内容,模式
	  Show_Str(16,24,48,16,"当前值",16,0);	  	
  	  LCD_ShowNum(80,24,i,2,16,1);//X,Y起点坐标,显示数值,数值位数，字体大小  
	  ShowAllGraphRAM();
	
		switch(keyflag)
		{	
	case 4: 
	keyflag=0; 
  	main_ss();
	  AT24CXX_WriteOneByte(2,i);
	  ShowMsgSimple("设置已保存",600);
    CleanScreen();	
	  break;
	
	case 1:         //=====================================================================加调整=========================================
	keyflag=0;
   	main_ss();	
		i++;
	if(i>60)i=0;    //============================================================	
    break;
	
  case 2:         //====================================================================减调整========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==255)i=60; //============================================================小于0可以写成==255免得报错
    break;
		
	case 3:	
	keyflag=0;				
	return;		

  	default:
	  delay_ms(1);
			}					 
  }
}
void crset (void)  //对比度设置
{			
  u8 i;
  main_ss();	//初始化菜单显示时间
	
	while(1)
{

	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}			
	
		i =AT24CXX_ReadOneByte(4);     
	  Show_Str(24,0,96,16,"对比度调节",16,0);//	X,Y起点坐标,区域X,Y,字体大小,内容,模式
		Show_Str(16,24,48,16,"当前值",16,0);	
  	
  	LCD_ShowNum(80,24,i,2,16,1);//X,Y起点坐标,显示数值,数值位数，字体大小 
 
	  ShowAllGraphRAM();
	
	  St7565_WriteCmd(0x81);      //第18命令 对比度 6位电压值数据，0～63 共 64 级		
	  St7565_WriteCmd(i);         //范围0x00-0x3f 值越大越浓和上一条指令同时用0x81不动		
	

		switch(keyflag)
		{	
	case 4: 
	keyflag=0;
   	main_ss();		
	  ShowMsgSimple("设置自动保存",600);
	  CleanScreen();	
	  break;
	
	case 1: //=====================================================================加调整=========================================
	keyflag=0;
   	main_ss();	
		i++;
	if(i==61)i=25; //============================================================
	 AT24CXX_WriteOneByte(4,i);	
    break;
	
  case 2:    //====================================================================减调整========================================
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
void menutimeset (void)  //菜单时间设置
{			
	u8 i=0;  //按键返回值	
  main_ss();	//初始化菜单显示时间
	i =AT24CXX_ReadOneByte(5); 	
	while(1)
  {
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		

	Show_Str(16,0,96,16,"菜单时间调节",16,0);//	X,Y起点坐标,区域X,Y,字体大小,内容,模式
	Show_Str(16,24,48,16,"当前值",16,0);	 	
  	LCD_ShowNum(80,24,i,2,16,0);//X,Y起点坐标,显示数值,数值位数，字体大小  
	ShowAllGraphRAM();
	
		switch(keyflag)
		{	
		case 4: 
		keyflag=0;
		main_ss();
		AT24CXX_WriteOneByte(5,i);
		ShowMsgSimple("设置已保存",600);
		CleanScreen();	
		break;
		
		case 1:    //====================================================================加调整========================================
		keyflag=0;
		main_ss();	
		i--;
		if(i==4)i=59; //============================================================
		break;
			
		case 2: //=====================================================================减调整=========================================
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
void dingshiset1 (void)  //开启第一个增氧泵定时设置
{			
	u8 i=0;  //按键返回值	
  main_ss();	//初始化菜单显示时间
	i =AT24CXX_ReadOneByte(8); 	
	while(1)
  {
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}		

	Show_Str(16,0,96,16,"开启时间调节",16,0);//	X,Y起点坐标,区域X,Y,字体大小,内容,模式

     if(i==0){Show_Str(16,24,72,12,"开关一直开启",12,1);} 
else if(i==1){Show_Str(16,24,72,12,"每60分钟变换",12,1);}  
else if(i==2){Show_Str(16,24,72,12,"每30分钟变换",12,1);}  
else if(i==3){Show_Str(16,24,72,12,"每10分钟变换",12,1);}  
else if(i==4){Show_Str(16,24,72,12,"每02分钟变换",12,1);}  
else if(i==5){Show_Str(16,24,72,12,"每01分钟变换",12,1);} 
else if(i==6){Show_Str(16,24,72,12,"每日6-18开启",12,1);} 
	ShowAllGraphRAM();
	
		switch(keyflag)
		{	
		case 4: 
		keyflag=0;
		main_ss();
		AT24CXX_WriteOneByte(8,i);
		ShowMsgSimple("设置已保存",600);
		CleanScreen();	
		break;
		
		case 1:    //====================================================================加调整========================================
		keyflag=0;
		main_ss();	
		i--;
		if(i==255)i=6; //============================================================
		break;
			
		case 2: //=====================================================================减调整=========================================
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

	u16 i=0,a,b;  //按键返回值	
  main_ss();	//初始化菜单显示时间
	
		printf("电压=%d\r\n",INA226_GetVoltage(INA226_ADDR1));
		printf("电流=%d\r\n",INA226_GetCurrent(INA226_ADDR1));	
	
	while(1)
{
	main_return();
	menu_time_lcm();
  if(s_main==d_main) 	 	{	break;		}		

	  Show_Str(16,0,96,16,"INA226",16,0);//	X,Y起点坐标,区域X,Y,字体大小,内容,模式
	  Show_Str(16,24,48,12,"电池电压",12,1);	  	


	Show_Str(112,28,24,12,"mV",12,1);
	LCD_ShowNum(80,28,INA226_GetVoltage(INA226_ADDR1),4,12,1);
		
	Show_Str(112,40,24,12,"mA",12,1);		
  if(INA226_GetCurrent(INA226_ADDR1)<0x8000)		
    {
    a=(INA226_GetCurrent(INA226_ADDR1))/2/10;		
	LCD_ShowNum(80,40,a,4,12,1);		
    Show_Str(16,38,48,12,"放电电流",12,1);				
    }
else{ 
    b=(65536-INA226_GetCurrent(INA226_ADDR1)-5)/2/10;	
	LCD_ShowNum(80,40,b,4,12,1);
    Show_Str(16,38,48,12,"充电电流",12,1);	
    }
	ShowAllGraphRAM();
	
		switch(keyflag)
		{	
	case 4: 
	keyflag=0; 
  	main_ss();

    CleanScreen();	
	  break;
	
  case 1:         //====================================================================加调整========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==255)i=60; //============================================================小于0可以写成==255免得报错
    break;
		
	case 2:         //=====================================================================减调整=========================================
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
  main_ss();	//初始化菜单显示时间
	
		printf("CW2015 ID:%d\r\n",CW2015_ID());
		printf("电量百分之%d\r\n",CW2015_Soc(0));	
		printf("电池电压:%d\r\n\r\n",CW2015_Vcell());
		printf("使用时间:%d\r\n\r\n", CW2015_Time());	
	
		
	while(1)
{
	main_return();
	menu_time_lcm();
	if(s_main==d_main) 	 	{	break;		}			

	  Show_Str(16,0,96,16,"CW2015",16,0);//	X,Y起点坐标,区域X,Y,字体大小,内容,模式	
//  	  LCD_ShowNum(80,24,i,2,16,0);//X,Y起点坐标,显示数值,数值位数，字体大小  

	Show_Str(0,16,36,12,"ID：",12,0);
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
	
  case 1:         //====================================================================加调整========================================
	keyflag=0;
  main_ss();	
		i--;
	if(i==255)i=60; //============================================================小于0可以写成==255免得报错
    break;
		
	case 2:         //=====================================================================减调整=========================================
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


