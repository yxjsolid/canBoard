#include "stdio.h"
#include "string.h"
#include "reg51.h"
#include "RS485.h"


char ch;

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

RS485DataStruct rsData;
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
		frameLen = strlen(START_PATTERN);
	}
	else
	{
		frameLen++;

		if(searchDataStartPattern(chIn))
		{
			frameLen = strlen(START_PATTERN);
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

	return isDataReady;
}

void rsDataSend(uint8 *rsDataIn, int size)
{
	uint8 idata buffer[48];
	uint8 index = 0;
	uint8 i = 0;

	strcpy(buffer, START_PATTERN);
	index += strlen(START_PATTERN);

	buffer[index++] = ',';
	for( i = 0; i < size; i++)
	{
		buffer[index++] = ((uint8 *)rsDataIn)[i];
		if (i%2)
		{
			buffer[index++] = ',';
		}
	}

	if (size%2)
	{
		buffer[index++] = ',';
	}
	
	buffer[index++] = '*';
	serial_send_data(buffer, index);
}



