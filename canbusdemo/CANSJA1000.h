#include  "reg51.h"
//定义SJA1000的基址

//系统指针，指向SJA1000
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

#define         BTR0_Rate_20k      0x53          //20KBPS的预设值
#define         BTR1_Rate_20k      0x2F          //20KBPS的预设值
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

unsigned char xdata *SJA_BCANAdr;
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
          case ByteRate_10k:
               BTR0_num=0xef;
               BTR1_num=0xff;
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
               BTR0_num=0x45;                       //ok
               BTR1_num=0x3e;
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
          case ByteRate_250k  ://24MHZ
            //   BTR0_num=0xc5;  //
             //  BTR1_num=0xa5;
               BTR0_num=0x01;                        //ok
               BTR1_num=0x1c;
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
//CAN发送一个字节
void CAN_Send_onebyte(unsigned char CAN_TX_data,unsigned char length1)
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
    //memcpy(SJA_BCANAdr,CAN_TX_Buf,length1);  
    *SJA_BCANAdr=CAN_TX_data; 
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

 s=BCAN_SET_BANDRATE(ByteRate_125k);    //设置波特率125K 16MHZ
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