#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
#include "reg51.h"

#include "../common/RS485.h"
#include "../common/sja.h"
#include "../common/canData.h"

void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);
//定义SJA1000的基址
unsigned char xdata *SJA_BaseAdr = 0XFE00;
//unsigned char data canRecvData[13];
bit initDone = 0;

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
bit canFrameReceived = 0;
unsigned char num = 0;



void delay_ms(int t);
void rs485SetModeRx(void);
void rs485SetModeTx(void);

void send_something(void)
{
	
	rs485SetModeTx();
	
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

	SJA_BCANAdr = REG_CONTROL;
	SBUF=(*SJA_BCANAdr);
	while(!TI);
	TI=0;

	SJA_BCANAdr = REG_ERRCATCH;
	SBUF=(*SJA_BCANAdr);
	while(!TI);
	TI=0;

	
	rs485SetModeRx();
}

void canEnableRecvInter(uint8 flag)
{
	if (flag)
	{
		*((uint8 *)REG_INTENABLE) |= 0x1;
	}
	else
	{
		*((uint8 *)REG_INTENABLE) &= 0xfe;
	}
}

void ex0_int(void) interrupt 0 using 1
{
	uint8 frameInfo;
	
	rs485SetModeTx();
	SBUF=0x66;
	while(!TI);
	TI=0;
	rs485SetModeRx();

	if (IS_RECEIVE_INTR) //receive interrupt
	{  
		rs485SetModeTx();
		SBUF=BCAN_READ_REG(REG_INTERRUPT) ;
		while(!TI);
		TI=0;

		frameInfo = BCAN_READ_REG(REG_RXBuffer1);
//		length = frameInfo & 0x0F;

		SBUF = frameInfo;
		while(!TI);
		TI=0;


		SBUF=0x67;
		while(!TI);
		TI=0;
		rs485SetModeRx();

		if ( (frameInfo & 0x40) != 0x40)  //数据帧   = 为远程帧
		{  
			if (frameInfo & 0x80)  //eff
			{
				//memcpy(&(canFrameData), REG_RXBuffer1, 13);
				canEnableRecvInter(0);
				canFrameReceived = 1;

				/* DO NOT clear receive buffer here !!!*/
				return;
			}
			else //sff
			{
				//ignored
			}
		}


		BCAN_CMD_PRG(RRB_CMD);//clear receive buffer
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
	EX0=1;
	EA=0;
}

/****************************************************
**函数原型：   void main(void)
**功    能：   主程序部分:
**入口参数:    无 
**返 回 值:     
*****************************************************/



void rs485SetModeRx()
{
	P17=0; //output

	if (initDone)
	{
		ES=1;
	}
}

void rs485SetModeTx()
{
	P17=1; //output
	ES=0;
}

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
	TL0=0xaf;  //50MS定时初值（T0计时用）
}

void timer0initial()
{
	TMOD |= 0x1;        //工作方式16位定时计数器
	setTimer();	
    ET0=1;
	TR0=1;
	PT0=1;   // higher priority than EX0
}

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


typedef enum handle_can_frame_state
{
	
	RS_PREPARE_REQ,
	RS_SEND_REQ,
	RS_WAIT_REPLY,
	RS_REQ_TIMEOUT,
	RS_SEND_REQ_RETRY,
	RS_REPLY_RECEIVED,

	PREPARE_CMD_REPLY,
	SEND_CAN_REPLY
}handle_can_frame_state;




typedef struct canCmdDataStruct
{	
	uint8 cmd;
	uint8 boardType;
	uint8 boardId;
	uint8 status;
	uint8 cmdData;
}canCmdDataStruct;



canFrameStruct canFrameDataToSend;

void can_send_cmd_reply(uint8 stationId)
{
	*((uint16 *)&(canFrameDataToSend.id[2])) = (stationId << 3);
	CAN_Send_Frame(&(canFrameDataToSend), sizeof(canCmdDataStruct));
}


#if 1
void handleCanFrame(canFrameStruct * canFramePtr)
{
	uint8 state = RS_PREPARE_REQ;
	uint16 lastTicket = 0;
	uint16 ticketDiff = 0;
	uint8 reqRetry = 0;
	uint8 stationId = (*((uint16 *)&(canFramePtr->id[2]))) >> 3; //hacke, to get station from request
	canCmdDataStruct *cmdDataPtr =  (canCmdDataStruct *)&(canFramePtr->canData[0]);
	canCmdDataStruct *replyCmdPtr = 0;
	
	while(1)
	{
		switch(state)
		{
			case RS_PREPARE_REQ:
			{
				gRsData.boardId = cmdDataPtr->boardId;
				gRsData.boardType = cmdDataPtr->boardType;
				gRsData.cmd = cmdDataPtr->cmd | CMD_BIT_REQ;
				gRsData.status = cmdDataPtr->status;
				gRsData.rsData = cmdDataPtr->cmdData;

				state = RS_SEND_REQ;
				break;
			}
		
			case RS_SEND_REQ:
			{
				rs485SetModeTx();
				rsDataSend(&gRsData, sizeof(RS485DataStruct));
				rs485SetModeRx();
				
				lastTicket = timerTicket;
				state = RS_WAIT_REPLY;
				break;
			}

			case RS_WAIT_REPLY:
			{
				if (timerTicket > lastTicket)
				{
					ticketDiff = timerTicket - lastTicket;
				}
				else
				{
					ticketDiff = 0xffff - lastTicket + timerTicket; 
				}

				if (ticketDiff > 2)
				{
					state = RS_SEND_REQ_RETRY;
				}
				else if (rsFrameReceived)
				{	
					state = RS_REPLY_RECEIVED;
				}

				break;
			}

			case RS_REPLY_RECEIVED:
			{
				if ( cmdDataPtr->boardType == gRsData.boardType
						&& cmdDataPtr->boardId == gRsData.boardId)
				{

					replyCmdPtr = (canCmdDataStruct *)&(canFrameDataToSend.canData[0]);

					replyCmdPtr->boardId = cmdDataPtr->boardId;
					replyCmdPtr->boardType = cmdDataPtr->boardType;
					replyCmdPtr->cmd = cmdDataPtr->cmd;
					replyCmdPtr->status = gRsData.status;
					replyCmdPtr->cmdData = gRsData.rsData;

					rsFrameReceived = 0;
					reqRetry = 0;

					state = SEND_CAN_REPLY;
				}
				else
				{
					state = RS_SEND_REQ_RETRY;
				}

				break;
			}

			case RS_SEND_REQ_RETRY:
			{
				if (reqRetry > 2)
				{
					state = RS_REQ_TIMEOUT;
				}
				else
				{
					reqRetry++;
					state = RS_SEND_REQ;
				}
				break;
			}

			case RS_REQ_TIMEOUT:
			{
				replyCmdPtr = (canCmdDataStruct *)&(canFrameDataToSend.canData[0]);

				replyCmdPtr->boardId = cmdDataPtr->boardId;
				replyCmdPtr->boardType = cmdDataPtr->boardType;
				replyCmdPtr->cmd = cmdDataPtr->cmd;
				replyCmdPtr->status = Board_status_Disconnect;

				state = SEND_CAN_REPLY;
				break;
			}

			case SEND_CAN_REPLY:
			{
				can_send_cmd_reply(stationId);
				return;
			}

		}

	}
}

#endif



#if 0
void handleCanFrame(canFrameStruct * canFramePtr)
{
	uint8 state = RS_SEND_REQ;

	uint8 reqIndex = 0;
	uint16 lastTicket = 0;
	uint16 ticketDiff = 0;
	uint8 reqRetry = 0;


	uint8 sendBoardId = 0;
	uint8 sendBoardType = 0;
	uint8 sendRsData = 0;

	uint8 canDataIndex = 0;
	uint8 sendInputBoardCnt = 0;
	uint8 sendOutputBoardCnt = 0;

	uint8 canFrameType = canFramePtr->id[0] & 0x7f;
	uint8 inBoardCnt = (canFramePtr->id[1] & 0xf0) >> 4;
	uint8 outBoardCnt = (canFramePtr->id[1] & 0x0f);

	uint8 *canDataPtr = &(canFramePtr->canData[0]);

	//uint8 stationId = ((canFramePtr->id[2] & 0x7) << 5) & ((canFramePtr->id[3] & 0xf8) >> 3)

	uint8 stationId = (*((uint16 *)&(canFramePtr->id[2]))) >> 3; //hacke, to get station from request
	bit allOk = 0;
	
	while(1)
	{
		switch(state)
		{
			case RS_SEND_REQ:
			{
				if (reqRetry != 1)
				{
					reqRetry = 0;
					if (inBoardCnt)
					{
						sendBoardId = *(canDataPtr++);
						sendBoardType = BOARD_INPUT;
						inBoardCnt--;
					}
					else if (outBoardCnt)
					{
						if (CMD_STATUS_CHECK == canFrameType)
						{
							sendBoardId = *(canDataPtr++);
						}
						else if (CMD_SET_ACTION == canFrameType)
						{
							sendBoardId = *(canDataPtr++);
							sendRsData = *(canDataPtr++);
						}
						sendBoardType = BOARD_OUTPUT;
						outBoardCnt--;
					}
					else
					{
						state = SEND_CAN_REPLY;
						allOk = 1;
						break;
					}
				}

				gRsData.boardId = sendBoardId;
				gRsData.boardType = sendBoardType;
				gRsData.cmdType = canFrameType;
				gRsData.status = canFrameType;
				gRsData.rsData = sendRsData;

				rs485SetModeTx();
				rsDataSend(&gRsData, sizeof(RS485DataStruct));
				rs485SetModeRx();
				lastTicket = timerTicket;

				state = RS_WAIT_REPLY;

				break;
			}

			case RS_WAIT_REPLY:
			{
				if (timerTicket > lastTicket)
				{
					ticketDiff = timerTicket - lastTicket;
				}
				else
				{
					ticketDiff = 0xffff - lastTicket + timerTicket; 
				}

				if (ticketDiff > 2)
				{
					state = RS_SEND_REQ_RETRY;
				}
				
				if (rsFrameReceived)
				{	
					state = RS_REPLY_RECEIVED;
				}

				break;
			}

			case RS_REPLY_RECEIVED:
			{
				if ( sendBoardType == gRsData.boardType
					&& sendBoardId == gRsData.boardId)
				{
					canFrameDataToSend.canData[canDataIndex++] = gRsData.boardId;
					if (sendBoardType == BOARD_INPUT)
					{
						canFrameDataToSend.canData[canDataIndex++] = gRsData.rsData;
						sendInputBoardCnt++;
					}
					else
					{
						sendOutputBoardCnt++;
					}

					if (canDataIndex == 8)
					{
						state = SEND_CAN_REPLY;
						canDataIndex = 0;
					}
					else
					{
						state = RS_SEND_REQ;
					}

					rsFrameReceived = 0;

					reqRetry = 0;

				}
				else
				{
					state = RS_SEND_REQ_RETRY;
				}

				break;
			}

			case RS_SEND_REQ_RETRY:
			{
				reqRetry++;
				state = RS_SEND_REQ;
				break;
			}

			case SEND_CAN_REPLY:
			{
				canFrameDataToSend.id[0] = canFrameType & 0x80;
				canFrameDataToSend.id[1] = (sendInputBoardCnt << 4) | sendOutputBoardCnt;
				*((uint16 *)&(canFrameDataToSend.id[2])) = (stationId << 3);
				CAN_Send_Frame(&(canFrameDataToSend));
				
				if (!allOk)
				{
					state = RS_SEND_REQ;
				}
				else
				{
					return;
				}
				break;
			}

		}

	}
}

#endif



void main(void)
{  
	unsigned char ss;
	unsigned char num = 0;
		
	CS=0;                //片选择引脚
	
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

	EA=1; //初始化成功，开总中断
#endif


	initDone = 1;
	rs485SetModeRx();
	//次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
	while(1) 
	{
	
#if 0
		if (rsFrameReceived)
		{
			rs485SetModeTx();
			rsDataSend(&gRsData, sizeof(gRsData));
			rs485SetModeRx();
			rsFrameReceived = 0;
		}
#endif
		if (canFrameReceived)
		{
			//delay_ms(500);
			//
			//delay_ms(500);
			//delay_ms(500);
		

			rs485SetModeTx();
			SBUF=0x98;
			while(!TI);
			TI=0;
			rs485SetModeRx();
		
			while (BCAN_READ_REG(REG_STATUS)&0x1) //receive buffer not empty
			{

		#if 0	
				memcpy((uint8 *)&(canFrameData), (uint8 *)REG_RXBuffer1, 13);
				rs485SetModeTx();
				SBUF=0x99;
				while(!TI);
				TI=0;
				rs485SetModeRx();
		#endif
				handleCanFrame((uint8 *)REG_RXBuffer1);
				BCAN_CMD_PRG(RRB_CMD);
				//CAN_Send_Frame(&(canFrameData));
			}

			canFrameReceived = 0;
			canEnableRecvInter(1);
		}


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
				memset(&(gRsData), 0, sizeof(gRsData));

				gRsData.boardType = 0xf1;
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

				SJA_BCANAdr = REG_CONTROL;
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


