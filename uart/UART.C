#include <reg51.h>
#include <string.h>

#define INBUF_LEN 8   //���ݳ���

unsigned char inbuf1[INBUF_LEN];
unsigned char play[4]={0xFF,0xA5,0x01,0x00};
unsigned char play1[]="I love you,�й���ҵ��ѧ";
unsigned char checksum,count3;
bit           read_flag=0;

void init_serialcomm(void)
{
    SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    TH1   = 0xFA;       //Baud:9600  fosc=11.0592MHz 
    TL1   = 0xFA;
    IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 
   // TI=1; 
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


//���ڽ����жϺ��� 
void serial () interrupt 4 using 1 
{
    if(RI)
    {
        unsigned char ch;
        RI = 0;
        ch=SBUF;
        if(ch>127)
        {
             count3=0;
             inbuf1[count3]=ch;
             checksum= ch-128;
        }
        else 
        {
             inbuf1[count3]=ch;
             count3++;
             checksum ^= ch;
             if( count3==INBUF_LEN)
             {
                 count3=0;
                 read_flag=1;  //������ڽ��յ����ݴﵽINBUF_LEN������У��û���� 
                               //����λȡ����־ 
             }
        }
    }
}


main()
{
    init_serialcomm();  //��ʼ������ 
    read_flag=1;
    while(1)
    {
          if(read_flag)  //���ȡ����־����λ���ͽ����������Ӵ��ڷ��� 
          {
               read_flag=0; //ȡ����־��0 
               //send_string_com(inbuf1,INBUF_LEN);
                          //���´����Ƿ��͵�����
     //send_string_com(play,4);
    // send_string_com(play1,30);
     send_char_com(0x00);
     send_char_com(0x01);
     send_char_com(0x02);
     send_char_com(0x03);
     send_char_com(0x04);
     send_char_com(0x05);
     send_char_com(0x06);
     send_char_com(0x08);
 
          }
    }

}