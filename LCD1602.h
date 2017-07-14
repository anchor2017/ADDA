/*变量的定义,位声明和函数的声明放在头文件中，函数的定义放在C文件中*/

#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//LCD1602引脚定义
sbit RS=P3^5;
sbit RW=P3^6;
sbit EN=P3^4;

//LCD液晶忙状态判断，也就是读写检测STA7位的状态
void LCD_readBusy();

//LCD写指令
void LCD_writeCmd(uchar cmd);

//LCD写数据	  
//当输入LCD_write('0'+dat) 表示将单个字符写入光标之后
void LCD_writeDat(uchar dat);

//LCD液晶初始化
void LCD_init();

//LCD写入一个字符
void LCD_writeChar(uchar x, uchar y, uchar dat);

//LCD写入两个字节 uint
void LCD_writeTwoChar(uchar x, uchar y, uint dat);

//LCD写入一个字符串    自变量：显示x轴(0-39)，显示y轴(0-1)，显示字符串
//由于指针地址最大的67H是0110 0111,而80H是1000 0000，所以可以用按位或简单地实现相加，直接用+号应该也可以
void LCD_writeString(uchar x, uchar y, uchar *string);