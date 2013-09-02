#ifndef	__FUNCTION_h__
#define	__FUNCTION_h__
//	write your header here
#include "reg51.h"
void delay_ms(unsigned char t)
{
     unsigned char i,j;
     for (i=0;i<t;i++)
     	for(j=0;j<33;j++) 
     	;
    
}
void delay10ms(void) //ÑÓÊ±³ÌÐò

{

      unsigned char i,j;

      for(i=20;i>0;i--)

      for(j=248;j>0;j--);

}


void delay_s(unsigned char t)
{
	unsigned char i,j;
	for(i=0;i<t;i++)
	{
		for(j=0;j<34135;j++)
			;
	}
	
}

#endif
