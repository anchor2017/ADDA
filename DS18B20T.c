/*���ڵ����߶�ʱ���Ҫ��Ƚ��ϸ������õ���ʱ���Ծ���11.0592MHZΪ��׼�����*/
/*��������ͷ�ļ���һ����DS18B20T,����һ��LCD1602��Һ����ʾ����*/
/*�����Ķ���,λ�����ͺ�������������ͷ�ļ��У������Ķ������C�ļ���*/

#include <reg52.h>
#include <intrins.h>

#include "DS18B20T.h"
#include "LCD1602.h"

#define uint unsigned int
#define uchar unsigned char

//��ʱ6.5us��11.95us����
void Delay_us(uchar us)
{
	while(us--);
}

//������ʱ����
void delayMs(uint z)
{
	uint x, y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--);
}

//DS18B20�¶ȴ�����ʹ�õ�����ͨ��
//DS18B20��ʼ��ʱ��
bit DS18B20_init()
{
	bit i;
	DQ=1;
	_nop_();
	DQ=0;			//��������480us��λ����
	Delay_us(75);
	DQ=1;			//�ͷ�����
	Delay_us(4);   //�ȴ�15-60us   37.95us
	i=DQ;
	Delay_us(20);	 //�ȴ�60-240us 18B20�Ĵ����ź� �͵�ƽ
	DQ=1;		//�ȶ�����
	_nop_();
	return i;
}

//DS18B20�¶ȴ�������һ�ֽ� ʱ��
uchar DS18B20_readByte()
{
	uchar i, j, dat;
	for(i=0;i<8;i++)
	{			
		DQ=0;
		_nop_();   		//1��_nop_()ָ��(12����������)ִ��1.085us��ʱ��	
		DQ=1;
		_nop_();
		j = DQ;
		Delay_us(10);
		DQ=1;	//�ͷ�����
		_nop_();
		dat = (j<<7)|(dat>>1);
	}
	return dat;
}

//DS18B20�¶ȴ�����д��һ�ֽ� ʱ��
void DS18B20_writeByte(uchar dat)
{
	uchar i;
	for(i=0;i<8;i++)
	{
		DQ=0;
		_nop_();
		DQ= dat & 0x01;
		Delay_us(10);
		dat>>=1;
		DQ=1;
		_nop_();
	}	
}

//�¶ȴ������ȴ�
void DS18B20_wait()	   //�ж�DS1802�Ƿ��з���
{
	uint i;
	i=DS18B20_init();
	while(i);	
	/*
	while(DQ);	  //DS18B20ʱ������
	while(~DQ);
	i=4;
	while(i>0)
		i--;
	*/
}

//DS18B20 ������DS18B20���κ�ͨѶǰ����Ҫ�����ʼ��
//DS18B20 �����¶�ת��ָ��
void DS18B20_sendChangeCmd()
{
	DS18B20_init();
	DS18B20_wait();
	delayMs(1);

	DS18B20_writeByte(0xcc);   //����ROMָ��
	DS18B20_writeByte(0x44);   //�¶�ת��ָ��
}

//DS18B20 ���Ͷ�����ָ��
void DS18B20_sendReadCmd()
{
	DS18B20_init();	  
	DS18B20_wait();	  //���ﺯ����û�г���
	delayMs(1);
	
	DS18B20_writeByte(0xcc);   //����ROMָ��
	DS18B20_writeByte(0xbe);   //�¶�ת��ָ��	
}

//DS18B20 ȡ����ʵ�¶ȵĺ���
int DS18B20_TemToDat()
{
	uchar Th, Tl;
	int Tint;	 //ֻ��Ϊ�˱�֤���ȹ��ñ������ ��int������
	uint Tuint;
	float Tfloat; //������С�������¶�ֵ�Ĵ��
  	   
	DS18B20_init();
	DS18B20_writeByte(0xcc);
	DS18B20_writeByte(0xbe);	 
	
	Tl=DS18B20_readByte();	 //���ݴ����Ͱ�λ����
	//P1=Tl;

	Th=DS18B20_readByte();	 //���ݴ����߰�λ����
	//P1=Th;

	Tuint=Th;
	Tuint=Tuint<<8;	
	Tuint= Tuint|Tl;
	Tint=Tuint;

	Tfloat = Tint*0.0625;	   //�������¶�ֵ����������� ���������Ż����¶�ֵ
	Tint = Tfloat*100+0.5;			//Tint*0.0625�ó��Ľ�������Ǵ���С��������ֵ�Ǹ���������������õ���
	//����int��ԭ��Ҳ������������int,*100������ݻ������
							//+0.5ֻ��Ϊ�˺��һλ�Ľ�λ�������Ż����������
	//Tchar=Tfloat*10000;		//��λ����  ��λ�������֣���λС������	һ�㶼���������Ż���

	//Tchar=Tint&0x7f;	//0111 1111  ���ΪTint�ĺ���λ��Ҳ��������λ
	return Tint;			
}

//DS18B20�õ����¶ȵ�����ת��Ϊ��������+С�����ֵ�����
void DS18B20_TemToDisplay()
{
	uchar blank[]={" "};
	uchar i;
	int Tint;	  
	//uchar TemDat[7]={0, 0, 0, 0, 0, 0, 0};		//��λ����
	uchar TemDat[]={0, 0, 0, 0, 0};		   //��λ����	 

	Tint=DS18B20_TemToDat();	  

	TemDat[0]=Tint/10000;
	TemDat[1]=Tint%10000/1000;
	TemDat[2]=Tint%1000/100;

	TemDat[3]=Tint%100/10;
	TemDat[4]=Tint%10;

	LCD_writeString(4, 0, blank);

	for(i=1;i<5;i++)
	{
		LCD_writeDat('0'+TemDat[i]);	   //���������õ�LCD1602�ĺ���������
		if(i==2)
			LCD_writeDat('.');
	}
}