#include <reg52.h>
#include <intrins.h>
#include "DS18B20.h"
#include "cc1100.h"
#include "IO_def.h"

unsigned char ten,i,j; 
unsigned char bflag;//ָʾ�Ƿ���Ҫ��������

INT8U TxBuf[32];	 // 8�ֽ�, �����Ҫ���������ݰ�,����ȷ����


INT8U RxBuf[32];	

INT8U RxBuf[32];
INT8U RxBuf[32];

unsigned  char  re_buf,back_buf;
unsigned  char  keybuf;

unsigned char  module_address;

//***************���๦�ʲ������ÿ���ϸ�ο�DATACC1100Ӣ���ĵ��е�48-49ҳ�Ĳ�����******************
//INT8U PaTabel[8] = {0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04 ,0x04};  //-30dBm   ������С
//INT8U PaTabel[8] = {0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60 ,0x60};  //0dBm
INT8U PaTabel[8] = {0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0 ,0xC0};   //10dBm     �������


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
    ES=1;                //�����п��ж�
    PS=1;                //��ƴ��п��ж����ȼ�
    EA =1;               //��Ƭ���ж�����
}

void Sendchar(unsigned char c)
{
	SBUF=c;                                     
	while(TI==0);                                               
	TI=0;
}


//--------------------------------------------------------------------------------------------------
// �������ƣ� com_interrup()���ڽ����жϴ�����
// �������ܣ� ���հ�����ʼλ'S'���ڵ�ʮλ���ݵ����ݻ�����
//--------------------------------------------------------------------------------------------------
com_interrupt(void) interrupt 4 using 3
{
  
 unsigned char  re_leng;	
 unsigned char newdata;
  // char  RECEIVR_buffer;
  if(RI)                                //��������ж�
  {
     RI=0;    
	 back_buf=re_buf;                             //����жϱ�־λ
     re_buf=SBUF;
     TxBuf[j]=re_buf;
     j=j+1;
     
     if(back_buf==0xa1)			//���һ֡���ݵĿ�ʼ�ֽ�
	    {
		  if(re_buf==0xa2)
		     {
			  newdata=1;	    //��⵽һ֡���ݣ����־Ϊ��1��	
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

	  re_leng=TxBuf[9];			 //���ݳ���

		      
	  if(j>=10+re_leng+2)
	   {
	     j=0;
		 i=1;		              //һ֡���ݽ��������߷��ͱ�־�á�1��
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
	INT8U need_send;	 //���Է������ݱ�־����1�����Է�����0�������Է��� 
    INT8U module_address0; //�����
    INT8U module_address1; //ģ���


//	INT8U TxBuf[64];	 // 8�ֽ�, �����Ҫ���������ݰ�,����ȷ����
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

			TxBuf[31]=TxBuf[31]+1;	        // ����һ��64λ�Ĳ�����Ϣ
//			for(k=0;k<2;k++)
//			{                               //-----˵������ģ������ݸ�ʽΪ
                                            //-----���ݰ���������Ϊ32������TxBuf��30��Ϊ����������Ч���ȡ�
                                            //-----��������������ݳ���Ϊ17����TxBuf��30��=17.
                                            //-----��Ч���ݳ��Ȳ��ܳ���30
                                            //-----�������ϸ�ʽ������ģ�������ȷ��������
			  halRfSendPacket(TxBuf,32);	// ----Transmit Tx buffer data		
//		      delay(10);
//			 }
			tf=0;
			tf=0;

	   }

		
	    delay(50);		                         //�ӳ�ʱ����Ҫ����		    
//        ES=0;     //  �رմ����ж�
		leng =32; // Ԥ�ƽ���8 bytes


  		if(halRfReceivePacket(RxBuf,&leng))
		  {
						
			led0=0;
			led3=0;
//        	BELL = 0;
			P0=seg[RxBuf[2]];
	     	delay(1);


                        tf=1; 
                        for(k=0;k<31;k++)        //------����ģ��ÿ�η��͵�������32���ֽ�--
						  {                      //------���ν��յ�����Ч���ݰ�������RxBuf[30]�У�
                        TxBuf[k]=RxBuf[k];       //------���RxBuf[30]=17���򴮿�ģ�鷢��������Ч���ݾ���17���ֽڣ���RxBuf[0]~RxBuf[16]
                           }                     //------���������̾��ǰѽ��յ���������һ�����ͳ�ȥ


	   	   }
	    delay(100);	
	    ES=1;   //���������ж�
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