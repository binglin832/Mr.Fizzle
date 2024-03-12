#include "24c02.h" 
#include "delay.h"
#include "myiic.h"
#include "usart.h"//使用printf("内容\r\n");必须要包含这个
/***************************************************************************
- 功能描述：STM32f103 EEPORM初始化函数
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：无
- 返回说明：无
- 函数功能：实现I2C的初始化。
***************************************************************************/


void AT24CXX_Init(void)//24CXX初始化和检测通讯是否正常
{
	IIC_Init();
		
	if(AT24CXX_Check())//检测不到24c02
	{
		 printf("24C02初始化失败\r\n");
	}
else printf("24C02初始化成功\r\n");  
}
/***************************************************************************
- 功能描述：STM32f103 任意读取EEPORM一个字节的函数
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：ReadAddr为所读取EEPROM字节的地址
- 返回说明：无
- 函数功能：返回所要读取的地址的数据
- 函数说明：要任意读取EEPROM地址中的一个字节的步骤为:
(1)发送开始START信号
(2)发送器件器件地址，最后一位为0
(3)从机检测到主机发送的地址与自己的地址相同时发送一个应答信号ACK，而主机等待ACK
(4)发送待操作的字节地址
(5)等待一个ACK
(6)重新发送START起始信号
(7)发送7位器件地址，最后一位为1，表示读EEPROM。
(8)等待ACK，读取一个字节。
(9)I2C停止，返回读取的数据。
这里注意AT24C0x系列的地址有两字节和一字节之分，这里把两种情况都包含了
***************************************************************************/
u8 AT24CXX_ReadOneByte(u16 ReadAddr) 
{ 
u8 temp=0; 
IIC_Start(); 
if(EE_TYPE>AT24C16)
{
IIC_Send_Byte(AT24C02WriteAddress); //发送写命令
IIC_Wait_Ack();
IIC_Send_Byte(ReadAddr>>8);//发送高地址
IIC_Wait_Ack(); 
}
else 
IIC_Send_Byte(AT24C02WriteAddress+((ReadAddr/256)<<1)); //发送器件地址0XA0,写数据

IIC_Wait_Ack();

IIC_Send_Byte(ReadAddr%256); //发送低地址
IIC_Wait_Ack(); 
IIC_Start(); 
IIC_Send_Byte(AT24C02ReadAddress); //进入接收模式 
IIC_Wait_Ack(); 
temp=IIC_Read_Byte(0); 
IIC_Stop();//产生一个停止条件 
return temp;
}

/***************************************************************************
- 功能描述：STM32f103 任意写EEPORM一个字节的函数
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：WriteAddr :写入数据的目的地址 ，DataToWrite:要写入的数据
- 返回说明：无
- 函数功能：返回所要读取的地址的数据
- 函数说明：要任意读取EEPROM地址中的一个字节的步骤为:
(1)发送开始START信号
(2)发送器件器件地址，最后一位为0
(3)从机检测到主机发送的地址与自己的地址相同时发送一个应答信号ACK
(4)发送待操作的字节地址
(5)等待一个ACK
(6)发送字节数据
(7)等待一个ACK
(8)I2C停止，返回读取的数据。
***************************************************************************/
void AT24CXX_WriteOneByte(u16 WriteAddr,u8 DataToWrite)
{ 
IIC_Start(); 
if(EE_TYPE>AT24C16)
{
IIC_Send_Byte(AT24C02WriteAddress); //发送写命令
IIC_Wait_Ack();
IIC_Send_Byte(WriteAddr>>8);//发送高地址
IIC_Wait_Ack(); 
}
else
{
IIC_Send_Byte(AT24C02WriteAddress+((WriteAddr/256)<<1)); //发送器件地址0XA0,写数据 
} 
IIC_Wait_Ack(); 
IIC_Send_Byte(WriteAddr%256); //发送低地址
IIC_Wait_Ack(); 
IIC_Send_Byte(DataToWrite); //发送字节 
IIC_Wait_Ack(); 
IIC_Stop();//产生一个停止条件 
delay_ms(10); 
}


/***************************************************************************
- 功能描述：STM32f103 任意写EEPORM指定地址开始写入长度为Len的数据
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：WriteAddr :写入数据的目的地址 ，DataToWrite:数据数组首地址
Len:要写入数据的长度2,4
- 返回说明：无
- 函数功能：无
- 函数说明：就是调用len次 AT24CXX_WriteOneByte函数，该函数用于写入16bit或者32bit的数据.
***************************************************************************/
void AT24CXX_WriteLenByte(u16 WriteAddr,u32 DataToWrite,u8 Len)
{ 
u8 t;
for(t=0;t<Len;t++)
{
AT24CXX_WriteOneByte(WriteAddr+t,(DataToWrite>>(8*t))&0xff);
} 
}
/***************************************************************************
- 功能描述：STM32f103 任意写EEPORM指定地址开始读出长度为Len的数据
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：ReadAddr :开始读出的地址
Len:要写入数据的长度2,4
- 返回说明：无
- 函数功能：无
- 函数说明：就是调用len次AT24CXX_ReadOneByte函数，该函数用于读出16bit或者32bit的数据.
***************************************************************************/
u32 AT24CXX_ReadLenByte(u16 ReadAddr,u8 Len)
{ 
u8 t;
u32 temp=0;
for(t=0;t<Len;t++)
{
temp<<=8;
temp+=AT24CXX_ReadOneByte(ReadAddr+Len-t-1); 
}
return temp; 
}
/***************************************************************************
- 功能描述：STM32f103检查AT24CXX是否正常
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：无
- 返回说明：返回1:检测失败 返回0:检测成功
- 函数功能：无
- 函数说明：这里用了24XX的最后一个地址(255)来存储标志字. 返回0表示正常、返回1错误
***************************************************************************/
u8 AT24CXX_Check(void)
{
u8 temp;
temp=AT24CXX_ReadOneByte(255);//避免每次开机都写AT24CXX 
if(temp==55)
return 0; 
else//排除第一次初始化的情况
{
AT24CXX_WriteOneByte(255,55);
temp=AT24CXX_ReadOneByte(255); 	
if(temp==55)
return 0;
}

return 1;

}

/***************************************************************************
- 功能描述：STM32f103在AT24CXX里面的指定地址开始读出指定个数的数据
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：ReadAddr :开始读出的地址 对24c02为0~255
pBuffer :数据数组首地址
NumToRead:要读出数据的个数
- 返回说明：无
- 函数功能：无
***************************************************************************/
void AT24CXX_Read(u16 ReadAddr,u8 *pBuffer,u16 NumToRead)
{
while(NumToRead)
{
*pBuffer++=AT24CXX_ReadOneByte(ReadAddr++); 
NumToRead--;
}
}

/***************************************************************************
- 功能描述：STM32f103在AT24CXX里面的指定地址开始写入指定个数的数据
- 隶属模块：STM32 EEPROM操作
- 函数属性：外部，使用户使用
- 参数说明：ReadAddr :开始读出的地址 对24c02为0~255
pBuffer :数据数组首地址，所读到的数据存储在了pBuffer中
NumToRead:要读出数据的个数
- 返回说明：无
- 函数功能：无
***************************************************************************/

void AT24CXX_Write(u16 WriteAddr,u8 *pBuffer,u16 NumToWrite)
{
while(NumToWrite--)
{
AT24CXX_WriteOneByte(WriteAddr,*pBuffer);
WriteAddr++;
pBuffer++;
}
}
