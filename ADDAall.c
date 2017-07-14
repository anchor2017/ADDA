#include <reg52.h>
#include <intrins.h>

#include "DS18B20T.h"
#include "I2C_PCF8591.h"
#include "LCD1602.h"

#define uint unsigned int
#define uchar unsigned char

//数码管段选位选设置 主要是为了防止数码管因受到影响显示
sbit DU=P2^6;
sbit WE=P2^7;

void main()
{
	uchar Tstr[]={"T:"};
	uchar Pstr[]={"P:"};
	uchar blank[]={" "};
	
	/*由于数码管和LCD共用端口，关闭数码管的操作*/
	DU=1;	
	P0=0;
	DU=0;	
	WE=1;
	P0=0x7f;
	WE=0;

	/*初始化*/
	I2C_init();
	LCD_init();
	LCD_writeString(2, 0, Tstr);	//温度字母显示
	LCD_writeString(2, 1, Pstr);	//光照字母显示

	/*温度转换*/
	DS18B20_init();				  //初始化DS18B20
	DS18B20_writeByte(0xcc);	  //发送跳跃ROM指令
	DS18B20_writeByte(0x44);	  //温度转换指令

	while(1)
	{
		delayMs(1000);			 //温度转换需要一定的时间

		/*LCD显示*/
		DS18B20_TemToDisplay();	 //温度显示函数
		PlightDisplay();		 //光照显示函数，初始光照值为28
	
		/*温度转换*/
		DS18B20_init();				  //初始化DS18B20
		DS18B20_writeByte(0xcc);	  //发送跳跃ROM指令
		DS18B20_writeByte(0x44);	  //温度转换指令

	}	
}