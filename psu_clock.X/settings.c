#include "settings.h"

int gTimer;
int currentSettingGroup;



const uint8_t randByte[] = {
    89,243,136,195,5,248,173,68,82,186,246,186,152,240,123,188,
210,183,71,211,193,107,193,245,69,176,236,151,132,152,86,43,
46,32,89,140,0,217,168,135,165,112,234,249,20,207,145,61,
114,55,242,129,182,28,34,93,41,216,1,126,36,77,112,140,
124,55,159,109,54,205,245,249,78,91,56,199,158,212,34,24,
60,67,209,35,198,164,20,16,172,55,209,114,148,164,171,
236,150,79,252,180,89,17,105,231,137,30,90,133,168,97,191,
105,188,152,186,68,214,150,217,150,23,177,255,208,95,6,248,134,
130,76,113,17,130,108,223,126,251,105,175,187,188,75,1,185,
44,199,15,39,254,219,87,176,135,80,129,4,194,31,34,227,
48,252,66,81,95,221,241,141,37,44,235,240,66,142,187,210,
119,118,193,169,221,126,213,68,137,131,59,116,139,184,
40,213,151,123,32,106,160,125,226,152,19,1,215,187,116,68,
133,220,128,217,159,102,33,174,30,132,234,186,213,183,53,205,
231,177,38,202,149,183,163,164,207,30,89,220,69,17,162,
221,120,183,163,121,214,26,80,10,192,92,208,45,216,10,65,
222,246,200
};

struct settings_t{
	//main settings
	int glMode;
	int gmMode;
	int lmMode;
	
	//settings for each mode, need to add more?
	int gl_lrWave_width;
	int gl_shakiness;
	int gl_pulseLow;

	int sm_secondDuration;
	int sm_shakiness;
} settings;


void taskCal(void)
{
    int vSetting;
    int fadeDown;
    int fadeUp;
    int sector;
	//do different stuff based on the current setting group
	switch(currentSettingGroup){
			case SET_CLOCK:
                //current = hour
                //voltage = min
                
                //display the time
                dacSet(rtccMin()*0x10000/60,0);
                dacSet(rtccHr()*0x10000/12,1);
                dacLoad();
                rtccSetMin((switchGetKnob(0)*60)/0x10000);
                rtccSetHr((switchGetKnob(1)*12)/0x10000);
               
				break;
			case SET_GAUGELED:
                
                vSetting = (switchGetKnob(1)*5)/0x10000;
                dacSet((vSetting+1)*0xffff/15,1);
                dacLoad();
                
                settings.glMode = vSetting;
                
                //UART_print_dec(0,vSetting);
                switch(vSetting){
                    case GL_SOLID:
                        tlcSetChannel(GAUGE1,switchGetKnob(0));
                        tlcSetChannel(GAUGE2,switchGetKnob(0));
                        tlcSetChannel(GAUGE3,switchGetKnob(0));
                        tlcSetChannel(GAUGE4,switchGetKnob(0));
                        break;
                    case GL_LRWAVE:

                              
                        if     (TMR1<((0x8000*1)/8)) sector = 0;
                        else if(TMR1<((0x8000*3)/8)) sector = 1;
                        else if(TMR1<((0x8000*5)/8)) sector = 2;
                        else if(TMR1<((0x8000*7)/8)) sector = 3;
                        else                         sector = 4;
                       
                        if(gTimer&1) {
                            sector = 4-sector;
                            fadeDown = ((TMR1-(32687/8))<<3)&0xffff;
                            fadeUp = 0xffff-(((TMR1-(32768/8))<<3)&0xffff);
                        }
                        else
                        {
                            fadeUp = ((TMR1-(32687/8))<<3)&0xffff;
                            fadeDown = 0xffff-(((TMR1-(32768/8))<<3)&0xffff);
                        }
                        
  
                   
                        
                        
                        //goes from left/right/left every second
                        switch(sector) {
                            case 0://high left
                                tlcSetChannel(GAUGE1,0xffff);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,0);
                                break;
                            case 1:
                                tlcSetChannel(GAUGE1,fadeDown);
                                tlcSetChannel(GAUGE2,fadeUp);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,0);
                                break;
                            case 2:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,fadeDown);
                                tlcSetChannel(GAUGE3,fadeUp);
                                tlcSetChannel(GAUGE4,0);
                                break;
                            case 3:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,fadeDown);
                                tlcSetChannel(GAUGE4,fadeUp);
                                break;
                            case 4:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,0xffff);
                            default:
                                break;
                        }
                        break;
                    case GL_LSWIPE://swipes smooth from left to right
                        
                           
                        if     (TMR1<((0x8000*1)/8)) sector = 0;
                        else if(TMR1<((0x8000*2)/8)) sector = 1;
                        else if(TMR1<((0x8000*3)/8)) sector = 2;
                        else if(TMR1<((0x8000*4)/8)) sector = 3;
                        else if(TMR1<((0x8000*5)/8)) sector = 4;
                        else                         sector = 5;//five runs until end
                       
                        fadeUp = (TMR1<<4)&0xffff;
                        fadeDown = 0xffff-((TMR1<<4)&0xffff);
                        
                        //goes from left/right/left every second
                        switch(sector) {
                            case 0://high left
                                tlcSetChannel(GAUGE1,fadeUp);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,0);
                                break;
                            case 1:
                                tlcSetChannel(GAUGE1,fadeDown);
                                tlcSetChannel(GAUGE2,fadeUp);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,0);
                                break;
                            case 2:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,fadeDown);
                                tlcSetChannel(GAUGE3,fadeUp);
                                tlcSetChannel(GAUGE4,0);
                                break;
                            case 3:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,fadeDown);
                                tlcSetChannel(GAUGE4,fadeUp);
                                break;
                            case 4:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,fadeDown);
                                break;
                            case 5:
                            default:
                                tlcSetChannel(GAUGE1,0);
                                tlcSetChannel(GAUGE2,0);
                                tlcSetChannel(GAUGE3,0);
                                tlcSetChannel(GAUGE4,0);
                                break;
                        }
                        break;
                    case GL_SHAKEMIN: //as the #seconds grow, the leds start 'shaking'
                        dacSet(rtccSec()*0x10000/60,0);
                        dacSet(0,1);
                        dacLoad();
                        //total shake intensity controlled by knob 1 (should be virtual)
                        
                        tlcSetChannel(GAUGE1,switchGetKnob(0)+(randByte[TMR1&0xff]*rtccSec())/60);
                        tlcSetChannel(GAUGE2,switchGetKnob(0)+(randByte[(TMR1+64)&0xff]*rtccSec()/60));
                        tlcSetChannel(GAUGE3,switchGetKnob(0)+(randByte[(TMR1+64+64)&0xff]*rtccSec()/60));
                        tlcSetChannel(GAUGE4,switchGetKnob(0)+(randByte[(TMR1+64+64+64)&0xff]*rtccSec()/60));
                                                
                        
                        break;
                    case GL_PULSESEC://minimum intensity, with the last quadrant ramping up
                        fadeUp = switchGetKnob(0);
                        if(TMR1>((0x8000*3)/4)) {
                            fadeUp+= (TMR1<<2)-0x8000;
                        }
                        tlcSetChannel(GAUGE1,fadeUp);
                        tlcSetChannel(GAUGE2,fadeUp);
                        tlcSetChannel(GAUGE3,fadeUp);
                        tlcSetChannel(GAUGE4,fadeUp);
                        break;
                    default:
                        break; 
                }
                tlcAssemble(0b11110000000);
				break;
			case SET_GAUGEMODE:
                vSetting = (switchGetKnob(1)*2)/0x10000;
                 
                
                settings.glMode = vSetting;
				break;
			case SET_LED:
                
				break;
            case SET_CALHR:
				break;
            case SET_CALMIN:
				break;

			default:
				currentSettingGroup = SET_CLOCK;
				break;
                
                
            
	}
    
    
    tlcUpdate();
    if(switchGet(SW_MASK_LIMIT_DN))
    {
        UART_print_str("Switching to group ");
        UART_print_dec(0,currentSettingGroup);
        UART_print_str("\r\n");
        currentSettingGroup++;
    }
}

void taskCalInit(void)
{
	//set things up for calibrating/changeing settings
	currentSettingGroup = SET_GAUGELED;
}

