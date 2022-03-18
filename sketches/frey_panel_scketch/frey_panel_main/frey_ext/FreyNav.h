#ifndef FreyNav_h
#define FreyNav_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned int pinNav1EncoderIntCLK = 30;
const unsigned int pinNav1EncoderIntDIO = 31;

const unsigned int pinNav1EncoderFloatCLK = 32;
const unsigned int pinNav1EncoderFloatDIO = 33;

const unsigned int pinNav1ActiveDisplayCLK = 46;
const unsigned int pinNav1ActiveDisplayDIO = 47;

const unsigned int pinNav1StandbyDisplayCLK = 48;
const unsigned int pinNav1StandbyDisplayDIO = 49;

const unsigned int pinNav1SwitchStation = 10;


EncButton<EB_TICK, pinNav1EncoderIntCLK, pinNav1EncoderIntDIO> navIntEncoder;
EncButton<EB_TICK, pinNav1EncoderFloatCLK, pinNav1EncoderFloatDIO, pinNav1SwitchStation> navFloatEncoder;
EncButton<EB_TICK, pinNav1SwitchStation> navSwitchButton;

GyverTM1637 navActiveDisplay(pinNav1ActiveDisplayCLK, pinNav1ActiveDisplayDIO);
GyverTM1637 navStandByDisplay(pinNav1StandbyDisplayCLK, pinNav1StandbyDisplayDIO);


class FreyNav {
    private:
        bool valueChanged;
        int lastSended;
        unsigned char intStandByFreq;
        int floatStandByFreq;
        unsigned char intActiveFreq;
        unsigned char floatActiveFreq;
        void drawStandByFreq() {
            /*
                Частота NAV может быть от 108.00 до 120.95
                Задается она двумя отдельными энкодерами, один для единиц, другой для сотых
                Т.к. дисплей один, считаем, что "1" (сотня) всегда есть, мы ее дорисуем потом красками
                То дисплей с числами XX:YY нужно заполнить:
                    в XX десятки и единицы
                    в YY десятые и сотые
            */
          String sIntStandByFreq = (String)intStandByFreq;
          navStandByDisplay.display(0, ((String)sIntStandByFreq[1]).toInt());
          navStandByDisplay.display(1, ((String)sIntStandByFreq[2]).toInt());

          String sFloatStandByFreq = (String)floatStandByFreq;
          if (floatStandByFreq == 0) {
            navStandByDisplay.display(2, 0);
            navStandByDisplay.display(3, 0);
          } else if (floatStandByFreq < 10) {
            navStandByDisplay.display(2, 0);
            navStandByDisplay.display(3, ((String)sFloatStandByFreq[0]).toInt());
          } else {
            navStandByDisplay.display(2, ((String)sFloatStandByFreq[0]).toInt());
            navStandByDisplay.display(3, ((String)sFloatStandByFreq[1]).toInt());
          };
        };

        void drawActiveFreq() {
            /*
                Частота NAV может быть от 108.00 до 120.95
                Задается она двумя отдельными энкодерами, один для единиц, другой для сотых
                Т.к. дисплей один, считаем, что "1" (сотня) всегда есть, мы ее дорисуем потом красками
                То дисплей с числами XX:YY нужно заполнить:
                    в XX десятки и единицы
                    в YY десятые и сотые
            */
          String sIntActiveFreq = (String)intActiveFreq;
          navActiveDisplay.display(0, ((String)sIntActiveFreq[1]).toInt());
          navActiveDisplay.display(1, ((String)sIntActiveFreq[2]).toInt());

          String sFloatActiveFreq = (String)floatActiveFreq;
          if (floatActiveFreq == 0) {
            navActiveDisplay.display(2, 0);
            navActiveDisplay.display(3, 0);
          } else if (floatActiveFreq < 10) {
            navActiveDisplay.display(2, 0);
            navActiveDisplay.display(3, ((String)sFloatActiveFreq[0]).toInt());
          } else {
            navActiveDisplay.display(2, ((String)sFloatActiveFreq[0]).toInt());
            navActiveDisplay.display(3, ((String)sFloatActiveFreq[1]).toInt());
          };
        };

        void switchFreq() {
            /* поменять местами Active и StandBy частоты */
            unsigned int oldActiveInt = intActiveFreq;
            unsigned int oldActiveFloat = floatActiveFreq;
            intActiveFreq = intStandByFreq;
            floatActiveFreq = floatStandByFreq;
            intStandByFreq = oldActiveInt;
            floatStandByFreq = oldActiveFloat;

            hardSendState();
            drawActiveFreq();
            drawStandByFreq();
        }

    public:
        FreyNav() {};

        void prepare() {
            intActiveFreq = 108;
            floatActiveFreq = 50;
            intStandByFreq = 120;
            floatStandByFreq = 95;

            valueChanged = false;
            lastSended = millis();
            pinMode(pinNav1EncoderFloatCLK, INPUT_PULLUP);
            pinMode(pinNav1EncoderFloatDIO, INPUT_PULLUP);
            pinMode(pinNav1ActiveDisplayCLK, INPUT_PULLUP);
            pinMode(pinNav1ActiveDisplayDIO, INPUT_PULLUP);
            pinMode(pinNav1StandbyDisplayCLK, OUTPUT);
            pinMode(pinNav1StandbyDisplayDIO, OUTPUT);
            pinMode(pinNav1ActiveDisplayCLK, OUTPUT);
            pinMode(pinNav1ActiveDisplayDIO, OUTPUT);
            pinMode(pinNav1SwitchStation, INPUT_PULLUP);

            navStandByDisplay.clear();
            navActiveDisplay.clear();
            navStandByDisplay.brightness(3);
            navActiveDisplay.brightness(3);
            navStandByDisplay.point(true);
            navActiveDisplay.point(true);
            delay(100);

            navStandByDisplay.displayByte(0, _b);
            navStandByDisplay.display(1, 7);
            navStandByDisplay.display(2, 3);
            navStandByDisplay.display(3, 7);
            navStandByDisplay.displayByte(0, _b);
            navStandByDisplay.display(1, 7);
            navStandByDisplay.display(2, 3);
            navStandByDisplay.display(3, 7);

            delay(100);
            drawStandByFreq();
            drawActiveFreq();
        };

        void lap() {
            navIntEncoder.tick();
            navFloatEncoder.tick();
            if (navIntEncoder.turn()) {
                if (navIntEncoder.right()) {
                    intStandByFreq -= 1;
                }
                else {
                    intStandByFreq += 1;
                };
                if (intStandByFreq < 108) {
                    intStandByFreq = 117;
                }
                else if (intStandByFreq > 117) {
                    intStandByFreq = 108;
                };
                sendLog("Draw nav1i " + (String)intStandByFreq + ":" + (String)floatStandByFreq);
                drawStandByFreq();
                hardSendState();
            };
            if (navFloatEncoder.turn()) {
                if (navFloatEncoder.right()) {
                    floatStandByFreq -= 5;
                } else {
                    floatStandByFreq += 5;
                };
                if (floatStandByFreq < 0) {
                    floatStandByFreq = 95;
                } else if (floatStandByFreq > 95) {
                    floatStandByFreq = 0;
                };
                sendLog("Draw nav1 " + (String)intStandByFreq + ":" + (String)floatStandByFreq);
                drawStandByFreq();
                hardSendState();
            };

            navSwitchButton.tick();
            if (navSwitchButton.press() || navSwitchButton.release()) {
                sendLog("Switch standby <-> active");
                switchFreq();
            };

        };

        void readFullState(String fullState) {};

        void hardSendState() {
            sendPanelCommand(
                "NAV_FREQ_"
                    + (String)intActiveFreq + "." + (String)floatActiveFreq
                    + "_"
                    + (String)intStandByFreq + "." + (String)floatStandByFreq
            );
        };

};

#endif
