//===================================================================================
//	�������ƣ�	CAN�����շ�һ�����
//	����������	����CAN����ʵ��˫��can��������

//  IDE������   KEILC ��or later��
//	������ƣ�	��ⵥƬ���Ƽ��Ա���
//        
//	����ļ���	main.c, FUNCTION.h,CAN.H
//				
//	Ӳ�����ӣ�  STC89C5X+SJA1000+CTM8251+AMBE1000	
//
//	ά����¼��	2012-10-26	v1.0 
//===================================================================================

#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
#include  "reg51.h"

void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);

unsigned char data Tmod_data;
unsigned char run_lamp_flush_count = 0;
unsigned char run_lamp_flush_time = 10;
sbit  run_lamp = P1^0;
unsigned char data send_data[10],RevceData[10];
unsigned char data Com_RecBuff[8]={0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08};

unsigned char bdata flag_init;
//*************************
void display(unsigned char num);

#define INBUF_LEN 8   //���ݳ���

unsigned char inbuf1[INBUF_LEN];
unsigned char checksum,count3=0;
bit  read_flag=0;
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P16=P1^6;
sbit P37=P3^7;
sbit P34=P3^4;

sbit key=P3^3; //Button����
sbit P2_0=P2^0;//�����λѡ
sbit P2_1=P2^1;
sbit P2_2=P2^2;//�����λѡ
sbit P2_3=P2^3;

//***************************
sbit rcv_flag=flag_init^0;
sbit err_flag=flag_init^0;
sbit CS=P2^0;
unsigned char RECOK;
unsigned int msg;
//***************************
void time_intt0(void) interrupt 1 using 2 
{
  ET0=0;TR0=0;TH0=0xb8;TL0=0x00;
  CAN_Send_anylength(inbuf1,count3);
  count3=0;
  memset(inbuf1,0,8);
}
//���ڽ����жϺ��� 
void serial () interrupt 4 using 1 
{  
 
    if(RI)
    {
        
        unsigned char ch;
        RI = 0;
        ET0=1;TR0=1;
        ch=SBUF;
        {
             inbuf1[count3]=ch;
             count3=count3+1;
             if( count3==INBUF_LEN)
             {
                 ET0=0;TR0=0;TH0=0xb8;TL0=0x00;//�رն�ʱ�жϣ�ֹͣ��ʱ�ж�
                 count3=0;
                 CAN_Send_anylength(inbuf1,8);
                 memset(inbuf1,0,8);
                 
             }
          }
       }
}
/****************************************************
**����ԭ�ͣ�  void ex0_int(void) interrupt 0 //using 1
**��    �ܣ�  �жϽ��պ���
**��ڲ���:   �� 
**���ڲ���:   RevceData[]����   
**˵    ��:   ��sja1000 �յ���ȷ�ı���ʱ,�����int�ж�           
*****************************************************/
void ex0_int(void) interrupt 0 using 1
{
	unsigned char tt,tt1,length,i;
	static int cnt = 1;
 
 SJA_BCANAdr=REG_INTERRUPT;
 if((*SJA_BCANAdr)&0x01)                   //�����˽����ж�
 {  
    SJA_BCANAdr=REG_RXBuffer1;
    tt=*SJA_BCANAdr;
    tt1=*SJA_BCANAdr;
    length=tt1&0x0F;
     if ((tt&0x40)!=0x40)                   //����֡   = ΪԶ��֡
     {  
     SJA_BCANAdr =REG_RXBuffer4 ;           //�궨��ı�������memcpy(RevceData,REG_RXBuffer4,8); 
     
     memcpy(RevceData,SJA_BCANAdr,length);  //���ܣ���src��ָ�ڴ�������count���ֽڵ�dest��ָ�ڴ�����

#if 0
	 display(cnt);//�������ʾ 
	send_char_com(cnt);

	cnt++;
#endif	
#if 1
 
	 display(RevceData[0]);//�������ʾ 

                                            //���´����Ƿ��͵���
    for(i=0;i<length;i++)
     send_char_com(RevceData[i]);
 #endif	   
     }

     BCAN_CMD_PRG(RRB_CMD);                  //�ͷ�SJA1000���ջ�������****�Ѿ��޸�


 }
} 
/****************************************************
**����ԭ�ͣ�  void display(unsigned char num) 
**��    �ܣ�  �������ʾ
**��ڲ���:   Ҫ��ʾ�ַ� 
**���ڲ���:   ��
**˵    ��:   ��SJA1000�յ���ȷ�ı���ʱ,Ȼ����ʾ����           
*****************************************************/
unsigned char code table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void display(unsigned char num)  // ��ʾ�Ӻ���

{

		P2_0=0;//ѡͨ��һλ����ܵ�����λѡ��,��������ܵĹ���������
        P2_1=0;//ѡͨ�ڶ�λ����ܵ�����λѡ��,��������ܵĹ���������
        P2_2=0;//ѡͨ��һλ����ܵ�����λѡ��,��������ܵĹ���������
        P2_3=0;//ѡͨ�ڶ�λ����ܵ�����λѡ��,��������ܵĹ���������
        P1=table[num]; //����num����ʾ������P0��
}
//***************************************************

//��ʼ��cpu

//**************************************************
void Init_Cpu(void)                                  //��Ƭ����ʼ��,�����ⲿ�ж�0
{
     PX0=1;
     EX0=1;
     IT0=0;
    // EA=1;
}

/****************************************************
**����ԭ�ͣ�   void main(void)
**��    �ܣ�   �����򲿷�:
**��ڲ���:    �� 
**�� �� ֵ:     
*****************************************************/

void main(void)
{  
    unsigned char temptt,ss;
	unsigned char num=0;
    EA=0;
    Init_Cpu();  
    init_serialcomm();     //��ʼ������ 
    timer0initial();       //��ʱ��0��ʼ��
	display(10);		  //��ʾ�ð��A 
    //��ʼ��SJA1000     
    ss=Sja_1000_Init();
    if (ss!=0)             //��ʼ��ʧ��
      //send_string_com("init fail!");**********************
      send_char_com(0xBB);              //����ר�÷��͵����ڿ�״̬   
    else
      EA=1; //��ʼ���ɹ��������ж�

	 send_char_com(0xaa);
   
     //�α�ʶλ������Ϊ�����ڽ����꣬�ñ�־Ȼ���ͳ�ȥ���ߵ�����������******
   while(1) 
   { 
     
     //CAN_Send_anylength(Com_RecBuff,8);
     if(key==0) 
       {
        delay10ms();
   		while(key==0);
        num=num+1;if(num==16) num=0;
         //���Ͱ���״̬
	    CAN_Send_onebyte(num,1);
		display(num); 
        if(num==16) num=0;
        delay10ms();
	   }
     
     SJA_BCANAdr = REG_STATUS;    
     temptt=*SJA_BCANAdr;         
     delay_ms(100);
  if ((temptt&0x40)==0x40)                     //������״̬

    {  
    Sja_1000_Init();
    send_char_com(0xee);                    //����ר�÷��͵����ڿ�״̬                     
  }
   }   

}


