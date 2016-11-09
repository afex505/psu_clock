
#include "switches.h"


static int knobValues[2];
static uint8_t switchShadow = 0;
static uint8_t switchEvents = 0;
static uint8_t switchRaw = 0;


void switchInit(void)
{
    //KNOBV     RB3     AN5
    //KNOBA     RC0     AN6
    //KNOBTRIM  RB2     AN4
    
    //SWLIM     RC1
    //SWPOW     RC2
       
    //turn on analog select
    ANSELBSET = (1<<3)|(1<<2);
    ANSELCSET = (1<<0);
    
    ANSELCCLR = (1<<1)|(1<<2);
    
    
    //tristate
    TRISBSET = (1<<3)|(1<<2);
    TRISCSET = (0b111);
    
    //setup ADconverter
    AD1CON1bits.SSRC = 0b111;//auto convert
    AD1CON3bits.SAMC = 0b11111;//31tad
    AD1CON1bits.ON = 1; 
}

void readKnobs(void)
{
    knobValues[0] = switchKnobValue(0)<<6;
    knobValues[1] = switchKnobValue(1)<<6;   
}

int getKnob(int channel)
{
    return knobValues[channel];
}

int switchKnobValue(int channel)
{
    switch(channel){
        case KNOBV:
            AD1CHS = 5<<16;
            break;
        case KNOBC:
            AD1CHS = 6<<16;
            break;
        case 2:
            AD1CHS = 4<<16;
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
int switchValueRaw(int switchNum)
{
    if(switchNum) {
        return (switchRaw&0b10);
    }
    else {
        return (switchRaw&0b01);
    }
}

//updates the current switch event values
void switchRead(void)
{
    switchRaw = PORTC&0b110;
    if((switchShadow&0b10) != (PORTC&0b10)){
        if(PORTC&0b10){
            switchEvents |= 0b0010;
        }
        else {
            switchEvents |= 0b1000;
        }
    }
    
    if((switchShadow&0b1) != (PORTC&0b1)){
        if(PORTC&0b1){
            switchEvents |= 0b0001;
        }
        else {
            switchEvents |= 0b0100;
        }
    }
    
    return;
}

int switchGetEvents(void)
{
    //this is super hacky but it's early in the morning...
    int switchReturn = switchEvents;
    switchEvents = 0;
    return switchReturn;
}



