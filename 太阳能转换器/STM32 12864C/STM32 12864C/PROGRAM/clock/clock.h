#ifndef __clock_H
#define __clock_H	 
#include "sys.h"



void getdata(void);          //��ȡ��Ҫ��ʾ��ʹ�õ���Ϣ



void dht11disp(void);     //��ʾ�¶�ʪ��
void cw2015disp(void);    //��ʾCW2015���
void ina226disp(void);    //��ʾIN1226���

void dispmaohao (void);   //��ʾð��
void disptime1 (void);    //��ʾʱ�����1
void shijianset (void);   //ʱ������
void blltset (void);      //������������
void bltimeset (void);    //����ʱ������0һֱ��
void crset (void);        //�Աȶ�����
void menutimeset (void);  //�˵�ʱ������

void dingshiset1(void);   //��ʱ����1


#endif

