/*
Кастомная панель от freylis

FULLSTATE message format:
    [frey-cmd-x] FULLSTATE 1__100__2__101__102__3__4__5__6__7__8__9__1__2__3
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
        3 - [71] - Level Changed enabled\disabled

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
LEVEL_CHANGED - вкл LEVELCHANGED для актуализации параметров полёта под выбранные на дисплее

*/
#define EB_BETTER_ENC true
#define EB_HALFSTEP_ENC true
//#define SEND_HARD_STATE true
//#define DEBUG true

#include "frey_ext/FreyGear.h"
#include "frey_ext/FreyFlaps.h"
#include "frey_ext/FreySpeedBrakes.h"
//#include "frey_ext/FreyVTrim.h"
#include "frey_ext/FreyCourse.h"
#include "frey_ext/FreyNav.h"
#include "frey_ext/FreyAPSpeed.h"
#include "frey_ext/FreyAPButtons.h"
#include "frey_ext/FreyAPHeading.h"
#include "frey_ext/FreyAPAltitude.h"
#include "frey_ext/FreyEncoder.h"
#include "frey_ext/FreyCommand.h"

FreyFlaps flaps;
//FreyVTrim vtrim;
FreyGear gear;
FreySpeedBrakes sbrakes;
FreyNav nav1;
FreyCourse course;
FreyAPSpeed ap_speed;
FreyAPButtons ap_buttons;
FreyAPHeading ap_heading;
FreyAPAltitude ap_altitude;


unsigned int lapNumber = 0;
unsigned int moduleNumber = 0;
long EACHLAP = 10000;


void setup() {
    Serial.begin(9600);
    Serial.setTimeout(30);

    // speed brakes
    encoders[0].setPins(INPUT_PULLUP, 37, 36);
    
    // flaps
    encoders[1].setPins(INPUT_PULLUP, 35, 34);
    
    // course
    encoders[2].setPins(INPUT_PULLUP, 28, 29);
    
    // nav1 mhz
    encoders[3].setPins(INPUT_PULLUP, 30, 31);
    
    // nav1 khz
    encoders[4].setPins(INPUT_PULLUP, 32, 33);
    
    // speed
    encoders[5].setPins(INPUT_PULLUP, 26, 27, 3);
    
    // heading
    encoders[6].setPins(INPUT_PULLUP, 22, 23);
    
    // altitude
    encoders[7].setPins(INPUT_PULLUP, 24, 25);

    sendPong();

    flaps.prepare();
    //vtrim.prepare();
    gear.prepare();
    sbrakes.prepare();
    nav1.prepare();
    course.prepare();
    ap_speed.prepare();
    ap_buttons.prepare();
    ap_heading.prepare();
    ap_altitude.prepare();
}


void executeExternalCommand() {

    if (Serial.available() > 0) {
        String message = Serial.readString();
        message.trim();
        if (message == "") {return;};

        /* ping-pong for auto-determine COM-port */
        if (message == "[frey-ping]") {
            sendPong();
            return;
        };

        if (message.startsWith("[frey-cmd-x] FULLSTATE") || message.startsWith("[frey-cmd-x] STATEFULL")) {
            sendLog("Start handle command " + message);
            gear.readFullState(message);
            flaps.readFullState(message);
            sbrakes.readFullState(message);
            //vtrim.readFullState(message);
            nav1.readFullState(message);
            course.readFullState(message);
            ap_speed.readFullState(message);
            ap_buttons.readFullState(message);
            ap_heading.readFullState(message);
            ap_altitude.readFullState(message);
        };
    };

}


#ifdef SEND_HARD_STATE
void sendHardState() {
    lapNumber += 1;
    if (lapNumber == EACHLAP) {
        lapNumber = 0;
        switch (moduleNumber) {
            case 0:
                ap_altitude.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send altitude");
                break;
            case 1:
                ap_buttons.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send buttons");
                break;
            case 2:
                ap_heading.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send heading");
                break;
            case 3:
                ap_speed.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send speed");
                break;
            case 4:
                course.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send course");
                break;
            case 5:
                flaps.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send flaps");
                break;
            case 6:
                gear.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send gear");
                break;
            case 7:
                nav1.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send nav1");
                break;
            case 8:
                sbrakes.hardSendState();
                moduleNumber += 1;
                sendLog("Hard send sbrake");
                break;
            case 9:
                //vtrim.hardSendState();
                moduleNumber += 1;
                //sendLog("Hard send vtrim");
                break;
            default:
                moduleNumber = 0;
                sendLog("Set module number 0");
                break;
        };
    };
};
#endif


void loop() {

    flaps.lap();
    //vtrim.lap();
    gear.lap();
    sbrakes.lap();
    nav1.lap();
    ap_speed.lap();
    ap_buttons.lap();
    ap_heading.lap();
    ap_altitude.lap();
    course.lap();

    executeExternalCommand();

    /*
        Каждые N кругов, какой-нибудь модуль шлёт сообщение в xplane
        с своим состоянием
    */
    #ifdef SEND_HARD_STATE
    sendHardState();
    #endif

};
