#ifndef __TEXT_H__
#define __TEXT_H__	 
#include <stm32f10x.h>
#include "fontupd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//屏幕的长宽参数
#define LCD_LENGTH 128 //128
#define LCD_WIDTH   64   //64
#define PI  3.34159
//DrawChunk()函数需结合屏幕实际情况编写

//自己制作的简易GUI，仅限于单色点阵屏

/*****************************图形相关**************************/
extern unsigned char  GraphRAM[];


void CleanScreen(void);
void DrawPoint(unsigned char x, unsigned char y);
void CleanPoint(unsigned char x, unsigned char y);
unsigned char ReadPoint(unsigned char x, unsigned char y);
void DrawChunk(unsigned char column, unsigned char page, unsigned char dat);//纵向8字节为一个块
void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color);
void DrawRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color);
void InverseColor(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void ShowAllGraphRAM(void);//需要根据单片机情况修改
void DrawPic(unsigned char x, unsigned char y, unsigned char length, unsigned char width, unsigned char *pic);
void MsgBox(unsigned char x, unsigned char y, char *p);//显示一个固定位置的大窗口，提示字符
void MsgBoxSimple(unsigned char x, unsigned char y, char *p);//窗口位置，提示字符
void ShowMsgSimple(char *p, u16 t);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//显示数字


void LCD_zimu(u16 x,u16 y,u8 size,u8 mode,const  unsigned char *hz,unsigned char kaici);


					     
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//得到汉字的点阵码
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//在指定位置显示一个汉字
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//在指定位置显示一个字符串 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);

void circle(unsigned char c_x,unsigned char c_y, unsigned char r);//画圆
void clock(unsigned char c_x,unsigned char c_y,unsigned char r,unsigned char h,unsigned char m,unsigned char s);

#endif

