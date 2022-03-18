#ifndef FreyAPButtons_h
#define FreyAPButtons_h

#include "Arduino.h"
#include "FreyCommand.h"

const unsigned char pinButtonVNAV = 6;
const unsigned char pinButtonLNAV = 7;
const unsigned char pinButtonVORLOCK = 5;
const unsigned char pinButtonAPPROACH = 4;
const unsigned char pinButtonCMD = 11;
const unsigned char pinButtonCWS = 2;
const unsigned char pinButtonDisengage = 9;
const unsigned char pinButtonLevelChanged = 8;

const unsigned char pinVNAVEnabled = A7;
const unsigned char pinLNAVEnabled = A6;
const unsigned char pinVORLOCKEnabled = A8;
const unsigned char pinAPPROACHEnabled = A9;
const unsigned char pinCMDEnabled = A2;
const unsigned char pinCWSEnabled = A4;
const unsigned char pinLevelChangedEnabled = A5;


EncButton2<EB_BTN> apVNAVButton(INPUT, pinButtonVNAV);;
EncButton2<EB_BTN> apLNAVButton(INPUT, pinButtonLNAV);;
EncButton2<EB_BTN> apVORLOCKButton(INPUT, pinButtonVORLOCK);;
EncButton2<EB_BTN> apAPPROACHButton(INPUT, pinButtonAPPROACH);;
EncButton2<EB_BTN> apCMDButton(INPUT, pinButtonCMD);;
EncButton2<EB_BTN> apCWSButton(INPUT, pinButtonCWS);;
EncButton2<EB_BTN> apDisengageButton(INPUT, pinButtonDisengage);;
EncButton2<EB_BTN> apLvlChangedButton(INPUT, pinButtonLevelChanged);;


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
            // [frey-cmd-x] STATEFULL 1__000__1__0191__0__1__0__0__0__1__1__0__0__1__0
            if (fullState[46] == '1') {analogWrite(pinVNAVEnabled, KD2_LIGHT);}
             else {analogWrite(pinVNAVEnabled, LOW);};

            if (fullState[49] == '1') {analogWrite(pinLNAVEnabled, KD2_LIGHT);}
             else {analogWrite(pinLNAVEnabled, LOW);};

            if (fullState[52] == '1') {analogWrite(pinVORLOCKEnabled, KD2_LIGHT);}
             else {analogWrite(pinVORLOCKEnabled, LOW);};

            if (fullState[55] == '1') {analogWrite(pinAPPROACHEnabled, KD2_LIGHT);}
             else {analogWrite(pinAPPROACHEnabled, LOW);};

            if (fullState[58] == '1') {analogWrite(pinCMDEnabled, KD2_LIGHT);}
             else {analogWrite(pinCMDEnabled, LOW);};

            if (fullState[62] == '1') {analogWrite(pinCWSEnabled, KD2_LIGHT);}
             else {analogWrite(pinCWSEnabled, LOW);};

            if (fullState[70] == '1') {analogWrite(pinLevelChangedEnabled, KD2_LIGHT);}
             else {analogWrite(pinLevelChangedEnabled, LOW);};
        };

        void hardSendState() {};

};

#endif