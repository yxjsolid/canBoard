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

#define INBUF_LEN 8   //数据长度

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

sbit key=P3^3; //Button按键
sbit P2_0=P2^0;//数码管位选
sbit P2_1=P2^1;
sbit P2_2=P2^2;//数码管位选
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
//串口接收中断函数 
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
                 ET0=0;TR0=0;TH0=0xb8;TL0=0x00;//关闭定时中断，停止定时中断
                 count3=0;
                 CAN_Send_anylength(inbuf1,8);
                 memset(inbuf1,0,8);
                 
             }
          }
       }
}
/****************************************************
**函数原型：  void ex0_int(void) interrupt 0 //using 1
**功    能：  中断接收函数
**入口参数:   无 
**出口参数:   RevceData[]数组   
**说    明:   当sja1000 收到正确的报文时,会产生int中断           
*****************************************************/
void ex0_int(void) interrupt 0 using 1
{
	unsigned char tt,tt1,length,i;
	static int cnt = 1;
 
 SJA_BCANAdr=REG_INTERRUPT;
 if((*SJA_BCANAdr)&0x01)                   //产生了接收中断
 {  
    SJA_BCANAdr=REG_RXBuffer1;
    tt=*SJA_BCANAdr;
    tt1=*SJA_BCANAdr;
    length=tt1&0x0F;
     if ((tt&0x40)!=0x40)                   //数据帧   = 为远程帧
     {  
     SJA_BCANAdr =REG_RXBuffer4 ;           //宏定义的变量不能memcpy(RevceData,REG_RXBuffer4,8); 
     
     memcpy(RevceData,SJA_BCANAdr,length);  //功能：由src所指内存区域复制count个字节到dest所指内存区域

#if 0
	 display(cnt);//数码管显示 
	send_char_com(cnt);

	cnt++;
#endif	
#if 1
 
	 display(RevceData[0]);//数码管显示 

                                            //以下代码是发送到串
    for(i=0;i<length;i++)
     send_char_com(RevceData[i]);
 #endif	   
     }

     BCAN_CMD_PRG(RRB_CMD);                  //释放SJA1000接收缓冲区，****已经修改


 }
} 
/****************************************************
**函数原型：  void display(unsigned char num) 
**功    能：  数码管显示
**入口参数:   要显示字符 
**出口参数:   无
**说    明:   当SJA1000收到正确的报文时,然后显示出来           
*****************************************************/
unsigned char code table[]={0xC0,0xF9,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90,0x88,0x83,0xc6,0xa1,0x86,0x8e};
void display(unsigned char num)  // 显示子函数

{

		P2_0=0;//选通第一位数码管的阳极位选端,即给数码管的共阳极供电
        P2_1=0;//选通第二位数码管的阳极位选端,即给数码管的共阳极供电
        P2_2=0;//选通第一位数码管的阳极位选端,即给数码管的共阳极供电
        P2_3=0;//选通第二位数码管的阳极位选端,即给数码管的共阳极供电
        P1=table[num]; //将第num个显示编码送P0口
}
//***************************************************

//初始化cpu

//**************************************************
void Init_Cpu(void)                                  //单片机初始化,开放外部中断0
{
     PX0=1;
     EX0=1;
     IT0=0;
    // EA=1;
}

/****************************************************
**函数原型：   void main(void)
**功    能：   主程序部分:
**入口参数:    无 
**返 回 值:     
*****************************************************/

void main(void)
{  
    unsigned char temptt,ss;
	unsigned char num=0;
    EA=0;
    Init_Cpu();  
    init_serialcomm();     //初始化串口 
    timer0initial();       //定时器0初始化
	display(10);		  //显示该板号A 
    //初始化SJA1000     
    ss=Sja_1000_Init();
    if (ss!=0)             //初始化失败
      //send_string_com("init fail!");**********************
      send_char_com(0xBB);              //测试专用发送到串口看状态   
    else
      EA=1; //初始化成功，开总中断

	 send_char_com(0xaa);
   
     //次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
   while(1) 
   { 
     
     //CAN_Send_anylength(Com_RecBuff,8);
     if(key==0) 
       {
        delay10ms();
   		while(key==0);
        num=num+1;if(num==16) num=0;
         //发送按键状态
	    CAN_Send_onebyte(num,1);
		display(num); 
        if(num==16) num=0;
        delay10ms();
	   }
     
     SJA_BCANAdr = REG_STATUS;    
     temptt=*SJA_BCANAdr;         
     delay_ms(100);
  if ((temptt&0x40)==0x40)                     //读错误状态

    {  
    Sja_1000_Init();
    send_char_com(0xee);                    //测试专用发送到串口看状态                     
  }
   }   

}


