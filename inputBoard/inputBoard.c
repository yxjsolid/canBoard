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

//��ʼ��cpu

//**************************************************
void Init_Cpu(void)                                  //��Ƭ����ʼ��,�����ⲿ�ж�0
{
	PX0=0;
	IT0=0; // TCON set EXC0 trigge mode
	// EA=1;
	//EX0=1;
}

/****************************************************
**����ԭ�ͣ�   void main(void)
**��    �ܣ�   �����򲿷�:
**��ڲ���:    �� 
**�� �� ֵ:     
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
	TL0=0xaf;  //50MS��ʱ��ֵ��T0��ʱ�ã�
}

//��ʱ��0��ʼ��
void timer0initial()
{
	TMOD |= 0x1;        //������ʽ16λ��ʱ������
	setTimer();	
    ET0=1;
	TR0=1;
	PT0=1;   // higher priority than EX0
}

//��ʱ��0�ж�,����8�����ڴ˷���
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
void delay10ms(void) //��ʱ����
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
		
	CS=0;                //Ƭѡ������
	EA=0;
	
	P1 = 0;

	init_serialcomm(RS_Baudrate_4800, OSCILLA_FREQ_12M);  //��ʼ������
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

   
	EA=1; //��ʼ���ɹ��������ж�

	SBUF=0xd4;
	while(!TI);
	TI=0;
#endif

	rs485SetModeRx();
	//�α�ʶλ������Ϊ�����ڽ����꣬�ñ�־Ȼ���ͳ�ȥ���ߵ�����������******
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


