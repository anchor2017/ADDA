/*�����Ķ���ͺ�������������ͷ�ļ��У������Ķ������C�ļ���*/

#include <reg52.h>
#include <intrins.h>

#include "LCD1602.h"

#define uint unsigned int
#define uchar unsigned char

//LCDҺ��æ״̬�жϣ�Ҳ���Ƕ�д���STA7λ��״̬
void LCD_readBusy()
{
	uchar busy;
	P0=0xff;	//��λ����
	RS=0;
	RW=1;
	do
	{
		EN=1;
		busy=P0;
		EN=0;
	}while(busy&0x80);
}

//LCDдָ��
void LCD_writeCmd(uchar cmd)
{
	LCD_readBusy();
	
	RS=0;
	RW=0;
	P0=cmd;
	EN=1;
	EN=0;
}

//LCDд����
//������LCD_write('0'+dat) ��ʾ�������ַ�д����֮��
void LCD_writeDat(uchar dat)
{
	LCD_readBusy();
	RS=1;
	RW=0;
	P0=dat;
	EN=1;
	EN=0;
}

//LCDҺ����ʼ��
void LCD_init()
{
	LCD_writeCmd(0x38);  //16*2��ʾ��5*7����8λ���ݽӿ�
	LCD_writeCmd(0x0c);  //����ʾ������ʾ��꣬��겻��ʾ
	LCD_writeCmd(0x06);  //��д���һ���ַ����ַָ���1�ҹ���1
	LCD_writeCmd(0x01);  //����ָ��
}


//LCDд��һ���ַ�
void LCD_writeChar(uchar x, uchar y, uchar dat)
{
	if(y)			 //y=1 �ڶ���
		x |= 0x40;	
					//y=0 ��һ��
	x |= 0x80;
	
	LCD_writeCmd(x);	  //0x80Ҳ����80H ����ָ�����õ�ָ��
	LCD_writeDat(dat);						
}

//LCDд�������ֽ� uint
void LCD_writeTwoChar(uchar x, uchar y, uint dat)
{
	if(y)			 //y=1 �ڶ���
		x |= 0x40;	 
					//y=0 ��һ��
	x |= 0x80;
	
	LCD_writeCmd(x);	  //0x80Ҳ����80H ����ָ�����õ�ָ��
	LCD_writeDat(dat);						
}


//LCDд��һ���ַ���    �Ա�������ʾx��(0-1)����ʾy��(0-39)����ʾ�ַ���
//����ָ���ַ����67H��0110 0111,��80H��1000 0000�����Կ����ð�λ��򵥵�ʵ����ӣ�ֱ����+��Ӧ��Ҳ����
void LCD_writeString(uchar x, uchar y, uchar *string)
{
	if(y)			 //y=1 �ڶ���
		x |= 0x40;	 

	x |= 0x80;
	
	LCD_writeCmd(x);	  //0x80Ҳ����80H ����ָ�����õ�ָ��
	
	/*LCD_writeDat(*string);*/

	while(*string != '\0')
	{
		LCD_writeDat(*string++);	//ָ��ָ����׵�ַ��Ҳ���ǵ�һ���ַ�������ָ���ַ���ƣ���֮����ַ�	
	}
}