#ifndef __INA226_H
#define __INA226_H
#include "sys.h"


/////////////////////////////////////////////////////////////////
/////														/////
/////////////////////////////////////////////////////////////////

#define 	CFG_REG	 		0x00 //===���������ϵ�ѹ��---[0x00000],��������81.92mV,��С��λ��2.5uV
#define 	SV_REG 			0x01 //===���ߵ�ѹ---[0x00000],��������40.96V,��С��λ��1.25mV
#define 	BV_REG 			0x02 //===���ʼĴ���---[0x00000]
#define 	PWR_REG 		0x03 //===�����Ĵ���---[0x00000]
#define 	CUR_REG 		0x04 //===У׼�Ĵ���---[0x00000]
#define 	CAL_REG 		0x05 //===���ܿ��ƼĴ���---[0x00000]
#define 	ONFF_REG 		0x06 //===���õı�����ֵ---[0x00000]
#define 	AL_REG 			0x07 //===������ID---[0x5449]
#define   INA226_ADDR1	    0x8A //ȫ������  0X8A     ȫ������  0X80     ��Ҫ�ڵ�ַ7λ�������0����������    
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

