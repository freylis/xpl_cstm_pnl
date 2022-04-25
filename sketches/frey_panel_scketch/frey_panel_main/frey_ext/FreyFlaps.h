#ifndef FreyFlaps_h
#define FreyFlaps_h

#include "Arduino.h"
#include <GyverTM1637.h>
#include "FreyCommand.h"
#include "FreyEncoder.h"

const unsigned int pinFlapsEncoderCLK = 35;
const unsigned int pinFlapsEncoderDIO = 34;
const unsigned int pinFlapsDisplayCLK = 50;
const unsigned int pinFlapsDisplayDIO = 51;


GyverTM1637 flapsDisplay(pinFlapsDisplayCLK, pinFlapsDisplayDIO);


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
        delay(100);

        flapsDisplay.displayByte(0, _b);
        flapsDisplay.display(1, 7);
        flapsDisplay.display(2, 3);
        flapsDisplay.display(3, 7);

    };

    /* call it each loop and relax */
    void lap() {
        encoders[1].tick();
        if (encoders[1].left()) {
            sendPanelCommand("FLAPS_DOWN_ONE");
        } else if (encoders[1].right()) {
            sendPanelCommand("FLAPS_UP_ONE");
        };
    };

    void readFullState(String fullState) {
      unsigned int dFlaps = fullState.substring(26, 29).toInt();
      sendLog("Found " + (String)dFlaps + " flaps");
      switch (dFlaps) {
        // oldstyle. raw value from dataref(flaps)
        case 0:
          sendLog("Draw flaps 0");
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 0);
          break;
        case 12:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 1);
          break;
        case 25:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 2);
          break;
        case 37:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.displayByte(2, _empty);
          flapsDisplay.display(3, 5);
          break;
        case 50:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 1);
          flapsDisplay.display(3, 0);
          break;
        case 62:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 1);
          flapsDisplay.display(3, 5);
          break;
        case 75:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 2);
          flapsDisplay.display(3, 5);
          break;
        case 87:
          flapsDisplay.displayByte(0, _empty);
          flapsDisplay.displayByte(1, _empty);
          flapsDisplay.display(2, 3);
          flapsDisplay.display(3, 0);
          break;
        case 100:
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

    void hardSendState() {};

};

#endif
