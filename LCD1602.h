/*�����Ķ���,λ�����ͺ�������������ͷ�ļ��У������Ķ������C�ļ���*/

#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//LCD1602���Ŷ���
sbit RS=P3^5;
sbit RW=P3^6;
sbit EN=P3^4;

//LCDҺ��æ״̬�жϣ�Ҳ���Ƕ�д���STA7λ��״̬
void LCD_readBusy();

//LCDдָ��
void LCD_writeCmd(uchar cmd);

//LCDд����	  
//������LCD_write('0'+dat) ��ʾ�������ַ�д����֮��
void LCD_writeDat(uchar dat);

//LCDҺ����ʼ��
void LCD_init();

//LCDд��һ���ַ�
void LCD_writeChar(uchar x, uchar y, uchar dat);

//LCDд�������ֽ� uint
void LCD_writeTwoChar(uchar x, uchar y, uint dat);

//LCDд��һ���ַ���    �Ա�������ʾx��(0-39)����ʾy��(0-1)����ʾ�ַ���
//����ָ���ַ����67H��0110 0111,��80H��1000 0000�����Կ����ð�λ��򵥵�ʵ����ӣ�ֱ����+��Ӧ��Ҳ����
void LCD_writeString(uchar x, uchar y, uchar *string);