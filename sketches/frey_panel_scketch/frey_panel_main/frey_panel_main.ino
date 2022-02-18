/*
Кастомная панель от freylis

FULLSTATE message format:
    [frey-cmd-x] FULLSTATE 1__100__2__101__102__3__4__5__6__7__8__9__1
        1 - [23] - GEAR
        100 - [26..28] - FLAPS
        2 - [31] - SPEED BRAKES
        101 - [34..37] - VTRIM
        102 - [39..42] - COURSE
        3 - [44] - AP SPEED enabed/disabled
        4 - [47] - VNAV
        5 - [50] - LNAV
        6 - [53] - VORLOCK
        7 - [56] - APPROACH
        8 - [59] - CMD
        9 - [62] - CWS
        1 - [65] - HEADING enabled\disabled
*/
#define EB_BETTER_ENC true
#define EB_HALFSTEP_ENC true
#define DEBUG true

#include "frey_ext/FreyGear.h"
#include "frey_ext/FreyFlaps.h"
#include "frey_ext/FreySpeedBrakes.h"
#include "frey_ext/FreyVTrim.h"
#include "frey_ext/FreyCourse.h"
#include "frey_ext/FreyNav.h"
#include "frey_ext/FreyAPSpeed.h"
#include "frey_ext/FreyAPButtons.h"
#include "frey_ext/FreyAPHeading.h"


FreyGear gear;
FreyFlaps flaps;
FreySpeedBrakes sbrakes;
FreyVTrim vtrim;
FreyCourse course;
FreyNav nav1;
FreyAPSpeed ap_speed;
FreyAPButtons ap_buttons;
FreyAPHeading ap_heading;


void setup() {
    Serial.begin(9600);
    Serial.setTimeout(30);

    //gear.prepare();
    //flaps.prepare();
    //sbrakes.prepare();
    //vtrim.prepare();
    //course.prepare();
    //nav1.prepare();
    //ap_speed.prepare();
    // ap_buttons.prepare();
    ap_heading.prepare();
}

void executeFullState() {

    if (Serial.available() > 0) {
        String message = Serial.readString();
        message.trim();
        if (message == "") {
            return;
        };
        if (message.startsWith("[frey-cmd-x] FULLSTATE")) {
            sendLog("Start handle command " + message);
            //gear.readFullState(message);
            //flaps.readFullState(message);
            //sbrakes.readFullState(message);
            //vtrim.readFullState(message);
            //course.readFullState(message);
            //nav1.readFullState(message);
            //ap_speed.readFullState(message);
            //ap_buttons.readFullState(message);
            ap_heading.readFullState(message);
        };
    }

}


void loop() {
    // gear.lap();
    //flaps.lap();
    //sbrakes.lap();
    //vtrim.lap();
    //course.lap();
    //nav1.lap();
    //ap_speed.lap();
    //ap_buttons.lap();
    ap_heading.lap();

    executeFullState();
};
