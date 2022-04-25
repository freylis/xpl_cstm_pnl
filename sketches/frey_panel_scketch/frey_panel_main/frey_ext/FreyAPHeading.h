#ifndef FreyAPHeading_h
#define FreyAPHeading_h

#include "Arduino.h"
#include "FreyCommand.h"
#include "FreyEncoder.h"

const unsigned char pinAPHeadingEncoderCLK = 22;
const unsigned char pinAPHeadingEncoderDIO = 23;
const unsigned char pinAPHeadingDisplayCLK = 38;
const unsigned char pinAPHeadingDisplayDIO = 39;
const unsigned char pinAPHeadingButton = 13;
const unsigned char pinAPHeadingEnabled = A0;


GyverTM1637 apHeadingDisplay(pinAPHeadingDisplayCLK, pinAPHeadingDisplayDIO);
EncButton2<EB_BTN> apHeadingButton(INPUT, pinAPHeadingButton);;


class FreyAPHeading {

    public:
        FreyAPHeading() {};

        void prepare() {
            valueChanged = false;
            lastSended = millis();
            pinMode(pinAPHeadingEncoderCLK, INPUT_PULLUP);
            pinMode(pinAPHeadingEncoderDIO, INPUT_PULLUP);
            pinMode(pinAPHeadingButton, INPUT_PULLUP);
            pinMode(pinAPHeadingDisplayCLK, OUTPUT);
            pinMode(pinAPHeadingDisplayDIO, OUTPUT);
            pinMode(pinAPHeadingEnabled, OUTPUT);

            apHeadingDisplay.clear();
            apHeadingDisplay.brightness(3);

            _headingValue = 0;

            delay(100);
            apHeadingDisplay.displayByte(0, _b);
            apHeadingDisplay.display(1, 7);
            apHeadingDisplay.display(2, 3);
            apHeadingDisplay.display(3, 7);
            delay(300);

            drawHeading();
            hardSendState();
        };

        void lap() {

            encoders[6].tick();
            if (encoders[6].turn()) {
                valueChanged = true;
                if (encoders[6].left()) {
                    if (encoders[6].fast()) {
                        _headingValue -= 3;
                    } else {
                        _headingValue -= 1;
                    };
                } else {
                    if (encoders[6].fast()) {
                        _headingValue += 3;;
                    } else {
                        _headingValue += 1;
                    }
                };
                if (_headingValue < 0) {
                    _headingValue = 360;
                } else if (_headingValue > 360) {
                    _headingValue = 0;
                };
                drawHeading();
            };

            if (valueChanged && millis() > (lastSended + SEND_COMMAND_EVERY_MS)) {
                hardSendState();
                lastSended = millis();
                valueChanged = false;
            };

            /* toggle heading */
            apHeadingButton.tick();
            if (apHeadingButton.release()) {
                sendPanelCommand("AP_HEADING_TOGGLE");
            };

        };
        void readFullState(String fullState) {
            if (fullState[55] == '1') {
                analogWrite(pinAPHeadingEnabled, KD2_LIGHT);
            } else {
                analogWrite(pinAPHeadingEnabled, LOW);
            };

        };

        void hardSendState() {
            sendPanelCommand("AP_HEADING_" + (String)_headingValue);
        };

    private:
        int _headingValue;
        bool valueChanged;
        unsigned long lastSended;
        void drawHeading() {
          String sHeading = (String)_headingValue;
          apHeadingDisplay.displayByte(0, _empty);
          if (_headingValue < 10) {
            apHeadingDisplay.displayByte(1, _empty);
            apHeadingDisplay.displayByte(2, _empty);
            apHeadingDisplay.display(3, _headingValue);
          } else if (_headingValue < 100) {
            apHeadingDisplay.displayByte(1, _empty);
            apHeadingDisplay.display(2, (sHeading.substring(0, 1)).toInt());
            apHeadingDisplay.display(3, (sHeading.substring(1, 2)).toInt());
          } else {
            apHeadingDisplay.display(1, (sHeading.substring(0, 1)).toInt());
            apHeadingDisplay.display(2, (sHeading.substring(1, 2)).toInt());
            apHeadingDisplay.display(3, (sHeading.substring(2, 3)).toInt());
          };

        };

};

#endif