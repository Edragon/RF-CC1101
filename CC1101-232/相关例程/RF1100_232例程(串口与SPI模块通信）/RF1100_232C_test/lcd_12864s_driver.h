/****************************************************************************
 **             - lcd_12864s_driver.h -
 **
 **      LCD驱动程序的匹配c文档
 **
 **      杭州威步科技
 **      http://www.newmsg.com
 **   
 **      移植到STC单片机（理论上可以用于51核的单片机）
 **    	  **    by misswhile @ hud	email: misswhile@hotmail.com
 **      File version: $2009.3.13$
 **
 ***************************************************************************/
#ifndef lcd_12864s_driver_h
#define lcd_12864s_driver_h

#include "IO_def.h"


/*******************************函数定义**********************************/
void Lcd12864sWdat(unsigned char dat);
void Lcd12864sWcom(unsigned char com);
void Lcd12864sInitializtion(void);
void Lcd12864sLocate(unsigned char page,unsigned char column);
void Lcd12864sCls(void);
void Lcd12864sAscPrintf(unsigned char page,unsigned char column,unsigned char const *p);
void Lcd12864sBmpPrintf(unsigned char page,unsigned char column,unsigned char const *p);


#endif  //lcd_12864s_driver_h




















