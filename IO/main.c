#include  "stdio.h"
#include  "string.h"
#include  "intrins.h"
//#include "AT89X51.H"
#include "reg51.h"
void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1);
//定义SJA1000的基址

#define SJA_BaseAdr  0X7F00

// 控制寄存器

#define         REG_CONTROL       SJA_BaseAdr+0x00       //内部控制寄存器
#define         REG_COMMAND       SJA_BaseAdr+0x01       //命令寄存器
#define         REG_STATUS        SJA_BaseAdr+0x02       //状态寄存器
#define         REG_INTERRUPT     SJA_BaseAdr+0x03       //中断寄存器
#define         REG_INTENABLE     SJA_BaseAdr+0x04       //中断使能寄存器
#define         REG_BTR0          SJA_BaseAdr+0x06       //总线定时寄存器0
#define         REG_BTR1          SJA_BaseAdr+0x07       //总线定时寄存器1
#define         REG_OCR           SJA_BaseAdr+0x08       //输出控制寄存器
#define         REG_TEST          SJA_BaseAdr+0x09       //测试寄存器

#define         REG_RESVER1       SJA_BaseAdr+0x0A       //保留1
#define         REG_ARBITRATE     SJA_BaseAdr+0x0B       //仲裁丢失捕捉
#define         REG_ERRCATCH      SJA_BaseAdr+0x0C       //错误代码捕捉
#define         REG_ERRLIMIT      SJA_BaseAdr+0x0D       //错误报警限额

#define         REG_RXERR         SJA_BaseAdr+0x0E         //接收错误计数器
#define         REG_TXERR         SJA_BaseAdr+0x0F         //发送错误计数器

#define         REG_ACR1          SJA_BaseAdr+0x10       //验收代码寄存器
#define         REG_ACR2          SJA_BaseAdr+0x11       //验收代码寄存器
#define         REG_ACR3          SJA_BaseAdr+0x12       //验收代码寄存器
#define         REG_ACR4          SJA_BaseAdr+0x13       //验收代码寄存器
#define         REG_AMR1          SJA_BaseAdr+0x14       //验收屏蔽寄存器
#define         REG_AMR2          SJA_BaseAdr+0x15       //验收屏蔽寄存器
#define         REG_AMR3          SJA_BaseAdr+0x16       //验收屏蔽寄存器
#define         REG_AMR4          SJA_BaseAdr+0x17       //验收屏蔽寄存器

// 发送缓冲区寄存器
#define         REG_TXBuffer1     SJA_BaseAdr+0x10         //发送缓冲区1
#define         REG_TXBuffer2     SJA_BaseAdr+0x11         //发送缓冲区2
#define         REG_TXBuffer3     SJA_BaseAdr+0x12         //发送缓冲区3
#define         REG_TXBuffer4     SJA_BaseAdr+0x13         //发送缓冲区4
#define         REG_TXBuffer5     SJA_BaseAdr+0x14         //发送缓冲区5
#define         REG_TXBuffer6     SJA_BaseAdr+0x15         //发送缓冲区6
#define         REG_TXBuffer7     SJA_BaseAdr+0x16         //发送缓冲区7
#define         REG_TXBuffer8     SJA_BaseAdr+0x17         //发送缓冲区8
#define         REG_TXBuffer9     SJA_BaseAdr+0x18         //发送缓冲区9
#define         REG_TXBuffer10    SJA_BaseAdr+0x19         //发送缓冲区10
#define         REG_TXBuffer11    SJA_BaseAdr+0x1A         //发送缓冲区11
#define         REG_TXBuffer12    SJA_BaseAdr+0x1B         //发送缓冲区12
#define         REG_TXBuffer13    SJA_BaseAdr+0x1C         //发送缓冲区13

// 接收缓冲区寄存器
#define         REG_RXBuffer1       SJA_BaseAdr+0x10       //接收缓冲区1
#define         REG_RXBuffer2       SJA_BaseAdr+0x11       //接收缓冲区2
#define         REG_RXBuffer3      SJA_BaseAdr+0x12        //接收缓冲区3
#define         REG_RXBuffer4       SJA_BaseAdr+0x13       //接收缓冲区4
#define         REG_RXBuffer5      SJA_BaseAdr+0x14        //接收缓冲区5
#define         REG_RXBuffer6     SJA_BaseAdr+0x15         //接收缓冲区6
#define         REG_RXBuffer7     SJA_BaseAdr+0x16         //接收缓冲区7
#define         REG_RXBuffer8     SJA_BaseAdr+0x17         //接收缓冲区8
#define         REG_RXBuffer9     SJA_BaseAdr+0x18         //接收缓冲区9
#define         REG_RXBuffer10     SJA_BaseAdr+0x19        //接收缓冲区10
#define         REG_RXBuffer11     SJA_BaseAdr+0x1A        //接收缓冲区11
#define         REG_RXBuffer12     SJA_BaseAdr+0x1B        //接收缓冲区12
#define         REG_RXBuffer13     SJA_BaseAdr+0x1C        //接收缓冲区13

#define         REG_RXCOUNT       SJA_BaseAdr+0x1D         //RX报文计数器
#define         REG_RBSA          SJA_BaseAdr+0x1E         //接收缓冲区起始地址
#define         REG_CDR           SJA_BaseAdr+0x1F         //时钟分频寄存器
//功能说明:   CAN控制器SJA1000通讯波特率.SJA1000的晶振为必须为24MHZ*/
#define         BTR0_Rate_10k      0xEF          //20KBPS的预设值
#define         BTR1_Rate_10k      0xFF          //20KBPS的预设值

/*
功能说明:   CAN控制器SJA1000通讯波特率.SJA1000的晶振为必须为16MHZ*/


#define         BTR0_Rate_5k      0xBF          //5KBPS的预设值
#define         BTR1_Rate_5k      0xFF          //5KBPS的预设值

#define         BTR0_Rate_10k      0x31          //10KBPS的预设值
#define         BTR1_Rate_10k      0x1C          //10KBPS的预设值

#define         BTR0_Rate_20k      0x18          //20KBPS的预设值
#define         BTR1_Rate_20k      0x1C         //20KBPS的预设值

#define         BTR0_Rate_40k      0x87          //40KBPS的预设值
#define         BTR1_Rate_40k      0xFF          //40KBPS的预设值

#define         BTR0_Rate_50k      0x47          //50KBPS的预设值
#define         BTR1_Rate_50k      0x2F          //50KBPS的预设值

#define         BTR0_Rate_80k      0x83          //80KBPS的预设值
#define         BTR1_Rate_80k      0xFF          //80KBPS的预设值

#define         BTR0_Rate_100k     0x43          //100KBPS的预设值
#define         BTR1_Rate_100k     0x2f          //100KBPS的预设值

#define         BTR0_Rate_125k     0x03          //125KBPS的预设值
#define         BTR1_Rate_125k     0x1c          //125KBPS的预设值

#define         BTR0_Rate_200k     0x81          //200KBPS的预设值
#define         BTR1_Rate_200k     0xFA          //200KBPS的预设值

#define         BTR0_Rate_250k     0x01          //250KBPS的预设值
#define         BTR1_Rate_250k     0x1c          //250KBPS的预设值

#define         BTR0_Rate_400k     0x80          //400KBPS的预设值
#define         BTR1_Rate_400k     0xfa          //400KBPS的预设值

#define         BTR0_Rate_500k     0x00          //500KBPS的预设值
#define         BTR1_Rate_500k     0x1c          //500KBPS的预设值

#define         BTR0_Rate_666k     0x80          //666KBPS的预设值
#define         BTR1_Rate_666k     0xb6          //666KBPS的预设值

#define         BTR0_Rate_800k     0x00          //800KBPS的预设值
#define         BTR1_Rate_800k     0x16          //800KBPS的预设值

#define         BTR0_Rate_1000k    0x00          //1000KBPS的预设值
#define         BTR1_Rate_1000k    0x14          //1000KBPS的预设值
//BPS
#define         ByteRate_5k        5
#define         ByteRate_10k       10
#define         ByteRate_20k       20
#define         ByteRate_40k       40
#define         ByteRate_50k       50
#define         ByteRate_80k       80
#define         ByteRate_100k      100
#define         ByteRate_125k      125
#define         ByteRate_200k      200
#define         ByteRate_400k      400
#define         ByteRate_500k      500
#define         ByteRate_800k      800
#define         ByteRate_1000k     1000

//命令字
#define    TR_CMD     0X01
#define    AT_CMD     0X02
#define    RRB_CMD    0X04
#define    COS_CMD    0X08
#define    SRR_CMD    0X10
#define    GTS_CMD    0X10
//错误字

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

//系统指针，指向SJA1000

unsigned char xdata *SJA_BCANAdr;
unsigned char data Tmod_data;
unsigned char run_lamp_flush_count = 0;
unsigned char run_lamp_flush_time = 10;
sbit  run_lamp = P1^0;
unsigned char data send_data[10],RevceData[10];
//unsigned int data rxbuffer[10]={REG_RXBuffer1,REG_RXBuffer2,REG_RXBuffer3,REG_RXBuffer4,REG_RXBuffer5,REG_RXBuffer6,REG_RXBuffer7,REG_RXBuffer8,REG_RXBuffer9,REG_RXBuffer10};
//unsigned int data txbuffer[10]={REG_TXBuffer1,REG_TXBuffer2,REG_TXBuffer3,REG_TXBuffer4,REG_TXBuffer5,REG_TXBuffer6,REG_TXBuffer7,REG_TXBuffer8,REG_TXBuffer9,REG_TXBuffer10};
//unsigned char data Send_CAN_Info_ID[5]={0x08,0x55,0xe0,0x55,0x55};
//unsigned char data Send_CAN_Info_ID[5]={0x01,0x55,0xe0,0x55,0x55};
//unsigned char data Com_RecBuff[8]={0x01,0x02,0x03,0x4,0x05,0x06,0x07,0x08};
unsigned char data Com_RecBuff[8];
unsigned char bdata flag_init;
//*************************
#define INBUF_LEN 8   //数据长度

unsigned char inbuf1[INBUF_LEN];
unsigned char checksum,count3=0;
bit  read_flag=0;
sbit P10=P1^0;
sbit P11=P1^1;
sbit P12=P1^2;
sbit P13=P1^3;
sbit P14=P1^4;
sbit P15=P1^5;
sbit P16=P1^6;
sbit P17=P1^7;
sbit P35=P3^5;
sbit P37=P3^7;
sbit P34=P3^4;
//***************************
sbit rcv_flag=flag_init^0;
sbit err_flag=flag_init^0;
sbit CS=P2^0;
//sbit RECOK=P3^4;
unsigned char RECOK;
unsigned int msg;




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
#if 0
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
                 P34=0;
                 P10=!P10;
                 P11=!P11;
                 P12=!P12;
                 P13=!P13;
                 CAN_Send_anylength(inbuf1,8);
                 memset(inbuf1,0,8);
                 
             }
        
    }
    }
}
#endif




//***********************************************************************
/************************************************************************
*函数原型: bit BCAN_SET_OUTCLK( unsigned char Clock_Out)               *
*参数说明:                                                             *
*          Clock_Out:存放时钟分频寄存器(CDR)的参数设置                 *
*返回值:                                                               *
*           0 ;设置成功                                                *
*           1 ;设置失败                                                *
*说明:设置SJA1000的时钟分频 。该子程序只能用于复位模式                      
************************************************************************/
bit BCAN_SET_OUTCLK(unsigned char Clock_Out)
{
  SJA_BCANAdr=REG_CDR;           

  *SJA_BCANAdr=Clock_Out;        
  if(*SJA_BCANAdr != Clock_Out)
     return 1; 
   else 
     return 0;
} 

/************************************************************************
*函数原型: bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3     
                                              BCAN_AMR0, BCAN_AMR1, BCAN_AMR2, BCAN_AMR3     
*参数说明:                                                              *
*    BCAN_ACR(0-3):存放验收代码寄存器（ACR）的参数设置                  *
*    BCAN_AMR(0-3):存放接收屏蔽寄存器（AMR）的参数设置                  *
*返回值:                                                                *
*           0 ;设置成功                                                 *
*           1 ;设置失败                                                 *
*说明:设置CAN节点的通讯对象，允许接收的报文,是由AMR和ACR共同决定的.     *
*************************************************************************/ 
bit BCAN_SET_OBJECT(unsigned char  BCAN_ACR0,BCAN_ACR1,BCAN_ACR2,BCAN_ACR3,BCAN_AMR0,BCAN_AMR1,BCAN_AMR2,BCAN_AMR3)
{
  SJA_BCANAdr=REG_TXBuffer1;     

  *SJA_BCANAdr=BCAN_ACR0;        //写入参数
   
  SJA_BCANAdr=REG_TXBuffer2;
  *SJA_BCANAdr=BCAN_ACR1;

  SJA_BCANAdr=REG_TXBuffer3;  
  *SJA_BCANAdr=BCAN_ACR2;
   
  SJA_BCANAdr=REG_TXBuffer4; 
  *SJA_BCANAdr=BCAN_ACR3;  
 
  //校验写入值
  if(*SJA_BCANAdr != BCAN_ACR3)  return 1; 
  
  SJA_BCANAdr=REG_TXBuffer5;      

  *SJA_BCANAdr=BCAN_AMR0;     //写入参数
  
  SJA_BCANAdr=REG_TXBuffer6;         
  *SJA_BCANAdr=BCAN_AMR1;

  SJA_BCANAdr=REG_TXBuffer7; 
  *SJA_BCANAdr=BCAN_AMR2;

  SJA_BCANAdr=REG_TXBuffer8; 
  *SJA_BCANAdr=BCAN_AMR3;
  //校验写入值
  if(*SJA_BCANAdr != BCAN_AMR3) return 1; 

  return 0;
} 

/************************************************************************
;*函数原型:  bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)          *
;*返回值:                                                               *
;*           0 ;波特率设置成功                                          *
;*           1 ;波特率设置失败                                          *
;*                                                                      * 
;*说明:设置CAN控制器SJA1000通讯波特率.SJA1000的晶振必须为16MHZ,         *
;*     其它晶体的频率的值的波特率，需自己计算 。该子程序只能用于        *
;*     复位模式                                                         *  
;************************************************************************/ 
bit BCAN_SET_BANDRATE(unsigned char CAN_ByteRate)         //波特率选择
{
     unsigned char BR_Num= CAN_ByteRate,BTR0_num,BTR1_num;
       switch (BR_Num)
        {
          case ByteRate_5k:
               BTR0_num=0xBF;
               BTR1_num=0xFF;
               break;
          case ByteRate_10k:
               BTR0_num=0x31;
               BTR1_num=0x1C;
               break;
          case ByteRate_20k:
               BTR0_num=0x18;
               BTR1_num=0x1C;
               break;
          case ByteRate_40k  :
               BTR0_num=0x87;
               BTR1_num=0xff;
               break;
          case ByteRate_50k:
               //BTR0_num=0x47;
               //BTR1_num=0x2f;
               BTR0_num=0x0e;
               BTR1_num=0x1c;
               break;
          case ByteRate_80k  :
               //BTR0_num=0x83;
               //BTR1_num=0xff;
               BTR0_num=0x49;                      //ok
               BTR1_num=0x1b;
               break;
          case ByteRate_100k  :
               //BTR0_num=0x43;
               //BTR1_num=0x2f;
               BTR0_num=0x04;                       //ok
               BTR1_num=0x1C;
               break;
          case ByteRate_125k  :                      //SJA1000的晶振为必须为16MHZ,波特率设置为125kpbs
               BTR0_num=0x03;
               BTR1_num=0x1c;
               //BTR0_num=0x44;                        //ok
               //BTR1_num=0x1f;
               break;
          case ByteRate_200k  ://24MHZ
            //   BTR0_num=0xc5;  //
             //  BTR1_num=0xa5;
               BTR0_num=0x43;                        //ok
               BTR1_num=0x1b;
               break;
          /* case ByteRate_200k  ://24MHZ
               BTR0_num=0x81;
               BTR1_num=0xFA;
               break;*/
          case ByteRate_400k  :
               BTR0_num=0x80;
               BTR1_num=0xfa;
               break;
          case ByteRate_500k  :
               BTR0_num=0x01;
               BTR1_num=0x1c;
               break;
          case ByteRate_800k  :
               BTR0_num=0x00;
               BTR1_num=0x16;
               break;
          case ByteRate_1000k  :
               BTR0_num=0x00;
               BTR1_num=0x14;
               break;
          default :
               return 1;
               break;
        }

    SJA_BCANAdr=REG_BTR0;
    *SJA_BCANAdr=BTR0_num;
    if(*SJA_BCANAdr!=BTR0_num)
      {return 1;}
    SJA_BCANAdr=REG_BTR1;
    *SJA_BCANAdr=BTR1_num;
    if(*SJA_BCANAdr!=BTR1_num)
      {return 1;}
    return 0;
}
/************************************************************************
 *函数原型: bit BCAN_SET_CONTROL(unsigend char CMD)                     *
 *参数说明: 设置控制寄存器                                              *
 ************************************************************************/
bit BCAN_SET_CONTROL(unsigned char CMD)
{  unsigned char TempData;
 
  SJA_BCANAdr=REG_CONTROL;   //SJA_BaseAdr+0x00  控制寄存器
  TempData=  *SJA_BCANAdr; 
 
  *SJA_BCANAdr=CMD;
 
  if (*SJA_BCANAdr == CMD)
    return 0;
  else
    return 1;

}
/************************************************************************
 *函数原型:  bit   BCAN_CREATE_COMMUNATION(void)                        * 
 *参数说明:  无                                                         *
 *返回值:                                                               *
 *           0 ; 表示SJA1000接口正常                                    *
 *           1 ; 表示SJA1000与处理器接口不正常                          *
 *说明:该函数用于检测CAN控制器的接口是否正常                            *
 ************************************************************************/
 bit BCAN_CREATE_COMMUNATION(void)
 {  
    SJA_BCANAdr=REG_TEST;      
    *SJA_BCANAdr=0xaa;       //写入测试值
    if(*SJA_BCANAdr == 0xaa)
       return 0;            //读测试正确
    else
       return 1;
     
 }

 /************************************************************************
 *函数原型:      bit   BCAN_ENTER_RETMODEL(void)                        *
 *参数说明:  无                                                         *
 *返回值:                                                               *
 *           0 ; 表示成功进入复位工作模式                               *
 *           1 ; 表示不能进入复位工作模式                               *
 *                                                                      * 
 *说明:      CAN控制器进入复位工作模式                                  *
 ************************************************************************/ 
 bit   BCAN_ENTER_RETMODEL(void)     //置位复位请求
 {
    unsigned   char   TempData;
    SJA_BCANAdr  = REG_CONTROL;   

  TempData=  *SJA_BCANAdr;       
    *SJA_BCANAdr=0x09;                 //置位复位请求 和单滤波模式
    if((*SJA_BCANAdr&0x01) == 1)
     return   0;
    else
      return   1;   
 } 
/************************************************************************
 *函数原型:   BCAN_CMD_PRG(unsigned char cmd)                           *
 *参数说明:  unsigned char cmd                                          *
 *返回值:                                                               *
 *           0 ; 请求成功                                               *
 *           1 ; 请求失败                                               *
 *                                                                      * 
 *说明:      启动命令字                                                 *
 ************************************************************************/
bit  BCAN_CMD_PRG(unsigned char cmd)
 {
   SJA_BCANAdr=REG_COMMAND;            //访问地址指向命令寄存器
   *SJA_BCANAdr=cmd;                   //启动命令字
 
   switch(cmd)
   {    case  TR_CMD:                    

           return    0;
           break;

  case  SRR_CMD:      

      return 0;
     break;
    
  case  AT_CMD:                  

            SJA_BCANAdr = REG_STATUS;   //访问地址指向状态寄存器   
           if((*SJA_BCANAdr & 0x20)==0) //判断是否正在发送
             return  0;
           else
             return  1;              
           break; 
     case  RRB_CMD:                   
           SJA_BCANAdr = REG_STATUS;   //访问地址指向状态寄存器   
           if((*SJA_BCANAdr & 0x01)==1)
              return  1;
           else           
              return  0;               
           break;  
     case  COS_CMD:                  

           SJA_BCANAdr = REG_STATUS;   
           if((*SJA_BCANAdr & 0x02)==0)//判断清除超载是否成功
             return  0; 
           else
             return  1;             
           break; 
     default:
             return  1;
             break; 
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

 return;
 msg++;
 if(msg==5)
  {
  msg=0;
  P34=0;
  P10=!P10;
  P11=!P11;
  P12=!P12;
  P13=!P13;
  P14=!P14;
  P15=!P15;
  P16=!P16;
  P17=!P17;
  }    
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
    //memcpy(Com_RecBuff,RevceData,8);      //测试用的主要是把接收到的数据在发出去，验证数据的正确
                                            //以下代码是发送到串
    for(i=0;i<length;i++)
     send_char_com(RevceData[i]);
    /* send_char_com(RevceData[0]);
     send_char_com(RevceData[1]);           
     send_char_com(RevceData[2]);
     send_char_com(RevceData[3]);
     send_char_com(RevceData[4]);
     send_char_com(RevceData[5]);
     send_char_com(RevceData[6]);
     send_char_com(RevceData[7]);*/
     
    // RECOK=1;                              //测试用的主要是把接收到的数据在发出去，验证数据的正确
     }

     BCAN_CMD_PRG(RRB_CMD);                  //释放SJA1000接收缓冲区，****已经修改

 }
} 

unsigned char BCAN_DATA_WRITE(unsigned char *SendDataBuf)
 {  unsigned char temp;
   
    SJA_BCANAdr = REG_STATUS;    
    temp=*SJA_BCANAdr;

    if ((temp&0x08)==0) return  1;    //上次发送未完成
    if ((temp&0x04)==0) return  2;    //发送缓冲区是否锁定 
    if ((temp&0x10)==0x10) return 3;  //判断是否正在接收   

    SJA_BCANAdr = REG_RXBuffer1;      //访问地址指向发送缓冲区1,修改成头文件

    memcpy(SJA_BCANAdr,SendDataBuf,4);   
    BCAN_CMD_PRG(TR_CMD);             //请求发送         
    return 0;
}
//CAN发送任意长度字节
void CAN_Send_anylength(unsigned char *CAN_TX_Buf,unsigned char length1)
{
	unsigned char temptt;
	loop:
    SJA_BCANAdr = REG_STATUS;    
         temptt=*SJA_BCANAdr; 
	//temptt=Read_SJA1000(REG_STATUS);
	if((temptt&0x04)==0x00)  goto loop;               //循环检测等待                       
	//可以向发送缓冲器写数据
	{
    SJA_BCANAdr = REG_RXBuffer1;      //访问地址指向发送缓冲区1,修改成头文件
    *SJA_BCANAdr=length1;  
    SJA_BCANAdr = REG_RXBuffer2;      //访问地址指向发送缓冲区1,修改成头文件
    *SJA_BCANAdr=0x00;  
     SJA_BCANAdr = REG_RXBuffer3;     //访问地址指向发送缓冲区1,修改成头文件
    *SJA_BCANAdr=0x00;
    SJA_BCANAdr = REG_RXBuffer4; 
    memcpy(SJA_BCANAdr,CAN_TX_Buf,length1);   
	//数据发送请求
    BCAN_CMD_PRG(TR_CMD);            //请求发送  
	}
}

/****************************************************
**函数原型：   bit Sja_1000_Init(void)
**功    能：   初始化SJA10000
**入口参数:    无 
**返 回 值:     
      0： 初始化成功 
      1： 复位失败 
      2:  测试sja1000失败 
      3： 设置失败 
      4： 设置验收滤波器失败
      5： 设置波特率失败     
*****************************************************/

unsigned char Sja_1000_Init(void)
{
 bit s; 
 EA=0;                                 //关总中断
   s=BCAN_ENTER_RETMODEL();
 if (s==1) return 1; 
    s=BCAN_CREATE_COMMUNATION();       //建立通信
    if (s==1) return 2;

  s=BCAN_SET_OUTCLK(0x88);             //Pelican
 if (s==1) return 3;

 s=BCAN_SET_OBJECT(0xFF,0x4E,0x16,0x00,0xff,0xff,0xff,0xff);//屏蔽寄存器，都设为无关，接收所有报文 
                                                            //当屏蔽位为1，不滤波，0就滤波必须相等
 //s=BCAN_SET_OBJECT(0x55,0xe0,0xaa,0xa1,0x00,0x00,0xff,0xff);//验收码&屏蔽码

 if (s==1) return 4;

 s=BCAN_SET_BANDRATE(ByteRate_125k);    //设置波特率200K 24MHZ
    if (s==1) return 5;
    
 SJA_BCANAdr=REG_OCR ;                  //输出控制寄存器  
    *SJA_BCANAdr=0x1a;        
    SJA_BCANAdr=REG_INTENABLE;      

   *SJA_BCANAdr=0x1D;                  //设置中断，接收和发送中断
    
    // s=BCAN_SET_CONTROL(0x08);
    SJA_BCANAdr=REG_CONTROL;            //退出 复位模式
    *SJA_BCANAdr=*SJA_BCANAdr&0xfe;
     if(*SJA_BCANAdr!=0x00)
     return 6;

 //if (s==1) return 6;

    EA=1;
 return 0;   
} 
//***************************************************

//初始化cpu

//**************************************************
void Init_Cpu(void)                                  //单片机初始化,开放外部中断0
{
     //PX0=1;
     EX0=1;
    //IT0=0;
    // EA=1;
}

/****************************************************
**函数原型：   void main(void)
**功    能：   主程序部分:
**入口参数:    无 
**返 回 值:     
*****************************************************/
unsigned char a;
int flag = 0;


void rs485SetModeRx()
{
	//P17=0; //output

#if 0
	P11=0;
	P12=0;
	P13=0;
	P14=0;
	P15=0;
	P16=0;
#endif	
	
	
	P37=0; //output
}

void rs485SetModeTx()
{
	//P17=1; //output
	P37=1; //output

#if 0
	P11=1;
	P12=1;
	P13=1;
	P14=1;
	P15=1;
	P16=1;
#endif	
}

unsigned char num = 0;

void serial() interrupt 4
{
	static int flag_local = 0;
	//P1=SBUF;
	num++;
	a=SBUF;
	flag=1;
	RI=0;


	if (a == 0xa)
	{
		flag_local=0; //enable
		#if 0
		P10=0;
		P11=0;
		P12=0;
		P13=0;
		P14=0;
		P15=0;
		P16=0;
		P17=0;
#endif

		//P1=0;



	}
	else if (a == 0xb)
	{
		flag_local=1; //disable

#if 0
		P10=1;
		P11=1;
		P12=1;
		P13=1;
		P14=1;
		P15=1;
		P16=1;
		P17=1;
#endif
		//P1=0x1;

	}
	else
	{

#if 1
		if (flag_local)
		{
			P1 |= (0x1 << a);
		}
		else //enable --> set to 0
		{
			P1 &= ~(0x1 << a);
		}

#endif
	
	}
	
	
	
	
	#if 0

	P10=flag_local;
	P11=flag_local;
	P12=flag_local;
	P13=flag_local;
	P14=flag_local;
	P15=flag_local;
	P16=flag_local;
	P17=flag_local;
#endif

#if 0	
	
	if (flag_local)
	{
		P1 &= ~(1 << a);
		
		
	}
	else
	{
		P1 |= (1<<a);
	}
	
	

#endif


	
}

/************************************************************************
*函数原型: void init_serialcomm(void)            *
*参数说明: 串口初始化                            *                                                             *
*说明:     设值单片机的定时器1的方式选择波特率 。该子程序只能用于复位模式                      
************************************************************************/
void init_serialcomm(void)
{
#if 1
	SCON  = 0x50;       //SCON: serail mode 1, 8-bit UART, enable ucvr 
    TMOD |= 0x20;       //TMOD: timer 1, mode 2, 8-bit reload 
    PCON |= 0x80;       //SMOD=1; 
    TH1   = 0xF3;       //Baud:4800  fosc=12MHz  
    TL1   = 0xF3;       //baud:4800

	//TH1   = 0xF4;       //Baud:4800  fosc=11.0592MHz  
    //TL1   = 0xF4;       //baud:4800

	//TH1   = 0xF9;       //Baud:9600  fosc=12MHz  
    //TL1   = 0xF9;       //baud:9600     
    IE   |= 0x90;       //Enable Serial Interrupt 
    TR1   = 1;          // timer 1 run 
#endif

	//ES=1;
}



void setTimer(void)
{

	

	TH0=0x3c;
	TL0=0xaf;  //50MS定时初值（T0计时用）


	
}

//定时器0初始化
void timer0initial()
{
	TMOD |= 0x1;        //工作方式16位定时计数器
	setTimer();
		
    ET0=1;
	TR0=1;
}

//int timer_flag = 0;
//int timer_count = 0;

//定时器0中断,不够8个就在此发送
void time_intt0(void) interrupt 1 using 2
{
	static int timer_flag = 0;
	static int timer_count = 0;
	setTimer();
	if (timer_count == 6)
	{
	
		timer_flag = !timer_flag;
		P35=timer_flag;
		//P10=timer_flag;

		timer_count = 0;
	}
	else
	{
		timer_count++;
	}


	
}


void delay_ms(int t)
{
     int i,j;
     for (i=0;i<t;i++)
     	for(j=0;j<33;j++) 
     	;
    
}
void delay10ms(void) //延时程序

{

      unsigned char i,j;

      for(i=20;i>0;i--)

      for(j=248;j>0;j--);

}


void delay_s(unsigned char t)
{
	unsigned int i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<0xffff;j++)
			;
	}
	
}



void setSomething(void)
{
	static int flag = 0;

	flag = !flag;

	//P10=flag;
	P35=flag;

}

void main(void)
{  
	int test = 0x4f;
	
	CS=0;                //片选择引脚
	EA=0;
	Init_Cpu(); 

	init_serialcomm();     //初始化串口 

	timer0initial();

	EA=1; //初始化成功，开总中断

	P1 = 0xff;

	rs485SetModeRx();


	
	//rs485SetModeTx();

	//rs485SetModeTx();

	//次标识位可以作为，串口接收完，置标志然后发送出去或者当作按键发送******
	while(1) 
	{
		//delay_ms(500);
		//P11=0;
		//setSomething();
		
	
#if 0
		delay_ms(5000);
		P11=0;

		delay_ms(5000);
		P12=0;

		delay_ms(5000);
		P13=0;

		delay_ms(5000);
		P14=0;

		delay_s(1);
		P15=0;


		delay_s(2);
		P16=0;
		delay_ms(10000);
		P11=1;
		P12=1;
		P13=1;
		P14=1;
		P15=1;
		P16=1;
#endif

#if 1	
		if(flag==1)
		{
			rs485SetModeTx();
		
			ES=0;
			flag=0;
			SBUF=0xa;
			
			while(!TI);
			TI=0;
#if 1
			SBUF=0xb;
			while(!TI);
			TI=0;
			

			SBUF=0xc;
			while(!TI);
			TI=0;

			SBUF=num;
			while(!TI);
			TI=0;

			SBUF=num;
			while(!TI);
			TI=0;

			
		

			SBUF=a;
			while(!TI);
			TI=0;


			SBUF='#';
			while(!TI);
			TI=0;

#if 1
			SBUF=P1;
			while(!TI);
			TI=0;
			SBUF='#';
			while(!TI);
			TI=0;
#endif			
#endif			

			ES=1;

			

			//delay_ms(5000);
			//delay_ms(5000);
			//delay_ms(5000);

			//delay_ms(100);

			rs485SetModeRx();
			
		}
#endif

	}

}


