#include "Delay.h"

/*----------------------*
 *      ������ʱ����     *
 *----------------------*/	
void Delayms(u32 ms) 
{ 
    //uint32_t temp; 
    u32  i, j; 
  
    for( i = ms; i > 0; i-- ) 
    { 
        for( j = 120 ; j > 0 ; j-- ) 
        {} 
    } 
}
/*----------------------*
 *      ΢����ʱ����     *
 *----------------------*/
void Delayus(u32 us) 
{ 
    //uint32_t temp; 
    u32  i, j; 

    for( i = us; i > 0; i-- ) 
    { 
        for( j = 7 ; j > 0 ; j-- ) 
        {} 
    } 
} 
