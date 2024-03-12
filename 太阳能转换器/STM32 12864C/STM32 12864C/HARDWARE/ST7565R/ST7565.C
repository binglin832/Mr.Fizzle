#include "st7565.h"
//#include "zimu.h"
#include "usart.h"
#include "delay.h"
#include "text.h"	


void St7565_Port(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC  , ENABLE);
			
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14|GPIO_Pin_15;				        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			  
  GPIO_Init(GPIOC, &GPIO_InitStructure);
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB  , ENABLE);		

  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6|GPIO_Pin_7|GPIO_Pin_9;			        
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;			  
  GPIO_Init(GPIOB, &GPIO_InitStructure);
	
}

void delay(uint16_t ms)   
{      
    uint8_t j;   
    while(ms--)   
      for(j=0;j<=20;j++);                            
}          
       
  
/*********************************************写命令**********************************/   
   
void St7565_WriteCmd(uint8_t com)    
{   
	uint8_t i,j;   
	lcd_cs_low();
	lcd_a0_low();
	lcd_sck_low();
	
	for(i=0;i<8;i++)   
	{    
		j=com;    
		lcd_sck_low();
		if( (com&0x80)==0x80)
		{
			lcd_sda_high();
		}
		else
		{
			lcd_sda_low();
		}
		lcd_sck_high();       
		com=j<<1;   
	}  
	lcd_cs_high();
	lcd_a0_high();   
}   
   
 /*************************************************写数据************************************/     
void St7565_WriteDat(uint8_t dat)    
{   
	uint8_t i,j; 
	lcd_cs_low();   //CS=0
	lcd_a0_high();	//A0=1
	lcd_sck_low();  
	for(i=0;i<8;i++)   
	{    
		j=dat;   
		lcd_sck_low();
		if( (dat&0x80)==0x80 )
		{
			lcd_sda_high();
		}
		else
		{
			lcd_sda_low();
		}
		lcd_sck_high();      
		dat=j<<1;   
	} 
  lcd_cs_high();
	lcd_a0_low(); 
} 
/****************************************LED清屏函数*************************************/
void ClearDisplay()   
{   
	uint8_t  i,j;   
	for(i=0;i<8;i++)      
	{   
     St7565_WriteCmd(0xB0+i);               // 
     St7565_WriteCmd(0x10);   
     St7565_WriteCmd(0x01);       
		for(j=0;j<128;j++)            //
		{    
			St7565_WriteDat(0x00);   
		}   
	}   
}
/****************************************LED清屏函数*************************************/
/****************************************清除屏幕的下面3行************************************/
void ClearDisplayHF()   
{   
	uint8_t  i,j;   
	for(i=2;i<8;i++)      
	{   
     St7565_WriteCmd(0xB0+i);               // 
     St7565_WriteCmd(0x10);   
     St7565_WriteCmd(0x01);       
		for(j=0;j<128;j++)            //
		{    
			St7565_WriteDat(0x00);   
		}   
	}   
}

//=============================================地址设置=============================
void lcd_address(u16 page,u16 column)
{	
	lcd_cs_low();
	column=column+1;  						
//	page=page-1;     //初始页地址修改用
	St7565_WriteCmd(0xB0+page);   				//设置页地址。每页是8行。一个画面的64行被分成8个页。我们平常所说的第1页，在LCD驱动IC里是第0页，所以在这里减去1*/
	St7565_WriteCmd(((column>>4)&0x0f)+0x10);	//设置列地址的高4位
	St7565_WriteCmd(column&0x0f);				//设置列地址的低4位
}



/*=========================================================================================*/
//显示一个铺满全屏的图片
//此显示为每次显示每页的每一列，一共显示128列X8页
/*=========================================================================================*/
void LCD_Show_Full_Screen_Pic(unsigned char *p)
{
   unsigned int i,j;
	for(j=0; j<8; j++)
{
		lcd_address(j, 0);	
		for(i=0; i<128; i++)
	{
		St7565_WriteDat(*p);
		p++;
	}	
}	
}
  
//============================================绘图 128*64=======================================
//===============================在里面加延时可以出现渐变显示效果===============================
//======================display_128x64(0,1,abc);//在第0页第1列显示叫“abc”的图片=================
void display_128x64(u8 page,u8 column,const unsigned char *dp)
{
	int i,j;
	
	for(j=0;j<8;j++)
	{
		lcd_cs_low();
		lcd_address(page+j,column);
		for(i=0;i<128;i++)
		{
			 St7565_WriteDat(*dp); 
//    	delay(1000);	//渐变效果增加的延时		
			dp++;
		}
	}          
}
//=======================================================显示 8*8 自定义==================================================
//============================================在里面加延时可以出现渐变显示效果============================================
//==================== Ndisplay_8x8(1,2,100,SZ8X8,2);;//正显 在第2页 第100列 字模数组SZ8X8 的第2个数组====================
//=========================================u8 disp_flag=0为正显，u8 disp_flag=1为反显=====================================
void Ndisplay_8x8(u8 disp_flag,u8 page,u8 address, const unsigned char *hz,unsigned char kaici)
{  
   unsigned char i;	
   lcd_address(page,address);
	 for(i=0;i<8;i++)
	 {
	//  	delay(1000);	//渐变效果增加的延时		
		 if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*8));//0右半屏数据,1左半屏数据
		 else
	 St7565_WriteDat(~*(hz+i+kaici*8));//0右半屏数据,1左半屏数据
		}
	
}
//=======================================================显示 8*16 汉字图片===============================================
//============================================在里面加延时可以出现渐变显示效果============================================
//============== Ndisplay_8x16(1,2,100,HZ8X16,2);;//正显 在第2页 第100列 字模数组HZ8X16 的第2个数组====================
//=========================================u8 disp_flag=0为正显，u8 disp_flag=1为反显=====================================
//===========================以为每页实际是8行，所示实际是占用12X16个像素点===============================================

void Ndisplay_8x16(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici)
{  //16行16列注意，页是反过来的，这里要校正。
   unsigned char i;	
   lcd_address(page,address);
	 for(i=0;i<8;i++)
	 {
		 if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*16));//32代表一个字模数组的总数
		 else
	 St7565_WriteDat(~*(hz+i+kaici*16));
		}
	 lcd_address(page+1,address);
	 for(i=8;i<16;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*16));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*16));
	 }	
}

//=======================================================显示 16*16 汉字图片==============================================
//============================================在里面加延时可以出现渐变显示效果============================================
//============== Ndisplay_16x16(1,2,100,HZ16X16,2);;//正显 在第2页 第100列 字模数组HZ12X12 的第2个数组====================
//=========================================u8 disp_flag=0为正显，u8 disp_flag=1为反显=====================================
//===========================以为每页实际是8行，所示实际是占用12X16个像素点===============================================

void Ndisplay_16x16(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici)
{  //16行16列注意，页是反过来的，这里要校正。
   unsigned char i;	
   lcd_address(page,address);
	 for(i=0;i<16;i++)
	 {
		 if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*32));//32代表一个字模数组的总数
		 else
	 St7565_WriteDat(~*(hz+i+kaici*32));
		}
	 lcd_address(page+1,address);
	 for(i=16;i<32;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*32));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*32));
		}	
}

//=======================================================显示 16*32 数字图片==============================================
//================================此显示屏的页顺序不对，实际顺序是4，5，6，7，0，1，2，3==================================
//============================================在里面加延时可以出现渐变显示效果============================================
//============== Ndisplay_16x32(1,2,100,HZ16X32,2);;//正显 在第2页 第100列 字模数组HZ12X12 的第2个数组====================
//=========================================u8 disp_flag=0为正显，u8 disp_flag=1为反显=====================================
//==================================因为是16X32,每页只有8行，所以需要占用4页==============================================

void Ndisplay_16x32(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici)
{  
   unsigned char i;	
   lcd_address(page,address);
	 for(i=0;i<16;i++)
	 {
		 if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));     //64代表一个字模数组的总数
		 else
	 St7565_WriteDat(~*(hz+i+kaici*64));
		}
	 lcd_address(page+1,address);
	 for(i=16;i<32;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*64));
			 
		}	
	 	lcd_address(page+2,address);
	 for(i=32;i<48;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*64));			 
		}	
		 	lcd_address(page+3,address);
	 for(i=48;i<64;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*64));			 
		}	 
}

//=======================================================显示 32*32 汉字图片==============================================
//================================此显示屏的页顺序不对，实际顺序是4，5，6，7，0，1，2，3==================================
//============================================在里面加延时可以出现渐变显示效果============================================
//============== Ndisplay_32x32(1,2,100,HZ32X32,2);;//正显 在第2页 第100列 字模数组HZ32X32 的第2个数组====================
//=========================================u8 disp_flag=0为正显，u8 disp_flag=1为反显=====================================
//==================================因为是32X32,每页只有8行，所以需要占用4页==============================================
void Ndisplay_32x32(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici)
{  //16行16列注意，页是反过来的，这里要校正。
   unsigned char i;	
   lcd_address(page,address);
	 for(i=0;i<32;i++)
	 {
		 if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
		 else
	 St7565_WriteDat(~*(hz+i+kaici*64));
		}
	 lcd_address(page+1,address);
	 for(i=32;i<64;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*64));
			 
		}	
	 	lcd_address(page+2,address);
	 for(i=64;i<96;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*64));			 
		}	
		 	lcd_address(page+3,address);
	 for(i=96;i<128;i++)
	 {
		  if(disp_flag)	
	 St7565_WriteDat(*(hz+i+kaici*64));
			 else
	 St7565_WriteDat(~*(hz+i+kaici*64));			 
		}	 
}

/*******************************LCD初始化函数*******************************/
void Init_St7565(void)
{

	St7565_Port();  
	lcd_rst_low();
	delay(200); 
    lcd_rst_high();  
	delay(200);   

//ST6575R屏幕参数设置	
	St7565_WriteCmd(0xAF);//第1命令  AF开显示    AE关显示
	
	St7565_WriteCmd(0x40);//第2命令  显示初始行位置  8位数据 01XX XXXX 
	
	St7565_WriteCmd(0xB0);//第3命令  页地址设置 4位数据  1011 XXXX
	
	St7565_WriteCmd(0x10);//第4命令  高4位列地址
	St7565_WriteCmd(0x01);//         低4位列地址 //ST7565R这里是01
		
	St7565_WriteCmd(0xA0);//第8命令  显示列地址增减 A0 常规从左到右 A1反转从右到左
	
	St7565_WriteCmd(0xA6);//第9命令  显示正显/反显 A6正显 A7反显

	St7565_WriteCmd(0xA4);//第10命令 A4正常显示 A5显示全部点阵

	St7565_WriteCmd(0xA3);//第11命令 偏压比设置  A2  1/9  A3  1/7
	
	St7565_WriteCmd(0xE2);//第14命令 软件复位

	St7565_WriteCmd(0xC8);//第15命令 行扫描顺序选择 C8普通顺序 c0反向扫描

  St7565_WriteCmd(0x2F);//第16命令 电源控制  2F开  8位数据 0010 1XXX
	
	St7565_WriteCmd(0x25);//第17命令 内部电阻率设置  此液晶无效
		
	St7565_WriteCmd(0x81);//第18命令 对比度 6位电压值数据，0～63 共 64 级		
	St7565_WriteCmd(0x28);//         范围0x00-0x3f 值越大越浓和上一条指令同时用0x81不动	

  St7565_WriteCmd(0xAC);//第19命令 静态图标显示：开/关  AC关 AD开，本液晶屏无此图标.此指令在进入及退出睡眠模式时起作用
	
	St7565_WriteCmd(0xF8);//第20命令 升压倍数选择 此不变
	St7565_WriteCmd(0x01);//         00：2 倍，3 倍，4 倍01：5 倍11：6 倍。本模块外部已设置升压倍数为 4倍，不必使用此指令
//=============================部分ST7565/ST7565R显示屏页的位置是上下颠倒的，需要在最后添加如下指令===================================
//=======================================================必须添加在所有指令最后=======================================================
	St7565_WriteCmd(0x60);//添加的指令
	
	ClearDisplay(); 
	
	printf("LCD初始化完成\r\n");	

}


