#ifndef	__RS_485_h__
#define	__RS_485_h__

#include "types.h"

#define RS_Baudrate_4800 1
#define RS_Baudrate_9600 2


//$KA,XX,XX,XX,*
#define START_PATTERN "$KA"
#define END_PATTERN	'*'
#define SPLITER	','
#define RSDATA_SIZE	(sizeof(RS485DataStruct))
#define FRAME_SIZE(x) (5 + x + x/2 + x%2)
#define RSDATA_FRAME_SIZE FRAME_SIZE(RSDATA_SIZE)


#define CMD_BIT_REQ 			0x80
typedef struct RS485DataStruct
{	
	uint8 cmd;
	uint8 boardType;
	uint8 boardId;
	uint8 status;
	uint8 rsData;
}RS485DataStruct;


enum BoardStat
{
	STAT_IDLE = 0,//board not used
	STAT_OK,	// board replied
	STAT_WAIT_REPLY, // request send, reply not reveived
	STAT_RETRANS_EXCEED,
	STAT_CMD_ERROR, //board return recieved cmd error

	STAT_UNKNOWN,
	STAT_MAX = 15
};


#if 0
typedef enum rs_state
{
	RS_IDLE = 0,
	RS_READY_TO_SEND,
	RS_WAIT_REPLY,
	RS_REPLY_RECV
}rs_state;
#endif

extern RS485DataStruct gRsData;


extern uint8 boardID;
extern uint8 boardType;
extern uint8 boardStatus;

extern void init_serialcomm(uint8 baudRate, uint8 freq);
extern void serial_send_char(uint8 dataIn);
extern void serial_send_data(uint8 *dataIn, uint8 size);
extern void serial_send_string(uint8 *strIn);
extern uint8 rsDataReceive(uint8 chIn, uint8 * buf, uint8 bufSize);
extern void rsDataSend(uint8 *rsDataIn, int size);
extern  bit searchDataStartPattern(uint8 charIn);
extern void handleRsCmd(void);

#endif
