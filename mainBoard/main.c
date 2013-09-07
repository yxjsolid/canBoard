#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
#include "reg51.h"

#include "../common/RS485.h"
#include "../common/sja.h"

void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);
//定义SJA1000的基址
unsigned char xdata *SJA_BaseAdr = 0XFE00;



unsigned char data RevceData[13];


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

BoardStatus idata OutPutBoard[16];
BoardStatus idata InPutBoard[16];

void delay_ms(int t);
void rs485SetModeRx(void);
void rs485SetModeTx(void);


void send_something(unsigned char CAN_TX_data)
{

	

	rs485SetModeTx();

	SBUF=0xee;
	while(!TI);
	TI=0;
	
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

	SBUF=CAN_TX_data;
	while(!TI);
	TI=0;

	rs485SetModeRx();
}

/****************************************************
**函数原型：  void ex0_int(void) interrupt 0 //using 1
**功    能：  中断接收函数
**入口参数:   无 
**出口参数:   RevceData[]数组   
**说    明:   当sja1000 收到正确的报文时,会产生int中断           
*****************************************************/
#if 0
void ex0_int(void) interrupt 0 using 1
{  
	unsigned char tt,length,i;


	rs485SetModeTx();

	SBUF=0x1;
	while(!TI);
	TI=0;
	
	SJA_BCANAdr = REG_INTENABLE;
	SBUF=(*SJA_BCANAdr);
	while(!TI);
	TI=0;
 	rs485SetModeRx();



	SJA_BCANAdr=REG_INTERRUPT;


	

	if((*SJA_BCANAdr)&0x01)                   //产生了接收中断
	{  
		SJA_BCANAdr=REG_RXBuffer1;
		tt=*SJA_BCANAdr;
		length=tt&0x0F;
		if ((tt&0x40)!=0x40)                   //数据帧   = 为远程帧
		{  
			SJA_BCANAdr =REG_RXBuffer4 ;           //宏定义的变量不能memcpy(RevceData,REG_RXBuffer4,8); 
			//SJA_BCANAdr = REG_RXBuffer1 ;

			//memcpy(RevceData,SJA_BCANAdr,13);  //功能：由src所指内存区域复制count个字节到dest所指内存区域
			//memcpy(Com_RecBuff,RevceData,8);      //测试用的主要是把接收到的数据在发出去，验证数据的正确
			                                //以下代码是发送到串

			send_something(*(SJA_BCANAdr));
			
			//for(i=0;i<13;i++)
			//send_something(RevceData[i]);


		}


		rs485SetModeTx();

		SBUF=0x2;
		while(!TI);
		TI=0;

		SJA_BCANAdr = REG_INTENABLE;
		SBUF=(*SJA_BCANAdr);
		while(!TI);
		TI=0;
		rs485SetModeRx();

		BCAN_CMD_PRG(RRB_CMD);                  //释放SJA1000接收缓冲区，****已经修改
	}

	rs485SetModeTx();

	SBUF=0x3;
	while(!TI);
	TI=0;
		
	SJA_BCANAdr = REG_INTENABLE;
	SBUF=(*SJA_BCANAdr);
	while(!TI);
	TI=0;
 	rs485SetModeRx();
} 
#endif

void ex0_int(void) interrupt 0 using 1
{  
	unsigned char tt,length,i;
	SJA_BCANAdr=REG_INTERRUPT;


	if((*SJA_BCANAdr)&0x01)                   //产生了接收中断
	{  
		SJA_BCANAdr=REG_RXBuffer1;
		tt=*SJA_BCANAdr;
		length=tt&0x0F;
		if ((tt&0x40)!=0x40)                   //数据帧   = 为远程帧
		{  
			//SJA_BCANAdr =REG_RXBuffer4 ;           //宏定义的变量不能memcpy(RevceData,REG_RXBuffer4,8); 
			SJA_BCANAdr = REG_RXBuffer1 ;

			//memcpy(RevceData,SJA_BCANAdr,13);  //功能：由src所指内存区域复制count个字节到dest所指内存区域
			//memcpy(Com_RecBuff,RevceData,8);      //测试用的主要是把接收到的数据在发出去，验证数据的正确
			                                //以下代码是发送到串

			rs485SetModeTx();
			memcpy(RevceData,SJA_BCANAdr,13);
			for(i=0;i<13;i++)
			{
				SBUF=RevceData[i];
				while(!TI);
				TI=0;
			}
 			rs485SetModeRx();
		}

		BCAN_CMD_PRG(RRB_CMD);                  //释放SJA1000接收缓冲区，****已经修改
	}

} 


void Sja_test(unsigned char CAN_TX_data)
{
	unsigned char temptt;

	CAN_Send_onebyte(CAN_TX_data,1);
	
	SJA_BCANAdr = REG_STATUS;    
	      
	delay_ms(2000);

	temptt=*SJA_BCANAdr;   

	SBUF=0x88;
	while(!TI);
	TI=0;


	SBUF=temptt;
	while(!TI);
	TI=0;

	SBUF=0x99;
	while(!TI);
	TI=0;

	while(!(temptt&0x8))
	{
		temptt=*SJA_BCANAdr;
	
		delay_ms(200);
		SBUF=temptt;
		while(!TI);
		TI=0;
	}


	


	
	if ((temptt&0x40)==0x40)                     //读错误状态
	{  
		SBUF=0x99;
		while(!TI);
		TI=0;
	
		//Sja_1000_Init();
		serial_send_char(0xee);                    //测试专用发送到串口看状态    
	}


	SBUF=0xaa;
	while(!TI);
	TI=0;
}



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
unsigned char a;
bit flag = 0;

void rs485SetModeRx()
{
	P17=0; //output
	ES=1;
}

void rs485SetModeTx()
{
	P17=1; //output
	ES=0;
}

unsigned char num = 0;

void serial() interrupt 4
{
	num++;
	a=SBUF;
	flag=1;
	RI=0;
}
#if 0
/************************************************************************
*函数原型: void init_serialcomm(void)            *
*参数说明: 串口初始化                            *                                                             *
*说明:     设值单片机的定时器1的方式选择波特率 。该子程序只能用于复位模式                      
************************************************************************/
void init_serialcomm(void)
{
#if 1
	SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    TH1   = 0xF3;       //Baud:4800  fosc=12MHz  
    TL1   = 0xF3;       //baud:4800

	//TH1   = 0xF4;       //Baud:4800  fosc=11.0592MHz  
    //TL1   = 0xF4;       //baud:4800

	//TH1   = 0xF9;       //Baud:9600  fosc=12MHz  
    //TL1   = 0xF9;       //baud:9600     
    IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 
#endif

	//ES=1;
}
#endif

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

//int timer_flag = 0;
//int timer_count = 0;

//定时器0中断,不够8个就在此发送
void time_intt0(void) interrupt 1 using 2
{
	static unsigned char timer_flag = 0;
	static unsigned char timer_count = 0;
	setTimer();
	if (timer_count == 4)
	{
		timer_flag = !timer_flag;
		//P35=timer_flag;
		//P10=timer_flag;

		P10 = timer_flag;

		timer_count = 0;
	}
	else
	{
		timer_count++;
	}

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
		for(j=0;j<0xffff;j++)
			;
	}
}


void main(void)
{  
	unsigned char ss;
	unsigned char num = 0;
		
	CS=0;                //片选择引脚
	EA=0;
	Init_Cpu(); 

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

    //初始化SJA1000     
    ss=Sja_1000_Init(OSCILLA_FREQ_12M);
    if (ss!=0)             //初始化失败
    {

		SBUF=ss;
		while(!TI);
		TI=0;
	  //send_string_com("init fail!");**********************
      serial_send_char(0xBB);              //测试专用发送到串口看状态   
    }


	EA=1; //初始化成功，开总中断

	
  
	SBUF=0xd4;
	while(!TI);
	TI=0;

	SBUF=ss;
	while(!TI);
	TI=0;

	SJA_BCANAdr=REG_CONTROL;
	SBUF=(*SJA_BCANAdr);
	while(!TI);
	TI=0;
	


#endif

	rs485SetModeRx();
	//次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
	while(1) 
	{

		//delay_ms(500);
#if 1	
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
			
				
				rsDataSend(&rsData);
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


