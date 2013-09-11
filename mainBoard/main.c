#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
#include "reg51.h"

#include "../common/RS485.h"
#include "../common/sja.h"

void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);
//����SJA1000�Ļ�ַ
unsigned char code *SJA_BaseAdr = 0XFE00;
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

int timerTicket = 0;


//BoardStatus idata OutPutBoard[16];
//BoardStatus idata InPutBoard[16];

void delay_ms(int t);
void rs485SetModeRx(void);
void rs485SetModeTx(void);


void send_something(void)
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

	rs485SetModeRx();
}
void ex0_int(void) interrupt 0 using 1
{  
	unsigned char tt,length,i;
	SJA_BCANAdr=REG_INTERRUPT;


	rs485SetModeTx();

	SBUF=0x1;
	while(!TI);
	TI=0;	

	SBUF=0x2;
	while(!TI);
	TI=0;
	rs485SetModeRx();


	if((*SJA_BCANAdr)&0x01)                   //�����˽����ж�
	{  
		SJA_BCANAdr=REG_RXBuffer1;
		tt=*SJA_BCANAdr;
		length=tt&0x0F;
		if ((tt&0x40)!=0x40)                   //����֡   = ΪԶ��֡
		{  
			//SJA_BCANAdr =REG_RXBuffer4 ;           //�궨��ı�������memcpy(RevceData,REG_RXBuffer4,8); 
			SJA_BCANAdr = REG_RXBuffer1 ;

			//memcpy(RevceData,SJA_BCANAdr,13);  //���ܣ���src��ָ�ڴ�������count���ֽڵ�dest��ָ�ڴ�����
			//memcpy(Com_RecBuff,RevceData,8);      //�����õ���Ҫ�ǰѽ��յ��������ڷ���ȥ����֤���ݵ���ȷ
			                                //���´����Ƿ��͵���
		#if 0
			rs485SetModeTx();
			memcpy(RevceData,SJA_BCANAdr,13);
			for(i=0;i<13;i++)
			{
				SBUF=RevceData[i];
				while(!TI);
				TI=0;
			}
 			rs485SetModeRx();
		#endif	
		}

		BCAN_CMD_PRG(RRB_CMD);                  //�ͷ�SJA1000���ջ�������****�Ѿ��޸�
	}


	send_something();

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
	
	if ((temptt&0x40)==0x40)                     //������״̬
	{  
		SBUF=0x99;
		while(!TI);
		TI=0;
	
		//Sja_1000_Init();
		serial_send_char(0xee);                    //����ר�÷��͵����ڿ�״̬    
	}


	SBUF=0xaa;
	while(!TI);
	TI=0;
}



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
unsigned char a;
bit flag = 0;
bit rsFrameReceived = 0;

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
uint8 inBuffer[RSDATA_FRAME_SIZE];

void serial() interrupt 4
{

	num++;
	a = SBUF;
	flag = 1;
	RI = 0;
	
	rsFrameReceived = 0;
	if (rsDataReceive(a, &gRsData, sizeof(RS485DataStruct)))
	{
		rsFrameReceived = 1;
		rs485SetModeTx();//disable serial interrupt
	}
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

//int timer_flag = 0;
//int timer_count = 0;

//��ʱ��0�ж�,����8�����ڴ˷���
void time_intt0(void) interrupt 1 using 2
{
	static unsigned char timer_count = 0;
	setTimer();
	if (timer_count == 4)
	{
		P10 = !P10;
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
	
uint8 OPERA_STATE = OP_STAT_IDLE;


void stateMachine(uint8 state)
{
	switch(state)
	{
		case OP_STAT_IDLE:
		{

		}
			break;

		case OP_STAT_CMD_RECV:
		{

		}
			break;

		case OP_STAT_CMD_HANDLE:
		{

		}
			break;

		case OP_STAT_CMD_REPLY:
		{

		}
			break;
	}
}


rs_state rsState = RS_IDLE;

void rs485StateMachine(int ticketIn)
{
	static int lastTicket = 0;
	int ticketTmp = 0;
	
	switch(rsState)
	{
		case RS_IDLE:
		{

		}
			break;

		case RS_READY_TO_SEND:
		{	
			rs485SetModeTx();
			gRsData.boartType = 0xf1;
			gRsData.boardId = 0xf2;
			gRsData.status = 0xf3;
			gRsData.rsData = 0xf4;
			rsDataSend(&gRsData, sizeof(gRsData));
			rs485SetModeRx();
			lastTicket = ticketIn;

			rsState = RS_WAIT_REPLY;
		}
			break;

		case RS_WAIT_REPLY:
		{
			if (ticketIn > lastTicket)
			{
				ticketTmp = ticketIn - lastTicket;
			}
			else
			{
				ticketTmp = ticketIn + 0xffff - lastTicket;
			}
			
			if (rsFrameReceived)
			{	
				rs485SetModeTx();
				gRsData.boartType = *((uint8 *)ticketTmp);
				gRsData.boardId = *(((uint8 *)ticketTmp)+1);
				
				rsDataSend(&gRsData, sizeof(gRsData));
				rs485SetModeRx();
				rsFrameReceived = 0;
			}
		}
			break;

		case RS_REPLY_RECV:
		{

		}
			break;

		
	}
}



void main(void)
{  
	unsigned char ss;
	unsigned char num = 0;
		
	CS=0;                //Ƭѡ������
	EA=0;
	Init_Cpu(); 

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

    //��ʼ��SJA1000     
    ss=Sja_1000_Init(OSCILLA_FREQ_12M);
    if (ss!=0)             //��ʼ��ʧ��
    {

		SBUF=ss;
		while(!TI);
		TI=0;
	  //send_string_com("init fail!");**********************
      serial_send_char(0xBB);              //����ר�÷��͵����ڿ�״̬   
    }




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

	EA=1; //��ʼ���ɹ��������ж�
#endif

	rs485SetModeRx();
	//�α�ʶλ������Ϊ�����ڽ����꣬�ñ�־Ȼ���ͳ�ȥ���ߵ�����������******
	while(1) 
	{

		if (rsFrameReceived)
		{
			rs485SetModeTx();
			rsDataSend(&gRsData, sizeof(gRsData));
			rs485SetModeRx();
			rsFrameReceived = 0;
		}


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
				memset(&(gRsData), 0, sizeof(gRsData));

				gRsData.boartType = 0xf1;
				gRsData.boardId = 0xf2;
				gRsData.status= 0xf3;
				gRsData.rsData = 0xf4;
			
				
				rsDataSend(&gRsData, sizeof(gRsData));
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

				//Sja_test(a);
			}

			
			rs485SetModeRx();
			
		}
#endif
	}

}


