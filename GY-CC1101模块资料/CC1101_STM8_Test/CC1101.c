#include "CC1101.h"
#include "Delay.h"
u8 test;

//-----------------command---------------------------
#define 	WRITE_BURST     	0x40						//����д��
#define 	READ_SINGLE     	0x80						//��
#define 	READ_BURST      	0xC0						//������
#define 	BYTES_IN_RXFIFO     0x7F  						//���ջ���������Ч�ֽ���
#define 	CRC_OK              0x80 						//CRCУ��ͨ��λ��־
/////////////////////////////////////////////////////////////////
const RF_SETTINGS rfSettings = 
{
    0x00,
    0x06,   // FSCTRL1   Frequency synthesizer control.
    0x00,   // FSCTRL0   Frequency synthesizer control.
    0x10,   // FREQ2     Frequency control word, high byte.
    0xA7,   // FREQ1     Frequency control word, middle byte.
    0x62,   // FREQ0     Frequency control word, low byte.
    0xF9,   // MDMCFG4   Modem configuration.
    0xF8,   // MDMCFG3   Modem configuration.
    0x13,   // MDMCFG2   Modem configuration.//0x13->0x03
    0x22,   // MDMCFG1   Modem configuration.
    0xF8,   // MDMCFG0   Modem configuration.

    0x03,   // CHANNR    Channel number.
    0x15,   // DEVIATN   Modem deviation setting (when FSK modulation is enabled).
    0xB6,   // FREND1    Front end RX configuration.
    0x10,   // FREND0    Front end RX configuration.
    0x18,   // MCSM0     Main Radio Control State Machine configuration.
    0x00,   // MCSM1     Main Radio Control State Machine configuration.
    0x07,   // MCSM2      Main Radio Control State Machine configuration.
    0x17,   // FOCCFG    Frequency Offset Compensation Configuration.//0x1d->0x17
    0x1C,   // BSCFG     Bit synchronization Configuration.
    0xC7,   // AGCCTRL2  AGC control.
    0x00,   // AGCCTRL1  AGC control.
    0xB2,   // AGCCTRL0  AGC control.

    0xE9,   // FSCAL3    Frequency synthesizer calibration.
    0x2A,   // FSCAL2    Frequency synthesizer calibration.
    0x00,   // FSCAL1    Frequency synthesizer calibration.
    0x1F,   // FSCAL0    Frequency synthesizer calibration.
    0x59,   // FSTEST    Frequency synthesizer calibration.
    0x81,   // TEST2     Various test settings.
    0x31,   // TEST1     Various test settings.//0x35 ->0x31
    0x09,   // TEST0     Various test settings.
    0x06,   // IOCFG2    GDO2 output pin configuration.
    0x06,   // IOCFG0    GDO0 output pin configuration. Refer to SmartRF?Studio User Manual for detailed pseudo register explanation.

    0x04,   // PKTCTRL1  Packet automation control.
    0x05,   // PKTCTRL0  Packet automation control.
    0x00,   // ADDR      Device address.
    0x30    // PKTLEN    Packet length.
}; 


void halWait(u16 timeout) {
    do {
    Delayms(1);
    } while (--timeout);
}

void CC1101_init(void)
{
		
      
      PC_DDR|=(1<<CSN);    PC_CR1|=(1<<CSN);         
      PC_DDR&=~(1<<GDO0);  PC_CR1|=(1<<GDO0);     	
  	   
      CSN_H; 
      halWait(1); 
      CSN_L; 
      halWait(1); 
      CSN_H; 
      halWait(41); 

      CSN_L; 
      Delayus(5);
      SPI1_ReadWriteByte(CCxxx0_SRES); 		//д�븴λ����
      Delayus(5);
      CSN_H; 
      Delayms(5);
      halRfWriteRfSettings();
}


//*****************************************************************************************
//��������void halSpiWriteReg(INT8U addr, INT8U value)
//���룺��ַ��������
//�������
//����������SPIд�Ĵ���
//*****************************************************************************************
void halSpiWriteReg(u8 addr, u8 value) 
{
    CSN_L;
    Delayus(5);
    SPI1_ReadWriteByte(addr);		//д��ַ
    Delayus(5);
    SPI1_ReadWriteByte(value);		//д������
    CSN_H;
    Delayus(5);
}


//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//������üĴ�����������
//����������SPI���Ĵ���
//*****************************************************************************************
u8 halSpiReadReg(u8 addr) 
{
    u8 temp, value;
    temp = addr|READ_SINGLE;//���Ĵ�������
    CSN_L;
    SPI1_ReadWriteByte(temp); 
    value = SPI1_ReadWriteByte(0);
    CSN_H;
    return value;
}

//*****************************************************************************************
//��������void halSpiStrobe(INT8U strobe)
//���룺����
//�������
//����������SPIд����
//*****************************************************************************************
void halSpiStrobe(u8 strobe) 
{
    CSN_L;
    SPI1_ReadWriteByte(strobe);		//д������
    CSN_H;
	
}

//*****************************************************************************************
//��������void halRfWriteRfSettings(RF_SETTINGS *pRfSettings)
//���룺��
//�������
//��������������CC1100�ļĴ���
//*****************************************************************************************
void halRfWriteRfSettings(void) 
{

	  //halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL2);
    // Write register settings
    halSpiWriteReg(CCxxx0_FSCTRL1,  rfSettings.FSCTRL1);
    halSpiWriteReg(CCxxx0_FSCTRL0,  rfSettings.FSCTRL0);
    halSpiWriteReg(CCxxx0_FREQ2,    rfSettings.FREQ2);
    halSpiWriteReg(CCxxx0_FREQ1,    rfSettings.FREQ1);
    halSpiWriteReg(CCxxx0_FREQ0,    rfSettings.FREQ0);
    halSpiWriteReg(CCxxx0_MDMCFG4,  rfSettings.MDMCFG4);
    halSpiWriteReg(CCxxx0_MDMCFG3,  rfSettings.MDMCFG3);
    halSpiWriteReg(CCxxx0_MDMCFG2,  rfSettings.MDMCFG2);
    halSpiWriteReg(CCxxx0_MDMCFG1,  rfSettings.MDMCFG1);
    halSpiWriteReg(CCxxx0_MDMCFG0,  rfSettings.MDMCFG0);
    halSpiWriteReg(CCxxx0_CHANNR,   rfSettings.CHANNR);
    halSpiWriteReg(CCxxx0_DEVIATN,  rfSettings.DEVIATN);
    halSpiWriteReg(CCxxx0_FREND1,   rfSettings.FREND1);
    halSpiWriteReg(CCxxx0_FREND0,   rfSettings.FREND0);
    halSpiWriteReg(CCxxx0_MCSM0 ,   rfSettings.MCSM0 );
    halSpiWriteReg(CCxxx0_MCSM1 ,   rfSettings.MCSM1 );
    halSpiWriteReg(CCxxx0_MCSM2 ,   rfSettings.MCSM2 );
    halSpiWriteReg(CCxxx0_FOCCFG,   rfSettings.FOCCFG);
    halSpiWriteReg(CCxxx0_BSCFG,    rfSettings.BSCFG);//halSpiWriteReg(CCxxx0_AGCCTRL2, rfSettings.AGCCTRL2);//halSpiWriteReg(CCxxx0_AGCCTRL1, rfSettings.AGCCTRL1);
    halSpiWriteReg(CCxxx0_AGCCTRL0, rfSettings.AGCCTRL0);
    halSpiWriteReg(CCxxx0_FSCAL3,   rfSettings.FSCAL3);
    halSpiWriteReg(CCxxx0_FSCAL2,   rfSettings.FSCAL2);
    halSpiWriteReg(CCxxx0_FSCAL1,   rfSettings.FSCAL1);
    halSpiWriteReg(CCxxx0_FSCAL0,   rfSettings.FSCAL0);//halSpiWriteReg(CCxxx0_FSTEST,   rfSettings.FSTEST);
    halSpiWriteReg(CCxxx0_TEST2,    rfSettings.TEST2);
    halSpiWriteReg(CCxxx0_TEST1,    rfSettings.TEST1);
    halSpiWriteReg(CCxxx0_TEST0,    rfSettings.TEST0);
    halSpiWriteReg(CCxxx0_IOCFG2,   rfSettings.IOCFG2);
    halSpiWriteReg(CCxxx0_IOCFG0,   rfSettings.IOCFG0);    
    halSpiWriteReg(CCxxx0_PKTCTRL1, rfSettings.PKTCTRL1);
    halSpiWriteReg(CCxxx0_PKTCTRL0, rfSettings.PKTCTRL0);
    halSpiWriteReg(CCxxx0_ADDR,     rfSettings.ADDR);
    halSpiWriteReg(CCxxx0_PKTLEN,   rfSettings.PKTLEN);
    test=halSpiReadReg(CCxxx0_TEST0);
    halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
    halSpiStrobe(CCxxx0_SRX);		//�������״̬
}


//*****************************************************************************************
//��������INT8U halSpiReadReg(INT8U addr)
//���룺��ַ
//�������״̬�Ĵ�����ǰֵ
//����������SPI��״̬�Ĵ���
//*****************************************************************************************
u8 halSpiReadStatus(u8 addr) 
{
    u8 value,temp;
    temp = addr | READ_BURST;		//д��Ҫ����״̬�Ĵ����ĵ�ַͬʱд�������
    CSN_L;
    SPI1_ReadWriteByte(temp);
    value = SPI1_ReadWriteByte(0);
    CSN_H;
    return value;
	
}

//*****************************************************************************************
//��������void halSpiWriteBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ��д�뻺������д�����
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiWriteBurstReg(u8 addr, u8 *buffer, u8 count) 
{
    u8 i, temp;
    temp = addr | WRITE_BURST;
    CSN_L;
    SPI1_ReadWriteByte(temp);
    for (i = 0; i < count; i++)
    {
        SPI1_ReadWriteByte(buffer[i]);
		  
    }
    CSN_H;
}


//*****************************************************************************************
//��������void halSpiReadBurstReg(INT8U addr, INT8U *buffer, INT8U count)
//���룺��ַ���������ݺ��ݴ�Ļ��������������ø���
//�������
//����������SPI����д���üĴ���
//*****************************************************************************************
void halSpiReadBurstReg(u8 addr, u8 *buffer, u8 count) 
{
    u8 i,temp;
	  temp = addr | READ_BURST;		//д��Ҫ�������üĴ�����ַ�Ͷ�����
    CSN_L;
    SPI1_ReadWriteByte(temp);
    for (i = 0; i < count; i++) 
    {
        buffer[i] = SPI1_ReadWriteByte(0);
			 // Delayus(20);
    }
    CSN_H;
}

//*****************************************************************************************
//��������void halRfSendPacket(INT8U *txBuffer, INT8U size)
//���룺���͵Ļ��������������ݸ���
//�������
//����������CC1100����һ������
//*****************************************************************************************

void halRfSendPacket(u8 *txBuffer, u8 size) 
{
    halSpiWriteReg(CCxxx0_TXFIFO, size);
    halSpiWriteBurstReg(CCxxx0_TXFIFO, txBuffer, size);	//д��Ҫ���͵�����
    halSpiStrobe(CCxxx0_STX);		//���뷢��ģʽ��������
    // Wait for GDO0 to be set -> sync transmitted
    while (!GDO0_IDR);
    // Wait for GDO0 to be cleared -> end of packet
    while (GDO0_IDR);
    halSpiStrobe(CCxxx0_SFTX);
    halSpiStrobe(CCxxx0_SFRX);
    halSpiStrobe(CCxxx0_SRX);		//�������״̬
    Delayms(1);
	   
}

void setRxMode(void)
{
    halSpiStrobe(CCxxx0_SRX);		//�������״̬
}

//---------------------------------------------------------------------------
u8 halRfReceivePacket(u8 *rxBuffer, u8 *length) 
{
    u8 status[2];
    u8 packetLength;
    u16 i=5;  // �������Ҫ����datarate��length������
   while (GDO0_IDR)
   {
	Delayus(10);
	--i;
	if(i<1)
	return 0; 	    
   }	 
	 
	 
    if ((halSpiReadStatus(CCxxx0_RXBYTES) & BYTES_IN_RXFIFO)) //����ӵ��ֽ�����Ϊ0
	{
            packetLength = halSpiReadReg(CCxxx0_RXFIFO);//������һ���ֽڣ����ֽ�Ϊ��֡���ݳ���
        
            halSpiReadBurstReg(CCxxx0_RXFIFO, rxBuffer, packetLength); //�������н��յ�������
            *length = packetLength;				//�ѽ������ݳ��ȵ��޸�Ϊ��ǰ���ݵĳ���
        
            // Read the 2 appended status bytes (status[0] = RSSI, status[1] = LQI)
            halSpiReadBurstReg(CCxxx0_RXFIFO, status, 2); 	//����CRCУ��λ
						halSpiStrobe(CCxxx0_SFRX);		//��ϴ���ջ�����
			      halSpiStrobe(CCxxx0_SRX);		//�������״̬
		        Delayms(1);
            return (status[1] & CRC_OK);			//���У��ɹ����ؽ��ճɹ�
     
  } 
	 
 	return 0;
}



