/*变量的定义和函数的声明放在头文件中，函数的定义放在C文件中*/

#include <reg52.h>
#include <intrins.h>

#include "I2C_PCF8591.h"
#include "LCD1602.h"
#include "DS18B20T.h"

#define uint unsigned int
#define uchar unsigned char

//I2C引脚初始化
#define PCF8591addr 0x90

//5us延时函数
void delay_5us()
{
	_nop_();
}

//I2C初始化
void I2C_init()	
{
	SDA = 1;
	_nop_();
	SCL = 1;
	_nop_();
}

//I2C起始信号
void I2C_Start()  
{
	SCL = 1;
	_nop_();
	SDA = 1;
	delay_5us();
	SDA = 0;
	delay_5us();
}

//I2C终止信号
void I2C_Stop()
{
	SDA = 0;
	_nop_();
	SCL = 1;
	delay_5us();
	SDA = 1;
	delay_5us();
}

//主机发送应答
void Master_ACK(bit i)		
{
	SCL = 0; // 拉低时钟总线允许SDA数据总线上的数据变化
	_nop_(); // 让总线稳定
	if (i)	 //如果i = 1 那么拉低数据总线 表示主机应答
	{
		SDA = 0;
	}
	else	 
	{
		SDA = 1;	 //发送非应答
	}
	_nop_();//让总线稳定
	SCL = 1;//拉高时钟总线 让从机从SDA线上读走 主机的应答信号
	delay_5us();
	SCL = 0;//拉低时钟总线， 占用总线继续通信
	_nop_();
	SDA = 1;//释放SDA数据总线。
	_nop_();
}

//检测从机应答
bit Test_ACK()
{
	SCL = 1;
	delay_5us();
	if (SDA)	//从机发送非应答信号
	{
		SCL = 0;
		_nop_();
		I2C_Stop();
		return(0);
	}
	else		//从机应答
	{
		SCL = 0;
		_nop_();
		return(1);
	}
}

//发送一个字节
void I2C_send_byte(uchar byte)
{
	uchar i;
	for(i = 0 ; i < 8 ; i++) //发送数据给E2PROM，先从最高位开始发
	{
		SCL = 0;
		_nop_();				
		if (byte & 0x80)  //1000 0000
		{ 
			_nop_();				   
			SDA = 1;	
		}
		else
		{				
			SDA = 0;
			_nop_();
		}
		SCL = 1;
		_nop_();
		byte <<= 1;	// 0101 0100B 
	}
	SCL = 0;
	_nop_();
	_nop_();
	SDA = 1;
}


//I2C 读一字节
uchar I2C_read_byte()
{
	uchar dat,i;
	SCL = 0;
	_nop_();
	SDA = 1;   //主机接收数据
	_nop_();
	for(i = 0 ; i < 8 ; i++)
	{
		SCL = 1;
		_nop_();
		if (SDA)			    
		{
			 dat |= 0x01; //0000 0001 按位或，前八位同dat, 最后一位为1
		}
		else
		{
			dat &=  0xfe;	//1111 1110	 按位与，前八位同dat, 最后一位为0
		}
		_nop_();
		SCL = 0 ;
		_nop_();
		if(i < 7)
		{
			dat = dat << 1;	 //高低位互换，从低位左移到高位
		}
	}
	return(dat);
}

//I2C主机发送数据，每一次写入数据都需要检测从机应答
//每次使用这个函数只能写入一字节的数据
bit I2C_transData(uchar ADDR, DAT)	//先发送E2PROM地址，再发送数据
{								 
	I2C_Start();					//发送起始信号
	I2C_send_byte(PCF8591addr+0); //写入器件地址
	if (!Test_ACK())			   //检测从机应答，决定是否继续发送数据
	{
		return(0);
	}
	
	I2C_send_byte(ADDR); 		   //发送写入首地址，由编写者决定
	if (!Test_ACK())			   //检测从机应答
	{
		return(0);
	}
	
	I2C_send_byte(DAT);			 //发送一字节的数据	 
	if (!Test_ACK())		   //检测从机应答
	{
		return(0);
	}
	I2C_Stop();				   //I2C终止信号
	return(1);	
}

//I2C主机接收数据
//主机读取E2PROM数据，每次只能从E2PROM的特定地址读取一字节数据，并返回该数据
uchar I2C_receData(uchar ADDR)	 
{
	uchar DAT;
	I2C_Start();					 //起始信号
	I2C_send_byte(PCF8591addr+0);	 //发送器件地址并发送数据
	if (!Test_ACK())
	{
		return(0);
	}
	
	I2C_send_byte(ADDR);
	Master_ACK(0);		//主机发出非应答

	//------------------
	I2C_Start();
	I2C_send_byte(PCF8591addr+1); 	//发送器件地址并接受数据
	if (!Test_ACK())
	{
		return(0);
	}

	DAT = I2C_read_byte();	//主机读取到一字节之后
	Master_ACK(0);		   //主机发出非应答

	I2C_Stop();
	return(DAT);	
}

//LCD显示光照强度
void PlightDisplay()
{
	uchar Pvalue;
	uchar i;
	uchar Pstring[]={" "};
	uchar Pchar[]={0, 0, 0};

	Pvalue=I2C_receData(0x42);
	delayMs(500);		
	//模拟输入使能位置1，四线单端输入，自动增量标志位置0（多个器件才需要置1），输入通道2	
	
	Pchar[0]=Pvalue/100;
	Pchar[1]=Pvalue%100/10;
	Pchar[2]=Pvalue%10;

	LCD_writeString(4, 1, " ");
	for(i=1;i<3;i++)
	{	
		LCD_writeDat('0'+Pchar[i]);
	}
}