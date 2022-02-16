#ifndef FreyNav_h
#define FreyNav_h

#include "Arduino.h"
#include "FreyCommand.h"
#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned int pinNav1EncoderIntCLK = 10;
const unsigned int pinNav1EncoderIntDIO = 11;
const unsigned int pinNav1EncoderFloatCLK = 12;
const unsigned int pinNav1EncoderFloatDIO = 13;
const unsigned int pinNav1ActiveDisplayCLK = 8;
const unsigned int pinNav1ActiveDisplayDIO = 9;
const unsigned int pinNav1StandbyDisplayCLK = 6;
const unsigned int pinNav1StandbyDisplayDIO = 7;
const unsigned int pinNav1SwitchStation = 50;


EncButton<EB_TICK, pinNav1EncoderIntCLK, pinNav1EncoderIntDIO> navIntEncoder;
EncButton<EB_TICK, pinNav1EncoderFloatCLK, pinNav1EncoderFloatDIO> navFloatEncoder;
EncButton<EB_TICK, pinNav1SwitchStation> navSwitchButton;

GyverTM1637 navActiveDisplay(pinNav1ActiveDisplayCLK, pinNav1ActiveDisplayDIO);
GyverTM1637 navStandByDisplay(pinNav1StandbyDisplayCLK, pinNav1StandbyDisplayDIO);


class FreyNav {
    private:
        unsigned char intStandByFreq;
        unsigned char floatStandByFreq;
        unsigned char intActiveFreq;
        unsigned char floatActiveFreq;
        void drawIntStandByFreq() {
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
        };

        void drawFloatStandByFreq() {
          String sFloatStandByFreq = (String)floatStandByFreq;
          navStandByDisplay.display(0, ((String)sFloatStandByFreq[0]).toInt());
          navStandByDisplay.display(1, ((String)sFloatStandByFreq[1]).toInt());
        };

        void drawIntActiveFreq() {
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
        };

        void drawFloatActiveFreq() {
          String sFloatActiveFreq = (String)floatActiveFreq;
          navActiveDisplay.display(0, ((String)sFloatActiveFreq[0]).toInt());
          navActiveDisplay.display(1, ((String)sFloatActiveFreq[1]).toInt());
        };

        void switchFreq() {
            /* поменять местами Active и StandBy частоты */
            unsigned int oldActiveInt = intActiveFreq;
            unsigned int oldActiveFloat = floatActiveFreq;
            intActiveFreq = intStandByFreq;
            floatActiveFreq = floatStandByFreq;
            intStandByFreq = oldActiveInt;
            floatStandByFreq = oldActiveFloat;
            drawIntActiveFreq();
            drawFloatActiveFreq();
            drawIntActiveFreq();
            drawFloatStandByFreq();
        }

    public:
        FreyNav() {};

        void prepare() {
            intActiveFreq = 0;
            floatActiveFreq = 0;
            intActiveFreq = 0;
            floatActiveFreq = 0;
        };

        void lap() {
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
                drawIntStandByFreq();
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
                drawFloatStandByFreq();
            };

            if (navSwitchButton.release()) {
                switchFreq();
            };

        };

        void readFullState(String fullState) {
        }

};

#endif
