#ifndef __CW2015_H
#define __CW2015_H
#include "myiic.h"   
#include "sys.h"


#define CW_ReadAddr     0xC5    //������
#define CW_WriteAddr 	0XC4	//д����		

#define VERSION     0x00  // ��  оƬID

#define VCELL_H 	0x02  // ��  14λADC��ص�ѹ��λ
#define VCELL_L 	0x03  // ��  14λADC��ص�ѹ��λ

#define SOC_B       0x04  // ��  �ṩ��%Ϊ��λ�ĵ�������
#define SOC         0x05  // ��  ����ȷ�ĵ�����ʾ1/256% 

#define RRT_H       0x06  // ��  ʹ��ʣ��ʱ���λ
#define RRT_L       0x07  // ��  ʹ��ʣ��ʱ���λ
                              //[15]ALRT SOC�Ĵ�����ֵ������־λ��ֻ����IC�����
                              //[12-0]ʣ��ʱ�� ���ݵ�ǰ���ݼ���ʣ��ʱ�� ��λ����
#define CONFIG      0x08  // ��д      Ĭ�ϣ�0x50 10%   0xA0 20%
                          //         -3]SOC������ֵ���� ��ALRT�����ж�  
															//  [1]UPG ����ָʾ�����Ϣ����״̬��־λ
#define MOOD        0x0A  // ��д      Ĭ�ϣ�0x00
                          // [76]˯��ģʽ       2λ����  11ǿ�ƽ���˯��ģʽ  Ĭ�ϣ�00����
						  // [54]QSTAT��������  2λ����  11��ʼ              Ĭ�ϣ�00
						  //[3-0]��Դ��λ       4λ����  1111��λ            Ĭ�ϣ�0000

//#define REG_VTEMPL              0x0C
//#define REG_VTEMPH              0x0D
//#define REG_BATINFO             0x10





u8 CW2015_ReadOneByte(u8 ReadAddr);
u8 CW2015_WriteOneByte(u8 WriteAddr,u8 Data);
void CW2015_Init(void);
u8 CW2015_ID(void);                      //��CW2015��ID
u16 CW2015_Vcell(void);                  //����ص�ѹ
u8 CW2015_Soc(u8 mode);                  //����ص���
u16 CW2015_Time(void);                   //��ȡʣ��ʹ��ʱ�� ����
#endif
















