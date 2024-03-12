#ifndef __ds3231_H
#define __ds3231_H 			   
#include "sys.h"  



	 

#define DS3231_WriteAddress 0xD0    //器件写地址 
#define DS3231_ReadAddress  0xD1    //器件读地址
#define DS3231_SECOND       0x00    //秒
#define DS3231_MINUTE       0x01    //分
#define DS3231_HOUR         0x02    //时
#define DS3231_WEEK         0x03    //星期
#define DS3231_DAY          0x04    //日
#define DS3231_MONTH        0x05    //月
#define DS3231_YEAR         0x06    //年






u8 BCD2HEX(u8 val);
u8 HEX2BCD(u8 val);
u8 write_byte(u8 addr, u8 write_data);
u8 read_current(void);
u8 read_random(u8 random_addr);
u8 readdata(void);


void SetTime(u8 year,u8 month,u8 day,u8 hour,u8 min,u8 sec,u8 week);












#endif
