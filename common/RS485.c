#include "stdio.h"
#include "string.h"
#include "reg51.h"
#include "RS485.h"

RS485DataStruct gRsData;

void init_serialcomm(uint8 baudRate, uint8 freq)
{
    SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr
    TMOD &= 0x0f;
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 

	switch(baudRate)
	{
		case RS_Baudrate_4800:
		{

			if ( OSCILLA_FREQ_11M == freq )
			{
				TH1   = 0xF4;
   			 	TL1   = 0xF4;
			}
			else if ( OSCILLA_FREQ_12M == freq )
			{
				TH1   = 0xF3;
				TL1   = 0xF3;
			}
		}
			break;

		case RS_Baudrate_9600:
		{
			if ( OSCILLA_FREQ_11M == freq )
			{
				TH1   = 0xFA;
				TL1   = 0xFA;
			}
			else if ( OSCILLA_FREQ_12M == freq )
			{
				// tobe determined
			}
		}
			break;
			
	}

    IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 
    ET1  = 0;
}



void serial_send_char(uint8 dataIn)
{
	SBUF = dataIn;
	while(!TI);
	TI = 0;
}

void serial_send_data(uint8 *dataIn, uint8 size)
{
	uint8 i = 0;
	for( i = 0; i < size; i++)
	{
		serial_send_char(*(dataIn + i));
	}
}

void serial_send_string(uint8 *strIn)
{
	serial_send_data(strIn, strlen(strIn));
}

//$KA,XX,XX,XX,*
bit searchDataStartPattern(uint8 charIn)
{
	static uint8 state = 0;
	//char *pattern = START_PATTERN;

	/*
		$: state == 1
		K: state == 2
		A: state == 3
		
	*/
	switch(state)
	{
		case 0: //no date
		{
			if (charIn == '$')
			{
				
				state = 1;
			}
			break;
		}

		case 1: //'$' reveived, waiting 'K' 
		{

			if (charIn == 'K')
			{
				state = 2;
			}
			else
			{
				state = 0;
			}
			break;
		}

		case 2: //'$K' received, waiting 'A'
		{
			if (charIn == 'A')
			{
				state = 3;
			}
			else
			{
				state = 0;
			}
			break;
		}

		default:
			/*never happen*/
			break;
	}

	if (state == 3)
	{
		state = 0;
		return 1;
	}

	return 0;
}

uint8 rsDataReceive(uint8 chIn, uint8 * buf, uint8 bufSize)
{
	static bit frameFound = 0;
	static uint8 frameLen = 0;
	static uint8 dataIndex = 0;
	uint8 isDataReady = 0;
	uint8 *rsDataPtr = buf;

	if (!frameFound)
	{	
		frameFound = searchDataStartPattern(chIn);
		//frameLen = strlen(START_PATTERN);
		frameLen = 3;
	}
	else
	{
		frameLen++;
		if(searchDataStartPattern(chIn))
		{
			//frameLen = strlen(START_PATTERN);
			frameLen = 3;
			dataIndex = 0;
			return isDataReady;
		}	

		if (chIn == SPLITER)
		{
			//SKIP
		}
		else
		{
			if (dataIndex < bufSize)
			{
				rsDataPtr[dataIndex] = chIn;
			}
			dataIndex++;
		}
		
		if (frameLen == FRAME_SIZE(bufSize))
		{
			frameFound = 0;
			dataIndex = 0;
			frameLen = 0;
		
			if (chIn == END_PATTERN)
			{
				isDataReady = 1;
			}
		}
	}
	return isDataReady;
}

void rsDataSend(uint8 *rsDataIn, int size)
{
	uint8 idata buffer[30];
	uint8 index = 0;
	uint8 i = 0;

	strcpy(buffer, START_PATTERN);
	index += strlen(START_PATTERN);

	buffer[index++] = SPLITER;
	for( i = 0; i < size; i++)
	{
		buffer[index++] = ((uint8 *)rsDataIn)[i];
		if (i%2)
		{
			buffer[index++] = SPLITER;
		}
	}

	if (size%2)
	{
		buffer[index++] = SPLITER;
	}
	
	buffer[index++] = END_PATTERN;
	serial_send_data(buffer, index);
}



bit isRsCmdValid()
{
	if ( !(gRsData.status & STAT_BIT_REQ))
	{
		/*cmd is reply, ignored*/
		return 0;
	}
	else
	{
		if (gRsData.boartType == boardType && gRsData.boardId == boardID)
		{
			return 1;
		}
	}

	return 0;
}

uint8 getReplyStatus()
{
	uint8 status = gRsData.status;
	
	switch(boardStatus)
	{
		case STAT_BIT_INIT:
		{
			if (status & STAT_BIT_CONNECTED)
			{
				boardStatus = STAT_BIT_RECOVER;
				return STAT_BIT_RECOVER;
			}

			if (status & STAT_BIT_INIT)
			{
				boardStatus = STAT_BIT_CONNECTED;
				return STAT_BIT_CONNECTED;
			}

		}
			break;

		case STAT_BIT_CONNECTED:
		{
			return STAT_BIT_CONNECTED;
		}
			break;

		case STAT_BIT_RECOVER:
		{
			if (status & STAT_BIT_RECOVER_REPLY)
			{
				boardStatus = STAT_BIT_CONNECTED;
				return STAT_BIT_CONNECTED;
			}
			
			return STAT_BIT_RECOVER;
		}
			break;

		default:
			/*error?*/
			return 0;
			break;

	}
}

void handleRsCmd(void)
{
	uint8 status = 0;
	uint8 replyData = 0;
	RS485DataStruct rsReplyData;

	serial_send_string("handle\n");

	if ( isRsCmdValid())
	{
		serial_send_string("valid\n");

	
		status = getReplyStatus();

		switch(boardType)
		{
			case BOARD_INPUT:
			{
				replyData = P1;
			}
				break;

			case BOARD_OUTPUT:
			{
				P1 = gRsData.rsData;
			}
				break;
				
		}

		rsReplyData.boartType = boardType;
		rsReplyData.boardId = boardID;
		rsReplyData.status = status;
		rsReplyData.rsData = replyData;

		rsDataSend(&rsReplyData, sizeof(rsReplyData));
	}
	else
	{
		serial_send_string("error\n");
	}
}



