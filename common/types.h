#ifndef	__TYPES_h__
#define	__TYPES_h__
#include "reg51.h"

typedef unsigned char uint8;
typedef unsigned int uint16;

#define OSCILLA_FREQ_11M	1
#define OSCILLA_FREQ_12M	2
#define OSCILLA_FREQ_16M	3

#define BOARD_INPUT	1
#define BOARD_OUTPUT 2




typedef enum CMD_TYPE
{
	CMD_STATUS_UNKNOWN = 0,
	CMD_STATUS_CHECK,
	CMD_SET_ACTION,
}CMD_TYPE;


typedef enum Board_Status
{
	Board_status_Init = 0,
	Board_status_Connected,
	Board_status_Disconnected,
	Board_status_Recover,
	Board_status_recover_Reply,
	Board_status_Ok,
}Board_Status;


#endif

