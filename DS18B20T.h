/*由于单总线对时序的要求比较严格，这里用的延时是以晶振11.0592MHZ为标准计算的*/
/*变量的定义,位声明和函数的声明放在头文件中，函数的定义放在C文件中*/

#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//DS18B20单总线引脚定义
sbit DQ = P2^2;

//延时6.5us，11.95us函数
void Delay_us(uchar us);

//毫秒延时函数
void delayMs(uint z);

//DS18B20温度传感器使用单总线通信
//DS18B20初始化时序
bit DS18B20_init();

//DS18B20温度传感器读一字节 时序
uchar DS18B20_readByte();

//DS18B20温度传感器写入一字节 时序
void DS18B20_writeByte(uchar dat);

//温度传感器等待
//判断DS1802是否有返回
void DS18B20_wait();	   

//DS18B20 主机和DS18B20做任何通讯前都需要对其初始化
//DS18B20 发送温度转换指令
void DS18B20_sendChangeCmd();

//DS18B20 发送读数据指令
void DS18B20_sendReadCmd();

//DS18B20 取出真实温度的函数
int DS18B20_TemToDat();

//DS18B20得到的温度的数据转换为整数部分+小数部分的数据
void DS18B20_TemToDisplay();