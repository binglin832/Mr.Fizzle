#include "ina226.h"
#include "myiic.h"
#include "usart.h"

/////////////
//2毫欧方案//   
////////////

/////////////
//1毫欧方案//
////////////

//Configuration Register 00h  ===  0x43FF === D15-RST;D12:D14-000 ; D9:D11(AVG)-001 ; D6:D8(VBCT)-111 ; D3:D5(VSHCT)-111 ; D0-D2(MODE)-111
//Shunt Voltage Register 01h (Read-Only)2.5uV/bit  ===  D15:正负标志 ; D0:D14-DATA ; 负数:二进制补码 +1
//Bus Voltage Register 02h (Read-Only)1.25mV/bit  ===  d15:0 ; D0:D14 - DATA ; 不支持负电压检测
//Power Register 03h (Read-Only) --- 25*Current_LSB/bit === D0:D15 - DATA
//Current Register 04h (Read-Only)1mA/bit === d15:0 ; D0:D14 - DATA 
//Calibration Register 05h (Read/Write) === 0xA00 === d15:0 ; D0:D14 - DATA 
//Mask/Enable 06h (Read/Write) === 0x0000H ===
//Bit 15 - SOL: Shunt Voltage Over-Voltage  
//Bit 14 - SUL: Shunt Voltage Under-Voltage 
//Bit 13 - BOL: Bus Voltage Over-Voltage 
//Bit 12 - BUL: Bus Voltage Under-Voltage 
//Bit 11 - POL: Over-Limit Power
//Bit 10 - CNVR: Conversion Read 
//Bit 4 - AFF: Alert Function Flag 
//Bit 3 - CVRF: Conversion Ready Flag 
//Bit 2 - OVF: Math Overflow Flag 
//Bit 1 - APOL: Alert Polarity bit; sets the Alert pin polarity.
//Bit 0 - LEN: Alert Latch Enable; configures the latching feature of the Alert pin and Flag bits.
//Alert Limit 07h (Read/Write) === 0x0000H === D0:D15 - DATA 
//INA226 ADDR : 1000000 


/////////////
//50毫欧方案//
////////////
void INA226_Init(void)
{		
	INA226_SendData(INA226_ADDR1,CFG_REG,0x43FF);
	INA226_SendData(INA226_ADDR1,CAL_REG,1024);
}


void INA226_SetRegPointer(u8 addr,u8 reg)
{
	IIC_Start();

	IIC_Send_Byte(addr);
	IIC_Wait_Ack();

	IIC_Send_Byte(reg);
	IIC_Wait_Ack();

	IIC_Stop();
}

void INA226_SendData(u8 addr,u8 reg,u16 data)
{
	u8 temp=0;
	IIC_Start();

	IIC_Send_Byte(addr);
	IIC_Wait_Ack();

	IIC_Send_Byte(reg);
	IIC_Wait_Ack();
	
	temp = (u8)(data>>8);
	IIC_Send_Byte(temp);
	IIC_Wait_Ack();

	temp = (u8)(data&0x00FF);
	IIC_Send_Byte(temp);
	IIC_Wait_Ack();
	
	IIC_Stop();
}

u16 INA226_ReadData(u8 addr)
{
	u16 temp=0;
	IIC_Start();

	IIC_Send_Byte(addr+1);
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);
	temp<<=8;	
	temp |= IIC_Read_Byte(0);
	
	IIC_Stop();
	return temp;
}

u8 INA226_AlertAddr(void)
{
	u8 temp;
	IIC_Start();

	IIC_Send_Byte(INA226_GETALADDR);
	IIC_Wait_Ack();
	
	temp = IIC_Read_Byte(1);
	
	IIC_Stop();
	return temp;
}

//1mA/bit
u16 INA226_GetCurrent(u8 addr)
{
	u16 temp=0;	
	INA226_SetRegPointer(addr,SV_REG);
	temp = INA226_ReadData(addr);
	printf("电流= %d \r\n",temp);	
//	if(temp&0x8000)
//	{
//		temp = ~(temp - 1);	
//		temp = (temp*25 + 5) / 10 / 5;		
//	}
//	else
//	{
//		INA226_SetRegPointer(addr,CUR_REG);
//		temp = INA226_ReadData(addr);
//		temp = temp&0x7fff;
//	}
	
	return temp;
}

//1.25mV/bit
u16 INA226_GetVoltage(u8 addr)
{
	u32 temp=0;
	INA226_SetRegPointer(addr,BV_REG);
	temp = INA226_ReadData(addr);
	temp = (temp*125 + 50)/100;
	return (u16)temp;	
}

//2.5uV/bit
u16 INA226_GetShuntVoltage(u8 addr)
{
	u32 temp=0;
	INA226_SetRegPointer(addr,SV_REG);
	temp = INA226_ReadData(addr);
	if(temp&0x8000)	temp = ~(temp - 1);	
	temp = (temp*25 + 5) / 10;
	return (u16)temp;	
}










