/*
Кастомная панель от freylis

FULLSTATE message format:
    [frey-cmd-x] FULLSTATE 1__100__2__101__102__3__4__5__6__7__8__9__1__2
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
        2 - [68] - ALTITUDE enabled\disabled

Сообщения, отправляемые панелью в сторону xplane:
AP_ALTITUDE_TOGGLE - вкл/выкл автопилот высоты
AP_ALTITUDE_XXX - где XXX - высота в сотнях футов.
    Т.е. сообщение AP_ALTITUDE_200, означает задатчик высоты в 20000 фт
AP_HEADING_TOGGLE - вкл/выкл автопилот курса
AP_HEADING_XXX - сам курс. от 0 до 360. в градусах
AP_SPEED_TOGGLE - вкл/выкл автопилот скорости
AP_SPEED_XXX - скорость в узлах. от 100 до 500
DISENGAGE_TOGGLE - ВЫКЛ автопилот. Вкл НЕ этой кнопкой.
CWS_TOGGLE - вкл/выкл следование по текущей скорости, положению носа и курсу
CMD_TOGGLE - вкл/выкл базовый автопилот
APPROACH_TOGGLE - вкл/выкл подход к аэродрому в автоматическом режиме
VOR_LOCK_TOGGLE - вкл/выкл подход к аэродрому по VOR маяку (только по горизонтали)
LNAV_TOGGLE - вкл/выкл следование по горизонтальному маршруту, заданному в FMC
VNAV_TOGGLE - вкл/выкл следование по вертикальному маршруту, заданному в FMC
SET_COURSE_XXX - курс маяка для автоматической посадки. XXX - градусы.
FLAPS_UP_ONE - переключить закрылки вверх на одну позицию
FLAPS_DOWN_ONE - переключить закрылки вниз на одну позицию
GEARUP - шасси вверх
GEARDOWN - шасси вниз
NAV_FREQ_XXX.YY - установить передатчик NAV1 на частоту XXX.YY
SPEEDBRAKES_UP_ONE - воздушный тормоз на 1 позицию вверх
SPEEDBRAKES_DOWN_ONE - воздушный тормоз на 1 позицию вниз
VTRIM_UP_ONE - тример руля высоты вверх на 1 поз
VTRIM_DOWN_ONE - тример руля высоты вниз на 1 поз

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
#include "frey_ext/FreyAPAltitude.h"


FreyGear gear;
FreyFlaps flaps;
FreySpeedBrakes sbrakes;
FreyVTrim vtrim;
FreyCourse course;
FreyNav nav1;
FreyAPSpeed ap_speed;
FreyAPButtons ap_buttons;
FreyAPHeading ap_heading;
FreyAPAltitude ap_altitude;


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
    // ap_altitude.prepare();
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
            // ap_altitude.readFullState(message);
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
    // ap_altitude.lap();

    executeFullState();
};
