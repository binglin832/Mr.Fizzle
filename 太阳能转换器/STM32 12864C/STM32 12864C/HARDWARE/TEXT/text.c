#include "sys.h" 
//#include "fontupd.h"
#include "w25qxx.h"
#include "text.h"	
#include "string.h"			
#include "st7565.h"
#include "usart.h"
#include "zimu.h"

#include "delay.h"
#include "math.h" 


unsigned char  GraphRAM[LCD_LENGTH * LCD_WIDTH / 8] = {0};


void CleanScreen()
{
	unsigned short i;
	for(i=0; i<LCD_LENGTH * LCD_WIDTH/8; i++)
	{
		GraphRAM[i] = 0x00;
	}
}

//这两个是低位在前
void DrawPoint(unsigned char x, unsigned char y)
{
	//避免超出屏幕范围
	if(x < LCD_LENGTH && y < LCD_WIDTH)
	{
		GraphRAM[x + LCD_LENGTH * (y >> 3)] = GraphRAM[x + LCD_LENGTH * (y >> 3)] | (0x01 << (y & 0x07));
	}
}

void CleanPoint(unsigned char x, unsigned char y)
{
	//避免超出屏幕范围
	if(x < LCD_LENGTH && y < LCD_WIDTH)
	{
		GraphRAM[x + LCD_LENGTH * (y >> 3)] = GraphRAM[x + LCD_LENGTH * (y >> 3)] & (~(0x01 << (y & 0x07)));
	}
}

unsigned char ReadPoint(unsigned char x, unsigned char y)
{
	//避免超出屏幕范围
	if(x < LCD_LENGTH && y < LCD_WIDTH)
	{
		if(GraphRAM[x + LCD_LENGTH * (y >> 3)] & (0x01 << (y & 0x07)))
		{
			return 1;
		}
	}
	return 0;
}

void DrawChunk(unsigned char column, unsigned char page, unsigned char dat)
{
	GraphRAM[column + LCD_LENGTH * page] = dat;
}
/*=========================================================================================*/
//屏幕大小  X<128,Y<128
//画线函数
//从点x1，y1到点x2，y2画一条线
//color=1，使能显示
/*=========================================================================================*/
void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color)
{	
	//Bresenham算法
	extern double abs(double __x); 
  int dx = x2 - x1;
	int dy = y2 - y1;
	int ux = ((dx > 0) << 1) - 1;//x的增量方向，取或-1
	int uy = ((dy > 0) << 1) - 1;//y的增量方向，取或-1
	int x = x1, y = y1, eps;//eps为累加误差
	
	eps = 0;
	dx = abs(dx);
	dy = abs(dy);
//	(dx > 0) ? (dx) : (-dx);
//	(dy > 0) ? (dy) : (-dy);

	if(color)
	{
		if (dx > dy)
		{
			for (x = x1; x != x2+ux; x += ux)
			{
				DrawPoint(x, y);
				eps += dy;
				if ((eps << 1) >= dx)
				{
					y += uy; eps -= dx;
				}
			}
		}
		else
		{
			for (y = y1; y != y2+uy; y += uy)
			{
				DrawPoint(x, y);
				eps += dx;
				if ((eps << 1) >= dy)
				{
					x += ux; eps -= dy;
				}
			}
		}
	}
	else
	{
		if (dx > dy)
		{
			for (x = x1; x != x2+ux; x += ux)
			{
				CleanPoint(x, y);
				eps += dy;
				if ((eps << 1) >= dx)
				{
					y += uy; eps -= dx;
				}
			}
		}
		else
		{
			for (y = y1; y != y2+uy; y += uy)
			{
				CleanPoint(x, y);
				eps += dx;
				if ((eps << 1) >= dy)
				{
					x += ux; eps -= dy;
				}
			}
		}
	}
}
/*=======================================================================================*/
//画矩形
//模式0:黑色边框
//模式1:黑色填充
//模式2:白色边框
//模式3:白色填充
/*=======================================================================================*/
void DrawRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char mode)
{
	unsigned char x;
	x = x1;
	switch(mode)
	{
	case 0:
		DrawLine(x1, y1, x2, y1, 1);
		DrawLine(x1, y2, x2, y2, 1);
		DrawLine(x1, y1, x1, y2, 1);
		DrawLine(x2, y1, x2, y2, 1);
		break;
	case 1:
		for( ; y1<y2+1; y1++)
		{
			for(x1=x; x1<x2+1; x1++)
			{
				DrawPoint(x1, y1);
			}
		}
		break;
	case 2:
		DrawLine(x1, y1, x2, y1, 0);
		DrawLine(x1, y2, x2, y2, 0);
		DrawLine(x1, y1, x1, y2, 0);
		DrawLine(x2, y1, x2, y2, 0);
		break;
	case 3:
		for( ; y1<y2+1; y1++)
		{
			for(x1=x; x1<x2+1; x1++)
			{
				CleanPoint(x1, y1);
			}
		}
		break;
	default: return;
	}
}
/*=======================================================================================*/
/*=======================================================================================*/
/*=======================================================================================*/
//更新缓存
/*=======================================================================================*/
/*=======================================================================================*/
void ShowAllGraphRAM()
{
	LCD_Show_Full_Screen_Pic(GraphRAM);	
}


//纵向的数值都必须是8的整数倍
void DrawPic(unsigned char x, unsigned char y, unsigned char length, unsigned char width, unsigned char *pic)
{
	unsigned char i,j;
	unsigned char *p;
	
	for(i=y/8; i<width/8; i++)
	{
		p = (GraphRAM + LCD_LENGTH * i + x);
		for(j=x; j<x+length; j++)
		{
			*p = *pic;
			p++;
			pic++;
		}
	}
}

void InverseColor(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2)
{
	unsigned char a,b;

	//虽然实现了，但是效率不高
	for(a=x1; a<x2+1; a++)
	{
		for(b=y1; b<y2+1; b++)
		{
			GraphRAM[a + LCD_LENGTH * (b >> 3)] = GraphRAM[a + LCD_LENGTH * (b >> 3)] ^ (0x01 << (b & 0x07));//画点函数改成异或
		}
	}
}

void MsgBox(unsigned char x, unsigned char y, char *p)
{
	//一共34字节的空间，32个字符加2个结束符
	u8	string[34];
	char *pp = p;
	u8 *ps = string;

	//复制第一行字符
	while(*p!=0 && p<pp+16)
	{
		*ps = *p;
		p++;
		ps++;
	}
	*ps = 0;//如果不足一行，加个结束符，然后给第二行开头也放个结束符
	string[17] = 0;

	//如果多余16个字节，复制第二行字符
	if(p-pp>=1)
	{
		ps = string + 17;
		while(p < pp+34)
		{
			*ps = *p;
			p++;
			ps++;
		}
	}
	
	//加入结束符，方便后面显示字符
	string[16] = 0;
	string[33] = 0;

	DrawRectangle(x,y,x+100,y+46,3);//清除显示区域
	DrawRectangle(x+1,y+1,x+99,y+45,0);//画方框
	DrawLine(x+1,y+17, x+99, y+17, 1);//画分割线


		Show_Str(x+33,y+3,36,12,"提  示",12,0); 
		Show_Str(x+3,y+19,36,12,string,12,0);
		Show_Str(x+3,y+32,36,12,string+17,12,0); 	
}

//最多显示6个汉字
//如果出现“这是六 个汉字”这样的字符串，显示会出错，■■■■■■■
void MsgBoxSimple(unsigned char x, unsigned char y, char *p)
{
	//一共14字节的空间，12个字符加1个结束符
	u8 string[13];
	unsigned char i;
	
	for(i=0; i<12; i++)
	{
		string[i] = *p;
		p++;
	}
		
	string[12] = 0;
	
	DrawRectangle(x,y,x+76,y+17,3);//清除显示区域
	DrawRectangle(x+1,y+1,x+75,y+16,0);//画方框
	
	Show_Str(x+3,y+3,72,12,string,12,0);  
}

void ShowMsgSimple(char *p, u16 t)
{
	MsgBoxSimple(28, 23, p);
	ShowAllGraphRAM();
	delay_ms(t);
}


//code 字符指针开始
//从字库中查找出字模
//code 字符串的开始地址,GBK码
//mat  数据存放地址 (size/8+((size%8)?1:0))*(size) bytes大小	
//size:字体大小
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size)
{
  unsigned char c1,c2,c3,c4;
	unsigned char i;					  
	unsigned long foffset; 
	unsigned long h=0;
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	c1=*code;
	c2=*(++code);
	c3=*(++code);
	c4=*(++code);
	if(c2==0x7f)
	{   		    
	  for(i=0;i<csize;i++)*mat++=0x00;//填充满格
	  return; //结束访问
	}
	if(size==12)
	{	
			if(c1 >= 0xA1 && c1 <= 0xa9 && c2 >= 0xa1) //Section 1
				h = (c1 - 0xA1) * 94 + (c2 - 0xA1);
			else if(c1 >= 0xa8 && c1 <= 0xa9 && c2 < 0xa1) //Section 5
			{
				if(c2 > 0x7f)
					c2--;
				h = (c1 - 0xa8) * 96 + (c2 - 0x40) + 846;
			}
			if(c1 >= 0xb0 && c1 <= 0xf7 && c2 >= 0xa1) //Section 2
				h = (c1 - 0xB0) * 94 + (c2 - 0xA1) + 1038;
			else if(c1 < 0xa1 && c1 >= 0x81 && c2 >= 0x40 ) //Section 3
			{
				if(c2 > 0x7f)
					c2--;
					h=(c1-0x81)*190 + (c2-0x40) + 1038 +6768;
			}
			else if(c1>=0xaa && c2<0xa1) //Section 4
			{
			if(c2>0x7f)
			c2--;
			h=(c1-0xaa)*96 + (c2-0x40) + 1038 +12848;
			}
			foffset=h*24;
			SPI_Flash_Read(mat,foffset+0x093D0E,24);
	}
	
	else	if(size==16)
	{	
				if(c1 <= 0xA1 && c1 >= 0xA0 && c2 >= 0xa1) //Section 1 标点
					h = (c1 - 0xA1) * 94 + (c2 - 0xA1);
				else if(c1 >= 0xa8 && c1 <= 0xa9 && c2 < 0xa1) //Section 5
				{
					if(c2 > 0x7f)
						c2--;
					h = (c1 - 0xa8) * 96 + (c2 - 0x40) + 846;
				}
				if(c1 >= 0xb0 && c1 <= 0xf7 && c2 >= 0xa1) //Section 2
					h = (c1 - 0xB0) * 94 + (c2 - 0xA1) + 1038;
				else if(c1 < 0xa1 && c1 >= 0x81 && c2 >= 0x40 ) //Section 3
				{
					if(c2 > 0x7f)
						c2--;
					h = (c1 - 0x81) * 190 + (c2 - 0x40) + 1038 + 6768;
				}
				else if(c1 >= 0xaa && c2 < 0xa1) //Section 4
				{
					if(c2 > 0x7f)
						c2--;
					h = (c1 - 0xaa) * 96 + (c2 - 0x40) + 1038 + 12848;
				}
				else if(c1 == 0x81 && c2 >= 0x39) //四字节区 1
				{
					h = 1038 + 21008 + (c3 - 0xEE) * 10 + c4 - 0x39;
				}
				else if(c1 == 0x82) //四字节区 2
				{
					h = 1038 + 21008 + 161 + (c2 - 0x30) * 1260 + (c3 - 0x81) * 10 + c4 - 0x30;
				}
			foffset=h*32;
			SPI_Flash_Read(mat,foffset+0x114FDE,32);
	}
	if(size==24)
	{	
				if(c1 >= 0xA1 && c1 <= 0xAB && c2 >= 0xa1) //Section 1
					h = (c1 - 0xA1) * 94 + (c2 - 0xA1);
				else if(c1 >= 0xa8 && c1 <= 0xa9 && c2 < 0xa1) //Section 5
				{
					if(c2 > 0x7f)
						c2--;
					h = (c1 - 0xa8) * 96 + (c2 - 0x40) + 846;
				}
				if(c1 >= 0xb0 && c1 <= 0xf7 && c2 >= 0xa1) //Section 2
					h = (c1 - 0xB0) * 94 + (c2 - 0xA1) + 1038;
				else if(c1 < 0xa1 && c1 >= 0x81 && c2 >= 0x40) //Section 3
				{
					if(c2 > 0x7f)
						c2--;
					h = (c1 - 0x81) * 190 + (c2 - 0x40) + 1038 + 6768;
				}
				else if(c1 >= 0xaa && c2 < 0xa1) //Section 4
				{
					if(c2 > 0x7f)
						c2--;
					h = (c1 - 0xaa) * 96 + (c2 - 0x40) + 1038 + 12848;
				}
				else if(c1 == 0x81 && c2 >= 0x39) //四字节区  1
				{
					h = 1038 + 21008 + (c3 - 0xEE) * 10 + c4 - 0x39;
				}
				else if(c1 == 0x82) //四字节区  2
				{
					h = 1038 + 21008 + 161 + (c2 - 0x30) * 1260 + (c3 - 0x81) * 10 + c4 - 0x30;
				}
				foffset=h*72;
				SPI_Flash_Read(mat,foffset+0x1F43DE,72);
	} 		      	  												    
}  
//显示一个指定大小的汉字
//x,y :汉字的坐标
//font:汉字GBK码
//size:字体大小
//mode:1,正常显示,0,叠加显示	2,反色显示   
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode)
{
	u8 temp,temp1,t,t1,t2,t3;
	u16 x0=x,x1=x,x2=x,y1=y;
	u8 dzk[72];   
	u8 csize=(size/8+((size%8)?1:0))*(size);//得到字体一个字符对应点阵集所占的字节数	 
	if(size!=12&&size!=16&&size!=24)return;	//不支持的size
	Get_HzMat(font,dzk,size);	//得到相应大小的点阵数据  
	if(mode==0)
	{	 
	for(t=0;t<csize;t++)
	{
    temp=dzk[t];			      //得到点阵数据 
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)DrawPoint(x,y);			
			temp<<=1;
			x++;
			if((x-x0)==size)
			{
				x=x0;
				y++;
				break;
			}	
		}  	 
	}
  }
else if(mode==1)
  {
	for(t2=0;t2<csize;t2++)
	{
		temp1=dzk[t2];			      //得到点阵数据 	
		for(t3=0;t3<8;t3++)
		{
			CleanPoint(x2,y1);				
			temp1<<=1;
			x2++;
			if((x2-x1)==size)
			{
				x2=x1;
				y1++;
		}		
	 }			
  }
	for(t=0;t<csize;t++)
	{
    temp=dzk[t];			      //得到点阵数据 
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)DrawPoint(x,y);				
			temp<<=1;
			x++;
			if((x-x0)==size)
			{
				x=x0;
				y++;
				break;
			}	
		}  	 
	}		
}	
else	if(mode==2)
	{	 
	for(t=0;t<csize;t++)
	{
   temp=~dzk[t];			//取反得到的点阵数据
		for(t1=0;t1<8;t1++)
		{
			if(temp&0x80)DrawPoint(x,y);				
			temp<<=1;
			x++;
			if((x-x0)==size)
			{
				x=x0;
				y++;
				break;
			}	
		}  	 
	}
  }	
}

//在指定位置显示一个字符
//x,y:起始坐标
//num:要显示的字符:" "--->"~"  例如'5'
//size:字体大小 12/16/24
//mode:1,正常显示,0,叠加显示	2,反色显示  
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode)
{  							  
  u8 temp,t1,t;
	u16 y0=y,y1=y,x1=x;
	unsigned char i,j;	
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		//得到字体一个字符对应点阵集所占的字节数	
 	num=num-' ';//得到偏移后的值（ASCII字库是从空格开始取模，所以-' '就是对应字符的字库）
	
 if(mode==1|mode==2)
 {
	 for(j=0;j<size;j++)
  {	 
	 for(i=0;i<size/2;i++)	
   {	
		CleanPoint(x1+i,y1);
  	}
	  y1++;	
	 }			 
 }			
	for(t=0;t<csize;t++)
	{   
		     if(size==12)temp=asc2_1206[num][t]; 	//调用1206字体
		else if(size==16)temp=asc2_1608[num][t];	//调用1608字体
		else if(size==24)temp=asc2_2412[num][t];	//调用2412字体
		else return;								//没有的字库
	if(mode==2)
		{
			temp=~temp;
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}
	}
else if(mode==0)		
  {	
	  temp=temp;
		for(t1=0;t1<8;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}		
	}
else if(mode==1)
  {	
	temp=temp;		
	for(t1=0;t1<8;t1++)
	{			
		if(temp&0x80)DrawPoint(x,y);
		temp<<=1;
		y++;
		if((y-y0)==size)
		{
			y=y0;
			x++;
			break;
		}
	}		
}		
}
} 
//在指定位置显示一个在自定义字符
//x,y:起始坐标
//size:字体大小 宽X高    16X8(14)/16X16(26)/16X32(32)
//mode:1,正常显示,0,叠加显示	2,反色显示  
//const  unsigned char *hz;数组的名字
//unsigned char kaici;数组的第几个
//取模方式   阴码 顺向 列行式  16进制
void LCD_zimu(u16 x,u16 y,u8 size,u8 mode,const  unsigned char *hz,unsigned char kaici)
{  							  
  u8 temp,t1;
	u16 y0=y,y1=y,x1=x;	
   unsigned char i,j;	
if(mode==0)		
 {  	
  if(size==14)
  {	
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*16);	//调用16X16自定义字模字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==8)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
  }	 			
else  if(size==16)
  {	
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*32);	//调用16X16自定义字模字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=16;i<32;i++)
	 {  
   temp=*(hz+i+kaici*32);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-16,y+8);	
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
 }
else   if(size==32)
  {	
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*64);	//调用16X16自定义字模字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=16;i<32;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-16,y+8);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=32;i<48;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-32,y+16);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=48;i<64;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-48,y+24);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}	 
 }
}
else if(mode==2)		
 {  	
  if(size==14)
  {
	 for(j=0;j<8;j++)
   {
	 for(i=0;i<8;i++)	
   {	
		CleanPoint(x1,y1+i);
  	}
	  x1++;	
   }		
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*16);	//调用16X16自定义字模字体
    temp=~temp;		 
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==8)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
  }	 			
else  if(size==16)
  {	
	 for(j=0;j<16;j++)
   {
	 for(i=0;i<16;i++)	
   {	
		CleanPoint(x1,y1+i);
  	}
	  x1++;	
   }		
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*32);	//调用16X16自定义字模字体
    temp=~temp;	
		 for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=16;i<32;i++)
	 {  
   temp=*(hz+i+kaici*32);	//调用1608字体
    temp=~temp;	
		 for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-16,y+8);	
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
 }
else   if(size==32)
  {	
	 for(j=0;j<16;j++)
   {
	 for(i=0;i<32;i++)	
   {	
		CleanPoint(x1,y1+i);
  	}
	  x1++;	
   }		
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*64);	//调用16X16自定义字模字体
    temp=~temp;	
		 for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=16;i<32;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
    temp=~temp;	
		 for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-16,y+8);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=32;i<48;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
    temp=~temp;	
		 for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-32,y+16);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=48;i<64;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
    temp=~temp;	
		 for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-48,y+24);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}	 
 }
}
else if(mode==1)		
 {  	
  if(size==14)
  {	
	 for(j=0;j<8;j++)
   {
	 for(i=0;i<8;i++)	
   {	
		CleanPoint(x1,y1+i);
  	}
	  x1++;	
   }
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*16);	//调用16X16自定义字模字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==8)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
  }	 			
else  if(size==16)
  {
	 for(j=0;j<16;j++)
   {
	 for(i=0;i<16;i++)	
   {	
		CleanPoint(x1,y1+i);
  	}
	  x1++;	
   }		
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*32);	//调用16X16自定义字模字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=16;i<32;i++)
	 {  
   temp=*(hz+i+kaici*32);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-16,y+8);	
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
 }
else   if(size==32)
  {	
	 for(j=0;j<16;j++)
   {
	 for(i=0;i<32;i++)	
   {	
		CleanPoint(x1,y1+i);
  	}
	  x1++;	
   }				
	 for(i=0;i<16;i++)
	 {  
    temp=*(hz+i+kaici*64);	//调用16X16自定义字模字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x,y);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=16;i<32;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-16,y+8);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=32;i<48;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-32,y+16);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}
	 for(i=48;i<64;i++)
	 {  
   temp=*(hz+i+kaici*64);	//调用1608字体
		for(t1=0;t1<16;t1++)
		{			    
			if(temp&0x80)DrawPoint(x-48,y+24);
			temp<<=1;
			y++;
			if((y-y0)==16)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}	 
 }
} 
} 
//在指定位置开始显示一个字符串	    
//支持自动换行
//(x,y):起始坐标
//width,height:区域
//str  :字符串
//size :字体大小
//mode:0,非叠加方式;1,叠加方式    	   		   
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode)
{					
	u16 x0=x;
	u16 y0=y;							  	  
    u8 bHz=0;     //字符或者中文  	    				    				  	  
    while(*str!=0)//数据未结束
    { 
        if(!bHz)
        {
	        if(*str>0x80)bHz=1;//中文 
	        else              //字符
	        {      
                if(x>(x0+width-size/2))//换行
				{				   
					y+=size;
					x=x0;	   
				}							    
		        if(y>(y0+height-size))break;//越界返回      
		        if(*str==13)//换行符号
		        {         
		            y+=size;
					x=x0;
		            str++; 
		        }  
		        else LCD_ShowChar(x,y,*str,size,mode);//有效部分写入 
				str++; 
		        x+=size/2; //字符,为全字的一半 
	        }
        }else//中文 
        {     
            bHz=0;//有汉字库    
            if(x>(x0+width-size))//换行
			{	    
				y+=size;
				x=x0;		  
			}
	        if(y>(y0+height-size))break;//越界返回  						     
	        Show_Font(x,y,str,size,mode); //显示这个汉字,空心显示 
	        str+=2; 
	        x+=size;//下一个汉字偏移	    
        }						 
    }   
}  			 		 
   
//m^n函数
//返回值:m^n次方.
u32 LCD_Pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}	

//显示数字,高位为0,可以选择则是否显示
//x,y :起点坐标	 
//len :数字的位数
//num:数值(0~4294967295);	可以直接是变量   例如 a  b
//size:字体大小12/16/24
//mode:0，叠加显示 1,非叠加显示2,反色显示  
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode)
{         	
	u8 t,temp;
	u8 enshow=0;		
		
	for(t=0;t<len;t++)
	{		
		temp=(num/LCD_Pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))//1
		{
			if(temp==0)
			{
				LCD_ShowChar(x+(size/2)*t,y,'0',size,mode);   //如果是'0'高位的0就会显示，如果是' '高位为0就不会显示
				continue;
			}else enshow=1; 		 	 
		}
	 	LCD_ShowChar(x+(size/2)*t,y,temp+'0',size,mode); 
	}
} 
/*******************************************************************************
   画圆
参数： x :  圆心坐标 
       y：  圆心坐标 
       r：  圆半径
*******************************************************************************/
void circle(unsigned char c_x,unsigned char c_y, unsigned char r)
{
  unsigned int w=0;
  unsigned char x,y;
  float f_x=0,f_y=0;
  unsigned short temp_16;
  
  for(w=0;w<360;w++)
  {
    if(w<90)
    {
      f_x = c_x + r*cos(PI*w/180);      
      f_y = c_y + r*sin(PI*w/180);      
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawPoint(x,y);
    }
    else if(w<180)
    {
      f_x = c_x - r*cos(PI*(180-w)/180);   //PI/2 - W
      f_y = c_y + r*sin(PI*(180-w)/180);
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawPoint(x,y);
    }
    else if(w<270)
    {
      f_x = c_x - r*cos(PI*(w-180)/180);
      f_y = c_y - r*sin(PI*(w-180)/180);
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawPoint(x,y);
    }
    else if(w<360)
    {
      f_x = c_x + r*cos(PI*(w-270)/180);
      f_y = c_y - r*sin(PI*(w-270)/180);
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawPoint(x,y);
    }
  }
}
      

/*******************************************************************************
   钟表

参数：
       c_x;  圆心X坐标
       c_y;  圆心Y坐标
       r;    圆半径
       h :  时
       m：  分
       s：  秒
*******************************************************************************/
void clock(unsigned char c_x,unsigned char c_y,unsigned char r,unsigned char h,unsigned char m,unsigned char s)
{
  unsigned char i, x,y,finger1=5,finger2 = 3;
  float f_w,f_x=0,f_y=0; 
  unsigned short temp_16;
   
  //clr_lcd_map();
  
  circle(c_x,c_y,r);  //画圆
  circle(c_x,c_y,1);  //画圆
  circle(c_x,c_y,2);  //画圆
  
  DrawLine(c_x,c_y+r,c_x,c_y+r-finger1,1);
  DrawLine(c_x,c_y-r,c_x,c_y-r+finger1,1);
  DrawLine(c_x+r,c_y,c_x+r-finger1,c_y,1);
  DrawLine(c_x-r,c_y,c_x-r+finger1,c_y,1);
  
  for(i=1;i<15;i++)
  {
   f_w = PI*6*i;
   f_w /= 180;
   DrawLine(c_x+r*sin(f_w),c_y+r*cos(f_w),c_x+(r-finger2)*sin(f_w),c_y+(r-finger2)*cos(f_w),1);
  }
  for(i=16;i<30;i++)
  {
    f_w = PI*(6*i-90);
    f_w /= 180;
    DrawLine(c_x+r*cos(f_w),c_y-r*sin(f_w),c_x+(r-finger2)*cos(f_w),c_y-(r-finger2)*sin(f_w),1);
  }
  for(i=31;i<45;i++)
  {
    f_w = PI*(6*i-180);
    f_w /= 180;
    DrawLine(c_x-r*sin(f_w),c_y-r*cos(f_w),c_x-(r-finger2)*sin(f_w),c_y-(r-finger2)*cos(f_w),1);
  }
  for(i=46;i<60;i++)
  {
    f_w = PI*(6*i-270);
    f_w /= 180;
    DrawLine(c_x-r*cos(f_w),c_y+r*sin(f_w),c_x-(r-finger2)*cos(f_w),c_y+(r-finger2)*sin(f_w),1);
  }
  
  
  for(i=0;i<3;i++)
  {
    switch(i)
    {
    case 0:       //秒
      f_w = 6*s;
      r -= finger1;
      break;
    case 1:       //分
      f_w = s;
      f_w *= 6;
      f_w /= 60;
      f_w += 6*m;
      
     // f_w = 6*m;
     //f_w += 6*s/360;
      r -= (finger1+2);
      break;
    case 2:       //时
      h %= 12;
      f_w = m;
      f_w *= 30;
      f_w /= 60;
      f_w += 30*h;
      
      //f_w = 30*h;
      //f_w += 30*m/360;
      r -= (finger1+4);
      break;
    default:
      break;
    }   
  
  if(f_w<90)
   {
      f_y = c_y + r*cos(PI*f_w/180);      
      f_x = c_x + r*sin(PI*f_w/180);      
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawLine(c_x,c_y,x,y,1);
    }
    else if(f_w<180)
    {
      f_x = c_x + r*cos(PI*(f_w-90)/180);   //PI/2 - W
      f_y = c_y - r*sin(PI*(f_w-90)/180);
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawLine(c_x,c_y,x,y,0);
    }
    else if(f_w<270)
    {
      f_y = c_y - r*cos(PI*(f_w-180)/180);
      f_x = c_x - r*sin(PI*(f_w-180)/180);
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
     DrawLine(c_x,c_y,x,y,1);
    }
    else if(f_w<=360)
    {
      f_x = c_x - r*cos(PI*(f_w-270)/180);
      f_y = c_y + r*sin(PI*(f_w-270)/180);
      x = (unsigned char)f_x;
      y = (unsigned char)f_y;
      temp_16 = (unsigned short)(f_x*10);
      if(temp_16%10>=5)
        x += 1;
      temp_16 = (unsigned short)(f_y*10);
      if(temp_16%10>=5)
        y += 1;
      DrawLine(c_x,c_y,x,y,1);
    }
  }   
  //fill_lcd(); 
}









		  






