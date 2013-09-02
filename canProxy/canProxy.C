#include <reg51.h>
#include <string.h>




bit read_flag = 0;

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



//�򴮿ڷ���һ���ַ� 
void send_char_com(unsigned char ch)  
{
    SBUF=ch;
    while(!TI);
    TI=0;
}

//�򴮿ڷ���һ���ַ�����strlenΪ���ַ������� 
void send_string_com(unsigned char *str,unsigned int strlen)
{
    unsigned int k=0;
    do 
    {
        send_char_com(*(str + k));
        k++;
    } while(k < strlen);
}

//***************************************************

//��ʼ��cpu

//**************************************************
void Init_Cpu(void)                                  //��Ƭ����ʼ��,�����ⲿ�ж�0
{
	IE = 0;
	PX0=1;
	EX0=1;
	IT0=0;
	EA=0;
}


void init_serialcomm(void)
{
    SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr
    TMOD &= 0x0f;
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    //TH1   = 0xFA;       //Baud:9600  fosc=11.0592MHz 
    //TL1   = 0xFA;


	TH1   = 0xF4;       //Baud:4800  fosc=11.0592MHz 
    TL1   = 0xF4;
    IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 
    ET1  = 0;
    //EA = 1;
   // TI=1; 
}



//���ڽ����жϺ��� 
void serial () interrupt 4 using 1 
{
    if(RI)
    {
		unsigned char ch;
        RI = 0;
        ch=SBUF;
		read_flag=1;
    }
}
#if 1
void  main()
{
    init_serialcomm();  //��ʼ������ 
    read_flag=0;
    while(1)
    {
          if(read_flag)  //���ȡ����־����λ���ͽ����������Ӵ��ڷ��� 
          {
               read_flag=0; //ȡ����־��0 
               //ES = 0;
			     send_char_com(0x00);
			     send_char_com(0x01);
			     send_char_com(0x02);
			     send_char_com(0x03);
			     send_char_com(0x04);
			     send_char_com(0x05);
			     send_char_com(0x06);
			     send_char_com(0x08);
				  //ES = 1;
 
          }
    }

}

#else

/****************************************************
**����ԭ�ͣ�   void main(void)
**��    �ܣ�   �����򲿷�:
**��ڲ���:    �� 
**�� �� ֵ:     
*****************************************************/

void main(void)
{  
	IE = 0;
	//Init_Cpu();  
	init_serialcomm();     //��ʼ������ 
	//timer0initial();       //��ʱ��0��ʼ��
  

#if 0
	ss=Sja_1000_Init();
	if (ss!=0)             //��ʼ��ʧ��
	//send_string_com("init fail!");**********************
	send_char_com(0xBB);              //����ר�÷��͵����ڿ�״̬   
	else
	EA=1; //��ʼ���ɹ��������ж�
#endif

//�α�ʶλ������Ϊ�����ڽ����꣬�ñ�־Ȼ���ͳ�ȥ���ߵ�����������******
	while(1) 
	{ 
		if(read_flag)  //���ȡ����־����λ���ͽ����������Ӵ��ڷ��� 
		{
			read_flag=0; //ȡ����־��0 
			//ES = 0;
			send_char_com(0x00);
			send_char_com(0x01);
			send_char_com(0x02);
			send_char_com(0x03);
			send_char_com(0x04);
			send_char_com(0x05);
			send_char_com(0x06);
			send_char_com(0x08);
			//ES = 1;
		}
	}   

}

#endif

