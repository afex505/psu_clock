/* 
 * File:   main.c
 * Author: C15020
 *
 * Created on March 25, 2015, 7:39 AM
 */
#include "main.h"

#if defined(__32MX430F064H__)
// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (WDT Enabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)
#pragma config DEBUG = ON
#else

// DEVCFG3
// USERID = No Setting
#pragma config PMDL1WAY = OFF           // Peripheral Module Disable Configuration (Allow multiple reconfigurations)
#pragma config IOL1WAY = OFF            // Peripheral Pin Select Configuration (Allow multiple reconfigurations)
#pragma config FUSBIDIO = ON            // USB USID Selection (Controlled by the USB Module)
#pragma config FVBUSONIO = ON           // USB VBUS ON Selection (Controlled by USB Module)

// DEVCFG2
#pragma config FPLLIDIV = DIV_2         // PLL Input Divider (2x Divider)
#pragma config FPLLMUL = MUL_20         // PLL Multiplier (20x Multiplier)
#pragma config UPLLIDIV = DIV_2         // USB PLL Input Divider (2x Divider)
#pragma config UPLLEN = OFF             // USB PLL Enable (Disabled and Bypassed)
#pragma config FPLLODIV = DIV_2         // System PLL Output Clock Divider (PLL Divide by 2)

// DEVCFG1
#pragma config FNOSC = PRIPLL           // Oscillator Selection Bits (Primary Osc w/PLL (XT+,HS+,EC+PLL))
#pragma config FSOSCEN = ON             // Secondary Oscillator Enable (Enabled)
#pragma config IESO = OFF               // Internal/External Switch Over (Disabled)
#pragma config POSCMOD = XT             // Primary Oscillator Configuration (XT osc mode)
#pragma config OSCIOFNC = OFF           // CLKO Output Signal Active on the OSCO Pin (Disabled)
#pragma config FPBDIV = DIV_1//DIV_1           // Peripheral Clock Divisor (Pb_Clk is Sys_Clk/1)
#pragma config FCKSM = CSDCMD           // Clock Switching and Monitor Selection (Clock Switch Disable, FSCM Disabled)
#pragma config WDTPS = PS1048576        // Watchdog Timer Postscaler (1:1048576)
#pragma config WINDIS = OFF             // Watchdog Timer Window Enable (Watchdog Timer is in Non-Window Mode)
#pragma config FWDTEN = ON              // Watchdog Timer Enable (WDT Enabled)
#pragma config FWDTWINSZ = WINSZ_25     // Watchdog Timer Window Size (Window Size is 25%)

// DEVCFG0
#pragma config JTAGEN = OFF              // JTAG Enable (JTAG Port Enabled)
#pragma config ICESEL = ICS_PGx1        // ICE/ICD Comm Channel Select (Communicate on PGEC1/PGED1)
#pragma config PWP = OFF                // Program Flash Write Protect (Disable)
#pragma config BWP = OFF                // Boot Flash Write Protect bit (Protection Disabled)
#pragma config CP = OFF                 // Code Protect (Protection Disabled)

#endif

int gTimer;
int clkPulse;
int clkPulseFlag;
int bufferTimer;


enum state curr_state = sm_init;
enum state next_state = sm_undefined;
/*
 * 
 */
int main(int argc, char** argv) {


    SYSTEMConfigPerformance(40000000);
    INTEnableSystemMultiVectoredInt();

    uint8_t switchEvents=0;
    int i=0;
    int j=0;
    
    init();
    
    UART_print_str("||Reset!||\r\n");
    LEDTRISCLR = LEDMASK;
    for(;;) {
        WDTCONCLR = 0x8000;
        switchLatch();
        
        clkPulseFlag = clkPulse;
            

       
        if(clkPulse) {
            clkPulse = 0;
//            LATC = (gTimer<<3) & 0b01111000;    
//            UART_print_str("Knobs: ");
//            UART_print_hex(16,getKnob(0));
//            UART_print_str("/");
//            UART_print_hex(16,getKnob(1));
//            UART_print_str("\t");
//            UART_print_str("\r\n");
        }
        
        
        //Main state machine
        next_state = sm_undefined;
        //UART_putc((char)((int)curr_state+0x61));
        switch(curr_state) {
            case sm_undefined:
                UART_print_str("UNDEFINED STATE!\r\n");
                next_state = sm_init;
                break;
            case sm_init:
                UART_print_str("Hello, i'm clocky!\r\n");
                next_state = sm_trans_to_clk;
                break;
            case sm_trans_to_cal:
                UART_print_str("sm_trans_to_cal\r\n");
                taskCalInit();
            
                next_state = sm_cal;
                break;
            case sm_cal:
                taskCal();

                if(switchGet(1)) {
                    next_state = sm_leaving_cal;
                } else {
                    next_state = sm_cal;
                }
                break;
            case sm_trans_to_clk:
                taskClockInit();
                UART_print_str("sm_trans_to_clk\r\n");
                next_state = sm_clk;
                break;
            case sm_clk:
                taskClock();

                if(switchGet(1) == 0) {
                    next_state = sm_leaving_clk;
                } else {
                    next_state = sm_clk;
                }
                break;
            case sm_leaving_clk:
                UART_print_str("sm_leaving_clk\r\n");
                next_state = sm_trans_to_cal;
                break;
            case sm_leaving_cal:
                UART_print_str("sm_leaving_cal\r\n");
                next_state = sm_trans_to_clk;
                break;
            default:
                next_state = sm_init;
                break;        
        }
        curr_state = next_state;
        
        
        //delay so that debugging is easier!
//        bufferTimer = 0;
//        while(bufferTimer<10);
        
        clkPulseFlag = 0;
    }
    
    return (EXIT_SUCCESS);
}



void taskInitClock(void)
{
    
    
    
}


void taskClock(void)
{
    //display the time
    dacSet(rtccSec()*0x10000/60,0);
    dacSet(rtccMin()*0x10000/60,1);
    dacLoad();
    
    //change options...
    
    const int ledLevel = switchKnobValue(0);
    tlcSetChannel(8,ledLevel);
    tlcSetChannel(9,ledLevel);
    tlcSetChannel(10,ledLevel);
    tlcSetChannel(11,ledLevel);
    tlcAssemble(0b11110000000);
    tlcUpdate();
    
}



void taskInitCal(void)
{
    //setup cal state machine



}
void taskCal(void)
{
    rtccSetMin((60*switchKnobValue(0))/0x10000);
    rtccSetHr((12*switchKnobValue(1))/0x10000);
    
    dacSet(rtccMin()*0x10000/60,0);
    dacSet(rtccHr()*0x10000/12,1);
    dacLoad();
    
    if(clkPulse)
    {
    UART_print_hex(16,switchKnobValue(0));
    UART_print_str("\r\n");
    UART_print_hex(16,switchKnobValue(1));
    UART_print_str("\r\n");
    }
    
    
}



void init(void)
{
    
    LEDTRISCLR = TRISCCLR;
    LEDSOFF();
    
    
    uartInit();
    rtccInit();
    switchInit();
    tlcInit();
#if defined(__32MX250F128D__)
    dacInit();
#endif
    
    initTMR();
}

void initTMR(void)
{
    //setup timer1 to go off every second
    
   
    
#if defined(__32MX430F064H__)
    OSCCONbits.SOSCEN = 1;
    T1CONbits.TCS = 0; //1=source is pbclk
    T1CONbits.TCKPS = 0b11; //1:256
    PR1 = 31250;
    PR1=0xffff;
#else
    OSCCONbits.SOSCEN = 1;
    T1CONbits.TCS = 1; //1=source is ext crystal
    T1CONbits.TCKPS = 0;
    PR1 = 32768;    
#endif
    IFS0bits.T1IF = 0;
    IEC0bits.T1IE = 1;
    IPC1bits.T1IP = 1;
    T1CONbits.ON = 1;
    
    
    
    //setup timer4 to go off every 50msec
    T4CONbits.TCKPS = 0b111;
    PR4 = 5*0x1000; //0x1000 = approx 50 msec
    TMR4 = 0;
    
    IFS0bits.T4IF = 0;
    IEC0bits.T4IE = 1;
    IPC4bits.T4IP = 1;
    T4CONbits.ON = 1;
} 
 

void __ISR(_TIMER_1_VECTOR,IPL1AUTO) _T1Interrupt()
{
    IFS0CLR = IFS0_T1IF;
    gTimer++;    
    clkPulse = 1;
}


//semi-rt tasks for low priority (50msec things), like reading knobs and switches
void __ISR(_TIMER_4_VECTOR,IPL1AUTO) _T4Interrupt()
{
    bufferTimer++;
    TMR4 = 0;
    IFS0CLR = IFS0_T4IF;
    switchRead();
}


//[KIT]ripped from plib
#if 0
inline unsigned int __attribute__((always_inline)) SYSTEMConfigPerformance(unsigned int sys_clock)
{
    // set up the wait states
    unsigned int pb_clk;
#ifdef _PCACHE
    unsigned int cache_status;
#endif
    unsigned int int_status;

    pb_clk = SYSTEMConfigWaitStatesAndPB(sys_clock);

    int_status=INTDisableInterrupts();

    mBMXDisableDRMWaitState();

#ifdef _PCACHE
    cache_status = mCheGetCon();
    cache_status |= CHE_CONF_PF_ALL;
    mCheConfigure(cache_status);
    CheKseg0CacheOn();
#endif

    INTRestoreInterrupts(int_status);

    return pb_clk;

}
#endif