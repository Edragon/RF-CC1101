/****************************************************************************
 **             - lcd_12864s_driver.C -
 **
 **    lcd_12864s_driver驱动程序的匹配C文档
 **
 **     
 **    移植到STC单片机（理论上可以用于51核的单片机）
 **    by misswhile @ hud	email: misswhile@hotmail.com
 **    File version: $2009.3.13$
 **
 ***************************************************************************/
#include "lcd_12864s_driver.h"


#define LCD_12864S_IO_INITIALIZTION()  \
LCD_12864s_CS1 = 1;\
LCD_12864s_RES = 0;\
_nop_();\
_nop_();\
_nop_();\
_nop_();\
LCD_12864s_RES = 1;\
_nop_();\
_nop_();\
_nop_();\
_nop_()

/*******************宏函数**************************/
#define LCD_128164s_READ_DB() \
(LCD_12864s_DB  = 0xff)

#define LCD_12864s_WRITE_DB() \
(LCD_12864s_DB  = 0x00)

#ifdef ENABLE_LCD_12864s_BUSY
#define LCD_12864s_BUSY() \
LCD_128164s_READ_DB();\
LCD_12864s_DB  = 0xff;\
LCD_12864s_A0P = 0;\
LCD_12864s_WR  = 1;\
LCD_12864s_RD  = 0;\
LCD_12864s_CS1 = 0;\
while(LCD_12864s_DB_PIN>>7);\
(LCD_12864s_CS1 = 1)
#endif 


#define LCD_12864s_COMMEND_PROCESS(com_type,format,com) ((com_type&(~format))|(com&format))

#define LCD_12864s_DISPLAY_ON_OFF(factor) LCD_12864s_COMMEND_PROCESS(0xae,0x01,factor)

#define LCD_12864s_START_LINE(factor) LCD_12864s_COMMEND_PROCESS(0x40,0x3f,factor)

#define LCD_12864s_PAGE_ADDRESS(factor) LCD_12864s_COMMEND_PROCESS(0xb0,0x0f,factor)

#define LCD_12864s_COLUMN_ADDRESS_H(factor)  LCD_12864s_COMMEND_PROCESS(0x10,0x0f,factor)
#define LCD_12864s_COLUMN_ADDRESS_L(factor)  LCD_12864s_COMMEND_PROCESS(0x00,0x0f,factor)

#define LCD_12864s_ADC_SELECT(factor) LCD_12864s_COMMEND_PROCESS(0xa0,0x01,factor)

#define LCD_12864s_DISPLAY_NORMAL_REVERSE(factor) LCD_12864s_COMMEND_PROCESS(0xa6,0x01,factor)

#define LCD_12864s_BIAS_SET(factor) LCD_12864s_COMMEND_PROCESS(0xa2,0x01,factor)

#define LCD_12864s_RESET(factor) LCD_12864s_COMMEND_PROCESS(0xe2,0x00,factor)

#define LCD_12864s_COMMON_OUTPUT_MODE(factor) LCD_12864s_COMMEND_PROCESS(0xc0,0x0f,factor)

#define LCD_12864s_POWER_CONTROLLER(factor)  LCD_12864s_COMMEND_PROCESS(0x28,0x07,factor)

#define LCD_12864s_V5_REGULATOR(factor)  LCD_12864s_COMMEND_PROCESS(0x20,0x07,factor)

#define LCD_12864s_ELECTRONIC_VOLUME_H(factor)  LCD_12864s_COMMEND_PROCESS(0x81,0x00,factor)
#define LCD_12864s_ELECTRONIC_VOLUME_L(factor)  LCD_12864s_COMMEND_PROCESS(0x00,0x3f,factor)


/******************************************基本函数********************************************/


void Lcd12864sWdat(unsigned char dat)
{
#ifdef ENABLE_LCD_12864s_BUSY
  LCD_12864s_BUSY();
#endif
  LCD_12864s_WRITE_DB();
  LCD_12864s_A0P = 1;
  LCD_12864s_WR  = 0;
  LCD_12864s_RD  = 1;
  LCD_12864s_CS1 = 0;
  LCD_12864s_DB = dat;
  LCD_12864s_WR  = 1;
  LCD_12864s_WR  = 1;
  LCD_12864s_CS1 = 1;
}


void Lcd12864sWcom(unsigned char com)
{
#ifdef ENABLE_LCD_12864s_BUSY
  LCD_12864s_BUSY();
#endif
  LCD_12864s_WRITE_DB();
  LCD_12864s_A0P = 0;
  LCD_12864s_WR  = 0;
  LCD_12864s_RD  = 1;
  LCD_12864s_CS1 = 0;
  LCD_12864s_DB = com;
  LCD_12864s_WR  = 1;
  LCD_12864s_WR  = 1;
  LCD_12864s_CS1 = 1;
}

/****************************************扩展函数*******************************************/
/*初始化函数*/
void Lcd12864sInitializtion()
{
  LCD_12864S_IO_INITIALIZTION();
  Lcd12864sWcom(LCD_12864s_BIAS_SET(0x01));
  Lcd12864sWcom(LCD_12864s_ADC_SELECT(0x00));
  Lcd12864sWcom(LCD_12864s_COMMON_OUTPUT_MODE(0x00));
  Lcd12864sWcom(LCD_12864s_V5_REGULATOR(0x02));
  Lcd12864sWcom(LCD_12864s_ELECTRONIC_VOLUME_H(0x00));
  Lcd12864sWcom(LCD_12864s_ELECTRONIC_VOLUME_L(0x13));
  Lcd12864sWcom(LCD_12864s_POWER_CONTROLLER(0x07));
  Lcd12864sWcom(LCD_12864s_ADC_SELECT(0x01));
  Lcd12864sWcom(LCD_12864s_START_LINE(0x00));
  Lcd12864sWcom(LCD_12864s_DISPLAY_ON_OFF(0x01));
}


void Lcd12864sLocate(unsigned char page,unsigned char column)
{
  Lcd12864sWcom(LCD_12864s_PAGE_ADDRESS(page));
  Lcd12864sWcom(LCD_12864s_COLUMN_ADDRESS_H((column+4)>>4));
  Lcd12864sWcom(LCD_12864s_COLUMN_ADDRESS_L(column+4));
}

void Lcd12864sCls()
{
  unsigned char temp1,temp2;
  for(temp1=0;temp1<8;temp1++)
  {
    Lcd12864sLocate(temp1,0x00);
    for(temp2=0;temp2<128;temp2++)
    {
      Lcd12864sWdat(0x00);
    }
    Lcd12864sWdat(0x00);
  }
}

/****************************************常用函数*******************************************/

void Lcd12864sAscPrintf(unsigned char page,unsigned char column,unsigned char const *p)
{ 
  unsigned char temp;
  
  Lcd12864sLocate(page,column);
  for (temp=0;temp<8;temp++)
  {
    Lcd12864sWdat(p[temp]);
  }
  Lcd12864sLocate(page+1,column);
  for (temp=8;temp<16;temp++)
  {
    Lcd12864sWdat(p[temp]);
  }
}



void Lcd12864sBmpPrintf(unsigned char page,unsigned char column,unsigned char const *p)
{
  unsigned char width,height;
  unsigned int temp = 0x02;
  
  for(height=0;height< p[1]/8+((p[1]%8!=0x00)&0x01);height++)
  {
    Lcd12864sLocate(page+height,column);
    for(width=0;width<p[0];width++)
    {
      Lcd12864sWdat(p[temp]);
      temp++;
    }
  } 
}

