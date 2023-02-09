#include "SPI.h"
#include "CC1101.h"
#include "Delay.h"
u8 Wireless_Command_TxBuf[32]={0x3c,0x72,0x35,0x0d,0x0a,0x44,0x61,0x03,0x1f,0x76,0xa9,0x3e};
//u8 Device_ID[6]={0x44,0x};
u8 Wireless_RxBuf[32];
int main( void )
{ 
   u8 lenth;
   SPI_init();
   CC1101_init();
   while(1)
   {
     halRfSendPacket(Wireless_Command_TxBuf,12);
     Delayms(10);
     if(halRfReceivePacket(Wireless_RxBuf,&lenth))
     {
        
     }
   }
}
