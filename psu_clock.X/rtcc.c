#include "rtcc.h"



void rtccInit(void)
{
    //_CP0_BIS_STATUS(0x00001800); // Set CPU to priority level 6 (only ISR at level 7 will be served)
    SYSKEY = 0xaa996655; // write first unlock key to SYSKEY
    SYSKEY = 0x556699aa; // write second unlock key to SYSKEY
    RTCCONSET = 0x8; // set RTCWREN in RTCCONSET
    //_CP0_BIS_STATUS(0x00001C00); // Set CPU to priority level 0 (all will be served)
    
    
    
    //          ON  |   SOSC  |CLKON | RW_EN
    RTCCON = (1<<15)|((01)<<9)|(1<<6)|(1<<3);
    
    //          ON   |   10 sec
    RTCALRM = (1<<15)|((0b0010)<<8);
    
        /*
    The following code example illustrates an RTCC initialization with interrupts enabled.
    When the RTCC alarm interrupt is generated, the cpu will jump to the vector assigned to
    RTCC interrupt.
    */
    /*// assume RTCC write is enabled i.e. RTCWREN (RTCCON<3>) =1;*/
    IEC1CLR=0x00008000; // disable RTCC interrupts
    RTCCONCLR=0x8000; // turn off the RTCC
    while(RTCCON&0x40); // wait for clock to be turned off
    IFS1CLR=0x00008000; // clear RTCC existing event
    IPC8CLR=0x1f000000; // clear the priority
    IPC8SET=0x0d000000; // Set IPL=3, subpriority 1
    IEC1SET=0x00008000; // Enable RTCC interrupts
    
//    RTCTIME=0x16153300; // safe to update time to 16 hr, 15 min, 33 sec
//    RTCDATE=0x06102705; // update the date to Friday 27 Oct 2006
//    ALRMTIME=0x16154300; // set alarm time to 16 hr, 15 min, 43 sec
//    ALRMDATE=0x06102705; // set alarm date to Friday 27 Oct 2006
    
    
    //          ON   |   10 sec
    RTCALRM = (1<<15)|((0b0010)<<8);
    //          ON  |   SOSC  |CLKON | RW_EN
    RTCCON = (1<<15)|((01)<<9)|(1<<6)|(1<<3);
    while(!(RTCCON&0x40)); // wait for clock to be turned on   
}


void __ISR(_RTCC_VECTOR, IPL3AUTO) __RTCCInterrupt(void)
{
    // ... perform application specific operations
    // in response to the interrupt
    IFS1CLR=0x00008000; // be sure to clear RTCC interrupt flag
    // before exiting the service routine.
    
    
    //LATCINV = 0xff;
}
int rtccSec()
{
    return ((RTCTIMEbits.SEC01) + (RTCTIMEbits.SEC10*10));
}
int rtccMin()
{
    return ((RTCTIMEbits.MIN01) + (RTCTIMEbits.MIN10*10));
}
int rtccHr()
{
    return ((RTCTIMEbits.HR01) + (RTCTIMEbits.HR10*10));
}
int rtccSetSec(int seconds)
{
    RTCTIMEbits.SEC01 = seconds%10;
    RTCTIMEbits.SEC10 = seconds/10;
}
int rtccSetMin(int minutes)
{
    if(minutes>59) minutes = 59;
    else if(minutes<0) minutes = 0;
    RTCTIMEbits.MIN01 = minutes%10;
    RTCTIMEbits.MIN10 = minutes/10;
}
int rtccSetHr(int hours)
{        
    if(hours>11) hours = 11;
    else if(hours<0) hours = 0;
    RTCTIMEbits.HR01 = hours%10;
    RTCTIMEbits.HR10 = hours/10;
}