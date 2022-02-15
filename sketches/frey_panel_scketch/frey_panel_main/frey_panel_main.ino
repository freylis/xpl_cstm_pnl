/*
Кастомная панель от freylis

FULLSTATE message format:
	[frey-cmd-x] FULLSTATE 1__100__2__101
		1 - [23] - GEAR
		100 - [26..28] - FLAPS
		2 - [31] - SPEED BRAKES
		101 - [34..36] - VTRIM
*/

#include "frey_ext/FreyGear.h"
#include "frey_ext/FreyFlaps.h"
#include "frey_ext/FreySpeedBrakes.h"

#define EB_BETTER_ENC true
#define EB_HALFSTEP_ENC true

byte pinGearUp = 1;
byte pinGearDown = 2;
byte pinGearLightUp = 3;
byte pinGearLightDown = 4;

byte pinFlapsEncCLK = 5;
byte pinFlapsEncDIO = 6;
byte pinFlapsDisplayCLK = 7;
byte pinFlapsDisplayDIO = 8;

byte pinSpeedBrakesEncCLK = 9;
byte pinSpeedBrakesEncDIO = 10;
byte pinSpeedBrakesDisplayCLK = 10;
byte pinSpeedBrakesDisplayDIO = 11;


FreyGear gear(pinGearUp, pinGearDown, pinGearLightUp, pinGearLightDown);
FreyFlaps flaps(pinFlapsEncCLK, pinFlapsEncDIO, pinFlapsDisplayCLK, pinFlapsDisplayDIO);
FreySpeedBrakes sbrakes(pinSpeedBrakesEncCLK, pinSpeedBrakesEncDIO, pinSpeedBrakesDisplayCLK, pinSpeedBrakesDisplayDIO);


void setup() {
    gear.prepare();
    flaps.prepare();
    sbrakes.prepare();
}


void loop() {
    gear.lap();
    flaps.lap();
    sbrakes.lap();
};
