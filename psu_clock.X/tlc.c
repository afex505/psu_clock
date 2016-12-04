
#include <proc/p32mx430f064h.h>
#include "tlc.h"


unsigned short values[16];
int spiMessage[6];

int waiting = 0;

void tlcInit(void)
{
    // BLANK    RD8 (OC1)       / IO2
    // GSCLK    RD0 ((OC2)))/ BOOSTPWM
    // SDO      RB8 (SDO1)      / IN13
    // SCK      RD2 (SCK1))     / BOOSTCLK
    // XLAT     RD9             / MGCSDA
    // VPRG     n/c (float = gnd))
    // XERR     RB7             / IN12
    
    // BLANK: Pop high to start counting
    // GSCLK: straight clock
    // XLAT: pop high to latch in data (after xmission)
    // VPRG: low = GS, high = DC
    
    
    //setup our clocks
    
    //GSCLK, fast (1Mhz?) OC2
    TRISDbits.TRISD0 = 0;
    
    RPD0R = 0b1011;   //OC2 muxed
    OC2CON = 0x0000; // Turn off the OC2 when performing the setup
    OC2R = 0x0004; // Initialize primary Compare register
    OC2RS = 0x0004; // Initialize secondary Compare register
    OC2CON = 0b0110; // PWM, no fault, TMR2

    // enabled
    PR2 = (0x0008)-1; // Set period
    // Configure Timer2
    T2CONSET = 0x8000; // Enable Timer2
    OC2CONSET = 0x8000; // Enable OC2
    
    
    //BLANK clock, timer is TMR2*4096
    TRISDbits.TRISD8 = 0;
    RPD8R = 0b1100; //OC1 muxed
    
    OC1CON = 0x0000; // Turn off the OC2 when performing the setup
    OC1R =  0x0002; // Initialize primary Compare register
    OC1RS = 0x0002; // Initialize secondary Compare register
    OC1CON = 0b1110; // PWM, no fault, TMR3

    // enabled
    PR3 = (0x0008*1024)-1; // Set period
    T3CONbits.TCKPS = 0b010;  // 1:4 prescaler
    // Configure Timer2
    T3CONSET = 0x8000; // Enable Timer2
    OC1CONSET = 0x8000; // Enable OC2
    
    
    //setup SPI
    // SDO      RB8 (SDO1)
    // SCK      RD2 (SCK1))
    TRISBCLR = (1<<8);
    TRISDCLR = (1<<2);
    ANSELDCLR = (1<<2);
    ANSELBCLR = (1<<8);
    RPB8R = 0b1000; //set RB8 to SDO1

    //setup SP1
    SPI1CONbits.MODE32 = 1;
    SPI1CONbits.DISSDI = 1; //no SDI needed
    SPI1CONbits.MSTEN = 1;
    SPI1CONbits.ENHBUF = 1;
    SPI1CONbits.CKE = 1;

    SPI1BRG=0x40; // use FPB/4 clock frequency
    SPI1STATCLR=0x20; // clear the Overflow
   
    //setup Interrupt to fire when empty, but DO NOT ENABLE (we do this when we write))
    IEC1bits.SPI1TXIE = 0;
    IEC1bits.SPI1RXIE = 0;
    IFS1bits.SPI1TXIF = 0;
    IPC7bits.SPI1IP = 1;
    
    SPI1CONbits.ON = 1;      
    
    //setup XLAT
    TRISDbits.TRISD9 = 0;
    XLAT_UP();
    
    //setup XERR
    TRISBbits.TRISB7 = 1;
    CNPUBbits.CNPUB7 = 1;
    
    
    tlcClr();
    tlcUpdate();
}

void tlcSetChannel(int channel, int value)
{
#if 0
    const int lookup[] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01,
0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x01, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02, 0x02,
0x02, 0x02, 0x03, 0x03, 0x03, 0x03, 0x03, 0x03, 0x04, 0x04, 0x04, 0x04, 0x04, 0x05, 0x05, 0x05,
0x05, 0x06, 0x06, 0x06, 0x07, 0x07, 0x07, 0x08, 0x08, 0x08, 0x09, 0x09, 0x0A, 0x0A, 0x0B, 0x0B,
0x0C, 0x0C, 0x0D, 0x0D, 0x0E, 0x0F, 0x0F, 0x10, 0x11, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1F, 0x20, 0x21, 0x23, 0x24, 0x26, 0x27, 0x29, 0x2B, 0x2C,
0x2E, 0x30, 0x32, 0x34, 0x36, 0x38, 0x3A, 0x3C, 0x3E, 0x40, 0x43, 0x45, 0x47, 0x4A, 0x4C, 0x4F,
0x51, 0x54, 0x57, 0x59, 0x5C, 0x5F, 0x62, 0x64, 0x67, 0x6A, 0x6D, 0x70, 0x73, 0x76, 0x79, 0x7C,
0x7F, 0x82, 0x85, 0x88, 0x8B, 0x8E, 0x91, 0x94, 0x97, 0x9A, 0x9C, 0x9F, 0xA2, 0xA5, 0xA7, 0xAA,
0xAD, 0xAF, 0xB2, 0xB4, 0xB7, 0xB9, 0xBB, 0xBE, 0xC0, 0xC2, 0xC4, 0xC6, 0xC8, 0xCA, 0xCC, 0xCE,
0xD0, 0xD2, 0xD3, 0xD5, 0xD7, 0xD8, 0xDA, 0xDB, 0xDD, 0xDE, 0xDF, 0xE1, 0xE2, 0xE3, 0xE4, 0xE5,
0xE6, 0xE7, 0xE8, 0xE9, 0xEA, 0xEB, 0xEC, 0xED, 0xED, 0xEE, 0xEF, 0xEF, 0xF0, 0xF1, 0xF1, 0xF2,
0xF2, 0xF3, 0xF3, 0xF4, 0xF4, 0xF5, 0xF5, 0xF6, 0xF6, 0xF6, 0xF7, 0xF7, 0xF7, 0xF8, 0xF8, 0xF8,
0xF9, 0xF9, 0xF9, 0xF9, 0xFA, 0xFA, 0xFA, 0xFA, 0xFA, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFB, 0xFC,
0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFC, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD,
0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFD, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFF, 0xFF};
    
    
    if(value>0xfff)
        value = 0xfff;
    
    if(value < 0)
        value = 0;
    
    values[channel] = lookup[value>>4]<<4;
    
#else
    values[channel] = value&0xfff;
#endif
}

void tlcAssemble(int changeFlags)
{
    //only assembe the messages that would have changed
    if(changeFlags&0b1110000000000000) spiMessage[0] = (values[13]>>4)+(values[14]<<8)+(values[15]<<20);
    if(changeFlags&0b0011110000000000) spiMessage[1] = (values[10]>>8)+(values[11]<<4)+(values[12]<<16)+(values[13]<<28);
    if(changeFlags&0b0000011100000000) spiMessage[2] = values[8]+(values[9]<<12)+(values[10]<<24);
    if(changeFlags&0b0000000011100000) spiMessage[3] = (values[5]>>4)+(values[6]<<8)+(values[7]<<20);
    if(changeFlags&0b0000000000111100) spiMessage[4] = (values[2]>>8)+(values[3]<<4)+(values[4]<<16)+(values[5]<<28);
    if(changeFlags&0b0000000000000111) spiMessage[5] = values[0]+(values[1]<<12)+(values[2]<<24);
}

void tlcClr(void)
{
    int i;

    for(i = 0; i < 6; i++) {
        spiMessage[i] = 0;
    }
    for(i = 0; i < 16; i++){
        values[i] = 0;
    }
}

void tlcFull(void)
{
    int i;

    for(i = 0; i < 6; i++) {
        spiMessage[i] = 0xffffffff;
    }
    for(i = 0; i < 16; i++){
        values[i] = 0xfff;
    }
}

void tlcUpdate(void)
{
    //make sure previous transition completed
    while(waiting){};
    //load up buffer!

    //drop XLAT
    XLAT_DOWN();
    Nop();
    Nop();
    SPI1BUF = spiMessage[0]; 
    SPI1BUF = spiMessage[1];
    SPI1BUF = spiMessage[2];
    SPI1BUF = spiMessage[3];
    //    //the FIFO is only 4 deep, so we need to make sure we wait for room in the buffer for these last two
    while(SPI1STAT & SPI1STAT_SPITBF){};
    SPI1BUF = spiMessage[4]; 
    while(SPI1STAT & SPI1STAT_SPITBF){};
    SPI1BUF = spiMessage[5]; 
    
//    SPI1BUF = 0xfffeeedd;
//    SPI1BUF = 0xdcccbbba;
//    SPI1BUF = 0xaa999888;
//    SPI1BUF = 0x77766655;
//    //the FIFO is only 4 deep, so we need to make sure we wait for room in the buffer for these last two
//    while(SPI1STAT & SPI1STAT_SPITBF){};
//    SPI1BUF = 0x54443332;
//    while(SPI1STAT & SPI1STAT_SPITBF){};
//    SPI1BUF = 0x22111000;
    
    IFS1CLR = IFS1_SPI1TX_IF;
    IEC1SET = IEC1_SPI1TX_IE;
    waiting = 1;
}

void __ISR(_SPI_1_VECTOR,IPL1AUTO) _SPI1Interrupt()
{
    //disable the interrupt
    IEC1CLR = IEC1_SPI1TX_IE;
    //pop XLAT
    Nop();
    Nop();
    Nop();
    XLAT_UP();
    Nop();
    Nop();
    Nop();
    IFS1CLR = IFS1_SPI1TX_IF;
    Nop();
    Nop();
    Nop();
    waiting = 0;
}

