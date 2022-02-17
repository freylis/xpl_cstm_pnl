#ifndef FreyAPSpeed_h
#define FreyAPSpeed_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned int pinATEnabled = A0;
const unsigned int pinFDEnabled = A1;
const unsigned int pinAPSpeedDisplayCLK = 2;
const unsigned int pinAPSpeedDisplayDIO = 3;
const unsigned int pintAPSpeedEncoderCLK = 4;
const unsigned int pintAPSpeedEncoderDIO = 5;
const unsigned int pinAPSpeedButton = 10;
const unsigned int pinAPSpeedEnabled = A4;


GyverTM1637 apSpeedDisplay(pinAPSpeedDisplayCLK, pinAPSpeedDisplayDIO);
EncButton<EB_TICK, pintAPSpeedEncoderCLK, pintAPSpeedEncoderDIO, pinAPSpeedButton> apSpeedEncoder;


class FreyAPSpeed {
    public:
        FreyAPSpeed() {};

        void prepare() {
            pinMode(pinATEnabled, INPUT_PULLUP);
            pinMode(pinFDEnabled, INPUT_PULLUP);
            pinMode(pintAPSpeedEncoderCLK, INPUT_PULLUP);
            pinMode(pintAPSpeedEncoderDIO, INPUT_PULLUP);
            pinMode(pinAPSpeedDisplayCLK, OUTPUT);
            pinMode(pinAPSpeedDisplayDIO, OUTPUT);
            pinMode(pinAPSpeedEnabled, OUTPUT);
            _at_enabled = false;
            _at_last_ms = 0;
            _fd_last_ms = 0;
            _current_ms = 0;
            _speedValue = 100;

            apSpeedDisplay.clear();
            apSpeedDisplay.brightness(5);

            drawSpeed();
        };

        void lap() {
            _current_ms = millis();
            handleATToggle();
            handleFDToggle();
            handleEncoder();

        };

        void readFullState(String fullState) {
            char apSpeedStatus = fullState[44];
            sendLog("APSPD: " + (String)apSpeedStatus);
            if (apSpeedStatus == '0') {
                digitalWrite(pinAPSpeedEnabled, LOW);
                sendLog("Draw low");
            } else if (apSpeedStatus == '1') {
                digitalWrite(pinAPSpeedEnabled, HIGH);
                sendLog("Draw high");
            };
        };

    private:
        bool _at_enabled;
        unsigned long _at_last_ms;
        unsigned long _fd_last_ms;
        unsigned long _current_ms;
        bool _fd_enabled;
        bool _ap_spd_enabled;
        int _speedValue;

        void handleATToggle() {
            /*
                AutoThrottle
                Check it once per 10ms
            */
            if (_current_ms >= _at_last_ms + 50) {

                int at_current = !digitalRead(pinATEnabled);
                if (at_current == 1 && !_at_enabled) {
                    sendPanelCommand("AT_1");
                    _at_enabled = true;
                } else if (at_current == 0 && _at_enabled) {
                    sendPanelCommand("AT_0");
                    _at_enabled = false;
                };

                _at_last_ms = _current_ms;
            };

        };

        void handleFDToggle() {
            /*
                Flight Director
                Check it once per 50ms
            */
            if (_current_ms >= _fd_last_ms + 50) {

                int fd_current = !digitalRead(pinFDEnabled);
                if (fd_current == 1 && !_fd_enabled) {
                    sendPanelCommand("FD_1");
                    _fd_enabled = true;
                } else if (fd_current == 0 && _fd_enabled) {
                    sendPanelCommand("FD_0");
                    _fd_enabled = false;
                };

                _fd_last_ms = _current_ms;
            };
        };

        void handleEncoder() {
            apSpeedEncoder.tick();
            if (apSpeedEncoder.turn()) {
                if (apSpeedEncoder.left()) {
                    if (apSpeedEncoder.fast()) {
                        _speedValue -= 10;
                    } else {
                        _speedValue -= 1;
                    };
                } else {
                    if (apSpeedEncoder.fast()) {
                        _speedValue += 10;
                    } else {
                        _speedValue += 1;
                    }
                };
                _speedValue = min(500, max(100, _speedValue));
                drawSpeed();
            };

            if (apSpeedEncoder.release()) {
                sendPanelCommand("AP_SPEED_TOGGLE");
            };

        };

        void drawSpeed() {
          String sSpeed = (String)_speedValue;
          sendPanelCommand("AP_SPEED_" + sSpeed);
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.display(1, sSpeed.substring(0, 1).toInt());
          flapsDisplay.display(2, sSpeed.substring(1, 2).toInt());
          flapsDisplay.display(3, sSpeed.substring(2, 3).toInt());
        };

};

#endif
