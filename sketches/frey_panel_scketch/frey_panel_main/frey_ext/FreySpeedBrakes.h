#ifndef FreySpeedBrakes_h
#define FreySpeedBrakes_h

#include "Arduino.h"
#include <GyverTM1637.h>
#include <EncButton.h>

const unsigned char pinEncCLK = 4;
const unsigned char pinEncDIO = 5;
const unsigned char pinDisplayCLK = 2;
const unsigned char pinDisplayDIO = 3;


GyverTM1637 speedBrakesDisplay(pinDisplayCLK, pinDisplayDIO);
EncButton<EB_TICK, pinEncCLK, pinEncDIO> speedBrakesEncoder;


class FreySpeedBrakes {

  private:
    int _brakesValue;

  public:
    FreySpeedBrakes() {};

    /* call it once in setup func */
    void prepare() {
        _brakesValue = 0;
        pinMode(pinEncCLK, INPUT_PULLUP);
        pinMode(pinEncDIO, INPUT_PULLUP);
        pinMode(pinDisplayCLK, OUTPUT);
        pinMode(pinDisplayDIO, OUTPUT);
    };
    /* call it each loop and relax */
    void lap() {
        speedBrakesEncoder.tick();
        if (speedBrakesEncoder.left()) {
            sendPanelCommand("SPEEDBRAKES_DOWN_ONE");
        } else if (speedBrakesEncoder.right()) {
            sendPanelCommand("SPEEDBRAKES_UP_ONE");
        };
    };
    void readFullState(String fullState) {
      // read flaps value from fullString
      int rbrake = fullState.substring(31, 32).toInt();
      sendLog("Got sbrake: " + (String)rbrake);
      switch (rbrake) {
        case 0:
          speedBrakesDisplay.displayByte(0, _empty);
          speedBrakesDisplay.displayByte(1, _O);
          speedBrakesDisplay.displayByte(2, _F);
          speedBrakesDisplay.displayByte(3, _F);
          break;
        case 1:
          speedBrakesDisplay.displayByte(0, _empty);
          speedBrakesDisplay.displayByte(1, _A);
          speedBrakesDisplay.displayByte(2, _r);
          speedBrakesDisplay.displayByte(3, _d);
          break;
        case 2:
          speedBrakesDisplay.displayByte(0, _F);
          speedBrakesDisplay.displayByte(1, _l);
          speedBrakesDisplay.displayByte(2, _D);
          speedBrakesDisplay.displayByte(3, _t);
          break;
        case 3:
          speedBrakesDisplay.displayByte(0, _empty);
          speedBrakesDisplay.displayByte(1, _empty);
          speedBrakesDisplay.displayByte(2, _U);
          speedBrakesDisplay.displayByte(3, _P);
          break;
        default:
          speedBrakesDisplay.displayByte(0, _empty);
          speedBrakesDisplay.displayByte(1, _E);
          speedBrakesDisplay.displayByte(2, _r);
          speedBrakesDisplay.displayByte(3, _r);
          break;
      };
    };

    void hardSendState() {};

};

#endif
