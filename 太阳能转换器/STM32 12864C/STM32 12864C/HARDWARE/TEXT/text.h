#ifndef __TEXT_H__
#define __TEXT_H__	 
#include <stm32f10x.h>
#include "fontupd.h"
//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////
//��Ļ�ĳ������
#define LCD_LENGTH 128 //128
#define LCD_WIDTH   64   //64
#define PI  3.34159
//DrawChunk()����������Ļʵ�������д

//�Լ������ļ���GUI�������ڵ�ɫ������

/*****************************ͼ�����**************************/
extern unsigned char  GraphRAM[];


void CleanScreen(void);
void DrawPoint(unsigned char x, unsigned char y);
void CleanPoint(unsigned char x, unsigned char y);
unsigned char ReadPoint(unsigned char x, unsigned char y);
void DrawChunk(unsigned char column, unsigned char page, unsigned char dat);//����8�ֽ�Ϊһ����
void DrawLine(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color);
void DrawRectangle(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2, unsigned char color);
void InverseColor(unsigned char x1, unsigned char y1, unsigned char x2, unsigned char y2);
void ShowAllGraphRAM(void);//��Ҫ���ݵ�Ƭ������޸�
void DrawPic(unsigned char x, unsigned char y, unsigned char length, unsigned char width, unsigned char *pic);
void MsgBox(unsigned char x, unsigned char y, char *p);//��ʾһ���̶�λ�õĴ󴰿ڣ���ʾ�ַ�
void MsgBoxSimple(unsigned char x, unsigned char y, char *p);//����λ�ã���ʾ�ַ�
void ShowMsgSimple(char *p, u16 t);
void LCD_ShowNum(u16 x,u16 y,u32 num,u8 len,u8 size,u8 mode);//��ʾ����


void LCD_zimu(u16 x,u16 y,u8 size,u8 mode,const  unsigned char *hz,unsigned char kaici);


					     
void Get_HzMat(unsigned char *code,unsigned char *mat,u8 size);			//�õ����ֵĵ�����
void Show_Font(u16 x,u16 y,u8 *font,u8 size,u8 mode);					//��ָ��λ����ʾһ������
void Show_Str(u16 x,u16 y,u16 width,u16 height,u8*str,u8 size,u8 mode);	//��ָ��λ����ʾһ���ַ��� 
void Show_Str_Mid(u16 x,u16 y,u8*str,u8 size,u8 len);
void LCD_ShowChar(u16 x,u16 y,u8 num,u8 size,u8 mode);

void circle(unsigned char c_x,unsigned char c_y, unsigned char r);//��Բ
void clock(unsigned char c_x,unsigned char c_y,unsigned char r,unsigned char h,unsigned char m,unsigned char s);

#endif

