/*���ڵ����߶�ʱ���Ҫ��Ƚ��ϸ������õ���ʱ���Ծ���11.0592MHZΪ��׼�����*/
/*�����Ķ���,λ�����ͺ�������������ͷ�ļ��У������Ķ������C�ļ���*/

#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//DS18B20���������Ŷ���
sbit DQ = P2^2;

//��ʱ6.5us��11.95us����
void Delay_us(uchar us);

//������ʱ����
void delayMs(uint z);

//DS18B20�¶ȴ�����ʹ�õ�����ͨ��
//DS18B20��ʼ��ʱ��
bit DS18B20_init();

//DS18B20�¶ȴ�������һ�ֽ� ʱ��
uchar DS18B20_readByte();

//DS18B20�¶ȴ�����д��һ�ֽ� ʱ��
void DS18B20_writeByte(uchar dat);

//�¶ȴ������ȴ�
//�ж�DS1802�Ƿ��з���
void DS18B20_wait();	   

//DS18B20 ������DS18B20���κ�ͨѶǰ����Ҫ�����ʼ��
//DS18B20 �����¶�ת��ָ��
void DS18B20_sendChangeCmd();

//DS18B20 ���Ͷ�����ָ��
void DS18B20_sendReadCmd();

//DS18B20 ȡ����ʵ�¶ȵĺ���
int DS18B20_TemToDat();

//DS18B20�õ����¶ȵ�����ת��Ϊ��������+С�����ֵ�����
void DS18B20_TemToDisplay();