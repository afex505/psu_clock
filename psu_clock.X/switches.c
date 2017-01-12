
#include "switches.h"


static int knobValues[2];
static int switchShadow[2];
static uint8_t switchEvents = 0;

static int swBuffered[2];
static int switchStates;

static int virtKnob;
static int virtKnobShadow;

void switchInit(void)
{
    KNOB_GPIO_INIT();
    SW_GPIO_INIT();
    
    //setup ADconverter
    AD1CON1bits.SSRC = 0b111;//auto convert
    AD1CON3bits.SAMC = 0b11111;//31tad
    AD1CON1bits.ON = 1; 
    

    switchRead();
    switchShadow[0] = swBuffered[0];
    switchShadow[1] = swBuffered[1];
}

int inline switchGetKnob(int channel)
{
    return (knobValues[channel]);
}

int inline switchGet(int mask)
{
    if(mask)
        return (switchStates&mask);
    else
        return (switchStates);
}


//runs ADC Conversion
int switchKnobValue(int channel)
{
    switch(channel){
        case KNOBV:
            AD1CHS = KNOBV_ADCCH<<16;
            break;
        case KNOBA:
            AD1CHS = KNOBA_ADCCH<<16;
            break;
        case KNOBTRM:
            AD1CHS = KNOBTRM_ADCCH<<16;
            break;
        default:
            return 0;
            break;
    }
    
    //set SAMP
    AD1CON1CLR = (1<<0); //clear DONE
    AD1CON1SET = (1<<1); //set SAMP bit
    while(!(AD1CON1&(0b1))){}; //wait for DONE
    
    //return the result
    if(channel == KNOBV)
    {
        return 0x400-ADC1BUF0;
    }
    return ADC1BUF0;   
}



//run this function at the top of each superloop, it
//latches in new vales
void switchLatch(void)
{    
    //clear old events
    switchStates = 0;

    //check for changes
    if((switchShadow[1]) != swBuffered[1]){
        if(SW_1_STATE){
            switchStates |= 0b001000;
        } else {
            switchStates |= 0b100000;
        }
        switchShadow[1] = swBuffered[1];
    }
    
    if((switchShadow[0]) != swBuffered[0]){
        if(SW_0_STATE){
            switchStates |= 0b000100;
        } else {
            switchStates |= 0b010000;
        }
        switchShadow[0] = swBuffered[0];
    }

    //latch in new realtime values
    if(swBuffered[0])
        switchStates |= 0b01;

    if(swBuffered[1])
        switchStates |= 0b10;

    return;
}

//run this function only as needed, it actually polls
//switches and runs ADC on knobs
void switchRead(void)
{
    swBuffered[0] = SW_0_STATE;
    swBuffered[1] = SW_1_STATE;

    int i;
    //50/50 IIR for a little smoothing
    
   
    for(i = 0; i < 2; i++) {
//        knobValues[i] *= 3;    
//        knobValues[i] += switchKnobValue(i)<<6;
//        knobValues[i] >>= 2;
        knobValues[i] *= 1;    
        knobValues[i] += switchKnobValue(i)<<6;
        knobValues[i] >>= 1;
    }
    return;
}


int virtualKnob = 0;
int virtualKnobShadow = 0;

int switchVirtGet(void)
{
    int change;

    virtKnob += switchGetKnob(0) - virtualKnobShadow;

    //bounds check
    if(virtKnob > 0xffff)
        virtKnob  = 0xffff;
    else if(virtKnob < 0)
        virtKnob = 0;

    virtKnobShadow = switchGetKnob(0);
    
    return virtKnob;
}

int switchVirtLatch(void)
{

}

void switchVertSet(int value)
{
    virtKnob = value;
    virtKnobShadow = value;
}

