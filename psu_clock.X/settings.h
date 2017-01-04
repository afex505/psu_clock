
#include "switches.h"



enum settingsGroup_t{
	set_clock,
	set_gaugeLED,
	set_gaugeMode,
	set_LED,

	//calibration
	set_calhr,
	set_calmin
};

enum gaugeLEDModes_t{
	gl_solid,
	gl_lrWave,
	gl_lSwipe,
	gl_rSwipe,
	gl_shakeMin,
	gl_pulseSec
};

enum gaugeModes_t{
	gm_secondFlip,
	gm_shaking
};

enum frontLEDModes_t{
	lm_fade_hrmin,
	lm_fade_minsec,
	lm_solid,
	lm_pulse_solid,
	lm_pulse_spectrum
};



void taskCal(void);
void taskCalInit(void);

