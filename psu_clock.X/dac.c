#include "dac.h"


int dacMax[] = {0xfff,0xfff};

void dacInit(void)
{
    //init the SPI bus
    //RA10 = LDAC   / 5
    //RB9  = CS     / 2
    //RB15 = SCK2   / 3
    //RB5  = SDO2  / 4
    
    CS_SET();
    LDAC_SET();
    
    TRISBCLR = (1<<5)|(1<<9)|(1<<15);
    TRISACLR = (1<<10);
    
    ANSELBbits.ANSB15 = 0;
    
    RPB5R = 0b0100; //set RB5 to SDO2
    //RPB9R = 0b0100; //set RB9 to SS
    
    
    
    
    //setup SPI2
    SPI2CONbits.MODE16 = 1;
    SPI2CONbits.DISSDI = 1; //no SDI needed
    SPI2CONbits.MSTEN = 1;
    SPI2CONbits.CKE = 1;
    //SPI2CONbits.MSSEN = 1;
    
    SPI2BRG=0x8; // use FPB/4 clock frequency
    SPI2STATCLR=0x20; // clear the Overflow
   
    
    SPI2CONbits.ON = 1;   
    
    
    dacSet(0,0);
    dacSet(0,1);
    dacLoad();
}


int dacSet(int voltage, int channel)
{
    //adjust for max
    voltage = (voltage*dacMax[channel])>>(12+4);
    //voltage = 0xfff;
    
    uint16_t message = (1<<13)|(1<<12);
    //sets either CHA or CHB
    
    //voltage is vref * VSET / 4096

    //15: !A/B
    //14: dnc
    //13: Gain: 1 = 1x, 0 = 2x
    //12: !SHDN (always 1 for active)
    //11:0: voltage setting
    
    //wait for previous to go out
    if(!channel)
        message |= (1<<15);
    message |= (voltage&0xfff);
    IFS1bits.SPI2TXIF = 0;
    
    
    //wait for previous xaction
    while(SPI2STATbits.SPIBUSY){};
    
    CS_CLR();
    Nop();
    Nop();
    SPI2BUF = message;
    Nop();
    Nop();
    while(SPI2STATbits.SPIBUSY){};
    Nop();
    Nop();
    CS_SET();
    Nop();
    Nop();
}

void dacLoad(void)
{
    //wait for xmission
    Nop();
    Nop();
    LDAC_CLR();
    Nop();
    Nop();
    Nop();
    Nop();
    LDAC_SET();
}

void dacSetMax(int ch1, int ch2)
{
    dacMax[0] = ch1;
    dacMax[1] = ch2;
}