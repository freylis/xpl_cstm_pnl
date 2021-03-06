#ifndef FreyAPAltitude_h
#define FreyAPAltitude_h

#include "Arduino.h"
#include "FreyCommand.h"
#include "FreyEncoder.h"

const unsigned char pinAPAltitudeEncoderCLK = 24;
const unsigned char pinAPAltitudeEncoderDIO = 25;
const unsigned char pinAPAltitudeDisplayCLK = 40;
const unsigned char pinAPAltitudeDisplayDIO = 41;
const unsigned char pinAPAltitudeButton = 12;
const unsigned char pinAPAltitudeEnabled = A1;


GyverTM1637 apAltitudeDisplay(pinAPAltitudeDisplayCLK, pinAPAltitudeDisplayDIO);
EncButton2<EB_BTN> apAltitudeButton(INPUT, pinAPAltitudeButton);;


class FreyAPAltitude {
    public:
        FreyAPAltitude() {};

        void prepare() {
            lastSended = millis();
            valueChanged = false;
            pinMode(pinAPAltitudeEncoderCLK, INPUT_PULLUP);
            pinMode(pinAPAltitudeEncoderDIO, INPUT_PULLUP);
            pinMode(pinAPAltitudeButton, INPUT_PULLUP);
            pinMode(pinAPAltitudeDisplayCLK, OUTPUT);
            pinMode(pinAPAltitudeDisplayDIO, OUTPUT);
            pinMode(pinAPAltitudeEnabled, OUTPUT);

            apAltitudeDisplay.clear();
            apAltitudeDisplay.brightness(3);

            delay(100);
            apAltitudeDisplay.displayByte(0, _b);
            apAltitudeDisplay.display(1, 7);
            apAltitudeDisplay.display(2, 3);
            apAltitudeDisplay.display(3, 7);
            delay(300);

            _altitudeValue = 100;

            drawAltitude();
        };
        void lap() {

            /* set altitude with encoder */
            encoders[7].tick();
            if (encoders[7].turn()) {
                valueChanged = true;
                if (encoders[7].left()) {
                    if (encoders[7].fast()) {
                        _altitudeValue -= 5;
                    } else {
                        _altitudeValue -= 1;
                    };
                } else {
                    if (encoders[7].fast()) {
                        _altitudeValue += 5;;
                    } else {
                        _altitudeValue += 1;
                    };
                };
                if (_altitudeValue < 1) {
                    _altitudeValue = 1;
                } else if (_altitudeValue > 500) {
                    _altitudeValue = 500;
                };
                drawAltitude();
            };

            if (valueChanged && millis() > (lastSended + SEND_COMMAND_EVERY_MS)) {
                hardSendState();
                lastSended = millis();
                valueChanged = false;
            };

            /* toggle altitude */
            apAltitudeButton.tick();
            if (apAltitudeButton.release()) {
                sendPanelCommand("AP_ALTITUDE_TOGGLE");
            };
        };

    void readFullState(String fullState) {
        if (fullState[58] == '1') {
            analogWrite(pinAPAltitudeEnabled, KD2_LIGHT);
        } else {
            analogWrite(pinAPAltitudeEnabled, LOW);
        };
    };

    void hardSendState() {
        sendPanelCommand("AP_ALTITUDE_" + (String)(_altitudeValue * 100));
    };

    private:
        unsigned int _altitudeValue;
        unsigned long lastSended;
        bool valueChanged;

        void drawAltitude() {
            String sAltitude = (String)_altitudeValue;
            apAltitudeDisplay.displayByte(0, _empty);
            if (_altitudeValue < 10) {
                apAltitudeDisplay.displayByte(1, _empty);
                apAltitudeDisplay.displayByte(2, _empty);
                apAltitudeDisplay.display(3, (sAltitude.substring(0, 1).toInt()));
            } else if (_altitudeValue < 100) {
                apAltitudeDisplay.displayByte(1, _empty);
                apAltitudeDisplay.display(2, (sAltitude.substring(0, 1).toInt()));
                apAltitudeDisplay.display(3, (sAltitude.substring(1, 2).toInt()));
            } else {
                apAltitudeDisplay.display(1, (sAltitude.substring(0, 1).toInt()));
                apAltitudeDisplay.display(2, (sAltitude.substring(1, 2).toInt()));
                apAltitudeDisplay.display(3, (sAltitude.substring(2, 3).toInt()));
            };

        };

};

#endif