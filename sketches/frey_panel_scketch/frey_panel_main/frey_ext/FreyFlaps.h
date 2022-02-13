#ifndef FreyFlaps_h
#define FreyFlaps_h

#include <GyverTM1637.h>
#include <EncButton.h>
#include "FreyCommand.h"

#define PIN_FLAPS_ENC_CLK 0
#define PIN_FLAPS_ENC_DIO 0


// GyverTM1637 flapsDisplay(PIN_FLAPS_ENC_CLK, PIN_FLAPS_ENC_DIO);
EncButton<EB_TICK, PIN_FLAPS_DISPLAY_CLK, PIN_FLAPS_DISPLAY_DIO> flapsEnc;


class FreyFlaps {

  private:
    unsigned char _flapsVal;
    unsigned char _pinEncCLK;
    unsigned char _pinEncDIO;
    unsigned char _pinDisplayCLK;
    unsigned char _pinDisplayDIO;
    GyverTM1637() flapsDisplay;

  public:
    FreyFlaps(unsigned char pinEncCLK, unsigned char pinEncDIO, unsigned char pinDisplayCLK, unsigned char pinDisplayDIO) {
        unsigned char _pinEncCLK = pinEncCLK;
        unsigned char _pinEncDIO = pinEncDIO;
        unsigned char _pinDisplayCLK = pinEncCLK;
        unsigned char _pinDisplayDIO = pinEncDIO;
        GyverTM1637 flapsDisplay(pinDisplayCLK, pinDisplayDIO);
    };
    /* call it once in setup func */
    void prepare() {
        _flapsVal = 0;
        pinMode(_pinEncCLK, INPUT_PULLUP);
        pinMode(_pinEncDIO, INPUT_PULLUP);
        pinMode(_pinDisplayCLK, OUTPUT);
        pinMode(_pinDisplayDIO, OUTPUT);
    };
    /* call it each loop and relax */
    void lap() {
        flapsEnc.tick();
        if (flapsEnc.left()) {
            sendPanelCommand("FLAPS_DOWN_ONE");
        } else if (flapsEnc.right()) {
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
