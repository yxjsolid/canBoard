#ifndef	__CAN_DATA_h__
#define	__CAN_DATA_h__

#include "types.h"


#define GET_DATA_SIZE(x)	(((x)->info)&0x0f)
#define GET_CMD(x)		((x->id[0])&0x7f)


#define FRAME_TYPE_REQUEST	1
#define FRAME_TYPE_REPLY	0



#define HEART_BEAT_FRAME	0x01



typedef struct canFrameStruct
{	
	uint8 info;
	uint8 id[4];
	uint8 canData[8];
}canFrameStruct;





//extern canFrameStruct canFrameData;

#endif

