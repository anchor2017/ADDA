/*变量的定义和函数的声明放在头文件中，函数的定义放在C文件中*/

#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//I2C引脚初始化
#define PCF8591addr 0x90
sbit SDA=P2^0;  //数据总线
sbit SCL=P2^1;  //时钟总线

//5us延时函数
void delay_5us();

//I2C初始化
void I2C_init();

//I2C起始信号
void I2C_Start();

//I2C终止信号
void I2C_Stop();

//主机发送应答
void Master_ACK(bit i);

//检测从机应答
bit Test_ACK();

//发送一个字节
void I2C_send_byte(uchar byte);

//I2C 读一字节
uchar I2C_read_byte();

//I2C主机发送数据，每一次写入数据都需要检测从机应答
//每次使用这个函数只能写入一字节的数据
//先发送E2PROM地址，再发送数据
bit I2C_transData(uchar ADDR, DAT);	

//I2C主机接收数据
//主机读取E2PROM数据，每次只能从E2PROM的特定地址读取一字节数据，并返回该数据
uchar I2C_receData(uchar ADDR);

//LCD显示光照强度
void PlightDisplay();