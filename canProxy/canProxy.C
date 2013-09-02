#include <reg51.h>
#include <string.h>




bit read_flag = 0;

//===================================================================================
//	工程名称：	CAN总线收发一体程序
//	功能描述：	利用CAN总线实现双向can语音传输

//  IDE环境：   KEILC （or later）
//	程序设计：	曙光单片机科技淘宝店
//        
//	组成文件：	main.c, FUNCTION.h,CAN.H
//				
//	硬件连接：  STC89C5X+SJA1000+CTM8251+AMBE1000	
//
//	维护记录：	2012-10-26	v1.0 
//===================================================================================



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

//***************************************************

//初始化cpu

//**************************************************
void Init_Cpu(void)                                  //单片机初始化,开放外部中断0
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



//串口接收中断函数 
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
    init_serialcomm();  //初始化串口 
    read_flag=0;
    while(1)
    {
          if(read_flag)  //如果取数标志已置位，就将读到的数从串口发出 
          {
               read_flag=0; //取数标志清0 
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
**函数原型：   void main(void)
**功    能：   主程序部分:
**入口参数:    无 
**返 回 值:     
*****************************************************/

void main(void)
{  
	IE = 0;
	//Init_Cpu();  
	init_serialcomm();     //初始化串口 
	//timer0initial();       //定时器0初始化
  

#if 0
	ss=Sja_1000_Init();
	if (ss!=0)             //初始化失败
	//send_string_com("init fail!");**********************
	send_char_com(0xBB);              //测试专用发送到串口看状态   
	else
	EA=1; //初始化成功，开总中断
#endif

//次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
	while(1) 
	{ 
		if(read_flag)  //如果取数标志已置位，就将读到的数从串口发出 
		{
			read_flag=0; //取数标志清0 
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


