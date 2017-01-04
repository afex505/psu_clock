#include "settings.h"



enum settingsGroup_t currentSettingGroup;


struct settings {
	//main settings
	enum gaugeModes_t gMode;
	enum gaugeLEDModes_t glMode;
	enum frontLEDModes_t flMode;
	
	//settings for each mode
	int gl_lrWave_width;
	int gl_shakiness;
	int gl_pulseLow;

	int sm_secondDuration;
	int sm_shakiness;
};


void taskCal(void)
{


	//do different stuff based on the current setting group

	switch(currentSettingGroup){


			case set_clock:


				break;
			case set_gaugeLED:
				break;
			case set_gaugeMode:
				break;
			case set_LED:
				break;

			default:
				currentSettingGroup = set_clock;
				break;


	}





}

void taskCalInit(void)
{
	//set things up for calibrating/changeing settings
	currentSettingGroup = set_clock;
}