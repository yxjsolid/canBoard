typedef unsigned char uint8;

//$KA,XX,XX,XX,*
#define START_PATTERN "$KA"
#define END_PATTERN	'*'
#define SPLITER	','
#define DATA_SIZE	(sizeof(RS485DataStruct))
#define FRAME_SIZE (5+ DATA_SIZE/2 + DATA_SIZE%2 + DATA_SIZE)

typedef struct RS485DataStruct
{	
	uint8 boartType;
	uint8 boardId;
	uint8 cmd;
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
	STAT_MAX = 15;
};

typedef struct BoardStatus
{
	uint8 boardStat;
	uint8 reTransmit:4,
		  lastTicket:4;
}BoardStatus;







extern RS485DataStruct rsData;

extern void serial_send_char(uint8 dataIn);
extern void serial_send_data(uint8 *dataIn, uint8 size);
extern void rsDataSend(RS485DataStruct *rsData);

