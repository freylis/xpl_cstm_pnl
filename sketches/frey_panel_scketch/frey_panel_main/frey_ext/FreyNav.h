#ifndef FreyNav_h
#define FreyNav_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned int pinNav1EncoderIntCLK = 5;
const unsigned int pinNav1EncoderIntDIO = 4;
const unsigned int pinNav1EncoderFloatCLK = 8;
const unsigned int pinNav1EncoderFloatDIO = 9;
const unsigned int pinNav1ActiveDisplayCLK = 6;
const unsigned int pinNav1ActiveDisplayDIO = 7;
const unsigned int pinNav1StandbyDisplayCLK = 2;
const unsigned int pinNav1StandbyDisplayDIO = 3;
const unsigned int pinNav1SwitchStation = 10;


EncButton<EB_TICK, pinNav1EncoderIntCLK, pinNav1EncoderIntDIO> navIntEncoder;
EncButton<EB_TICK, pinNav1EncoderFloatCLK, pinNav1EncoderFloatDIO, pinNav1SwitchStation> navFloatEncoder;

GyverTM1637 navActiveDisplay(pinNav1ActiveDisplayCLK, pinNav1ActiveDisplayDIO);
GyverTM1637 navStandByDisplay(pinNav1StandbyDisplayCLK, pinNav1StandbyDisplayDIO);


class FreyNav {
    private:
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
          sendLog("Draw standby " + ((String)sIntStandByFreq[1]) + ((String)sIntStandByFreq[2]) + ":__");
          navStandByDisplay.display(0, ((String)sIntStandByFreq[1]).toInt());
          navStandByDisplay.display(1, ((String)sIntStandByFreq[2]).toInt());

          String sFloatStandByFreq = (String)floatStandByFreq;
          if (floatStandByFreq == 0) {
            sendLog("Draw standby __:00");
            navStandByDisplay.display(2, 0);
            navStandByDisplay.display(3, 0);
          } else if (floatStandByFreq < 10) {
            sendLog("Draw standby __:0" + ((String)sFloatStandByFreq[0]));
            navStandByDisplay.display(2, 0);
            navStandByDisplay.display(3, ((String)sFloatStandByFreq[0]).toInt());
          } else {
            sendLog("Draw standby __:" + ((String)sFloatStandByFreq[0]) + ((String)sFloatStandByFreq[1]));
            navStandByDisplay.display(2, ((String)sFloatStandByFreq[0]).toInt());
            navStandByDisplay.display(3, ((String)sFloatStandByFreq[1]).toInt());
          }

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
          sendLog("Draw active " + ((String)sIntActiveFreq[1]) + ((String)sIntActiveFreq[2]) + ":__");
          navActiveDisplay.display(0, ((String)sIntActiveFreq[1]).toInt());
          navActiveDisplay.display(1, ((String)sIntActiveFreq[2]).toInt());

          String sFloatActiveFreq = (String)floatActiveFreq;
          if (floatActiveFreq == 0) {
            sendLog("Draw active __:00");
            navActiveDisplay.display(2, 0);
            navActiveDisplay.display(3, 0);
          } else if (floatActiveFreq < 10) {
            sendLog("Draw active __:0" + ((String)sFloatActiveFreq[0]));
            navActiveDisplay.display(2, 0);
            navActiveDisplay.display(3, ((String)sFloatActiveFreq[0]).toInt());
          } else {
            sendLog("Draw active __:" + ((String)sFloatActiveFreq[0]) + ((String)sFloatActiveFreq[1]));
            navActiveDisplay.display(2, ((String)sFloatActiveFreq[0]).toInt());
            navActiveDisplay.display(3, ((String)sFloatActiveFreq[1]).toInt());
          }
        };

        void switchFreq() {
            /* поменять местами Active и StandBy частоты */
            unsigned int oldActiveInt = intActiveFreq;
            unsigned int oldActiveFloat = floatActiveFreq;
            intActiveFreq = intStandByFreq;
            floatActiveFreq = floatStandByFreq;
            intStandByFreq = oldActiveInt;
            floatStandByFreq = oldActiveFloat;

            sendPanelCommand("NAV_FREQ_" + (String)intActiveFreq + "." + (String)floatActiveFreq);
            drawActiveFreq();
            drawStandByFreq();
        }

    public:
        FreyNav() {};

        void prepare() {
            intActiveFreq = 108;
            floatActiveFreq = 50;
            intStandByFreq = 108;
            floatStandByFreq = 50;

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
            navStandByDisplay.brightness(5);
            navActiveDisplay.brightness(5);

            navStandByDisplay.point(true);
            navActiveDisplay.point(true);

            drawStandByFreq();
            drawActiveFreq();
        };

        void lap() {
            navIntEncoder.tick();
            navFloatEncoder.tick();
            if (navIntEncoder.turn()) {
                if (navIntEncoder.left()) {
                    intStandByFreq -= 1;
                }
                else {
                    intStandByFreq += 1;
                };
                if (intStandByFreq < 108) {
                    intStandByFreq = 120;
                }
                else if (intStandByFreq > 120) {
                    intStandByFreq = 108;
                };
                sendLog("Draw nav1i " + (String)intStandByFreq + ":" + (String)floatStandByFreq);
                drawStandByFreq();
            };
            if (navFloatEncoder.turn()) {
                if (navFloatEncoder.left()) {
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
            };

            if (navFloatEncoder.release()) {
                sendLog("Switch standby <-> active");
                switchFreq();
            };

        };

        void readFullState(String fullState) {
        }

};

#endif
