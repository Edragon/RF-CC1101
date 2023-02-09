/****************************************
 *              
 *             文件   ：SPI.C
 *  
 *             作者   :   冰点
 *             
 *
 *             日期   ：2014.7.17
 *  
 *         
 ***************************************/
#include<iostm8s103k3.h>
#include"SPI.h"
void SPI_init(void)
{
    PC_DDR_DDR5=1;PC_CR1_C15=1;PC_CR2_C25=1;      //SCK
    PC_DDR_DDR6=1;PC_CR1_C16=1;PC_CR2_C26=1;      //MOSI
    PC_DDR_DDR7=0;PC_CR1_C17=1;PC_CR2_C27=0;      //MISO
 
    SPI_CR1=0x06;
    SPI_CR1|=(7<<3);
    SPI_CR2=0x03;//|=(1<<1)|(1<<0);//使能软件设置从设备 bit0置位做主模式 
    SPI_CR1|=0x40;
    SPI_CR1|=(1<<0);//数据采集从第二个时钟边沿开启
}
u8 SPI1_ReadWriteByte(u8 dat)
{ 
    while(!(SPI_SR&0x02)); //等待发送缓冲区为空
    SPI_DR=dat; //发送数据
    while(!(SPI_SR&0x01));//等待缓冲区有数据
    dat=SPI_DR;//接收数据
    return dat; 
}