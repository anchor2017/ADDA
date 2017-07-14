/*由于单总线对时序的要求比较严格，这里用的延时是以晶振11.0592MHZ为标准计算的*/
/*包含两个头文件，一个是DS18B20T,还有一个LCD1602的液晶显示函数*/
/*变量的定义,位声明和函数的声明放在头文件中，函数的定义放在C文件中*/

#include <reg52.h>
#include <intrins.h>

#include "DS18B20T.h"
#include "LCD1602.h"

#define uint unsigned int
#define uchar unsigned char

//延时6.5us，11.95us函数
void Delay_us(uchar us)
{
	while(us--);
}

//毫秒延时函数
void delayMs(uint z)
{
	uint x, y;
	for(x=z;x>0;x--)
		for(y=114;y>0;y--);
}

//DS18B20温度传感器使用单总线通信
//DS18B20初始化时序
bit DS18B20_init()
{
	bit i;
	DQ=1;
	_nop_();
	DQ=0;			//拉低总线480us复位器件
	Delay_us(75);
	DQ=1;			//释放总线
	Delay_us(4);   //等待15-60us   37.95us
	i=DQ;
	Delay_us(20);	 //等待60-240us 18B20的存在信号 低电平
	DQ=1;		//稳定总线
	_nop_();
	return i;
}

//DS18B20温度传感器读一字节 时序
uchar DS18B20_readByte()
{
	uchar i, j, dat;
	for(i=0;i<8;i++)
	{			
		DQ=0;
		_nop_();   		//1条_nop_()指令(12个机器周期)执行1.085us的时间	
		DQ=1;
		_nop_();
		j = DQ;
		Delay_us(10);
		DQ=1;	//释放总线
		_nop_();
		dat = (j<<7)|(dat>>1);
	}
	return dat;
}

//DS18B20温度传感器写入一字节 时序
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

//温度传感器等待
void DS18B20_wait()	   //判断DS1802是否有返回
{
	uint i;
	i=DS18B20_init();
	while(i);	
	/*
	while(DQ);	  //DS18B20时钟总线
	while(~DQ);
	i=4;
	while(i>0)
		i--;
	*/
}

//DS18B20 主机和DS18B20做任何通讯前都需要对其初始化
//DS18B20 发送温度转换指令
void DS18B20_sendChangeCmd()
{
	DS18B20_init();
	DS18B20_wait();
	delayMs(1);

	DS18B20_writeByte(0xcc);   //忽略ROM指令
	DS18B20_writeByte(0x44);   //温度转换指令
}

//DS18B20 发送读数据指令
void DS18B20_sendReadCmd()
{
	DS18B20_init();	  
	DS18B20_wait();	  //这里函数就没有出来
	delayMs(1);
	
	DS18B20_writeByte(0xcc);   //忽略ROM指令
	DS18B20_writeByte(0xbe);   //温度转换指令	
}

//DS18B20 取出真实温度的函数
int DS18B20_TemToDat()
{
	uchar Th, Tl;
	int Tint;	 //只是为了保证精度够用避免溢出 用int型数据
	uint Tuint;
	float Tfloat; //真正带小数部分温度值的存放
  	   
	DS18B20_init();
	DS18B20_writeByte(0xcc);
	DS18B20_writeByte(0xbe);	 
	
	Tl=DS18B20_readByte();	 //读暂存器低八位数据
	//P1=Tl;

	Th=DS18B20_readByte();	 //读暂存器高八位数据
	//P1=Th;

	Tuint=Th;
	Tuint=Tuint<<8;	
	Tuint= Tuint|Tl;
	Tint=Tuint;

	Tfloat = Tint*0.0625;	   //真正的温度值，在这里！！！ 不带精度优化的温度值
	Tint = Tfloat*100+0.5;			//Tint*0.0625得出的结果并不是带有小数，它的值是个整数，经过计算得到的
	//采用int的原因也在这里，如果不用int,*100后的数据会溢出的
							//+0.5只是为了后边一位的进位，精度优化在这里！！！
	//Tchar=Tfloat*10000;		//七位数据  三位整数部分，四位小数部分	一点都不带精度优化的

	//Tchar=Tint&0x7f;	//0111 1111  结果为Tint的后七位，也就是数据位
	return Tint;			
}

//DS18B20得到的温度的数据转换为整数部分+小数部分的数据
void DS18B20_TemToDisplay()
{
	uchar blank[]={" "};
	uchar i;
	int Tint;	  
	//uchar TemDat[7]={0, 0, 0, 0, 0, 0, 0};		//七位数据
	uchar TemDat[]={0, 0, 0, 0, 0};		   //五位数据	 

	Tint=DS18B20_TemToDat();	  

	TemDat[0]=Tint/10000;
	TemDat[1]=Tint%10000/1000;
	TemDat[2]=Tint%1000/100;

	TemDat[3]=Tint%100/10;
	TemDat[4]=Tint%10;

	LCD_writeString(4, 0, blank);

	for(i=1;i<5;i++)
	{
		LCD_writeDat('0'+TemDat[i]);	   //在这里引用的LCD1602的函数！！！
		if(i==2)
			LCD_writeDat('.');
	}
}