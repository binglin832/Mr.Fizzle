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
	u8 current;   //��ǰ״̬������
	u8 up;        //���ϰ���
	u8 down;	    //���°���
	u8 enter;     //ȷ������
	u8 exit;      //�˳�����
	void (*current_operation)(void); //��ǰ״̬Ӧ��ִ�еĲ���
}
Menu_table;
//=============�ṹ��˵��===================//
//       {0,8,8,1,1,(*fun0)},      ��һ��0-----�����������ż��û���Ҫִ���ĸ������Ĵ���
//                                 �����Ǵ���4����������֮ǰ�ĺ�����˳������
//                                 (*fun0)Ϊ��ǰ��ʾ�Ľ���
//���У�x,y,z,a��δ֪�����Ȳ�ȷ�������Ƕ�Ӧ�����ĸ�������Ҫָʾ������ֵ�������������������δ֪������ȷ������֮ǰ������Ҫ�˽��Լ�����ִ��ʲô���
//���뿪ʼʱʱִ����������ĵ�һ����table[0]�������ڴ˰��ϼ�����һ��������ִ�к���fun6����ôtable[0]������Ҫ��������
//��0��5��y,z,a,(*fun1)��,
//ͬ�������ϣ�����¼�ִ��fun7����Ҫ����Ϊ
//��0��x��6,z,a,(*fun1)����
//���ϣ����ȷ�ϼ�ִ��fun3����Ҫ����Ϊ
//��0��x��y,2,a,(*fun1)����
//���ϣ�����˳���ִ��fun4����Ҫ����Ϊ
//��0��x��y,z,3,(*fun1)����
//������������������Ҫ������Ϊ
//��0��5��6,2,3,(*fun1)��.��������Ԫ��һ�����ơ�

Menu_table  table[79]=//79
{
       //   ����  �ϣ�  �£�  ȷ����   �˳�
			{0,   0,    0,    1,       0,(*menu11)},//     ��ҳ��
			
			{1,   6,    2,    7,       0,(*menu20)},// �ڶ���	���ܽ���	ѡ��1��ʱ
			{2,   1,    3,   13,       0,(*menu20)},// �ڶ���	���ܽ���	ѡ��2����					
			{3,   2,    4,   19,       0,(*menu20)},// �ڶ��� ���ܽ���	ѡ��3 
			{4,   3,    5,   25,       0,(*menu20)},// �ڶ���	���ܽ���	ѡ��4					
			{5,   4,    6,   31,       0,(*menu20)},// �ڶ���	���ܽ���	ѡ��5
			{6,   5,    1,   37,       0,(*menu20)},// �ڶ���	���ܽ���	ѡ��6ϵͳ
			
			{7,   9,    8,   43,       1,(*menu21)},// ������   ѡ��1  ʱ�����ý���  ѡ��1�ֶ�����				
			{8,   7,    9,   44,       1,(*menu21)},// ������   ѡ��1  ʱ�����ý���	 ѡ��2�Զ�����
			{9,   8,    7,   45,       1,(*menu21)},// ������	  ѡ��1  ʱ�����ý���  ѡ��3Զ�̸���
			{10,  8,    7,   46,       1,(*menu21)},
			{11,  8,    7,   47,       1,(*menu21)},
			{12,  8,    7,   48,       1,(*menu21)},			
			
			{13, 18,   14,   49,       2,(*menu22)},// ������	ѡ��2  �������ý���  ѡ��1��������                       
			{14, 13,   15,   50,       2,(*menu22)},// ������	ѡ��2  �������ý���  ѡ��2����ʱ��   
			{15, 14,   16,   51,       2,(*menu22)},// ������	ѡ��2  �������ý���  ѡ��3�Աȶȵ���                     
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
			
			{43,  0,    0,    0,       0,(*menu211)},//	�ֶ�����ʱ�����		
			{44,  0,    0,    0,       0,(*menu212)},//	�Զ�����ʱ�����					
			{45,  0,    0,    0,       0,(*menu213)},//	Զ�̸���ʱ�����	
			{46,  0,    0,    0,       0,(*menu214)},
			{47,  0,    0,    0,       0,(*menu215)},
			{48,  0,    0,    0,       0,(*menu216)},			

			{49,  0,    0,    0,       0,(*menu221)},//	
			{50,  0,    0,    0,       0,(*menu222)},// ����ʱ������		
			{51,  0,    0,    0,       0,(*menu223)},//	�Աȶȵ���	
			{52,  0,    0,    0,       0,(*menu224)},//	�˵���ʾʱ���	
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
		printf("�ϼ�����\r\n");	
		CleanScreen();    //����
		ShowAllGraphRAM();			
		func_index=table[func_index].up;  //���Ϸ� 
		break;			


		case 2:
		keyflag=0;
		printf("�¼�����\r\n");
		CleanScreen();    //����
		ShowAllGraphRAM();					
		func_index=table[func_index].down;//���·�
		break;		


		case 4:
		keyflag=0;
		printf("ȷ������\r\n");
		CleanScreen();    //����
		ShowAllGraphRAM();					
		func_index=table[func_index].enter;//ȷ��	
		break;			


		case 3:	
		keyflag=0;
		printf("�˳�����\r\n");
		CleanScreen();    //����
		ShowAllGraphRAM();					
		func_index=table[func_index].exit;//	
		break;	
		default:
		delay_ms(1);
        getdata();  		
	}

current_operation_index=table[func_index].current_operation;
(*current_operation_index)();//ִ�е�ǰ��������
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
	Show_Str(32,0,64,16,"���ܲ˵�",16,0);	
	Show_Str(0,16,64,16,"1.��ʱ",16,t1);		Show_Str(80,16,64,16,"2.����",16,t2);	
	Show_Str(0,32,64,16,"3.����",16,t3);		Show_Str(80,32,64,16,"4.����",16,t4);		
	Show_Str(0,48,64,16,"5.����",16,t5);		Show_Str(80,48,64,16,"6.ϵͳ",16,t6);		
	

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

		Show_Str(32,0,64,16,"ʱ������",16,0);	
		Show_Str(0,16,96,16,"1.�ֶ�����",16,t1);		
		Show_Str(0,32,96,16,"2.�Զ�����",16,t2);		
		Show_Str(0,48,96,16,"3.Զ�̸���",16,t3);			

		ShowAllGraphRAM();

}

	
void  menu22(void)         //�������ý���
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
		{	Show_Str(32,0, 64,16,"ϵͳ����",16,0);	
			Show_Str(0,16, 96,16,"1.��������",16,t1);		
			Show_Str(0,32, 96,16,"2.����ʱ��",16,t2);		
			Show_Str(0,48,112,16,"3.�Աȶȵ���",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"ϵͳ����",16,0);	
			Show_Str(0,16,96,16,"4.�˵�ʱ��",16,t4);		
			Show_Str(0,32,96,16,"5.Ԥ��2",16,t5);		
			Show_Str(0,48,96,16,"6.Ԥ��3",16,t6);			

			ShowAllGraphRAM();
		}	
}
void  menu23(void)   //������ƽ���
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
		{	Show_Str(32,0, 64,16,"���ؿ���",16,0);	
			Show_Str(0,16, 96,16,"1.����1",16,t1);		
			Show_Str(0,32, 96,16,"2.����ʱ��",16,t2);		
			Show_Str(0,48,112,16,"3.�Աȶȵ���",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"ϵͳ����",16,0);	
			Show_Str(0,16,96,16,"4.�˵�ʱ��",16,t4);		
			Show_Str(0,32,96,16,"5.Ԥ��2",16,t5);		
			Show_Str(0,48,96,16,"6.Ԥ��3",16,t6);			

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
		{	Show_Str(32,0, 64,16,"ϵͳ����",16,0);	
			Show_Str(0,16, 96,16,"1.��������",16,t1);		
			Show_Str(0,32, 96,16,"2.����ʱ��",16,t2);		
			Show_Str(0,48,112,16,"3.�Աȶȵ���",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"ϵͳ����",16,0);	
			Show_Str(0,16,96,16,"4.�˵�ʱ��",16,t4);		
			Show_Str(0,32,96,16,"5.Ԥ��2",16,t5);		
			Show_Str(0,48,96,16,"6.Ԥ��3",16,t6);			

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
		{	Show_Str(32,0, 64,16,"ϵͳ����",16,0);	
			Show_Str(0,16, 96,16,"1.�����Ϣ",16,t1);		
			Show_Str(0,32, 96,16,"2.��ʪ��Ϣ",16,t2);		
			Show_Str(0,48,112,16,"3.�Աȶȵ���",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"ϵͳ����",16,0);	
			Show_Str(0,16,96,16,"4.�˵�ʱ��",16,t4);		
			Show_Str(0,32,96,16,"5.Ԥ��2",16,t5);		
			Show_Str(0,48,96,16,"6.Ԥ��3",16,t6);			

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
		{	Show_Str(32,0, 64,16,"�߼�����",16,0);	
			Show_Str(0,16, 96,16,"1.������Ϣ",16,t1);		
			Show_Str(0,32, 96,16,"2.�����Ϣ",16,t2);		
			Show_Str(0,48,112,16,"3.��ʪ����Ϣ",16,t3);			

			ShowAllGraphRAM();
		}
	else
		{	Show_Str(32,0,64,16,"�߼�����",16,0);	
			Show_Str(0,16,96,16,"4.ADC��Ϣ",16,t4);		
			Show_Str(0,32,96,16,"5.Ԥ��2",16,t5);		
			Show_Str(0,48,96,16,"6.Ԥ��3",16,t6);			

			ShowAllGraphRAM();
		}	
}	
void  menu211(void)
{
	shijianset();
	func_index=7;//���ؽṹ�����
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
	func_index=13;//���ؽṹ�����
}
void  menu222(void)
{
	bltimeset();
	func_index=14;//���ؽṹ�����
}
void  menu223(void)
{
	crset();
	func_index=15;//���ؽṹ�����
}	
void  menu224(void)
{
	menutimeset();
	func_index=16;//���ؽṹ�����
}	

void  menu225(void)
{}	
void  menu226(void)
{}	
void  menu231(void)
{	
	dingshiset1();
	func_index=19;//���ؽṹ�����
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
  func_index=37;//���ؽṹ�����
}	
void  menu262(void)
{
  cw2015disp();
  func_index=38;//���ؽṹ�����	
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
