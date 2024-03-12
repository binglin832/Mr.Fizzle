#ifndef __INA226_H
#define __INA226_H
#include "sys.h"


/////////////////////////////////////////////////////////////////
/////														/////
/////////////////////////////////////////////////////////////////

#define 	CFG_REG	 		0x00 //===采样电阻上的压降---[0x00000],满量程是81.92mV,最小单位是2.5uV
#define 	SV_REG 			0x01 //===总线电压---[0x00000],满量程是40.96V,最小单位是1.25mV
#define 	BV_REG 			0x02 //===功率寄存器---[0x00000]
#define 	PWR_REG 		0x03 //===电流寄存器---[0x00000]
#define 	CUR_REG 		0x04 //===校准寄存器---[0x00000]
#define 	CAL_REG 		0x05 //===功能控制寄存器---[0x00000]
#define 	ONFF_REG 		0x06 //===设置的报警阈值---[0x00000]
#define 	AL_REG 			0x07 //===制造商ID---[0x5449]
#define   INA226_ADDR1	    0x8A //全部上拉  0X8A     全部到地  0X80     需要在地址7位后面加上0！！！！！    
#define   INA226_GETALADDR	0x14 


void INA226_Init(void);
void INA226_SetRegPointer(u8 addr,u8 reg);
void INA226_SendData(u8 addr,u8 reg,u16 data);

u16 INA226_ReadData(u8 addr);
u8	INA226_AlertAddr(void);
u16 INA226_GetVoltage(u8 addr);
u16 INA226_GetCurrent(u8 addr);
u16 INA226_GetShuntVoltage(u8 addr);


#endif

