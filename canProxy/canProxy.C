#include <reg51.h>
#include <string.h>

#include "../common/types.h"
#include "../common/RS485.h"
#include "../common/canData.h"
#include "../common/sja.h"

uint8 xdata *SJA_BaseAdr = 0X7F00;

sbit key1=P3^3; //Button按键
sbit key2=P3^4; //Button按键
sbit key3=P3^5; //Button按键

sbit P2_0=P2^0;//数码管位选
sbit P2_1=P2^1;
sbit P2_2=P2^2;//数码管位选
sbit P2_3=P2^3;

bit isRsDataRecived = 0;
bit isMsgReceived = 0;
uint8 ch_recv = 0;

uint8 inBuffer[28];
uint8 inDataSize = 0;

uint8 code testData[8]={'a','b','c','d','e','f','g','h'};
uint8 code table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void display(unsigned char num)  // 显示子函数
{
	P2_0 = 0;
	P2_1 = 0;
	P2_2 = 0;
	P2_3 = 0;
	P1=table[num];
}


void ex0_int(void) interrupt 0 using 1
{
	unsigned char tt,length,i;
	unsigned char xdata *canAddr;
	static int cnt = 1;
 
	canAddr = REG_INTERRUPT;

	if((*canAddr)&0x01)                   //产生了接收中断
	{  
		canAddr = REG_RXBuffer1;
		tt=*canAddr;
		length=tt&0x0F;


		
		if ((tt&0x40)!=0x40)                   //数据帧   = 为远程帧
		{  
			if (tt&0x80)  //eff
			{
				canAddr = REG_RXBuffer6;
			}
			else //sff
			{
				canAddr = REG_RXBuffer4;
			}
			serial_send_char(*canAddr);
			//emcpy(RevceData,canAddr,length);  //功能：由src所指内存区域复制count个字节到dest所指内存区域

			display((*canAddr)%16);

		}

		BCAN_CMD_PRG(RRB_CMD);                  //释放SJA1000接收缓冲区，****已经修改
	}
} 

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


void  main()
{
	int i = 0;
	uint8 num;
	Init_Cpu();
	init_serialcomm(RS_Baudrate_4800, OSCILLA_FREQ_11M);  //初始化串口
	
	isRsDataRecived = 0;

	serial_send_string("start\n");


	if ( Sja_1000_Init(OSCILLA_FREQ_16M))
	{
		serial_send_char(0xee);
	}
	else
	{
		EA = 1;
	}


	display(10);
	serial_send_string("begin\n");

	

	while(1)
    {

#if 0	
		//if(isRsDataRecived)
		if(0)
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
#endif
		if(key1 == 0) 
		{
			delay10ms();
			while(key1==0);

			serial_send_char(0x01);

			num=num+1;if(num==16) num=0;
			//CAN_Send_onebyte(num,1);

			CAN_Send_Data(num);
			
			display(num); 
        	delay10ms();
			SJA_BCANAdr = REG_STATUS;

			serial_send_char(0x02);
         
     		delay10ms();
			delay10ms();
			delay10ms();
			if ((*SJA_BCANAdr&0x40)==0x40)
			{

				serial_send_char(0xee);
				Sja_1000_Init();
				
			}

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


