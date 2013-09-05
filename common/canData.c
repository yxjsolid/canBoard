#include "stdio.h"
#include "string.h"
#include "canData.h"




canFrameStruct canData;

void setBoard(uint8 id, uint8 dataIn)
{



}


void handleCmdSet(int dataSize, uint8 *dataIn)
{
	unsigned char i = 0;
	uint8 * dataPtr = dataIn;

	for(i = 0; i < dataSize/2; i++)
	{
		dataPtr += i*2;
		setBoard(*dataPtr, *(dataPtr+1));
	}
}



void handleCanRequest(canFrameStruct * canFrame)
{
	uint8 cmd = GET_CMD(canFrame);
	int dataSize = GET_DATA_SIZE(canFrame);

	switch(cmd)
	{
		case CMD_SET:
		{
			handleCmdSet(dataSize, canFrame->canData);
			break;
		}


		case CMD_READ:
		{

			break;
		}
		
		

		

			
		
	}
	



}


void sendCanReply(canFrameStruct * canFrame)
{



}






