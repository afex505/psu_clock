
#include "switches.h"
#include "tlc.h"


extern int gTimer;

//SETTINGS
#define SET_CLOCK 0
#define SET_GAUGELED 1
#define SET_GAUGEMODE 2
#define SET_LED 3
#define SET_CALHR 4
#define SET_CALMIN 5
#define SET_MAX 6

//GAUGE MODES
#define GM_SECONDFLIP 0
#define GM_SHAKING 1
#define GM_MAX 2

//GAUGE LED MODES
#define GL_SOLID 0
#define GL_LRWAVE 1
#define GL_LSWIPE 2
#define GL_SHAKEMIN 3
#define GL_PULSESEC 4
#define GL_MAX 5

//FRONT LED MODES
#define LM_FADE_HRMIN 0
#define LM_FADE_MINSEC 1
#define LM_SOLID 2
#define LM_PULSE_SOLID 3
#define LM_PULSE_SPECTRUM 4


void taskCal(void);
void taskCalInit(void);

