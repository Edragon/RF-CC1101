#include <reg52.h>
#include <intrins.h>
#include "DS18B20.h"
#include "cc1100.h"
#include "IO_def.h"

unsigned char ten,i,j; 
unsigned char bflag;//指示是否需要发送无线

INT8U TxBuf[32];	 // 8字节, 如果需要更长的数据包,请正确设置


INT8U RxBuf[32];	

INT8U RxBuf[32];
INT8U RxBuf[32];

unsigned  char  re_buf,back_buf;
unsigned  char  keybuf;

unsigned char  module_address;

//***************更多功率参数设置可详细参考DATACC1100英文文档中第48-49页的参数表******************
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04};  //-30dBm   功率最小
//INT8U PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm
INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};   //10dBm     功率最大


INT8U seg[10]={0xC0,0xCF,0xA4,0xB0,0x99,0x92,0x82,0xF8,0x80,0x90}; 

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


//--------------------------------------------------------------------------------------------------
// 函数名称： com_interrup()串口接收中断处理函数
// 函数功能： 接收包括起始位'S'在内的十位数据到数据缓冲区
//--------------------------------------------------------------------------------------------------
com_interrupt(void) interrupt 4 using 3
{
  
 unsigned char  re_leng;	
 unsigned char newdata;
  // char  RECEIVR_buffer;
  if(RI)                                //处理接收中断
  {
     RI=0;    
	 back_buf=re_buf;                             //清除中断标志位
     re_buf=SBUF;
     TxBuf[j]=re_buf;
     j=j+1;
     
     if(back_buf==0xa1)			//检测一帧数据的开始字节
	    {
		  if(re_buf==0xa2)
		     {
			  newdata=1;	    //检测到一帧数据，设标志为‘1’	
			  j=2;
			  TxBuf[0]=0xa1;
			  TxBuf[1]=0xa2;
			  }
		   else
		      newdata=8;
		 }
	  else
	     {
		      newdata=8;
		 }

	  re_leng=TxBuf[9];			 //数据长度

		      
	  if(j>=10+re_leng+2)
	   {
	     j=0;
		 i=1;		              //一帧数据结束，无线发送标志置‘1’
         led1=0;
       }  	
	
	 	
// delay(50);
// BUF=keybuf;

// delay(10);
  }
}

//void print_string(unsigned char* p)
//{
//while(*p !='\0')
//{
//  Sendchar(*p++);
//}
//}





void main()
{

    INT8U  m,k;
	INT8U leng=4;
	INT8U tf=0;
	INT8U rxcount;
	INT8U need_send;	 //可以发送数据标志，‘1’可以发，‘0’不可以发。 
    INT8U module_address0; //车间号
    INT8U module_address1; //模块号


//	INT8U TxBuf[64];	 // 8字节, 如果需要更长的数据包,请正确设置
//	INT8U RxBuf[4];	
    
  	module_address=0x07;
	rxcount=0xff;
	TxBuf[31]=0;
    i=0;
	j=0;
	m=0;
	tf=0;
	need_send=0;
    module_address0=0x00;
	module_address1=0x01;

	BELL = 0;
	led0=0;
	led1=0;
	P0=seg[0];		
	UART_init();


	CpuInit();
	POWER_UP_RESET_CC1100();
	halRfWriteRfSettings();
	halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);

	//TxBuf[0] = 8 ;
	TxBuf[1] = 1 ;
	TxBuf[2] = 1 ;
	halRfSendPacket(TxBuf,4);	// Transmit Tx buffer data
	delay(6000);


    BELL = 1;	
	led0=1;
	led1=1;
	while (1)
	{
	    
	    //ReadTemperature();
		//Init_DS18B20();

        BELL = 1;
    	if(KEY1 ==0 ) 
	  	{

		    delay(200);
			if(KEY1==0)
			  {
		       TxBuf[1] = 1 ;
			   TxBuf[2] = 0 ;
			   keybuf=1;
	      	   led0=0;
			   	P0=seg[0];
		       tf = 1 ; 
			   }
			//while(KEY1 ==0);
	    }
	   if(KEY2 ==0 )
	   {
			delay(200);
			if(KEY2==0)
			  {
		       TxBuf[1] = 0 ;
		       TxBuf[2] = 1 ;
			   keybuf=1;
	    	   led1=0;
			   	P0=seg[0];
			   tf = 1 ; 
			   }
	   }

	   if (tf==1)
       {	
            
//          TxBuf[0]=0xa1;
//			TxBuf[1]=0xa2;

			TxBuf[31]=TxBuf[31]+1;	        // 发送一个64位的测试信息
//			for(k=0;k<2;k++)
//			{                               //-----说明串口模块的数据格式为
                                            //-----数据包长度设置为32，其中TxBuf【30】为本次数据有效长度。
                                            //-----例如如果本次数据长度为17，则TxBuf【30】=17.
                                            //-----有效数据长度不能超过30
                                            //-----符合以上格式，串口模块就能正确接收数据
			  halRfSendPacket(TxBuf,32);	// ----Transmit Tx buffer data		
//		      delay(10);
//			 }
			tf=0;
			tf=0;

	   }

		
	    delay(50);		                         //延迟时间需要调试		    
//        ES=0;     //  关闭串口中断
		leng =32; // 预计接受8 bytes


  		if(halRfReceivePacket(RxBuf,&leng))
		  {
						
			led0=0;
			led3=0;
//        	BELL = 0;
			P0=seg[RxBuf[2]];
	     	delay(1);


                        tf=1; 
                        for(k=0;k<31;k++)        //------串口模块每次发送的数据是32个字节--
						  {                      //------本次接收到的有效数据包长度在RxBuf[30]中，
                        TxBuf[k]=RxBuf[k];       //------如果RxBuf[30]=17，则串口模块发过来的有效数据就是17个字节，即RxBuf[0]~RxBuf[16]
                           }                     //------本测试例程就是把接收到的数据再一样发送出去


	   	   }
	    delay(100);	
	    ES=1;   //开启串口中断
        m=m+1;
		if(m>=0xff)
		{
		  m=0;
		  POWER_UP_RESET_CC1100();
	      halRfWriteRfSettings();
	      halSpiWriteBurstReg(CCxxx0_PATABLE, PaTabel, 8);
		 }

	    led0=1;
		led1=1;
		led2=1;
		led3=1;
        	
	}
	
}