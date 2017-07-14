#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//数码管段选位选设置 主要是为了防止数码管因受到影响显示
sbit DU=P2^6;
sbit WE=P2^7;

//延时6.5us，进入函数11.95us
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

/*------------------------------------------LCD1602--------------------------------------------*/
//LCD1602引脚定义
sbit RS=P3^5;
sbit RW=P3^6;
sbit EN=P3^4;

//LCD液晶忙状态判断，也就是读写检测STA状态
void LCD_readBusy()
{
	uchar busy;
	P0=0xff;	//复位数据
	RS=0;
	RW=1;
	do
	{
		EN=1;
		busy=P0;
		EN=0;
	}while(busy&0x80);
}

/*
//LCD读状态
uchar LCD_readSta()
{
	uchar dat;
	uchar sta;
	EN=0;
	RW=1;
	RS=1;
	_nop_();

	RS=0;
	RW=1;
	EN=1;
	dat=P0;
	return sta;
}
//LCD读数据
uchar LCD_readDat()
{
	uchar dat;
	EN=0;
	RW=1;
	RS=0;
	_nop_();

	RS=1;
	RW=1;
	EN=1;
	dat=P0;
	return dat;
}
*/

//LCD写指令
void LCD_writeCmd(uchar cmd)
{
	LCD_readBusy();

	RS=0;
	RW=0;
	P0=cmd;
	EN=1;
	EN=0;
}

//LCD写数据
void LCD_writeDat(uchar dat)
{
	LCD_readBusy();
	RS=1;
	RW=0;
	P0=dat;
	EN=1;
	EN=0;
}

//LCD液晶初始化
void LCD_init()
{
	LCD_writeCmd(0x38);  //16*2显示，5*7点阵，8位数据接口
	LCD_writeCmd(0x0c);  //开显示，不显示光标，光标不显示
	LCD_writeCmd(0x06);  //当写或读一个字符后地址指针加1且光标加1
	LCD_writeCmd(0x01);  //清屏指令
}


//LCD写入一个字符
void LCD_writeChar(uchar x, uchar y, uchar dat)
{
	if(y)			 //y=1 第二行
		x |= 0x40;	 //0x40不应该0xc0吗，0x80+(0x40到0x67)=0xc0+    0x80+0x40=0xc0
					//y=0 第一行
	x |= 0x80;

	LCD_writeCmd(x);	  //0x80也就是80H 数据指针设置的指令
	LCD_writeDat(dat);
}

//LCD写入两个字节 uint
void LCD_writeTwoChar(uchar x, uchar y, uint dat)
{
	if(y)			 //y=1 第二行
		x |= 0x40;	 //0x40不应该0xc0吗，0x80+(0x40到0x67)=0xc0+    0x80+0x40=0xc0
					//y=0 第一行
	x |= 0x80;

	LCD_writeCmd(x);	  //0x80也就是80H 数据指针设置的指令
	LCD_writeDat(dat);
}


//LCD写入一个字符串    自变量：显示x轴(0-1)，显示y轴(0-39)，显示字符串
//由于指针地址最大的67H是0110 0111,而80H是1000 0000，所以可以用按位或简单地实现相加，直接用+号应该也可以
void LCD_writeString(uchar x, uchar y, uchar *string)
{
	if(y)			 //y=1 第二行
		x |= 0x40;	 //0x40不应该0xc0吗，0x80+(0x40到0x67)=0xc0+    0x80+0x40=0xc0

	x |= 0x80;

	LCD_writeCmd(x);	  //0x80也就是80H 数据指针设置的指令

	/*LCD_writeDat(*string);*/

	while(*string != '\0')
	{
		LCD_writeDat(*string++);	//指针指向的首地址，也就是第一个字符，接着指针地址下移，读之后的字符
	}
}
/*------------------------------------------LCD1602----------------------------------------*/


/*------------------------------------------DS18B20----------------------------------*/
//DS18B20单总线引脚定义
sbit DQ = P2^2;

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
		LCD_writeDat('0'+TemDat[i]);
		if(i==2)
			LCD_writeDat('.');
	}
}
/*------------------------------------------DS18B20----------------------------------*/

/*-------------------------------------I2C的PCF8591-----------------------------------*/
//I2C引脚初始化
#define PCF8591addr 0x90
sbit SDA=P2^0;  //数据总线
sbit SCL=P2^1;  //时钟总线

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
	delayMs(2000);
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
/*-------------------------------------I2C的PCF8591-----------------------------------*/



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
		delayMs(1000);
		DS18B20_TemToDisplay();	 //温度显示函数
		PlightDisplay();

		/*温度转换*/
		DS18B20_init();				  //初始化DS18B20
		DS18B20_writeByte(0xcc);	  //发送跳跃ROM指令
		DS18B20_writeByte(0x44);	  //温度转换指令

	}
}
