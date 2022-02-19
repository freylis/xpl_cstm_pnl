#ifndef FreyAPHeading_h
#define FreyAPHeading_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <EncButton.h>

const unsigned char pinAPHeadingEncoderCLK = 4;
const unsigned char pinAPHeadingEncoderDIO = 5;
const unsigned char pinAPHeadingDisplayCLK = 2;
const unsigned char pinAPHeadingDisplayDIO = 3;
const unsigned char pinAPHeadingButton = 10;
const unsigned char pinAPHeadingEnabled = A4;


GyverTM1637 apHeadingDisplay(pinAPHeadingDisplayCLK, pinAPHeadingDisplayDIO);
EncButton<EB_TICK, pinAPHeadingEncoderCLK, pinAPHeadingEncoderDIO> apHeadingEncoder;
EncButton<EB_TICK, pinAPHeadingButton> apHeadingButton;


class FreyAPHeading {

    public:
        FreyAPHeading() {};

        void prepare() {
            pinMode(pinAPHeadingEncoderCLK, INPUT_PULLUP);
            pinMode(pinAPHeadingEncoderDIO, INPUT_PULLUP);
            pinMode(pinAPHeadingButton, INPUT_PULLUP);
            pinMode(pinAPHeadingDisplayCLK, OUTPUT);
            pinMode(pinAPHeadingDisplayDIO, OUTPUT);
            pinMode(pinAPHeadingEnabled, OUTPUT);

            apHeadingDisplay.clear();
            apHeadingDisplay.brightness(5);

            _headingValue = 0;

            drawHeading();
        };

        void lap() {

            apHeadingEncoder.tick();
            if (apHeadingEncoder.turn()) {
                if (apHeadingEncoder.left()) {
                    if (apHeadingEncoder.fast()) {
                        _headingValue -= 10;
                    } else {
                        _headingValue -= 1;
                    };
                } else {
                    if (apHeadingEncoder.fast()) {
                        _headingValue += 10;
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
                sendPanelCommand("AP_HEADING_" + (String)_headingValue);
            };

            /* toggle heading */
            apHeadingButton.tick();
            if (apHeadingButton.release()) {
                sendPanelCommand("AP_HEADING_TOGGLE");
            };

        };
        void readFullState(String fullState) {
            if (fullState[65] == '1') {
                pinMode(pinAPHeadingEnabled, HIGH);
            } else {
                pinMode(pinAPHeadingEnabled, LOW);
            };

        };

        void hardSendState() {};

    private:
        int _headingValue;
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