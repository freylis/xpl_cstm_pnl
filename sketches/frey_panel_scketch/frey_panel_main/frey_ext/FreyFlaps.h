#ifndef FreyFlaps_h
#define FreyFlaps_h

#include "Arduino.h"
#include <GyverTM1637.h>
#include <EncButton.h>
#include "FreyCommand.h"

const unsigned int pinFlapsEncoderCLK = A5;
const unsigned int pinFlapsEncoderDIO = A4;
const unsigned int pinFlapsDisplayCLK = A7;
const unsigned int pinFlapsDisplayDIO = A6;


GyverTM1637 flapsDisplay(pinFlapsDisplayCLK, pinFlapsDisplayDIO);
EncButton<EB_TICK, pinFlapsEncoderCLK, pinFlapsEncoderDIO> flapsEncoder;


class FreyFlaps {

  private:
    unsigned char _flapsVal;

  public:
    FreyFlaps() {};
    /* call it once in setup func */
    void prepare() {
        _flapsVal = 0;
        pinMode(pinFlapsEncoderCLK, INPUT_PULLUP);
        pinMode(pinFlapsEncoderDIO, INPUT_PULLUP);
        pinMode(pinFlapsDisplayCLK, OUTPUT);
        pinMode(pinFlapsDisplayDIO, OUTPUT);
    };
    /* call it each loop and relax */
    void lap() {
        flapsEncoder.tick();
        if (flapsEncoder.left()) {
            sendPanelCommand("FLAPS_DOWN_ONE");
        } else if (flapsEncoder.right()) {
            sendPanelCommand("FLAPS_UP_ONE");
        };
    };
    void readFullState(String fullState) {
      unsigned int dFlaps = 10;
      switch (dFlaps) {
        case 0:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 0);
          break;
        case 1:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 1);
          break;
        case 2:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 2);
          break;
        case 5:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 5);
          break;
        case 10:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 1);
          flapsDisplay.display(3, 0);
          break;
        case 15:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 1);
          flapsDisplay.display(3, 5);
          break;
        case 25:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 2);
          flapsDisplay.display(3, 5);
          break;
        case 30:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 3);
          flapsDisplay.display(3, 0);
          break;
        case 40:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 4);
          flapsDisplay.display(3, 0);
          break;
        default:
          flapsDisplay.displayInt(dFlaps);
          flapsDisplay.displayByte(0, _dash);
          break;
      };
    };
};

#endif
