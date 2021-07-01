#include "DS18B20.h"
#include <intrins.h>

unsigned char tempL=0; //��ȫ�ֱ���
unsigned char tempH=0; 
unsigned char integer = 0;//���������¶ȵ���������
unsigned char decimal1 = 0;//С����һλ
unsigned char decimal2 = 0;//С���ڶ�λ
unsigned char decimal = 0;//��λС��
unsigned char fg =0;


void delay_1ms(void)
{
	unsigned int i;

	for (i=1; i<(unsigned int)(xtal*142-2); ++i);   //xtalΪ����Ƶ�ʣ���λMHz
}

void delay_ms(unsigned int n)
{
	unsigned int i;

	for (i=0; i<n; ++i)
		delay_1ms();
}

void delay_n_us(unsigned char n)
{
	unsigned char i = 0;
	for (i=0; i<n-1; ++i)
	{
		_nop_();
	}
}
/******************************************************************************/
void Delay_us(unsigned char i)//��ʱ����		 num = 1  18.44 us 4   48us  8 65us	  10  77.04us   100   663us
{
	while(i--);

}	
/******************************************************************************/
void Init_DS18B20(void)//��ʼ��ds1820
{
	unsigned char x=0;

	DQ = 1;    //DQ��λ
	//Delay_us(8);  //������ʱ
	delay_n_us(10);
	DQ = 0;    //��Ƭ����DQ����
	//Delay_us(80); //��ȷ��ʱ ���� 480us
	delay_n_us(480);
	DQ = 1;    //��������
//	Delay_us(8);	  //65us
	delay_n_us(65);

	//x = DQ;      //������ʱ�� ���x=0���ʼ���ɹ� x=1���ʼ��ʧ��
	while (DQ == 0);


	Delay_us(20);
}
/******************************************************************************/
unsigned char ReadOneChar_18B20(void)//��һ���ֽ�
{
	unsigned char i=0;
	unsigned char dat = 0;
	for (i=8;i>0;i--)
	{
		DQ = 0; // �������ź�
		dat>>=1;
		DQ = 1; // �������ź�
		if(DQ)
		dat|=0x80;
		//Delay_us(4);
		delay_n_us(60);
	}
	return(dat);
}
/******************************************************************************/
void WriteOneChar_18B20(unsigned char dat)//дһ���ֽ�
{										//�����ߴӸߵ�ƽ�����͵�ƽ������д��ʼ�źš�15us֮�ڽ�����д��λ�͵���������
	unsigned char i=0;
	for (i=8; i>0; i--)
	{
		DQ = 0;
		DQ = dat&0x01;
		//Delay_us(5);
	    delay_n_us(60);
		DQ = 1;
		dat>>=1;
	}
}
/******************************************************************************/
void Adjust_res_18B20(char res) ///res �ֱ���� 0x1f, 0x3f, 0x5f �¶ȶ����ֱ��ʷֱ��Ӧ
                           //              0.5, 0.25, 0.125   
{
  Init_DS18B20();        //��λ
  WriteOneChar_18B20(0xcc);  //����Rom
 WriteOneChar_18B20(0x4e);  //д�ݴ���
 WriteOneChar_18B20(0x02);  //дTH 
 WriteOneChar_18B20(0x01);  //дTL
  //WriteOneChar_18B20(0x5f);  //д�ṹ�Ĵ���
  WriteOneChar_18B20(res);
  Init_DS18B20();       //��λ
  WriteOneChar_18B20(0xcc); //����Rom 
  WriteOneChar_18B20(0x48); //���ݴ�������д��EPRam��
}

/********************************************************************************/
void ReadTemperature(void)//��ȡ�¶�
{
	//unsigned char a=0;
	//unsigned char b=0;
	//unsigned int t=0;
	//float tt=0;
	Init_DS18B20();
	WriteOneChar_18B20(0xCC); // ����������кŵĲ���
	WriteOneChar_18B20(0x44); // �����¶�ת��
	delay_ms(800);
	Init_DS18B20();
	WriteOneChar_18B20(0xCC); //����������кŵĲ���
	WriteOneChar_18B20(0xBE); //��ȡ�¶ȼĴ���

	tempL=ReadOneChar_18B20(); //�����¶ȵĵ�λLSB
 	tempH=ReadOneChar_18B20(); //�����¶ȵĸ�λMSB 
  
	if(tempH>0x7f)      //���λΪ1ʱ�¶��Ǹ�
	{
		tempL=~tempL;         //����ת����ȡ����һ
		tempH=~tempH+1;       
		fg=0;      //��ȡ�¶�Ϊ��ʱfg=0
    }
	integer = tempL/16+tempH*16;      //��������
	decimal1 = (tempL&0x0f)*10/16; //С����һλ
	decimal2 = (tempL&0x0f)*100/16%10;//С���ڶ�λ
	decimal=decimal1*10+decimal2; //С����λ
	/*
	a=ReadOneChar_18B20();  //����8λ
	b=ReadOneChar_18B20(); //����8λ
	t=b;
	t<<=8;
	t=t|a;
	tt=t*0.0625;
	t= tt*10+0.5; //�Ŵ�10���������������
	return(t);
	*/


}
/******************************************************************************/