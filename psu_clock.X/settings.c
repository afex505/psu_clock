#include "settings.h"



settingsGroup_t currentSettingGroup;


void taskCal(void)
{


	//do different stuff based on the current setting group

	switch currentSettingGroup{


			case set_clock:

				rtcc


				break;
			case set_gaugeLED:
				break;
			case set_gaugeLED:
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