#include <reg52.h>
#include <intrins.h>

#include "DS18B20T.h"
#include "I2C_PCF8591.h"
#include "LCD1602.h"

#define uint unsigned int
#define uchar unsigned char

//����ܶ�ѡλѡ���� ��Ҫ��Ϊ�˷�ֹ��������ܵ�Ӱ����ʾ
sbit DU=P2^6;
sbit WE=P2^7;

void main()
{
	uchar Tstr[]={"T:"};
	uchar Pstr[]={"P:"};
	uchar blank[]={" "};
	
	/*��������ܺ�LCD���ö˿ڣ��ر�����ܵĲ���*/
	DU=1;	
	P0=0;
	DU=0;	
	WE=1;
	P0=0x7f;
	WE=0;

	/*��ʼ��*/
	I2C_init();
	LCD_init();
	LCD_writeString(2, 0, Tstr);	//�¶���ĸ��ʾ
	LCD_writeString(2, 1, Pstr);	//������ĸ��ʾ

	/*�¶�ת��*/
	DS18B20_init();				  //��ʼ��DS18B20
	DS18B20_writeByte(0xcc);	  //������ԾROMָ��
	DS18B20_writeByte(0x44);	  //�¶�ת��ָ��

	while(1)
	{
		delayMs(1000);			 //�¶�ת����Ҫһ����ʱ��

		/*LCD��ʾ*/
		DS18B20_TemToDisplay();	 //�¶���ʾ����
		PlightDisplay();		 //������ʾ��������ʼ����ֵΪ28
	
		/*�¶�ת��*/
		DS18B20_init();				  //��ʼ��DS18B20
		DS18B20_writeByte(0xcc);	  //������ԾROMָ��
		DS18B20_writeByte(0x44);	  //�¶�ת��ָ��

	}	
}