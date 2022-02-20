#ifndef FreyAPButtons_h
#define FreyAPButtons_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <EncButton.h>

const unsigned char pinButtonVNAV = 10;
const unsigned char pinButtonLNAV = 11;
const unsigned char pinButtonVORLOCK = 12;
const unsigned char pinButtonAPPROACH = 12;
const unsigned char pinButtonCMD = 12;
const unsigned char pinButtonCWS = 12;
const unsigned char pinButtonDisengage = 12;
const unsigned char pinButtonLevelChanged = 12;

const unsigned char pinVNAVEnabled = A4;
const unsigned char pinLNAVEnabled = 2;
const unsigned char pinVORLOCKEnabled = 3;
const unsigned char pinAPPROACHEnabled = 3;
const unsigned char pinCMDEnabled = 3;
const unsigned char pinCWSEnabled = 3;
const unsigned char pinLevelChangedEnabled = 3;


EncButton<EB_TICK, pinButtonVNAV> apVNAVButton;
EncButton<EB_TICK, pinButtonLNAV> apLNAVButton;
EncButton<EB_TICK, pinButtonVORLOCK> apVORLOCKButton;
EncButton<EB_TICK, pinButtonAPPROACH> apAPPROACHButton;
EncButton<EB_TICK, pinButtonCMD> apCMDButton;
EncButton<EB_TICK, pinButtonCWS> apCWSButton;
EncButton<EB_TICK, pinButtonCWS> apDisengageButton;
EncButton<EB_TICK, pinButtonLevelChanged> apLvlChangedButton;


class FreyAPButtons {

    public:
        FreyAPButtons(){};

        void prepare() {
            pinMode(pinButtonLNAV, INPUT_PULLUP);
            pinMode(pinButtonVNAV, INPUT_PULLUP);
            pinMode(pinButtonVORLOCK, INPUT_PULLUP);
            pinMode(pinButtonAPPROACH, INPUT_PULLUP);
            pinMode(pinButtonCMD, INPUT_PULLUP);
            pinMode(pinButtonCWS, INPUT_PULLUP);
            pinMode(pinButtonDisengage, INPUT_PULLUP);
            pinMode(pinButtonLevelChanged, INPUT_PULLUP);
            pinMode(pinLNAVEnabled, OUTPUT);
            pinMode(pinVNAVEnabled, OUTPUT);
            pinMode(pinAPPROACHEnabled, OUTPUT);
            pinMode(pinVORLOCKEnabled, OUTPUT);
            pinMode(pinCMDEnabled, OUTPUT);
            pinMode(pinCWSEnabled, OUTPUT);
            pinMode(pinLevelChangedEnabled, OUTPUT);
        };

        void lap() {
            apVNAVButton.tick();
            if (apVNAVButton.release()) {
                sendPanelCommand("VNAV_TOGGLE");
            };

            apLNAVButton.tick();
            if (apLNAVButton.release()) {
                sendPanelCommand("LNAV_TOGGLE");
            };

            apVORLOCKButton.tick();
            if (apVORLOCKButton.release()) {
                sendPanelCommand("VOR_LOCK_TOGGLE");
            };

            apAPPROACHButton.tick();
            if (apAPPROACHButton.release()) {
                sendPanelCommand("APPROACH_TOGGLE");
            };

            apCMDButton.tick();
            if (apCMDButton.release()) {
                sendPanelCommand("CMD_TOGGLE");
            };

            apCWSButton.tick();
            if (apCWSButton.release()) {
                sendPanelCommand("CWS_TOGGLE");
            };

            apDisengageButton.tick();
            if (apDisengageButton.release()) {
                sendPanelCommand("DISENGAGE_TOGGLE");
            };

            apLvlChangedButton.tick();
            if (apLvlChangedButton.release()) {
                sendPanelCommand("LEVEL_CHANGED");
            };

        };

        void readFullState(String fullState) {

            if (fullState[47] == '1') {digitalWrite(pinVNAVEnabled, HIGH);}
             else {digitalWrite(pinVNAVEnabled, LOW);};

            if (fullState[50] == '1') {digitalWrite(pinLNAVEnabled, HIGH);}
             else {digitalWrite(pinLNAVEnabled, LOW);};

            if (fullState[53] == '1') {digitalWrite(pinVORLOCKEnabled, HIGH);}
             else {digitalWrite(pinVORLOCKEnabled, LOW);};

            if (fullState[56] == '1') {digitalWrite(pinAPPROACHEnabled, HIGH);}
             else {digitalWrite(pinAPPROACHEnabled, LOW);};

            if (fullState[59] == '1') {digitalWrite(pinCMDEnabled, HIGH);}
             else {digitalWrite(pinCMDEnabled, LOW);};

            if (fullState[62] == '1') {digitalWrite(pinCWSEnabled, HIGH);}
             else {digitalWrite(pinCWSEnabled, LOW);};

            if (fullState[71] == '1') {digitalWrite(pinLevelChangedEnabled, HIGH);}
             else {digitalWrite(pinLevelChangedEnabled, LOW);};
        };

        void hardSendState() {};

};

#endif