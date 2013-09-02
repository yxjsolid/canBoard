#include <reg51.h>
#include <string.h>

#define INBUF_LEN 8   //数据长度

unsigned char inbuf1[INBUF_LEN];
unsigned char play[4]={0xFF,0xA5,0x01,0x00};
unsigned char play1[]="I love you,中国矿业大学";
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

//向串口发送一个字符 
void send_char_com(unsigned char ch)  
{
    SBUF=ch;
    while(!TI);
    TI=0;
}

//向串口发送一个字符串，strlen为该字符串长度 
void send_string_com(unsigned char *str,unsigned int strlen)
{
    unsigned int k=0;
    do 
    {
        send_char_com(*(str + k));
        k++;
    } while(k < strlen);
}


//串口接收中断函数 
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
                 read_flag=1;  //如果串口接收的数据达到INBUF_LEN个，且校验没错， 
                               //就置位取数标志 
             }
        }
    }
}


main()
{
    init_serialcomm();  //初始化串口 
    read_flag=1;
    while(1)
    {
          if(read_flag)  //如果取数标志已置位，就将读到的数从串口发出 
          {
               read_flag=0; //取数标志清0 
               //send_string_com(inbuf1,INBUF_LEN);
                          //以下代码是发送到串口
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