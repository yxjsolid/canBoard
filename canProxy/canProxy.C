#include <reg51.h>
#include <string.h>

#include "../common/types.h"
#include "../common/RS485.h"
#include "../common/canData.h"

sbit key=P3^3; //Button按键
bit isRsDataRecived = 0;
bit isMsgReceived = 0;
uint8 ch_recv = 0;

uint8 inBuffer[28];
uint8 inDataSize = 0;


//***************************************************

//初始化cpu

//**************************************************
void Init_Cpu(void)                                  //单片机初始化,开放外部中断0
{
	IE = 0;
	PX0=1;
	EX0=1;
	IT0=0;
	EA=0;
}


void init_serialcomm(void)
{
    SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr
    TMOD &= 0x0f;
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    TH1   = 0xFA;       //Baud:9600  fosc=11.0592MHz 
    TL1   = 0xFA;


	//TH1   = 0xF4;       //Baud:4800  fosc=11.0592MHz 
    //TL1   = 0xF4;
    IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 
    ET1  = 0;
    //EA = 1;
   // TI=1; 
}



//串口接收中断函数 
void serial () interrupt 4 using 1 
{
    if(RI)
    {
        RI = 0;
        ch_recv = SBUF;
		isRsDataRecived = 1;

		inBuffer[inDataSize] = ch_recv;
		inDataSize++;
    }
}

void delay10ms(void) //延时程序
{
      unsigned char i,j;
      for(i=20;i>0;i--)
      for(j=248;j>0;j--);
}

uint8 testData[8]={'a','b','c','d','e','f','g','h'};

#if 1
void  main()
{
	int i = 0;
	init_serialcomm();  //初始化串口
	
	isRsDataRecived = 0;



	while(1)
    {
		if(isRsDataRecived)
		{
			delay10ms();
			delay10ms();
			delay10ms();
			delay10ms();
			delay10ms();
			delay10ms();
			delay10ms();
			delay10ms();
			delay10ms();
			
		
			isRsDataRecived = 0;



			serial_send_data(inBuffer, inDataSize);

			
#if 1			
			serial_send_char('!');
			for (i = 0; i < inDataSize; i++)
			{

				
				if ( rsDataReceive(inBuffer[i], &canData, sizeof(canData)))
				{	
					serial_send_char('o');
					serial_send_char('k');
					serial_send_char('\n');

					serial_send_char('#');
					rsDataSend(&(canData), sizeof(canData));
				}
				else
				{
					serial_send_char(0x02);
				}
			}

			serial_send_char('\n');

			inDataSize = 0;
#endif			
		}
		
		if(key == 0) 
		{
			delay10ms();
			while(key==0);

			serial_send_char(0x01);

			rsDataSend(&(testData), 8);
		#if 1
			//send_char_com(0x01);
			serial_send_char(0x02);
			serial_send_char(0x03);
			serial_send_char(0x03);
			serial_send_char(0x04);
			serial_send_char(0x05);
			serial_send_char(0x06);
			serial_send_char(0x08);
			#endif
		}

		  
    }

}

#else

/****************************************************
**函数原型：   void main(void)
**功    能：   主程序部分:
**入口参数:    无 
**返 回 值:     
*****************************************************/

void main(void)
{  
	IE = 0;
	//Init_Cpu();  
	init_serialcomm();     //初始化串口 
	//timer0initial();       //定时器0初始化
  

#if 0
	ss=Sja_1000_Init();
	if (ss!=0)             //初始化失败
	//send_string_com("init fail!");**********************
	send_char_com(0xBB);              //测试专用发送到串口看状态   
	else
	EA=1; //初始化成功，开总中断
#endif

//次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
	while(1) 
	{ 
		if(read_flag)  //如果取数标志已置位，就将读到的数从串口发出 
		{
			read_flag=0; //取数标志清0 
			//ES = 0;
			send_char_com(0x00);
			send_char_com(0x01);
			send_char_com(0x02);
			send_char_com(0x03);
			send_char_com(0x04);
			send_char_com(0x05);
			send_char_com(0x06);
			send_char_com(0x08);
			//ES = 1;
		}
	}   

}

#endif


