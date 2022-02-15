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
const unsigned int pinNav1ChangeStation = 50;


EncButton<EB_TICK, pinNav1EncoderIntCLK, pinNav1EncoderIntDIO> navIntEncoder;
EncButton<EB_TICK, pinNav1EncoderFloatCLK, pinNav1EncoderFloatDIO> navFloatEncoder;
GyverTM1637 navActiveDisplay(pinNav1ActiveDisplayCLK, pinNav1ActiveDisplayDIO);
GyverTM1637 navStandByDisplay(pinNav1StandbyDisplayCLK, pinNav1StandbyDisplayDIO);


class FreyNav {
    private:
        unsigned char intFreq;
        unsigned char floatFreq;
        void drawIntFreq() {
          String sIntFreq = (String)intFreq;
          if (intFreq == 0) {
              navStandByDisplay.displayByte(0, _empty);
              navStandByDisplay.displayByte(1, _empty);
              navStandByDisplay.displayByte(2, _empty);
              navStandByDisplay.display(3, 0);
    
          } else if (intFreq < 10) {
              // 1..9
              navStandByDisplay.displayByte(0, _empty);
              navStandByDisplay.displayByte(1, _empty);
              navStandByDisplay.displayByte(2, _empty);
              navStandByDisplay.display(3, ((String)sIntFreq[0]).toInt());
    
          } else if (intFreq < 100) {
              // 10..99
              navStandByDisplay.displayByte(0, _empty);
              navStandByDisplay.displayByte(1, _empty);
              navStandByDisplay.display(2, ((String)sIntFreq[0]).toInt());
              navStandByDisplay.display(3, ((String)sIntFreq[1]).toInt());
    
          } else {
              // 100..360
              navStandByDisplay.displayByte(0, _empty);
              navStandByDisplay.display(1, ((String)sIntFreq[0]).toInt());
              navStandByDisplay.display(2, ((String)sIntFreq[1]).toInt());
              navStandByDisplay.display(3, ((String)sIntFreq[2]).toInt());
          };
        };

        void drawFloatFreq() {
        };

    public:
        FreyNav() {};

        void prepare() {
            intFreq = 0;
            floatFreq = 0;
        };

        void lap() {
            if (navIntEncoder.turn()) {
                if (navIntEncoder.left()) {
                    intFreq -= 1;
                }
                else {
                    intFreq += 1;
                };
                if (intFreq < 108) {
                    intFreq = 120;
                }
                else if (intFreq > 120) {
                    intFreq = 108;
                };
            };
            if (navFloatEncoder.turn()) {
                if (navFloatEncoder.left()) {
                    floatFreq -= 5;
                } else {
                    floatFreq += 5;
                };
                if (floatFreq < 0) {
                    floatFreq = 95;
                } else if (floatFreq > 100) {
                    floatFreq = 0;
                }
            }
        };

        void readFullState(String fullState) {
        }

};

#endif
