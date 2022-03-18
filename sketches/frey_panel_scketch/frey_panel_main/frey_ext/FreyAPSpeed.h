#ifndef FreyAPSpeed_h
#define FreyAPSpeed_h

#include "Arduino.h"
#include "FreyCommand.h"
#include "FreyEncoder.h"
#include <GyverTM1637.h>

const unsigned int pinATEnabled = A11;
const unsigned int pinFDEnabled = A3;
const unsigned int pinAPSpeedDisplayCLK = 42;
const unsigned int pinAPSpeedDisplayDIO = 43;
const unsigned int pintAPSpeedEncoderCLK = 26;
const unsigned int pintAPSpeedEncoderDIO = 27;
const unsigned int pinAPSpeedButton = 3;
const unsigned int pinAPSpeedEnabled = A10;


GyverTM1637 apSpeedDisplay(pinAPSpeedDisplayCLK, pinAPSpeedDisplayDIO);


class FreyAPSpeed {

    public:
        FreyAPSpeed() {};

        void prepare() {
            lastSended = millis();
            valueChanged = false;
            pinMode(pinATEnabled, INPUT_PULLUP);
            pinMode(pinFDEnabled, INPUT_PULLUP);
            pinMode(pintAPSpeedEncoderCLK, INPUT_PULLUP);
            pinMode(pintAPSpeedEncoderDIO, INPUT_PULLUP);
            pinMode(pinAPSpeedDisplayCLK, OUTPUT);
            pinMode(pinAPSpeedDisplayDIO, OUTPUT);
            pinMode(pinAPSpeedEnabled, OUTPUT);
            pinMode(pinAPSpeedButton, INPUT_PULLUP);
            _at_enabled = false;
            _speedValue = 100;

            apSpeedDisplay.clear();
            apSpeedDisplay.brightness(5);

            delay(10);
            apSpeedDisplay.displayByte(0, _b);
            apSpeedDisplay.display(1, 7);
            apSpeedDisplay.display(2, 3);
            apSpeedDisplay.display(3, 7);
            delay(100);

            drawSpeed();
        };

        void lap() {
            handleATToggle();
            handleFDToggle();
            handleEncoder();
        };

        void readFullState(String fullState) {
            char apSpeedStatus = fullState[34];
            if (apSpeedStatus == '1') {
                analogWrite(pinAPSpeedEnabled, KD2_LIGHT);
            } else {
                analogWrite(pinAPSpeedEnabled, LOW);
            };
        };

        void hardSendState() {
            sendPanelCommand("AP_SPEED_" + (String)_speedValue);
            if (_at_enabled) {sendPanelCommand("AT_1");}
                else {sendPanelCommand("AT_0");};
            if (_fd_enabled) {sendPanelCommand("FD_1");}
                else {sendPanelCommand("FD_0");};
        };

    private:
        bool _at_enabled;
        bool _fd_enabled;
        bool _ap_spd_enabled;
        int _speedValue;
        bool valueChanged;
        unsigned long lastSended;

        void handleATToggle() {
            /*
                AutoThrottle
                Check it once per 10ms
            */
            int at_current = !digitalRead(pinATEnabled);
            if (at_current == 1 && !_at_enabled) {
                sendPanelCommand("AUTO_THROTTLE_1");
                _at_enabled = true;
            } else if (at_current == 0 && _at_enabled) {
                sendPanelCommand("AUTO_THROTTLE_0");
                _at_enabled = false;
            };

        };

        void handleFDToggle() {
            int fd_current = !digitalRead(pinFDEnabled);
            if (fd_current == 1 && !_fd_enabled) {
                sendPanelCommand("FLIGHT_DIRECTOR_0");
                _fd_enabled = true;
            } else if (fd_current == 0 && _fd_enabled) {
                sendPanelCommand("FLIGHT_DIRECTOR_1");
                _fd_enabled = false;
            };
        };

        void handleEncoder() {
            encoders[5].tick();
            if (encoders[5].turn()) {
                valueChanged = true;
                if (encoders[5].right()) {
                    if (encoders[5].fast()) {
                        _speedValue -= 10;
                    } else {
                        _speedValue -= 1;
                    };
                } else {
                    if (encoders[5].fast()) {
                        _speedValue += 10;
                    } else {
                        _speedValue += 1;
                    };
                };
                _speedValue = min(500, max(100, _speedValue));
                drawSpeed();
            };

            if (valueChanged && millis() > (lastSended + SEND_COMMAND_EVERY_MS)) {
                sendPanelCommand("AP_SPEED_" + (String)_speedValue);
                lastSended = millis();
                valueChanged = false;
            };

            if (encoders[5].release()) {
                sendPanelCommand("AP_SPEED_TOGGLE");
            };

        };

        void drawSpeed() {
          String sSpeed = (String)_speedValue;
          apSpeedDisplay.displayByte(0, _empty);
          apSpeedDisplay.display(1, sSpeed.substring(0, 1).toInt());
          apSpeedDisplay.display(2, sSpeed.substring(1, 2).toInt());
          apSpeedDisplay.display(3, sSpeed.substring(2, 3).toInt());
        };

};

#endif
