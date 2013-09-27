#include "stdio.h"
#include "string.h"
#include "canData.h"



//canFrameStruct canFrameData;

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




