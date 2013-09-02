#include  "reg51.h"
/************************************************************************
*����ԭ��: void init_serialcomm(void)            *
*����˵��: ���ڳ�ʼ��                            *                                                             *
*˵��:     ��ֵ��Ƭ���Ķ�ʱ��1�ķ�ʽѡ������ �����ӳ���ֻ�����ڸ�λģʽ                      
************************************************************************/
void init_serialcomm(void)
{
    SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    TH1   = 0xFa;       //Baud:9600  fosc=11.0592MHz  
    TL1   = 0xFa;       //baud:57600      fosc=22.1184MHz  0xFE
    //IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 

	ES=0;
	
    // TI=1; 
}
//��ʱ��0��ʼ��
void timer0initial()
{
	TMOD |= 0x1;        //������ʽ16λ��ʱ������
	TH0=0xb8;TL0=0x00;  //50MS��ʱ��ֵ��T0��ʱ�ã�
    IE   |= 0x90;       //Enable Serial Interrupt 
    ET0=0;TR0=0;
}
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
//��ʱ��0�ж�,����8�����ڴ˷���