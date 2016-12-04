
#include "switches.h"


static int knobValues[2];
static uint8_t switchShadow = 0;
static uint8_t switchEvents = 0;


void switchInit(void)
{
    KNOB_GPIO_INIT();
    SW_GPIO_INIT();
    
    //setup ADconverter
    AD1CON1bits.SSRC = 0b111;//auto convert
    AD1CON3bits.SAMC = 0b11111;//31tad
    AD1CON1bits.ON = 1; 
}

int getKnob(int channel)
{
    return (knobValues[channel]);
}

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
    
    return ADC1BUF0;   
}


//returns a raw (but debounced!) value of the switches
//also logic flips them (PWR switch is active low) if they need it
int switchValueRaw(int switchNum)
{
    if(switchNum) {
        return (SW_1_STATE);
    } else {
        return (SW_0_STATE);
    }
}

//updates the current switch event values
void switchRead(void)
{    
    if((switchShadow&0b10) != SW_1_STATE){
        if(SW_1_STATE){
            switchEvents |= 0b0010;
        } else {
            switchEvents |= 0b1000;
        }
    }
    
    if((switchShadow&0b1) != SW_0_STATE){
        if(SW_0_STATE){
            switchEvents |= 0b0001;
        } else {
            switchEvents |= 0b0100;
        }
    }
    
    switchShadow = (SW_1_STATE | SW_0_STATE);
    
    knobValues[0] = switchKnobValue(0)<<6;
    knobValues[1] = switchKnobValue(1)<<6;  
    
    return;
}

int switchGetEvents(void)
{
    //this is super hacky but it's early in the morning...
    int switchReturn = switchEvents;
    switchEvents = 0;
    return switchReturn;
}



