#ifndef FreyAPAltitude_h
#define FreyAPAltitude_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <EncButton.h>

const unsigned char pinAPAltitudeEncoderCLK = 4;
const unsigned char pinAPAltitudeEncoderDIO = 5;
const unsigned char pinAPAltitudeDisplayCLK = 2;
const unsigned char pinAPAltitudeDisplayDIO = 3;
const unsigned char pinAPAltitudeButton = 10;
const unsigned char pinAPAltitudeEnabled = A0;


GyverTM1637 apAltitudeDisplay(pinAPAltitudeDisplayCLK, pinAPAltitudeDisplayDIO);
EncButton<EB_TICK, pinAPAltitudeEncoderCLK, pinAPAltitudeEncoderDIO> apAltitudeEncoder;
EncButton<EB_TICK, pinAPAltitudeButton> apAltitudeButton;


class FreyAPAltitude {
    public:
        FreyAPAltitude() {};

        void prepare() {
            pinMode(pinAPAltitudeEncoderCLK, INPUT_PULLUP);
            pinMode(pinAPAltitudeEncoderDIO, INPUT_PULLUP);
            pinMode(pinAPAltitudeButton, INPUT_PULLUP);
            pinMode(pinAPAltitudeDisplayCLK, OUTPUT);
            pinMode(pinAPAltitudeDisplayDIO, OUTPUT);
            pinMode(pinAPAltitudeEnabled, OUTPUT);

            apAltitudeDisplay.clear();
            apAltitudeDisplay.brightness(5);

            _altitudeValue = 100;

            drawAltitude();
        };
        void lap() {

            /* set altitude with encoder */
            apAltitudeEncoder.tick();
            if (apAltitudeEncoder.turn()) {
                if (apAltitudeEncoder.left()) {
                    if (apAltitudeEncoder.fast()) {
                        _altitudeValue -= 10;
                    } else {
                        _altitudeValue -= 1;
                    };
                } else {
                    if (apAltitudeEncoder.fast()) {
                        _altitudeValue += 10;
                    } else {
                        _altitudeValue += 1;
                    };
                };
                _altitudeValue = min(max(100, _altitudeValue), 500);
                hardSendState();
                drawAltitude();
            };

            /* toggle altitude */
            apAltitudeButton.tick();
            if (apAltitudeButton.release()) {
                sendPanelCommand("AP_ALTITUDE_TOGGLE");
            };
        };

    void readFullState(String fullState) {
        if (fullState[68] == '1') {
            pinMode(pinAPAltitudeEnabled, HIGH);
        } else {
            pinMode(pinAPAltitudeEnabled, LOW);
        };
    };

    void hardSendState() {
        sendPanelCommand("AP_ALTITUDE_" + (String)(_altitudeValue * 100));
    };

    private:
        unsigned int _altitudeValue;

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