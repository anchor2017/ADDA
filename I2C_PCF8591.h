/*�����Ķ���ͺ�������������ͷ�ļ��У������Ķ������C�ļ���*/

#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//I2C���ų�ʼ��
#define PCF8591addr 0x90
sbit SDA=P2^0;  //��������
sbit SCL=P2^1;  //ʱ������

//5us��ʱ����
void delay_5us();

//I2C��ʼ��
void I2C_init();

//I2C��ʼ�ź�
void I2C_Start();

//I2C��ֹ�ź�
void I2C_Stop();

//��������Ӧ��
void Master_ACK(bit i);

//���ӻ�Ӧ��
bit Test_ACK();

//����һ���ֽ�
void I2C_send_byte(uchar byte);

//I2C ��һ�ֽ�
uchar I2C_read_byte();

//I2C�����������ݣ�ÿһ��д�����ݶ���Ҫ���ӻ�Ӧ��
//ÿ��ʹ���������ֻ��д��һ�ֽڵ�����
//�ȷ���E2PROM��ַ���ٷ�������
bit I2C_transData(uchar ADDR, DAT);	

//I2C������������
//������ȡE2PROM���ݣ�ÿ��ֻ�ܴ�E2PROM���ض���ַ��ȡһ�ֽ����ݣ������ظ�����
uchar I2C_receData(uchar ADDR);

//LCD��ʾ����ǿ��
void PlightDisplay();