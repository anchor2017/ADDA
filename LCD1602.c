/*变量的定义和函数的声明放在头文件中，函数的定义放在C文件中*/

#include <reg52.h>
#include <intrins.h>

#include "LCD1602.h"

#define uint unsigned int
#define uchar unsigned char

//LCD液晶忙状态判断，也就是读写检测STA7位的状态
void LCD_readBusy()
{
	uchar busy;
	P0=0xff;	//复位数据
	RS=0;
	RW=1;
	do
	{
		EN=1;
		busy=P0;
		EN=0;
	}while(busy&0x80);
}

//LCD写指令
void LCD_writeCmd(uchar cmd)
{
	LCD_readBusy();
	
	RS=0;
	RW=0;
	P0=cmd;
	EN=1;
	EN=0;
}

//LCD写数据
//当输入LCD_write('0'+dat) 表示将单个字符写入光标之后
void LCD_writeDat(uchar dat)
{
	LCD_readBusy();
	RS=1;
	RW=0;
	P0=dat;
	EN=1;
	EN=0;
}

//LCD液晶初始化
void LCD_init()
{
	LCD_writeCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
	LCD_writeCmd(0x0c);  //开显示，不显示光标，光标不显示
	LCD_writeCmd(0x06);  //当写或读一个字符后地址指针加1且光标加1
	LCD_writeCmd(0x01);  //清屏指令
}


//LCD写入一个字符
void LCD_writeChar(uchar x, uchar y, uchar dat)
{
	if(y)			 //y=1 第二行
		x |= 0x40;	
					//y=0 第一行
	x |= 0x80;
	
	LCD_writeCmd(x);	  //0x80也就是80H 数据指针设置的指令
	LCD_writeDat(dat);						
}

//LCD写入两个字节 uint
void LCD_writeTwoChar(uchar x, uchar y, uint dat)
{
	if(y)			 //y=1 第二行
		x |= 0x40;	 
					//y=0 第一行
	x |= 0x80;
	
	LCD_writeCmd(x);	  //0x80也就是80H 数据指针设置的指令
	LCD_writeDat(dat);						
}


//LCD写入一个字符串    自变量：显示x轴(0-1)，显示y轴(0-39)，显示字符串
//由于指针地址最大的67H是0110 0111,而80H是1000 0000，所以可以用按位或简单地实现相加，直接用+号应该也可以
void LCD_writeString(uchar x, uchar y, uchar *string)
{
	if(y)			 //y=1 第二行
		x |= 0x40;	 

	x |= 0x80;
	
	LCD_writeCmd(x);	  //0x80也就是80H 数据指针设置的指令
	
	/*LCD_writeDat(*string);*/

	while(*string != '\0')
	{
		LCD_writeDat(*string++);	//指针指向的首地址，也就是第一个字符，接着指针地址下移，读之后的字符	
	}
}