#ifndef FreyFlaps_h
#define FreyFlaps_h

#include "Arduino.h"
#include <GyverTM1637.h>
#include <EncButton.h>
#include "FreyCommand.h"

const unsigned int pinFlapsEncoderCLK = 4;
const unsigned int pinFlapsEncoderDIO = 5;
const unsigned int pinFlapsDisplayCLK = 2;
const unsigned int pinFlapsDisplayDIO = 3;


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
        flapsDisplay.clear();
        flapsDisplay.brightness(5);

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
      unsigned int dFlaps = fullState.substring(26, 29).toInt();
      sendLog("Found " + (String)dFlaps + " flaps");
      switch (dFlaps) {
        case 0:
          sendLog("Draw flaps 0");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 0);
          break;
        case 1:
          sendLog("Draw flaps 1");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 1);
          break;
        case 2:
          sendLog("Draw flaps 2");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 2);
          break;
        case 5:
          sendLog("Draw flaps 5");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 5);
          break;
        case 10:
          sendLog("Draw flaps 10");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 1);
          flapsDisplay.display(3, 0);
          break;
        case 15:
          sendLog("Draw flaps 15");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 1);
          flapsDisplay.display(3, 5);
          break;
        case 25:
          sendLog("Draw flaps 25");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 2);
          flapsDisplay.display(3, 5);
          break;
        case 30:
          sendLog("Draw flaps 30");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 3);
          flapsDisplay.display(3, 0);
          break;
        case 40:
          sendLog("Draw flaps 40");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 4);
          flapsDisplay.display(3, 0);
          break;
        default:
          sendLog("Draw flaps xz: " + (String)dFlaps);
          flapsDisplay.displayInt(dFlaps);
          flapsDisplay.displayByte(0, _dash);
          break;
      };
    };
};

#endif
