#ifndef __24C02_H
#define __24C02_H
#include "myiic.h"

/***************************************************************
- 说明：以下参数是AT24Cxx的寻址空间，C0x ,X 表示XK 如C01表示1K
- 127表示2^7 1Kbit/8=128Byte 128字节
- 255表示2^8 2Kbit/8=256Byte 256字节 
- 512表示2^9 4Kbit/8=512Byte 512字节 
- 
***************************************************************/
#define AT24C01 127
#define AT24C02 255
#define AT24C04 511
#define AT24C08 1023
#define AT24C16 2047
#define AT24C32 4095
#define AT24C64 8191
#define AT24C128 16383
#define AT24C256 32767 
/**************************************************************
--板子使用的是24c02，所以定义EE_TYPE为AT24C0

=========================================24C02和24C32不可直接调换 =================================

**************************************************************/
#define EE_TYPE AT24C02

/***************************************************************
24C02地址说明
A0，A1，A2都是接的VCC，所以为A0=1，A1=1，A2=1；可以知道AT24C02的从设备写地址为10101110(0xae)，读设备地址为10101111（0xaf）；
A0，A1，A2都是接的GND，所以为A0=0，A1=0，A2=0；可以知道AT24C02的从设备写地址为10100000(0xa0)，读设备地址为10100001（0xa1）

1010（A0）（A1）（A2）（R/W）
**************************************************************/
#define	  AT24C02WriteAddress 0xA0	  //定义器件在IIC总线中的地址  a0 
#define	  AT24C02ReadAddress  0xA1     // a1



/**************************************************************
--EEPROM的操作函数
--24CXX驱动函数
**************************************************************/

u8 AT24CXX_ReadOneByte(u16 ReadAddr); //指定地址读取一个字节
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite); //指定地址写入一个字节

void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len);//指定地址开始写入指定长度的数据
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len); //指定地址开始读取指定长度数据

void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite); //从指定地址开始写入指定长度的数据
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead); //从指定地址开始读出指定长度的数据

u8 AT24CXX_Check(void); //检查器件
void AT24CXX_Init(void); //24CXX初始化和检测是否正常

#endif
