#ifndef __ST7565_H
#define __ST7565_H

#include "stm32f10x.h"


#define LCDBK PBout(9)

#define lcd_cs_high() GPIO_SetBits(GPIOC, GPIO_Pin_15);     
#define lcd_cs_low()  GPIO_ResetBits(GPIOC, GPIO_Pin_15); 

#define lcd_rst_high() GPIO_SetBits(GPIOC, GPIO_Pin_14);     
#define lcd_rst_low()  GPIO_ResetBits(GPIOC, GPIO_Pin_14); 

#define lcd_sck_high() GPIO_SetBits(GPIOB, GPIO_Pin_6);     
#define lcd_sck_low()  GPIO_ResetBits(GPIOB, GPIO_Pin_6); 

#define lcd_sda_high() GPIO_SetBits(GPIOB, GPIO_Pin_7);     
#define lcd_sda_low()  GPIO_ResetBits(GPIOB, GPIO_Pin_7);

#define lcd_a0_high() GPIO_SetBits(GPIOC, GPIO_Pin_13);     
#define lcd_a0_low()  GPIO_ResetBits(GPIOC, GPIO_Pin_13); 



//*******************************LCD控制用函数
void St7565_WriteCmd(uint8_t com);//LCD写命令   
void St7565_WriteDat(uint8_t dat);    
void Init_St7565(void);   //LCD初始化
void ClearDisplay(void);  //清屏全屏 
void ClearDisplayHF(void);    //清部分屏  

void display_128x64(u8 page,u8 column,const unsigned char *dp);
void Ndisplay_8x8(u8 disp_flag,u8 page,u8 address, const unsigned char *hz,unsigned char kaici);
void Ndisplay_8x16(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici);
void Ndisplay_16x16(u8 disp_flag,u8 page,u8 address, const unsigned char *hz,unsigned char kaici);
void Ndisplay_16x32(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici);
void Ndisplay_32x32(u8 disp_flag,u8 page,u8 address, const  unsigned char *hz,unsigned char kaici);

void LCD_Show_Full_Screen_Pic(unsigned char *p);

#endif
