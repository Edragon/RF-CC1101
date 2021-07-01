#include <reg52.h>
#include <intrins.h>
#define		INT8U		unsigned char
#define		INT16U		unsigned int

sfr   WDT_CONTR =0Xe1;

//***********************************按键********************************************************
sbit    KEY1    =P3^6;
sbit    KEY2    =P3^7;
//***********************************数码管位选**************************************************
sbit     led    =P2^5;


sbit	led3=P2^0;
sbit	led2=P2^1;
sbit	led1=P2^2;
sbit	led0=P2^3;


INT8U TxBuf[4];
INT8U send_f;




//when  i=100.  delay=1.5ms     实测 值 
static void delay(unsigned int s)
{
	unsigned int i;
	for(i=0; i<s; i++);
}


void UART_init()
{
	SCON = 0x50;                 /* uart in mode 1 (8 bit), REN=1 */
	TMOD = TMOD | 0x20 ;         /* Timer 1 in mode 2 */
	TH1  = 0xFD;                 /* 9600 Bds at 11.059MHz */
	TL1  = 0xFD;                 /* 9600 Bds at 11.059MHz */
	//IE =0x90;
	TR1 = 1;                     /* Timer 1 run */

    ET1 =0; 
    ES=1;                //允许串行口中断
    PS=1;                //设计串行口中断优先级
    EA =1;               //单片机中断允许
}

void Sendchar(unsigned char c)	     
{
	SBUF=c;                                     
	while(TI==0);                                               
	TI=0;
}

void print_string(unsigned char* p)		  //发送字符串	   
{
  while(*p !='\0')
   {
     Sendchar(*p++);
   }
}



//--------------------------------------------------------------------------------------------------
// 函数名称： com_interrup()串口接收中断处理函数
// 函数功能： 接收包括起始位'S'在内的十位数据到数据缓冲区
//--------------------------------------------------------------------------------------------------
com_interrupt(void) interrupt 4 using 3
{
  

  if(RI)                                //处理接收中断
  {
     RI=0;                                            //清除中断标志位
     TxBuf[2]=SBUF;

		 send_f=1;		              //设置数据发送标志
         send_f=1;
		 led=1;  			 	
  }
}




void main(void)
{
	INT8U leng =0;
	INT8U i,k=0;
	INT8U RxBuf[4]={0};	
	INT8U rxcount;
	INT8U TxBuf[30];

	led=1;
	rxcount=0xff;
	TxBuf[2]=2;
	TxBuf[3]=0;
    UART_init();
	P0=0x00;
    led1=0;
	for(i=0; i<30; i++)
	  {
	   TxBuf[i]=i;
	   }
	Sendchar(0xA3);  //设置波特率命令
	Sendchar(0x3A);
	Sendchar(0x02);  //设置波特率为9600 
    delay(6000);
	Sendchar(0xA7);  //设置信道命令
	Sendchar(0x7A);
	Sendchar(0x00);  //设置信道为  06 
    delay(6000);
	Sendchar(0xAB);  //设置功率
	Sendchar(0xBA);
	Sendchar(0x0A);  //设置功率为10DBM
    delay(6000);
	Sendchar(0xA9);  //设置模块ID号
	Sendchar(0x9A);
	Sendchar(0x00);  //设置模块ID号
	Sendchar(0x01);  //设置ID号
    delay(6000);
    delay(6000);
//    WDT_CONTR=0x3c;
	while(1)
	{
      WDT_CONTR=0x3c;
      if(KEY1==0)
	  {
        delay(10);
		while(!KEY1)			 //当检测到按键时发送测试数据 
		  {
		  delay(10);
		  }
		  led1=0;

		  print_string("abcdefghijklmnopqrstuvwxyzABCD");	//第一贞测试数据  ， 字符串或数据长度不要超过30个字节 

		  
		  led1=1;	
		  delay(750);	                                   //发送下一贞数据前要延迟6ms左右. 
		  for(i=0;i<23;i++)		     //发送23字节的数据包	       
		    {
		  Sendchar(TxBuf[i]);
		    }
		  delay(750);	                                   //发送下一贞数据前要延迟6ms左右. 
		  Sendchar(0x66); 	         //发送单字节 				  		  		  
	   }
       delay(450);
       led1=1;
       delay(100);

	}
	
}


