#ifndef	__SJA_h__
#define	__SJA_h__

#include "types.h"
#include "canData.h"

extern uint8 xdata *SJA_BaseAdr;
extern uint8 xdata *SJA_BCANAdr;

#define         REG_CONTROL       SJA_BaseAdr+0x00       //�ڲ����ƼĴ���
#define         REG_COMMAND       SJA_BaseAdr+0x01       //����Ĵ���
#define         REG_STATUS        SJA_BaseAdr+0x02       //״̬�Ĵ���
#define         REG_INTERRUPT     SJA_BaseAdr+0x03       //�жϼĴ���
#define         REG_INTENABLE     SJA_BaseAdr+0x04       //�ж�ʹ�ܼĴ���
#define         REG_BTR0          SJA_BaseAdr+0x06       //���߶�ʱ�Ĵ���0
#define         REG_BTR1          SJA_BaseAdr+0x07       //���߶�ʱ�Ĵ���1
#define         REG_OCR           SJA_BaseAdr+0x08       //������ƼĴ���
#define         REG_TEST          SJA_BaseAdr+0x09       //���ԼĴ���

#define         REG_RESVER1       SJA_BaseAdr+0x0A       //����1
#define         REG_ARBITRATE     SJA_BaseAdr+0x0B       //�ٲö�ʧ��׽
#define         REG_ERRCATCH      SJA_BaseAdr+0x0C       //������벶׽
#define         REG_ERRLIMIT      SJA_BaseAdr+0x0D       //���󱨾��޶�

#define         REG_RXERR         SJA_BaseAdr+0x0E         //���մ��������
#define         REG_TXERR         SJA_BaseAdr+0x0F         //���ʹ��������

#define         REG_ACR1          SJA_BaseAdr+0x10       //���մ���Ĵ���
#define         REG_ACR2          SJA_BaseAdr+0x11       //���մ���Ĵ���
#define         REG_ACR3          SJA_BaseAdr+0x12       //���մ���Ĵ���
#define         REG_ACR4          SJA_BaseAdr+0x13       //���մ���Ĵ���
#define         REG_AMR1          SJA_BaseAdr+0x14       //�������μĴ���
#define         REG_AMR2          SJA_BaseAdr+0x15       //�������μĴ���
#define         REG_AMR3          SJA_BaseAdr+0x16       //�������μĴ���
#define         REG_AMR4          SJA_BaseAdr+0x17       //�������μĴ���

// ���ͻ������Ĵ���
#define         REG_TXBuffer1     SJA_BaseAdr+0x10         //���ͻ�����1
#define         REG_TXBuffer2     SJA_BaseAdr+0x11         //���ͻ�����2
#define         REG_TXBuffer3     SJA_BaseAdr+0x12         //���ͻ�����3
#define         REG_TXBuffer4     SJA_BaseAdr+0x13         //���ͻ�����4
#define         REG_TXBuffer5     SJA_BaseAdr+0x14         //���ͻ�����5
#define         REG_TXBuffer6     SJA_BaseAdr+0x15         //���ͻ�����6
#define         REG_TXBuffer7     SJA_BaseAdr+0x16         //���ͻ�����7
#define         REG_TXBuffer8     SJA_BaseAdr+0x17         //���ͻ�����8
#define         REG_TXBuffer9     SJA_BaseAdr+0x18         //���ͻ�����9
#define         REG_TXBuffer10    SJA_BaseAdr+0x19         //���ͻ�����10
#define         REG_TXBuffer11    SJA_BaseAdr+0x1A         //���ͻ�����11
#define         REG_TXBuffer12    SJA_BaseAdr+0x1B         //���ͻ�����12
#define         REG_TXBuffer13    SJA_BaseAdr+0x1C         //���ͻ�����13

// ���ջ������Ĵ���
#define         REG_RXBuffer1       SJA_BaseAdr+0x10       //���ջ�����1
#define         REG_RXBuffer2       SJA_BaseAdr+0x11       //���ջ�����2
#define         REG_RXBuffer3      SJA_BaseAdr+0x12        //���ջ�����3
#define         REG_RXBuffer4       SJA_BaseAdr+0x13       //���ջ�����4
#define         REG_RXBuffer5      SJA_BaseAdr+0x14        //���ջ�����5
#define         REG_RXBuffer6     SJA_BaseAdr+0x15         //���ջ�����6
#define         REG_RXBuffer7     SJA_BaseAdr+0x16         //���ջ�����7
#define         REG_RXBuffer8     SJA_BaseAdr+0x17         //���ջ�����8
#define         REG_RXBuffer9     SJA_BaseAdr+0x18         //���ջ�����9
#define         REG_RXBuffer10     SJA_BaseAdr+0x19        //���ջ�����10
#define         REG_RXBuffer11     SJA_BaseAdr+0x1A        //���ջ�����11
#define         REG_RXBuffer12     SJA_BaseAdr+0x1B        //���ջ�����12
#define         REG_RXBuffer13     SJA_BaseAdr+0x1C        //���ջ�����13

#define         REG_RXCOUNT       SJA_BaseAdr+0x1D         //RX���ļ�����
#define         REG_RBSA          SJA_BaseAdr+0x1E         //���ջ�������ʼ��ַ
#define         REG_CDR           SJA_BaseAdr+0x1F         //ʱ�ӷ�Ƶ�Ĵ���


/*
����˵��:   CAN������SJA1000ͨѶ������.SJA1000�ľ���Ϊ����Ϊ16MHZ*/

#define         BTR0_Rate_20k      0x53          //20KBPS��Ԥ��ֵ
#define         BTR1_Rate_20k      0x2F          //20KBPS��Ԥ��ֵ
#define         BTR0_Rate_40k      0x87          //40KBPS��Ԥ��ֵ
#define         BTR1_Rate_40k      0xFF          //40KBPS��Ԥ��ֵ
#define         BTR0_Rate_50k      0x47          //50KBPS��Ԥ��ֵ
#define         BTR1_Rate_50k      0x2F          //50KBPS��Ԥ��ֵ
#define         BTR0_Rate_80k      0x83          //80KBPS��Ԥ��ֵ
#define         BTR1_Rate_80k      0xFF          //80KBPS��Ԥ��ֵ
#define         BTR0_Rate_100k     0x43          //100KBPS��Ԥ��ֵ
#define         BTR1_Rate_100k     0x2f          //100KBPS��Ԥ��ֵ
#define         BTR0_Rate_125k     0x03          //125KBPS��Ԥ��ֵ
#define         BTR1_Rate_125k     0x1c          //125KBPS��Ԥ��ֵ
#define         BTR0_Rate_200k     0x81          //200KBPS��Ԥ��ֵ
#define         BTR1_Rate_200k     0xFA          //200KBPS��Ԥ��ֵ
#define         BTR0_Rate_250k     0x01          //250KBPS��Ԥ��ֵ
#define         BTR1_Rate_250k     0x1c          //250KBPS��Ԥ��ֵ
#define         BTR0_Rate_400k     0x80          //400KBPS��Ԥ��ֵ
#define         BTR1_Rate_400k     0xfa          //400KBPS��Ԥ��ֵ
#define         BTR0_Rate_500k     0x00          //500KBPS��Ԥ��ֵ
#define         BTR1_Rate_500k     0x1c          //500KBPS��Ԥ��ֵ
#define         BTR0_Rate_666k     0x80          //666KBPS��Ԥ��ֵ
#define         BTR1_Rate_666k     0xb6          //666KBPS��Ԥ��ֵ
#define         BTR0_Rate_800k     0x00          //800KBPS��Ԥ��ֵ
#define         BTR1_Rate_800k     0x16          //800KBPS��Ԥ��ֵ
#define         BTR0_Rate_1000k    0x00          //1000KBPS��Ԥ��ֵ
#define         BTR1_Rate_1000k    0x14          //1000KBPS��Ԥ��ֵ
//BPS
#define         ByteRate_10k       10 //24MHZ**********************
#define         ByteRate_40k       40
#define         ByteRate_50k       50
#define         ByteRate_80k       80
#define         ByteRate_100k      100
#define         ByteRate_125k      125
#define         ByteRate_200k      200
#define         ByteRate_250k      250
#define         ByteRate_400k      400
#define         ByteRate_500k      500
#define         ByteRate_800k      800
#define         ByteRate_1000k     1000

//������
#define    TR_CMD     0X01
#define    AT_CMD     0X02
#define    RRB_CMD    0X04 //reset receive buffer
#define    COS_CMD    0X08
#define    SRR_CMD    0X10
#define    GTS_CMD    0X10
//������

#define CAN_INTERFACE_OK      0
#define CAN_BUS_OK            0
#define CAN_INTERFACE_ERR     0XFF
#define CAN_ENTERSET_ERR      0XFE
#define CAN_QUITSET_ERR       0XFD
#define CAN_INITOBJECT_ERR    0XFC
#define CAN_INITBTR_ERR       0XFB
#define CAN_INITOUTCTL_ERR    0XFA
#define CAN_INTCLKDIV_ERR     0XF9
#define CAN_BUS_ERR           0XF8


#define REG_TX_FAME_INFO	REG_TXBuffer1
#define REG_TX_FAME_SIG1	REG_TXBuffer2
#define REG_TX_FAME_SIG2	REG_TXBuffer3
#define REG_TX_FAME_SIG3	REG_TXBuffer4
#define REG_TX_FAME_SIG4	REG_TXBuffer5
#define REG_TX_FAME_DATA1	REG_TXBuffer6
#define REG_TX_FAME_DATA2	REG_TXBuffer7
#define REG_TX_FAME_DATA3	REG_TXBuffer8
#define REG_TX_FAME_DATA4	REG_TXBuffer9
#define REG_TX_FAME_DATA5	REG_TXBuffer10
#define REG_TX_FAME_DATA6	REG_TXBuffer11
#define REG_TX_FAME_DATA7	REG_TXBuffer12
#define REG_TX_FAME_DATA8	REG_TXBuffer13






#define BCAN_READ_REG(x) (*((uint8 *)(x)))


#define IS_RECEIVE_INTR (BCAN_READ_REG(REG_INTERRUPT) & 0x01)

extern uint8 Sja_1000_Init(uint8);
extern bit  BCAN_CMD_PRG(uint8 cmd);
extern void CAN_Send_onebyte(unsigned char CAN_TX_data,unsigned char length1);
extern void CAN_Send_Data(uint8 num);
extern void CAN_Send_Frame(canFrameStruct * frame, uint8 len);



#endif

