#include  "reg51.h"
/************************************************************************
*函数原型: void init_serialcomm(void)            *
*参数说明: 串口初始化                            *                                                             *
*说明:     设值单片机的定时器1的方式选择波特率 。该子程序只能用于复位模式                      
************************************************************************/
void init_serialcomm(void)
{
    SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    TH1   = 0xFa;       //Baud:9600  fosc=11.0592MHz  
    TL1   = 0xFa;       //baud:57600      fosc=22.1184MHz  0xFE
    //IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 

	ES=0;
	
    // TI=1; 
}
//定时器0初始化
void timer0initial()
{
	TMOD |= 0x1;        //工作方式16位定时计数器
	TH0=0xb8;TL0=0x00;  //50MS定时初值（T0计时用）
    IE   |= 0x90;       //Enable Serial Interrupt 
    ET0=0;TR0=0;
}
//向串口发送一个字符 
void send_char_com(unsigned char ch)  
{
    SBUF=ch;
    while(!TI);
    TI=0;
}

//向串口发送一个字符串，strlen为该字符串长度 
void send_string_com(unsigned char *str,unsigned int strlen)
{
    unsigned int k=0;
    do 
    {
        send_char_com(*(str + k));
        k++;
    } while(k < strlen);
}
//定时器0中断,不够8个就在此发送