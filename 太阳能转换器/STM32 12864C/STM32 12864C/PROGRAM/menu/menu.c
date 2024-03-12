#include "menu.h"
#include <ds3231.h> 
#include "myiic.h"
#include "w25qxx.h" 
#include "st7565.h"
#include "zimu.h"
#include "key.h"
#include "clock.h"
#include "24c02.h"
#include "timer.h"
#include "text.h"	
#include "usart.h"
#include "delay.h"
#include "exti.h"


extern u8 keyflag;

u8 func_index;

void (*current_operation_index)(void);//

typedef struct
{
	u8 current;   //当前状态索引号
	u8 up;        //向上按键
	u8 down;	    //向下按键
	u8 enter;     //确定按键
	u8 exit;      //退出按键
	void (*current_operation)(void); //当前状态应该执行的操作
}
Menu_table;
//=============结构体说明===================//
//       {0,8,8,1,1,(*fun0)},      第一个0-----函数的索引号即用户想要执行哪个函数的代号
//                                 后面是代表4个按键，按之前的函数的顺序排列
//                                 (*fun0)为当前显示的界面
//其中，x,y,z,a是未知数，先不确定，他们对应的是四个键按下要指示的索引值，如果三个键，就三个未知数，在确定他们之前，必须要了解自己函数执行什么命令。
//加入开始时时执行数组里面的第一个即table[0]，而想在此按上键（第一个按键）执行函数fun6，那么table[0]里面需要这样设置
//｛0，5，y,z,a,(*fun1)｝,
//同样，如果希望按下键执行fun7则需要设置为
//｛0，x，6,z,a,(*fun1)｝，
//如果希望按确认键执行fun3则需要设置为
//｛0，x，y,2,a,(*fun1)｝；
//如果希望按退出键执行fun4则需要设置为
//｛0，x，y,z,3,(*fun1)｝；
//如果上面三种情况都想要就设置为
//｛0，5，6,2,3,(*fun1)｝.其它数组元素一次类推。

Menu_table  table[79]=//79
{
       //   索引  上，  下，  确定，   退出
			{0,   0,    0,    1,       0,(*menu11)},//     主页面
			
			{1,   6,    2,    7,       0,(*menu20)},// 第二级	功能界面	选中1调时
			{2,   1,    3,   13,       0,(*menu20)},// 第二级	功能界面	选中2调参					
			{3,   2,    4,   19,       0,(*menu20)},// 第二级 功能界面	选中3 
			{4,   3,    5,   25,       0,(*menu20)},// 第二级	功能界面	选中4					
			{5,   4,    6,   31,       0,(*menu20)},// 第二级	功能界面	选中5
			{6,   5,    1,   37,       0,(*menu20)},// 第二级	功能界面	选中6系统
			
			{7,   9,    8,   43,       1,(*menu21)},// 第三级   选项1  时间设置界面  选中1手动调节				
			{8,   7,    9,   44,       1,(*menu21)},// 第三级   选项1  时间设置界面	 选中2自动更新
			{9,   8,    7,   45,       1,(*menu21)},// 第三级	  选项1  时间设置界面  选中3远程更新
			{10,  8,    7,   46,       1,(*menu21)},
			{11,  8,    7,   47,       1,(*menu21)},
			{12,  8,    7,   48,       1,(*menu21)},			
			
			{13, 18,   14,   49,       2,(*menu22)},// 第三级	选项2  参数设置界面  选中1背光亮度                       
			{14, 13,   15,   50,       2,(*menu22)},// 第三级	选项2  参数设置界面  选中2背光时间   
			{15, 14,   16,   51,       2,(*menu22)},// 第三级	选项2  参数设置界面  选中3对比度调节                     
			{16, 15,   17,   52,       2,(*menu22)},//
			{17, 16,   18,   53,       2,(*menu22)},//				
			{18, 17,   13,   54,       2,(*menu22)},//
						
			{19, 24,   20,   55,       3,(*menu23)},//
			{20, 19,   21,   56,       3,(*menu23)},//			
			{21, 20,   22,   57,       3,(*menu23)},//
			{22, 21,   23,   58,       3,(*menu23)},//	
			{23, 22,   24,   59,       3,(*menu23)},//
			{24, 23,   19,   60,       3,(*menu23)},//	
			
			{25, 30,   26,   61,       4,(*menu24)},//
			{26, 25,   27,   62,       4,(*menu24)},//			
			{27, 26,   28,   63,       4,(*menu24)},//
			{28, 27,   29,   63,       4,(*menu24)},//	
			{29, 28,   30,   65,       4,(*menu24)},//
			{30, 29,   25,   66,       4,(*menu24)},//				
			
			{31, 36,   32,   67,       5,(*menu25)},//
			{32, 31,   33,   68,       5,(*menu25)},//			
			{33, 32,   34,   69,       5,(*menu25)},//
			{34, 33,   35,   70,       5,(*menu25)},//	
			{35, 34,   36,   71,       5,(*menu25)},//
			{36, 35,   37,   72,       5,(*menu25)},//	

			{37, 42,   38,   73,       6,(*menu26)},//
			{38, 37,   39,   74,       6,(*menu26)},//			
			{39, 38,   40,   75,       6,(*menu26)},//
			{40, 39,   41,   76,       6,(*menu26)},//	
			{41, 40,   42,   77,       6,(*menu26)},//
			{42, 41,   37,   78,       6,(*menu26)},//	
			
			{43,  0,    0,    0,       0,(*menu211)},//	手动调节时间界面		
			{44,  0,    0,    0,       0,(*menu212)},//	自动更新时间界面					
			{45,  0,    0,    0,       0,(*menu213)},//	远程更新时间界面	
			{46,  0,    0,    0,       0,(*menu214)},
			{47,  0,    0,    0,       0,(*menu215)},
			{48,  0,    0,    0,       0,(*menu216)},			

			{49,  0,    0,    0,       0,(*menu221)},//	
			{50,  0,    0,    0,       0,(*menu222)},// 背光时间设置		
			{51,  0,    0,    0,       0,(*menu223)},//	对比度调节	
			{52,  0,    0,    0,       0,(*menu224)},//	菜单显示时间节	
			{53,  0,    0,    0,       0,(*menu225)},
			{54,  0,    0,    0,       0,(*menu226)},
			
			{55,  0,    0,    0,       0,(*menu231)},	
			{56,  0,    0,    0,       0,(*menu232)},	
			{57,  0,    0,    0,       0,(*menu233)},	
			{58,  0,    0,    0,       0,(*menu234)},	
			{59,  0,    0,    0,       0,(*menu235)},	
			{60,  0,    0,    0,       0,(*menu236)},	
			
			{61,  0,    0,    0,       0,(*menu241)},	
			{62,  0,    0,    0,       0,(*menu242)},	
			{63,  0,    0,    0,       0,(*menu243)},	
			{64,  0,    0,    0,       0,(*menu244)},	
			{65,  0,    0,    0,       0,(*menu245)},	
			{66,  0,    0,    0,       0,(*menu246)},	

			{67,  0,    0,    0,       0,(*menu251)},	
			{68,  0,    0,    0,       0,(*menu252)},	
			{69,  0,    0,    0,       0,(*menu253)},	
			{70,  0,    0,    0,       0,(*menu254)},	
			{71,  0,    0,    0,       0,(*menu255)},	
			{72,  0,    0,    0,       0,(*menu256)},	

			{73,  0,    0,    0,       0,(*menu261)},	
			{74,  0,    0,    0,       0,(*menu262)},	
			{75,  0,    0,    0,       0,(*menu263)},	
			{76,  0,    0,    0,       0,(*menu264)},	
			{77,  0,    0,    0,       0,(*menu265)},	
			{78,  0,    0,    0,       0,(*menu266)},						
			
};



void  Menu_key_set(void)
{
	switch(keyflag)
	{	
		case 1:
		keyflag=0;				
		printf("上键按下\r\n");	
		CleanScreen();    //清屏
		ShowAllGraphRAM();			
		func_index=table[func_index].up;  //向上翻 
		break;			


		case 2:
		keyflag=0;
		printf("下键按下\r\n");
		CleanScreen();    //清屏
		ShowAllGraphRAM();					
		func_index=table[func_index].down;//向下翻
		break;		


		case 4:
		keyflag=0;
		printf("确定按下\r\n");
		CleanScreen();    //清屏
		ShowAllGraphRAM();					
		func_index=table[func_index].enter;//确定	
		break;			


		case 3:	
		keyflag=0;
		printf("退出按下\r\n");
		CleanScreen();    //清屏
		ShowAllGraphRAM();					
		func_index=table[func_index].exit;//	
		break;	
		default:
		delay_ms(1);
        getdata();  		
	}

current_operation_index=table[func_index].current_operation;
(*current_operation_index)();//执行当前操作函数
}

void  menu11(void)
{ 	
   disptime1();
}

void  menu20(void)
{		
	u8 t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
	   	 CleanScreen();
  if(func_index==1)
	{	t1=2;	
	}
else if(func_index==2)
	{	t2=2;	
	}
else if(func_index==3)
	{	t3=2;	
	}	
else if(func_index==4)
	{	t4=2;	
	}	
else if(func_index==5)
	{	t5=2;	
	}
else if(func_index==6)
	{	t6=2;	
	}	
	Show_Str(32,0,64,16,"功能菜单",16,0);	
	Show_Str(0,16,64,16,"1.调时",16,t1);		Show_Str(80,16,64,16,"2.调参",16,t2);	
	Show_Str(0,32,64,16,"3.控制",16,t3);		Show_Str(80,32,64,16,"4.其它",16,t4);		
	Show_Str(0,48,64,16,"5.工具",16,t5);		Show_Str(80,48,64,16,"6.系统",16,t6);		
	

	ShowAllGraphRAM();
}

void  menu21(void)
{
		u8 t1=0,t2=0,t3=0;
	   	 CleanScreen();
		if(func_index==7)
		{	t1=2;	
		}
	else if(func_index==8)
		{	t2=2;	
		}
	else if(func_index==9)
		{	t3=2;	
		}	

		Show_Str(32,0,64,16,"时间设置",16,0);	
		Show_Str(0,16,96,16,"1.手动调节",16,t1);		
		Show_Str(0,32,96,16,"2.自动更新",16,t2);		
		Show_Str(0,48,96,16,"3.远程更新",16,t3);			

		ShowAllGraphRAM();

}

	
void  menu22(void)         //参数设置界面
{
		u8 t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
	   	 CleanScreen();
		 if(func_index==13)
		{	t1=2;	
		}
	else if(func_index==14)
		{	t2=2;	
		}
	else if(func_index==15)
		{	t3=2;	
		}	
	else if(func_index==16)
		{	t4=2;	
		}	
	else if(func_index==17)
		{	t5=2;	
		}	
	else if(func_index==18)
		{	t6=2;	
		}	
	if(func_index==13||func_index==14||func_index==15)
		{	Show_Str(32,0, 64,16,"系统参数",16,0);	
			Show_Str(0,16, 96,16,"1.背光亮度",16,t1);		
			Show_Str(0,32, 96,16,"2.背光时间",16,t2);		
			Show_Str(0,48,112,16,"3.对比度调节",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"系统参数",16,0);	
			Show_Str(0,16,96,16,"4.菜单时间",16,t4);		
			Show_Str(0,32,96,16,"5.预制2",16,t5);		
			Show_Str(0,48,96,16,"6.预制3",16,t6);			

			ShowAllGraphRAM();
		}	
}
void  menu23(void)   //控制设计界面
{	
	u8 t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
	   	 CleanScreen();
		if(func_index==19)
		{	t1=2;	
		}
	else if(func_index==20)
		{	t2=2;	
		}
	else if(func_index==21)
		{	t3=2;	
		}	
	else if(func_index==22)
		{	t4=2;	
		}	
	else if(func_index==23)
		{	t5=2;	
		}	
	else if(func_index==24)
		{	t6=2;	
		}	
	if(func_index==19||func_index==20||func_index==21)
		{	Show_Str(32,0, 64,16,"开关控制",16,0);	
			Show_Str(0,16, 96,16,"1.开关1",16,t1);		
			Show_Str(0,32, 96,16,"2.背光时间",16,t2);		
			Show_Str(0,48,112,16,"3.对比度调节",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"系统参数",16,0);	
			Show_Str(0,16,96,16,"4.菜单时间",16,t4);		
			Show_Str(0,32,96,16,"5.预制2",16,t5);		
			Show_Str(0,48,96,16,"6.预制3",16,t6);			

			ShowAllGraphRAM();
		}	
	
}
void  menu24(void)
{
	u8 t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
	   	 CleanScreen();
		if(func_index==25)
		{	t1=2;	
		}
	else if(func_index==26)
		{	t2=2;	
		}
	else if(func_index==27)
		{	t3=2;	
		}	
	else if(func_index==28)
		{	t4=2;	
		}	
	else if(func_index==29)
		{	t5=2;	
		}	
	else if(func_index==30)
		{	t6=2;	
		}	
	if(func_index==25||func_index==26||func_index==27)
		{	Show_Str(32,0, 64,16,"系统参数",16,0);	
			Show_Str(0,16, 96,16,"1.背光亮度",16,t1);		
			Show_Str(0,32, 96,16,"2.背光时间",16,t2);		
			Show_Str(0,48,112,16,"3.对比度调节",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"系统参数",16,0);	
			Show_Str(0,16,96,16,"4.菜单时间",16,t4);		
			Show_Str(0,32,96,16,"5.预制2",16,t5);		
			Show_Str(0,48,96,16,"6.预制3",16,t6);			

			ShowAllGraphRAM();
		}	
}
void  menu25(void)
{
	u8 t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
	   	 CleanScreen();
		if(func_index==31)
		{	t1=2;	
		}
	else if(func_index==32)
		{	t2=2;	
		}
	else if(func_index==33)
		{	t3=2;	
		}	
	else if(func_index==34)
		{	t4=2;	
		}	
	else if(func_index==35)
		{	t5=2;	
		}	
	else if(func_index==36)
		{	t6=2;	
		}	
	if(func_index==31||func_index==32||func_index==33)
		{	Show_Str(32,0, 64,16,"系统参数",16,0);	
			Show_Str(0,16, 96,16,"1.电池信息",16,t1);		
			Show_Str(0,32, 96,16,"2.温湿信息",16,t2);		
			Show_Str(0,48,112,16,"3.对比度调节",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"系统参数",16,0);	
			Show_Str(0,16,96,16,"4.菜单时间",16,t4);		
			Show_Str(0,32,96,16,"5.预制2",16,t5);		
			Show_Str(0,48,96,16,"6.预制3",16,t6);			

			ShowAllGraphRAM();
		}	
}
void  menu26(void)
{
	u8 t1=0,t2=0,t3=0,t4=0,t5=0,t6=0;
	   	 CleanScreen();
		if(func_index==37)
		{	t1=2;	
		}
	else if(func_index==38)
		{	t2=2;	
		}
	else if(func_index==39)
		{	t3=2;	
		}	
	else if(func_index==40)
		{	t4=2;	
		}	
	else if(func_index==41)
		{	t5=2;	
		}	
	else if(func_index==42)
		{	t6=2;	
		}	
	if(func_index==37||func_index==38||func_index==39)
		{	Show_Str(32,0, 64,16,"高级参数",16,0);	
			Show_Str(0,16, 96,16,"1.电流信息",16,t1);		
			Show_Str(0,32, 96,16,"2.电池信息",16,t2);		
			Show_Str(0,48,112,16,"3.温湿度信息",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"高级参数",16,0);	
			Show_Str(0,16,96,16,"4.ADC信息",16,t4);		
			Show_Str(0,32,96,16,"5.预制2",16,t5);		
			Show_Str(0,48,96,16,"6.预制3",16,t6);			

			ShowAllGraphRAM();
		}	
}	
void  menu211(void)
{
	shijianset();
	func_index=7;//返回结构体入口
}	
	
void  menu212(void)
{}
void  menu213(void)
{}	
void  menu214(void)
{}	
void  menu215(void)
{}	
void  menu216(void)
{}		
void  menu221(void)
{
	blltset();
	func_index=13;//返回结构体入口
}
void  menu222(void)
{
	bltimeset();
	func_index=14;//返回结构体入口
}
void  menu223(void)
{
	crset();
	func_index=15;//返回结构体入口
}	
void  menu224(void)
{
	menutimeset();
	func_index=16;//返回结构体入口
}	

void  menu225(void)
{}	
void  menu226(void)
{}	
void  menu231(void)
{	
	dingshiset1();
	func_index=19;//返回结构体入口
}		
void  menu232(void)
{}	
void  menu233(void)
{}	
void  menu234(void)
{}	
void  menu235(void)
{}	
void  menu236(void)
{}		
	
void  menu241(void)
{
}	
void  menu242(void)
{}	
void  menu243(void)
{}	
void  menu244(void)
{}	
void  menu245(void)
{}	
void  menu246(void)
{}		
	
void  menu251(void)
{}	
void  menu252(void)
{}	
void  menu253(void)
{}	
void  menu254(void)
{}	
void  menu255(void)
{}	
void  menu256(void)
{}		
	
void  menu261(void)
{
  ina226disp();
  func_index=37;//返回结构体入口
}	
void  menu262(void)
{
  cw2015disp();
  func_index=38;//返回结构体入口	
}	
void  menu263(void)
{}	
void  menu264(void)
{}	
void  menu265(void)
{}	
void  menu266(void)
{
}
