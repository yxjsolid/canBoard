#include "stdio.h"
#include "string.h"
#include "reg51.h"
#include "RS485.h"

RS485DataStruct rsData;

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
	char *pattern = START_PATTERN;

	/*
		$: state == 1
		K: state == 2
		A: state == 3
		
	*/

	switch(state)
	{
		case 0: //no date
		{
			if (charIn == pattern[state])
			{
				state = 1;
			}
			break;
		}

		case 1: //'$' reveived, waiting 'K' 
		{

			if (charIn == pattern[state])
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
			if (charIn == pattern[state])
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

	//sendTest(0x1);

	if (!frameFound)
	{
		//sendTest(0x2);
	
		frameFound = searchDataStartPattern(chIn);
		//sendTest(0x3);
		//frameLen = strlen(START_PATTERN);
		frameLen = 3;
	
		//sendTest(0x4);
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
			if (chIn == END_PATTERN)
			{
				isDataReady = 1;
			}
			else
			{
				/* frame error*/
				frameFound = 0;
				dataIndex = 0;
				frameLen = 0;	
			}
		}
	}

	//sendTest(0x5);
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



