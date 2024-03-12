#include <light.h>
 #include "adc.h"
 
 
 
u8 Get_light(void)
{
	u16 adcx;

		adcx=Get_Adc_Average(ADC_Channel_0,20);
		return (u8)adcx>>2;
}
 
 

