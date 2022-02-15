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
#include "frey_ext/FreyVTrim.h"


#define EB_BETTER_ENC true
#define EB_HALFSTEP_ENC true



FreyGear gear;
FreyFlaps flaps;
FreySpeedBrakes sbrakes;
FreyVTrim vtrim;


void setup() {
    gear.prepare();
    flaps.prepare();
    sbrakes.prepare();
    vtrim.prepare();
}


void loop() {
    gear.lap();
    flaps.lap();
    sbrakes.lap();
    vtrim.lap();
};
