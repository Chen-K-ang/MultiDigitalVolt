/************************************************		   			            
*   使用说明： 在需要使用延时函数时添加此头文件“delay.h”
************************************************/
#ifndef __DELAY_H__
#define __DELAY_H__
#include <reg52.h>
//#include <string.h>
#define uchar unsigned char
#define uint  unsigned int
extern void          _nop_     (void);
/************************************************
** 函数名称 ： Delay(uint T)
** 函数功能 ： 延时函数
** 输    入 ： T--延时倍数 大小--0~65535
** 输    出 ： 无
** 说    明 ：
************************************************/
void Delay(uint T)
{
    uchar i;
    for(T;T>0;T--)
        for(i=200;i>0;i--);
}
/************************************************
** 函数名称 ： Delay11us(uint i)
** 函数功能 ： 延时函数
** 输    入 ： T--延时倍数 大小--0~65535
** 输    出 ： 无
** 说    明 ：
************************************************/
void Delay11us(uint T)   
{
    for(;T>0;T--);
}
#endif
