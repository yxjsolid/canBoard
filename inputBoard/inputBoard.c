#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
#include "reg51.h"

#include "../common/RS485.h"

//*************************
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;
sbit P15=P1^5;
sbit P16=P1^6;
sbit P17=P1^7;
sbit P35=P3^5;
sbit P37=P3^7;
sbit P34=P3^4;
//***************************

sbit CS=P2^0;
int timerTicket = 0;
unsigned char a;
bit flag = 0;
bit rsFrameReceived = 0;

unsigned char num = 0;


void delay_ms(int t);
void rs485SetModeRx(void);
void rs485SetModeTx(void);


//***************************************************

//初始化cpu

//**************************************************
void Init_Cpu(void)                                  //单片机初始化,开放外部中断0
{
	PX0=0;
	IT0=0; // TCON set EXC0 trigge mode
	// EA=1;
	//EX0=1;
}

/****************************************************
**函数原型：   void main(void)
**功    能：   主程序部分:
**入口参数:    无 
**返 回 值:     
*****************************************************/
void rs485SetModeRx()
{
	P37=0; //output
	ES=1;
}
void rs485SetModeTx()
{
	P37=1; //output
	ES=0;
}
void sendTest(uint8 test)
{
	rs485SetModeTx();
	SBUF=test;
	while(!TI);
	TI=0;
	rs485SetModeRx();

}

uint8 buffer[20];
void serial() interrupt 4 using 1
{

	
	a = SBUF;
	flag = 1;
	RI = 0;

	//while(1);

	buffer[num] = a;
	num++;
	num %= 20;
	
#if 1
	rsFrameReceived = 0;
	if (rsDataReceive(a, &rsData, sizeof(RS485DataStruct)))
	{
		rsFrameReceived = 1;
		rs485SetModeTx();//disable serial interrupt
	}
#endif	
}

void setTimer(void)
{
	TH0=0x3c;
	TL0=0xaf;  //50MS定时初值（T0计时用）
}

//定时器0初始化
void timer0initial()
{
	TMOD |= 0x1;        //工作方式16位定时计数器
	setTimer();	
    ET0=1;
	TR0=1;
	PT0=1;   // higher priority than EX0
}

//定时器0中断,不够8个就在此发送
void time_intt0(void) interrupt 1 using 2
{
	static uint8 timer_count = 0;
	setTimer();
	if (timer_count == 4)
	{
		P35 = !P35;
		timer_count = 0;
	}
	else
	{
		timer_count++;
	}

	timerTicket++;
}


void delay_ms(int t)
{
     int i,j;
     for (i=0;i<t;i++)
     	for(j=0;j<33;j++) 
     	;
    
}
void delay10ms(void) //延时程序
{

      unsigned char i,j;

      for(i=20;i>0;i--)

      for(j=248;j>0;j--);

}
void delay_s(unsigned char t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<0xffff;j++);
	}
}


enum opera_state
{
	OP_STAT_IDLE = 0,
	OP_STAT_CMD_RECV,
	OP_STAT_CMD_HANDLE,
	OP_STAT_CMD_REPLY,
};
	
void main(void)
{  
		
	CS=0;                //片选择引脚
	EA=0;
	
	P1 = 0;

	init_serialcomm(RS_Baudrate_4800, OSCILLA_FREQ_12M);  //初始化串口
	timer0initial();


	rs485SetModeTx();
	SBUF=0xd1;
	while(!TI);
	TI=0;

	SBUF=0xd2;
	while(!TI);
	TI=0;

#if 1
	
	SBUF=0xd1;
	while(!TI);
	TI=0;

	SBUF=0xd2;
	while(!TI);
	TI=0;

   
	EA=1; //初始化成功，开总中断

	SBUF=0xd4;
	while(!TI);
	TI=0;
#endif

	rs485SetModeRx();
	//次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
	//rsFrameReceived = 1;
	while(1) 
	{
		if (rsFrameReceived)
		{
			rs485SetModeTx();
			rsData.boartType = 0xf1;
				rsData.boardId = 0xf2;
				rsData.cmd = 0xf3;
				rsData.rsData = 0xf4;
			
			rsDataSend(&rsData, sizeof(rsData));
			rs485SetModeRx();
			rsFrameReceived = 0;
		}
#if 1
		if (num == 11)
		{
			rs485SetModeTx();
			
			
			serial_send_data(buffer, 11);
			rs485SetModeRx();

			num = 0;
		}
#endif		
		

#if 0
		if(flag==1)
		{
			if (a == 'a')
			{
				rs485SetModeTx();
				flag=0;
				SBUF=0xa;
				
				while(!TI);
				TI=0;
				rs485SetModeRx();
			}
		}
#endif		


		//delay_ms(500);
#if 0	
		if(flag==1)
		{
			rs485SetModeTx();

			flag=0;
			SBUF=0xa;
			
			while(!TI);
			TI=0;

			SBUF=0xb;
			while(!TI);
			TI=0;
			
			SBUF=0xc;
			while(!TI);
			TI=0;

			
			SBUF=num++;
			while(!TI);
			TI=0;
			
			SBUF=a;
			while(!TI);
			TI=0;
		
			if (a == 'a')
			{
				memset(&(rsData), 0, sizeof(rsData));

				rsData.boartType = 0xf1;
				rsData.boardId = 0xf2;
				rsData.cmd = 0xf3;
				rsData.rsData = 0xf4;
			
				
				rsDataSend(&rsData, sizeof(rsData));
			}
			else if (a == 'b')
			{
				SJA_BCANAdr = REG_INTERRUPT;
				SBUF=(*SJA_BCANAdr);
				while(!TI);
				TI=0;

				SJA_BCANAdr = REG_INTENABLE;
				SBUF=(*SJA_BCANAdr);
				while(!TI);
				TI=0;

				SJA_BCANAdr = REG_STATUS;
				SBUF=(*SJA_BCANAdr);
				while(!TI);
				TI=0;

				SJA_BCANAdr = REG_ERRCATCH;
				SBUF=(*SJA_BCANAdr);
				while(!TI);
				TI=0;
				
			}
			else
			{

				Sja_test(a);
			}

			
			rs485SetModeRx();
			
		}
#endif
	}

}


