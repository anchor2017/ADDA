#include <reg52.h>
#include <intrins.h>
#define uint unsigned int
#define uchar unsigned char

//����ܶ�ѡλѡ���� ��Ҫ��Ϊ�˷�ֹ��������ܵ�Ӱ����ʾ
sbit DU=P2^6;
sbit WE=P2^7;

//��ʱ6.5us�����뺯��11.95us
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

/*------------------------------------------LCD1602--------------------------------------------*/
//LCD1602���Ŷ���
sbit RS=P3^5;
sbit RW=P3^6;
sbit EN=P3^4;

//LCDҺ��æ״̬�жϣ�Ҳ���Ƕ�д���STA״̬
void LCD_readBusy()
{
	uchar busy;
	P0=0xff;	//��λ����
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
//LCD��״̬
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
//LCD������
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

//LCDдָ��
void LCD_writeCmd(uchar cmd)
{
	LCD_readBusy();

	RS=0;
	RW=0;
	P0=cmd;
	EN=1;
	EN=0;
}

//LCDд����
void LCD_writeDat(uchar dat)
{
	LCD_readBusy();
	RS=1;
	RW=0;
	P0=dat;
	EN=1;
	EN=0;
}

//LCDҺ����ʼ��
void LCD_init()
{
	LCD_writeCmd(0x38);  //16*2��ʾ��5*7����8λ���ݽӿ�
	LCD_writeCmd(0x0c);  //����ʾ������ʾ��꣬��겻��ʾ
	LCD_writeCmd(0x06);  //��д���һ���ַ����ַָ���1�ҹ���1
	LCD_writeCmd(0x01);  //����ָ��
}


//LCDд��һ���ַ�
void LCD_writeChar(uchar x, uchar y, uchar dat)
{
	if(y)			 //y=1 �ڶ���
		x |= 0x40;	 //0x40��Ӧ��0xc0��0x80+(0x40��0x67)=0xc0+    0x80+0x40=0xc0
					//y=0 ��һ��
	x |= 0x80;

	LCD_writeCmd(x);	  //0x80Ҳ����80H ����ָ�����õ�ָ��
	LCD_writeDat(dat);
}

//LCDд�������ֽ� uint
void LCD_writeTwoChar(uchar x, uchar y, uint dat)
{
	if(y)			 //y=1 �ڶ���
		x |= 0x40;	 //0x40��Ӧ��0xc0��0x80+(0x40��0x67)=0xc0+    0x80+0x40=0xc0
					//y=0 ��һ��
	x |= 0x80;

	LCD_writeCmd(x);	  //0x80Ҳ����80H ����ָ�����õ�ָ��
	LCD_writeDat(dat);
}


//LCDд��һ���ַ���    �Ա�������ʾx��(0-1)����ʾy��(0-39)����ʾ�ַ���
//����ָ���ַ����67H��0110 0111,��80H��1000 0000�����Կ����ð�λ��򵥵�ʵ����ӣ�ֱ����+��Ӧ��Ҳ����
void LCD_writeString(uchar x, uchar y, uchar *string)
{
	if(y)			 //y=1 �ڶ���
		x |= 0x40;	 //0x40��Ӧ��0xc0��0x80+(0x40��0x67)=0xc0+    0x80+0x40=0xc0

	x |= 0x80;

	LCD_writeCmd(x);	  //0x80Ҳ����80H ����ָ�����õ�ָ��

	/*LCD_writeDat(*string);*/

	while(*string != '\0')
	{
		LCD_writeDat(*string++);	//ָ��ָ����׵�ַ��Ҳ���ǵ�һ���ַ�������ָ���ַ���ƣ���֮����ַ�
	}
}
/*------------------------------------------LCD1602----------------------------------------*/


/*------------------------------------------DS18B20----------------------------------*/
//DS18B20���������Ŷ���
sbit DQ = P2^2;

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
		LCD_writeDat('0'+TemDat[i]);
		if(i==2)
			LCD_writeDat('.');
	}
}
/*------------------------------------------DS18B20----------------------------------*/

/*-------------------------------------I2C��PCF8591-----------------------------------*/
//I2C���ų�ʼ��
#define PCF8591addr 0x90
sbit SDA=P2^0;  //��������
sbit SCL=P2^1;  //ʱ������

//5us��ʱ����
void delay_5us()
{
	_nop_();
}

//I2C��ʼ��
void I2C_init()
{
	SDA = 1;
	_nop_();
	SCL = 1;
	_nop_();
}

//I2C��ʼ�ź�
void I2C_Start()
{
	SCL = 1;
	_nop_();
	SDA = 1;
	delay_5us();
	SDA = 0;
	delay_5us();
}

//I2C��ֹ�ź�
void I2C_Stop()
{
	SDA = 0;
	_nop_();
	SCL = 1;
	delay_5us();
	SDA = 1;
	delay_5us();
}

//��������Ӧ��
void Master_ACK(bit i)
{
	SCL = 0; // ����ʱ����������SDA���������ϵ����ݱ仯
	_nop_(); // �������ȶ�
	if (i)	 //���i = 1 ��ô������������ ��ʾ����Ӧ��
	{
		SDA = 0;
	}
	else
	{
		SDA = 1;	 //���ͷ�Ӧ��
	}
	_nop_();//�������ȶ�
	SCL = 1;//����ʱ������ �ôӻ���SDA���϶��� ������Ӧ���ź�
	delay_5us();
	SCL = 0;//����ʱ�����ߣ� ռ�����߼���ͨ��
	_nop_();
	SDA = 1;//�ͷ�SDA�������ߡ�
	_nop_();
}

//���ӻ�Ӧ��
bit Test_ACK()
{
	SCL = 1;
	delay_5us();
	if (SDA)	//�ӻ����ͷ�Ӧ���ź�
	{
		SCL = 0;
		_nop_();
		I2C_Stop();
		return(0);
	}
	else		//�ӻ�Ӧ��
	{
		SCL = 0;
		_nop_();
		return(1);
	}
}

//����һ���ֽ�
void I2C_send_byte(uchar byte)
{
	uchar i;
	for(i = 0 ; i < 8 ; i++) //�������ݸ�E2PROM���ȴ����λ��ʼ��
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


//I2C ��һ�ֽ�
uchar I2C_read_byte()
{
	uchar dat,i;
	SCL = 0;
	_nop_();
	SDA = 1;   //������������
	_nop_();
	for(i = 0 ; i < 8 ; i++)
	{
		SCL = 1;
		_nop_();
		if (SDA)
		{
			 dat |= 0x01; //0000 0001 ��λ��ǰ��λͬdat, ���һλΪ1
		}
		else
		{
			dat &=  0xfe;	//1111 1110	 ��λ�룬ǰ��λͬdat, ���һλΪ0
		}
		_nop_();
		SCL = 0 ;
		_nop_();
		if(i < 7)
		{
			dat = dat << 1;	 //�ߵ�λ�������ӵ�λ���Ƶ���λ
		}
	}
	return(dat);
}

//I2C�����������ݣ�ÿһ��д�����ݶ���Ҫ���ӻ�Ӧ��
//ÿ��ʹ���������ֻ��д��һ�ֽڵ�����
bit I2C_transData(uchar ADDR, DAT)	//�ȷ���E2PROM��ַ���ٷ�������
{
	I2C_Start();					//������ʼ�ź�
	I2C_send_byte(PCF8591addr+0); //д��������ַ
	if (!Test_ACK())			   //���ӻ�Ӧ�𣬾����Ƿ������������
	{
		return(0);
	}

	I2C_send_byte(ADDR); 		   //����д���׵�ַ���ɱ�д�߾���
	if (!Test_ACK())			   //���ӻ�Ӧ��
	{
		return(0);
	}

	I2C_send_byte(DAT);			 //����һ�ֽڵ�����
	if (!Test_ACK())		   //���ӻ�Ӧ��
	{
		return(0);
	}
	I2C_Stop();				   //I2C��ֹ�ź�
	return(1);
}

//I2C������������
//������ȡE2PROM���ݣ�ÿ��ֻ�ܴ�E2PROM���ض���ַ��ȡһ�ֽ����ݣ������ظ�����
uchar I2C_receData(uchar ADDR)
{
	uchar DAT;
	I2C_Start();					 //��ʼ�ź�
	I2C_send_byte(PCF8591addr+0);	 //����������ַ����������
	if (!Test_ACK())
	{
		return(0);
	}

	I2C_send_byte(ADDR);
	Master_ACK(0);		//����������Ӧ��

	//------------------
	I2C_Start();
	I2C_send_byte(PCF8591addr+1); 	//����������ַ����������
	if (!Test_ACK())
	{
		return(0);
	}

	DAT = I2C_read_byte();	//������ȡ��һ�ֽ�֮��
	Master_ACK(0);		   //����������Ӧ��

	I2C_Stop();
	return(DAT);
}

//LCD��ʾ����ǿ��
void PlightDisplay()
{
	uchar Pvalue;
	uchar i;
	uchar Pstring[]={" "};
	uchar Pchar[]={0, 0, 0};

	Pvalue=I2C_receData(0x42);
	delayMs(2000);
	//ģ������ʹ��λ��1�����ߵ������룬�Զ�������־λ��0�������������Ҫ��1��������ͨ��2

	Pchar[0]=Pvalue/100;
	Pchar[1]=Pvalue%100/10;
	Pchar[2]=Pvalue%10;

	LCD_writeString(4, 1, " ");
	for(i=1;i<3;i++)
	{
		LCD_writeDat('0'+Pchar[i]);
	}
}
/*-------------------------------------I2C��PCF8591-----------------------------------*/



void main()
{
	uchar Tstr[]={"T:"};
	uchar Pstr[]={"P:"};
	uchar blank[]={" "};

	/*��������ܺ�LCD���ö˿ڣ��ر�����ܵĲ���*/
	DU=1;
	P0=0;
	DU=0;
	WE=1;
	P0=0x7f;
	WE=0;

	I2C_init();
	LCD_init();
	LCD_writeString(2, 0, Tstr);	//�¶���ĸ��ʾ
	LCD_writeString(2, 1, Pstr);	//������ĸ��ʾ

	/*�¶�ת��*/
	DS18B20_init();				  //��ʼ��DS18B20
	DS18B20_writeByte(0xcc);	  //������ԾROMָ��
	DS18B20_writeByte(0x44);	  //�¶�ת��ָ��

	while(1)
	{
		delayMs(1000);
		DS18B20_TemToDisplay();	 //�¶���ʾ����
		PlightDisplay();

		/*�¶�ת��*/
		DS18B20_init();				  //��ʼ��DS18B20
		DS18B20_writeByte(0xcc);	  //������ԾROMָ��
		DS18B20_writeByte(0x44);	  //�¶�ת��ָ��

	}
}
