#include <reg52.h>
#include <intrins.h>
#define		INT8U		unsigned char
#define		INT16U		unsigned int

sfr   WDT_CONTR =0Xe1;

//***********************************����********************************************************
sbit    KEY1    =P3^6;
sbit    KEY2    =P3^7;
//***********************************�����λѡ**************************************************
sbit     led    =P2^5;


sbit	led3=P2^0;
sbit	led2=P2^1;
sbit	led1=P2^2;
sbit	led0=P2^3;


INT8U TxBuf[4];
INT8U send_f;




//when  i=100.  delay=1.5ms     ʵ�� ֵ 
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

void print_string(unsigned char* p)		  //�����ַ���	   
{
  while(*p !='\0')
   {
     Sendchar(*p++);
   }
}



//--------------------------------------------------------------------------------------------------
// �������ƣ� com_interrup()���ڽ����жϴ�����
// �������ܣ� ���հ�����ʼλ'S'���ڵ�ʮλ���ݵ����ݻ�����
//--------------------------------------------------------------------------------------------------
com_interrupt(void) interrupt 4 using 3
{
  

  if(RI)                                //��������ж�
  {
     RI=0;                                            //����жϱ�־λ
     TxBuf[2]=SBUF;

		 send_f=1;		              //�������ݷ��ͱ�־
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
	Sendchar(0xA3);  //���ò���������
	Sendchar(0x3A);
	Sendchar(0x02);  //���ò�����Ϊ9600 
    delay(6000);
	Sendchar(0xA7);  //�����ŵ�����
	Sendchar(0x7A);
	Sendchar(0x00);  //�����ŵ�Ϊ  06 
    delay(6000);
	Sendchar(0xAB);  //���ù���
	Sendchar(0xBA);
	Sendchar(0x0A);  //���ù���Ϊ10DBM
    delay(6000);
	Sendchar(0xA9);  //����ģ��ID��
	Sendchar(0x9A);
	Sendchar(0x00);  //����ģ��ID��
	Sendchar(0x01);  //����ID��
    delay(6000);
    delay(6000);
//    WDT_CONTR=0x3c;
	while(1)
	{
      WDT_CONTR=0x3c;
      if(KEY1==0)
	  {
        delay(10);
		while(!KEY1)			 //����⵽����ʱ���Ͳ������� 
		  {
		  delay(10);
		  }
		  led1=0;

		  print_string("abcdefghijklmnopqrstuvwxyzABCD");	//��һ���������  �� �ַ��������ݳ��Ȳ�Ҫ����30���ֽ� 

		  
		  led1=1;	
		  delay(750);	                                   //������һ������ǰҪ�ӳ�6ms����. 
		  for(i=0;i<23;i++)		     //����23�ֽڵ����ݰ�	       
		    {
		  Sendchar(TxBuf[i]);
		    }
		  delay(750);	                                   //������һ������ǰҪ�ӳ�6ms����. 
		  Sendchar(0x66); 	         //���͵��ֽ� 				  		  		  
	   }
       delay(450);
       led1=1;
       delay(100);

	}
	
}


