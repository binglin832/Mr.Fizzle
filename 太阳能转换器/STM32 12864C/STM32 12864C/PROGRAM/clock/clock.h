#ifndef __clock_H
#define __clock_H	 
#include "sys.h"



void getdata(void);          //获取需要显示和使用的信息



void dht11disp(void);     //显示温度湿度
void cw2015disp(void);    //显示CW2015相关
void ina226disp(void);    //显示IN1226相关

void dispmaohao (void);   //显示冒号
void disptime1 (void);    //显示时间界面1
void shijianset (void);   //时间设置
void blltset (void);      //背光亮度设置
void bltimeset (void);    //背光时间设置0一直亮
void crset (void);        //对比度设置
void menutimeset (void);  //菜单时间设置

void dingshiset1(void);   //定时设置1


#endif

