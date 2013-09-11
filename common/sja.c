#include "stdio.h"
#include "string.h"
#include "intrins.h"
#include <reg51.h>
#include "sja.h"
uint8 xdata *SJA_BCANAdr;

#define BCAN_GET_REG(pREG) (*((uint8 *)pREG))
bit BCAN_SET_REG(uint8 *pREG, uint8 val)
{
	*(pREG) = val;
	if ( BCAN_GET_REG(pREG) == val)
	{
		return 0;
	}
	else
	{
		return 1;
	}
}

/************************************************************************
*����ԭ��: bit BCAN_SET_OUTCLK( unsigned char Clock_Out)               *
*����˵��:                                                             *
*          Clock_Out:���ʱ�ӷ�Ƶ�Ĵ���(CDR)�Ĳ�������                 *
*����ֵ:                                                               *
*           0 ;���óɹ�                                                *
*           1 ;����ʧ��                                                *
*˵��:����SJA1000��ʱ�ӷ�Ƶ �����ӳ���ֻ�����ڸ�λģʽ                      
************************************************************************/
bit BCAN_SET_OUTCLK(unsigned char Clock_Out)
{

	return BCAN_SET_REG(REG_CDR, Clock_Out);
#if 0
  SJA_BCANAdr=REG_CDR;           

  *SJA_BCANAdr=Clock_Out;        
  if(*SJA_BCANAdr != Clock_Out)
     return 1; 
   else 
     return 0;
 #endif
} 

/************************************************************************
*����ԭ��: bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3     
                                              BCAN_AMR0, BCAN_AMR1, BCAN_AMR2, BCAN_AMR3     
*����˵��:                                                              *
*    BCAN_ACR(0-3):������մ���Ĵ�����ACR���Ĳ�������                  *
*    BCAN_AMR(0-3):��Ž������μĴ�����AMR���Ĳ�������                  *
*����ֵ:                                                                *
*           0 ;���óɹ�                                                 *
*           1 ;����ʧ��                                                 *
*˵��:����CAN�ڵ��ͨѶ����������յı���,����AMR��ACR��ͬ������.     *
*************************************************************************/ 
bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3,BCAN_AMR0,BCAN_AMR1,BCAN_AMR2,BCAN_AMR3)
{
  SJA_BCANAdr=REG_TXBuffer1;     

  *SJA_BCANAdr=BCAN_ACR0;        //д�����
   
  SJA_BCANAdr=REG_TXBuffer2;
  *SJA_BCANAdr=BCAN_ACR1;

  SJA_BCANAdr=REG_TXBuffer3;  
  *SJA_BCANAdr=BCAN_ACR2;
   
  SJA_BCANAdr=REG_TXBuffer4; 
  *SJA_BCANAdr=BCAN_ACR3;  
 
  //У��д��ֵ
  if(*SJA_BCANAdr != BCAN_ACR3)  return 1; 
  
  SJA_BCANAdr=REG_TXBuffer5;      

  *SJA_BCANAdr=BCAN_AMR0;     //д�����
  
  SJA_BCANAdr=REG_TXBuffer6;         
  *SJA_BCANAdr=BCAN_AMR1;

  SJA_BCANAdr=REG_TXBuffer7; 
  *SJA_BCANAdr=BCAN_AMR2;

  SJA_BCANAdr=REG_TXBuffer8; 
  *SJA_BCANAdr=BCAN_AMR3;
  //У��д��ֵ
  if(*SJA_BCANAdr != BCAN_AMR3) return 1; 

  return 0;
} 

/************************************************************************
;*����ԭ��:  bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)          *
;*����ֵ:                                                               *
;*           0 ;���������óɹ�                                          *
;*           1 ;����������ʧ��                                          *
;*                                                                      * 
;*˵��:����CAN������SJA1000ͨѶ������.SJA1000�ľ������Ϊ16MHZ,         *
;*     ���������Ƶ�ʵ�ֵ�Ĳ����ʣ����Լ����� �����ӳ���ֻ������        *
;*     ��λģʽ                                                         *  
;************************************************************************/ 
bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate, uint8 freq)         //������ѡ��
{
	bit ret = 1;
	unsigned char BR_Num= CAN_ByteRate,BTR0_num,BTR1_num;
	switch (BR_Num)
	{
		case ByteRate_10k:
		case ByteRate_40k:
		case ByteRate_50k:
		case ByteRate_80k:
		case ByteRate_200k:
		case ByteRate_250k:
		case ByteRate_400k:
		case ByteRate_500k:
		case ByteRate_800k:
		case ByteRate_1000k:
			break;


		case ByteRate_100k:
		{
			if (freq == OSCILLA_FREQ_12M)
			{
				BTR0_num=0x02;//12MHZ
				BTR1_num=0x2f;
			}
			else if(freq == OSCILLA_FREQ_16M)
			{
				BTR0_num=0x43; //16MHZ
				BTR1_num=0x2f;
			}
		}
		   break;
		   
		case ByteRate_125k:
		{
			if (freq == OSCILLA_FREQ_12M)
			{	
				// to be determine
				//BTR0_num=0x02;//12MHZ
				//BTR1_num=0x2f;
			}
			else if(freq == OSCILLA_FREQ_16M)
			{
				BTR0_num=0x03; //16MHZ
				BTR1_num=0x1c;
			}
		}
			break;

		default:
			return 1;
			break;
		}

	ret = BCAN_SET_REG(REG_BTR0, BTR0_num);
	ret |= BCAN_SET_REG(REG_BTR1, BTR1_num);
	
    return ret;
}
/************************************************************************
 *����ԭ��: bit BCAN_SET_CONTROL(unsigend char CMD)                     *
 *����˵��: ���ÿ��ƼĴ���                                              *
 ************************************************************************/
bit BCAN_SET_CONTROL(unsigned char CMD)
{
	return BCAN_SET_REG(REG_CONTROL, CMD);
}
/************************************************************************
 *����ԭ��:  bit   BCAN_CREATE_COMMUNATION(void)                        * 
 *����˵��:  ��                                                         *
 *����ֵ:                                                               *
 *           0 ; ��ʾSJA1000�ӿ�����                                    *
 *           1 ; ��ʾSJA1000�봦�����ӿڲ�����                          *
 *˵��:�ú������ڼ��CAN�������Ľӿ��Ƿ�����                            *
 ************************************************************************/
bit BCAN_CREATE_COMMUNATION(void)
{
	return BCAN_SET_REG(REG_TEST, 0xaa);
}

 /************************************************************************
 *����ԭ��:      bit   BCAN_ENTER_RETMODEL(void)                        *
 *����˵��:  ��                                                         *
 *����ֵ:                                                               *
 *           0 ; ��ʾ�ɹ����븴λ����ģʽ                               *
 *           1 ; ��ʾ���ܽ��븴λ����ģʽ                               *
 *                                                                      * 
 *˵��:      CAN���������븴λ����ģʽ                                  *
 ************************************************************************/ 
bit BCAN_ENTER_RETMODEL(void)     //��λ��λ����
{
	BCAN_SET_REG(REG_CONTROL, 0x09);//��λ��λ���� �͵��˲�ģʽ

	// filter moder not readable
	if( (BCAN_GET_REG(REG_CONTROL)&0x01) == 1)
		return   0;
	else
		return   1;   
}
 
bit BCAN_QUIT_RETMODEL(void)
{
	BCAN_SET_REG(REG_CONTROL, BCAN_GET_REG(REG_CONTROL)&0xfe);
	if((BCAN_GET_REG(REG_CONTROL)) != 0)
		return   0;
	else
		return   1;   
} 


/************************************************************************
 *����ԭ��:   BCAN_CMD_PRG(unsigned char cmd)                           *
 *����˵��:  unsigned char cmd                                          *
 *����ֵ:                                                               *
 *           0 ; ����ɹ�                                               *
 *           1 ; ����ʧ��                                               *
 *                                                                      * 
 *˵��:      ����������                                                 *
 ************************************************************************/
bit  BCAN_CMD_PRG(unsigned char cmd)
{
	SJA_BCANAdr=REG_COMMAND;
	*SJA_BCANAdr=cmd;

	rs485SetModeTx();


	SBUF=0x99;
	while(!TI);
	TI=0;

	SBUF=cmd;
	while(!TI);
	TI=0;

		

	SBUF=0x99;
	while(!TI);
	TI=0;

	rs485SetModeRx();


	switch(cmd)
	{
		case  TR_CMD:                    

			return    0;
			break;

		case  SRR_CMD:      

			return 0;
			break;

		case  AT_CMD:                  

			SJA_BCANAdr = REG_STATUS;   //���ʵ�ַָ��״̬�Ĵ���   
			if((*SJA_BCANAdr & 0x20)==0) //�ж��Ƿ����ڷ���
				return  0;
			else
				return  1;              
			break; 

		case  RRB_CMD:                   
			SJA_BCANAdr = REG_STATUS;   //���ʵ�ַָ��״̬�Ĵ���   
           if((*SJA_BCANAdr & 0x01)==1)
              return  1;
           else           
              return  0;               
           break;  
               
		case  COS_CMD:                  

			SJA_BCANAdr = REG_STATUS;   
			if((*SJA_BCANAdr & 0x02)==0)//�ж���������Ƿ�ɹ�
			return  0; 
			else
			return  1;             
			break; 
		default:
			return  1;
			break; 
	}
}
unsigned char BCAN_DATA_WRITE(unsigned char *SendDataBuf)
 {  unsigned char temp;
   
    SJA_BCANAdr = REG_STATUS;    
    temp=*SJA_BCANAdr;

    if ((temp&0x08)==0) return  1;    //�ϴη���δ���
    if ((temp&0x04)==0) return  2;    //���ͻ������Ƿ����� 
    if ((temp&0x10)==0x10) return 3;  //�ж��Ƿ����ڽ���   

    SJA_BCANAdr = REG_RXBuffer1;      //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�

    memcpy(SJA_BCANAdr,SendDataBuf,4);   
    BCAN_CMD_PRG(TR_CMD);             //������         
    return 0;
}
//CAN�������ⳤ���ֽ�
void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1)
{
	unsigned char temptt;
	loop:
    SJA_BCANAdr = REG_STATUS;    
         temptt=*SJA_BCANAdr; 
	//temptt=Read_SJA1000(REG_STATUS);
	if((temptt&0x04)==0x00)  goto loop;               //ѭ�����ȴ�                       
	//�������ͻ�����д����
	{
    SJA_BCANAdr = REG_RXBuffer1;      //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=length1;  
    SJA_BCANAdr = REG_RXBuffer2;      //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=0x00;  
     SJA_BCANAdr = REG_RXBuffer3;     //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=0x00;
    SJA_BCANAdr = REG_RXBuffer4; 
    memcpy(SJA_BCANAdr,CAN_TX_Buf,length1);   
	//���ݷ�������
    BCAN_CMD_PRG(TR_CMD);            //������  
	}
}
//CAN����һ���ֽ�
void CAN_Send_onebyte(unsigned char CAN_TX_data,unsigned char length1)
{
	unsigned char temptt;
	loop:
    SJA_BCANAdr = REG_STATUS;    
         temptt=*SJA_BCANAdr; 
	//temptt=Read_SJA1000(REG_STATUS);
	if((temptt&0x04)==0x00)  goto loop;               //ѭ�����ȴ�                       
	//�������ͻ�����д����
	{
    SJA_BCANAdr = REG_RXBuffer1;      //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=length1;  
    SJA_BCANAdr = REG_RXBuffer2;      //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=0x00;  
     SJA_BCANAdr = REG_RXBuffer3;     //���ʵ�ַָ���ͻ�����1,�޸ĳ�ͷ�ļ�
    *SJA_BCANAdr=0x00;
    SJA_BCANAdr = REG_RXBuffer4; 
    //memcpy(SJA_BCANAdr,CAN_TX_Buf,length1);  
    *SJA_BCANAdr=CAN_TX_data; 
	//���ݷ�������
    BCAN_CMD_PRG(TR_CMD);            //������  
	}
}


//void CAN_Send_Data(unsigned char CAN_TX_data, uint8 length1)
void CAN_Send_Data(uint8 num)
{
	unsigned char temptt;
	uint8 xdata *canAddr;
	uint8 dataTmpStart = 0x2;
	uint8 i = 0;
	
	loop:
    canAddr = REG_STATUS;    
         temptt=*canAddr; 
	//temptt=Read_SJA1000(REG_STATUS);
	if((temptt&0x04)==0x00)  goto loop;               //ѭ�����ȴ�                       
	//�������ͻ�����д����
	{
	    canAddr = REG_RXBuffer1;

		if (0) //eff
		{
			*canAddr = 8|0x80;

			canAddr = REG_RXBuffer2;
			*canAddr=0xff;

			canAddr = REG_RXBuffer3;
			*canAddr=0xff;

			canAddr = REG_RXBuffer4;
			*canAddr=0xff;

			canAddr = REG_RXBuffer5;
			*canAddr=0xff;
		}
		else
		{
			*canAddr = 8;
			//*canAddr = 8|0x80;

			canAddr = REG_RXBuffer2;
			*canAddr=0xff;

			canAddr = REG_RXBuffer3;
			*canAddr=0xff;
		}

		

#if 1
		for ( i = 0; i < 8; i++)
		{
			canAddr++;
			*canAddr = num++;
		}
#endif		
	    BCAN_CMD_PRG(TR_CMD);            //������  
	}
}

/****************************************************
**����ԭ�ͣ�   bit Sja_1000_Init(void)
**��    �ܣ�   ��ʼ��SJA10000
**��ڲ���:    �� 
**�� �� ֵ:     
      0�� ��ʼ���ɹ� 
      1�� ��λʧ�� 
      2:  ����sja1000ʧ�� 
      3�� ����ʧ�� 
      4�� ���������˲���ʧ��
      5�� ���ò�����ʧ��     
*****************************************************/
unsigned char Sja_1000_Init(uint8 freq)
{
	bit s; 
	EX0 = 0;

	if ( BCAN_ENTER_RETMODEL())
	{
		return 1; 
	}

	if ( BCAN_CREATE_COMMUNATION())
	{
		return 2;
	}

	if ( BCAN_SET_OUTCLK(0x88))//Pelican
	{
		return 3;
	}

	s=BCAN_SET_OBJECT(0xFF,0x4E,0x16,0x00,0xff,0xff,0xff,0xff);//���μĴ���������Ϊ�޹أ��������б��� 
	                                                //������λΪ1�����˲���0���˲��������
	//s=BCAN_SET_OBJECT(0x55,0xe0,0xaa,0xa1,0x00,0x00,0xff,0xff);//������&������

	if (s==1) return 4;

	if ( BCAN_SET_BANDRATE(ByteRate_100k, freq) )
	{
		return 5;
	}
	
	BCAN_SET_REG(REG_OCR, 0x1a);//out put control
	BCAN_SET_REG(REG_INTENABLE, 0x1d);
	BCAN_SET_REG(REG_CONTROL, 0x1d);

	if ( BCAN_QUIT_RETMODEL() )
	return 6;

	EX0=1;
	return 0;   
} 
