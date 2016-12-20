
#include "switches.h"



enum settingGroups_t{
	set_clock,
	set_gaugeLED,
	set_gaugeMode,
	set_LED,

	//calibration
	set_calhr,
	set_calmin
}

enum gaugeLEDModes_t{
	gl_solid,
	gl_lrWave,
	gl_lSwipe,
	gl_rSwipe,
	gl_shakeMin,
	gl_pulseSec
}

enum gaugeModes_t{
	gm_secondFlip,
	gm_shaking
}

enum ledModes_t{
	lm_fade_hrmin,
	lm_fade_minsec,
	lm_solid,
	lm_pulse_solid,
	lm_pulse_spectrum
}


struct settings_t
{
	//main settings
	gaugeModes_t gMode,
	gaugeLEDModes_t glMode,
	frontLEDModes_t flMode,
	
	//settings for each mode
	int gl_lrWave_width,
	int gl_shakiness,
	int gl_pulseLow,

	int sm_secondDuration,
	int sm_shakiness,
}


void taskCal(void);
void taskCalInit(void);

